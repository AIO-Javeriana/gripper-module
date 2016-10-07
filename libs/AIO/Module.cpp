#ifndef MODULE_H
#define MODULE_H

#include <json.hpp>
#include <string>
#include <CommunicationChannel.cpp>

using json = nlohmann::json;
using namespace std;

class Module{
    
    string ID;
    Services* services;
    CommunicationChannel* communicationChannel;
    
    public:
        Module(string ID, string host, int port){
            this->ID = ID;
            this->services = new Services();
            this->communicationChannel = new CommunicationChannel(host, port, this->services);
        }
        
        void addService(Service* service){
            this->services->addService(service);
        }
        
        void start(){
            json module_info = this->services->getInfo();
            module_info["MODULE_ID"] = this->ID;
            this->communicationChannel->start(module_info.dump());
        }
};

#endif