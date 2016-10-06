
#ifndef SERVICES_H
#define SERVICES_H

#include <json.hpp>
#include <queue>
#include <map>
#include <list>

using json = nlohmann::json;
using namespace std;

class CommandInfo{
    json command;
    bool completed;
    bool error;

  public:
      CommandInfo(json command){
        this->command=command;
        this->completed=false;
        this->error=false;

      }
      void setCommand(json command){
        this->command=command;
      }
      json getCommand(){
        return this->command;
      }
      void completedWork(){
          this->completed=true;
      }
      void errorWork(){
          this->completed=true;
      }
      bool isCompletedTask(){
          return this->completed;
      }
      bool isError(){
          return this->error;
      }
};
 
class Service{
    typedef Service* (*crfnptr)(void);
    typedef map<string, crfnptr> CreatorMap;
    string name;
    list<string> params;
    bool interruptible;
    bool service;
    static CreatorMap creators;
    public:
        Service(){
        }
        
        Service(string name, list<string> params, bool interruptible, bool service){
            this->name = name;
            this->params = params;
            this->interruptible = interruptible;
            this->service = service;
        }
        
        virtual void execute(json params) = 0;
        
        string getName(){
            return this->name;
        }
        json getInfo(){
            json info;
            info["COMMAND"] = this->name;
            info["PARAMS"] = json::array();
            for(string param: params){
                info["PARAMS"].push_back(param);
            }
            info["INTERRUPTIBLE"] = this->interruptible;
            info["SERVICE"] = this->service;
            return info;
        }
        
        static Service* createFromString(string name)
        {
          CreatorMap::const_iterator it = creators.find(name);
          return it == creators.end() ? NULL : it->second();
        }
        
        static void registerClass(string name, crfnptr f)
        {
            cout << "Clase " << name << " registrada" << endl;
            creators[name] = f;
        }
};
map<string, Service* (*)(void)> Service::creators = map<string, Service* (*)(void)>();

class Services{
    map<long,queue<CommandInfo>> commandsInfo;
    map<string, Service*> services;    
    bool working;

  public:
      Services(){
          this->working=false;
      }

      bool assessWork(json command){
        map<string, Service*>::iterator it = services.find(command["COMMAND"]);
            if(it != services.end()){
              this->commandsInfo[command["GROUP_ID"]].push(CommandInfo(command));
              return true;
            }else{
              return false;
        }
      }

      CommandInfo getNextWork(long group_id){
            CommandInfo command=this->commandsInfo[group_id].front();
            this->commandsInfo[group_id].pop();
            return command;
      }

      bool isEmptyCommands(long group_id){
            return this->commandsInfo[group_id].empty();
      }

      void setWorking(bool working){
          this->working=working;
      }

      bool isWorking(){
          return this->working;
      }
      
      void addService(Service* service){
          this->services[service->getName()] = service;
      }
      
      json getInfo(){
          json module_info;
          module_info["COMMANDS"] = json::array();
          map<string, Service*>::iterator it = services.begin();
          for(; it != services.end(); it++){
              Service* service = it->second;
              module_info["COMMANDS"].push_back(service->getInfo());
          }
          return module_info;
      }

};

#endif