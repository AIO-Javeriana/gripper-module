#include "./libs/src/sio_client.h"
#include "./libs/json.hpp"
#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

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

class CommunicationChannel{
    //int participants ;
    sio::client h;
    connection_listener l;
    socket::ptr current_socket;
    bool connect_finish_;
    string url="";
public:


    CommunicationChannel(string host,int port):l(h){
        participants=-1;
        connect_finish_ = false;
        this->url=host+":"+to_string(port);
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
    }
    void subscription(string &module_info){
            current_socket->on("SUBSCRIPTION-REPLY", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck,message::list &ack_resp){
                _lock.lock();
				json dataJSON=json::parse((data->get_string()));
				module_id=dataJSON["ID"];
                HIGHLIGHT("MODULE SUBSCRIBED \n ID "<<module_id);//;
				// EM(user<<":"<<message);                
				_cond.notify_all();
                _lock.unlock();
                current_socket->off("login");
            }));
            current_socket->emit("SUBSCRIPTION", module_info);
            _lock.lock();
            _lock.unlock();
            //bind_events();

    }
    void bind_events(){
			current_socket->on("BLINK", sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck,message::list &ack_resp){
                _lock.lock();
				json dataJSON=json::parse((data->get_string()));
                EM("\t BLINK");
				json reply_info={
					  {"ID","gripper_module"},
					  {"STATUS","DONE"},
					  {"MSG",""}
					  
				};
				string reply="";
				current_socket->emit("ACTION_FINISH", reply_info.dump() );
                //participants = data->get_map()["numUsers"]->get_int();
                //id = data->get_map()["id"]->get_string();
                //bool plural = participants !=1;
                //HIGHLIGHT("Welcome to TAQUIN \nthere"<<(plural?" are ":"'s ")<< participants<<(plural?" participants":" participant")<<"\n Your ID "<<id);//;
				// EM(user<<":"<<message);                
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
    
    CommunicationChannel st("ws://10.220.16.18",9090);
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
