#ifndef MODULE_H
#define MODULE_H

#include <json.hpp>
#include <string>
#include <CommunicationChannel.cpp>

using json = nlohmann::json;
using namespace std;

class Module{
    
    string module_id;
    Services* services;
    CommunicationChannel* communicationChannel;
    
    public:
        Module(string module_id, string host, int port){
            this->module_id = module_id;
            this->services = new Services();
            this->communicationChannel = new CommunicationChannel(host, port, this->services,this);
        }
        
        void addService(Service* service){
            this->services->addService(service);
        }
        
        void start(){
            json module_info = this->services->getInfo();
            module_info["MODULE_ID"] = this->module_id;
            this->communicationChannel->start(module_info.dump());
        }
        
        string getModule_id(){
            return this->module_id;
        }
        void setModule_id(string module_id){
            this->module_id=module_id;
        }
};

#endif