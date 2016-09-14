#include "./libs/src/sio_client.h"
#include "./libs/json.hpp"
#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <queue>

#ifdef WIN32

#define HIGHLIGHT(__O__) std::cout<<__O__<<std::endl
#define EM(__O__) std::cout<<__O__<<std::endl
#include <stdio.h>
#include <tchar.h>

#else

#define HIGHLIGHT(__O__) std::cout<<"\e[1;31m"<<__O__<<"\e[0m"<<std::endl
#define EM(__O__) std::cout<<"\e[1;30;1m"<<__O__<<"\e[0m"<<std::endl

#endif

using json = nlohmann::json;
using namespace sio;
using namespace std;

std::mutex _lock;
std::condition_variable_any _cond;
bool connect_finish = false;
string module_id="gripper_module";



class connection_listener
{
    sio::client &handler;

public:

    connection_listener(sio::client& h):
    handler(h)
    {
    }


    void on_connected()
    {
        _lock.lock();
        _cond.notify_all();
        connect_finish = true;
        _lock.unlock();
    }
    void on_close(client::close_reason const& reason)
    {
        std::cout<<"sio closed "<<std::endl;
        exit(0);
    }

    void on_fail()
    {
        std::cout<<"sio failed "<<std::endl;
        exit(0);
    }
};

int participants = -1;

socket::ptr current_socket;

class TaskInfo{
    json task;
    bool completed;
    bool error;

  public:
      TaskInfo(json task){
        this->task=task;
        this->completed=false;
        this->error=false;

      }
      void setTask(json task){
        this->task=task;
      }
      json getTask(){
        return this->task;
      }
      void completedTask(){
          this->completed=true;
      }
      void errorTask(){
          this->completed=true;
      }
      bool isCompletedTask(){
          return this->completed;
      }
      bool isError(){
          return this->error;
      }
};

class Services{
    queue<TaskInfo> tasksInfo;


  public:
      Services(){

      }

      bool assessTask(json task){
          if (task["COMMAND"]=="BLINK") {
              this->tasksInfo.push(TaskInfo(task));
              return true;
          }
          return false;
      }

      TaskInfo makeTaskFront(){
            TaskInfo task=this->tasksInfo.front();
            this->tasksInfo.pop();
            task.completedTask();
            return task;
      }

      bool isEmptyTasks(){
            return this->tasksInfo.empty();
      }


};



class CommunicationChannel{
    //int participants ;
    sio::client h;
    connection_listener l;
    socket::ptr current_socket;
    Services services;
    bool subscribed;
    string url="";
public:


    CommunicationChannel(string host,int port,Services services):l(h){
       this->subscribed=false;
        this->url=host+":"+to_string(port);
        this->services=services;
    }

    void start(){
        h.set_open_listener(std::bind(&connection_listener::on_connected, &l));
        h.set_close_listener(std::bind(&connection_listener::on_close, &l,std::placeholders::_1));
        h.set_fail_listener(std::bind(&connection_listener::on_fail, &l));
        h.connect(url);
        _lock.lock();
        if(!connect_finish)
        {
            _cond.wait(_lock);
        }
        _lock.unlock();
        current_socket = h.socket();
		bind_events();
    }

   void subscription(string &module_info){
            current_socket->emit("SUBSCRIPTION", module_info);
    }

    void bind_events(){
		   current_socket->on("SUBSCRIPTION-REPLY", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck,message::list &ack_resp){
                _lock.lock();
        				json dataJSON=json::parse((data->get_string()));
        				module_id=dataJSON["ID"];
                        HIGHLIGHT("MODULE SUBSCRIBED \n ID "<<module_id);//;
        				// EM(user<<":"<<message);
        				this->subscribed=true;

        				_cond.notify_all();
                _lock.unlock();
                current_socket->off("login");
            }));
			current_socket->on("TASK-ASSIGN", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck,message::list &ack_resp){
                _lock.lock();
        				json dataJSON=json::parse((data->get_string()));
                EM("\t TASK-ASSING "<<dataJSON);
        				json reply_info={
        					  {"ID","gripper_module"},
        					  {"REPLY","ACCEPTED"},
        					  {"ID_TASK",dataJSON["ID_TASK"]}
                };
                if (!this->services.assessTask(dataJSON)){
                  reply_info["REPLY"]="REFUSE";
                }
        				current_socket->emit("TASK-ASSIGN-REPLY", reply_info.dump() );
    		        _lock.unlock();
            }));
      current_socket->on("ALL-BEGINS", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck,message::list &ack_resp){
                _lock.lock();
                json dataJSON=json::parse((data->get_string()));
                EM("\t ALL-BEGINS "<<dataJSON);
                json reply_info={
                    {"ID","gripper_module"},
                    {"STATUS","DONE"},//error
                    {"MSG",""},
                    {"ID_TASK",dataJSON["ID_TASK"]}

                };
                string reply="";
                while (!services.isEmptyTasks()){
                      //_lock.lock();
                      TaskInfo task=services.makeTaskFront();
                      reply_info["ID_TASK"]=task.getTask()["ID_TASK"];
                      if(!task.isError()){
                        reply_info["STATUS"]="DONE";
                      }else{
                          reply_info["STATUS"]="ERROR";
                      }
                      current_socket->emit("ACTION-FINISH", reply_info.dump() );
                      //_lock.unlock();
                }
                _lock.unlock();
            }));

        }

    void setURL(string url){
        this->url=url;
    }

    connection_listener getConnection_listenert(){
        return l;
    }

    socket::ptr getCurrent_socket(){
        return current_socket;
    }

    void endConnection(){
      h.sync_close();
      h.clear_con_listeners();
	  current_socket->off_all();
      current_socket->off_error();
    }

    ~CommunicationChannel(){
      cout<<"Destructor CommunicationChannel"<<endl;
      current_socket->off_all();
      current_socket->off_error();
      h.sync_close();
      h.clear_con_listeners();
      //delete h;
    }

    };


int main(int argc ,const char* args[])
{

    json module_info={
      {"ID",module_id},
      {"COMMANDS",json::array({
            {
              {"COMMAND","BLINK"},
              {"PARAMS",json::array({""})}
            }
          })
      }
    };
    //{"COMMANDS":[["COMMAND","BLINK"],["PARAMS",[""]]],"ID":"gripper_module"}
    //{"ID":"audiovisual_module","COMMANDS":[{"COMMAND":"BLINK","PARAMS":[]}]}
    Services services;
    CommunicationChannel st("ws://10.220.16.18",9090,services);
	//CommunicationChannel st("ws://localhost",9090);

	st.start();
    string info=module_info.dump();
    st.subscription(info);
	current_socket=st.getCurrent_socket();
    HIGHLIGHT("Start to Gripper Module\n'$exit' : exit chat\n");
    for (std::string line; std::getline(std::cin, line);) {
        if(line.length()>0)
        {
            if(line == "$exit")
            {
                break;
            }
			_lock.lock();
			_lock.unlock();


        }
    }


    HIGHLIGHT("Closing...");
	st.endConnection();
	return 0;
}
