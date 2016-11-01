#ifndef BLINKSERVICE_H
#define BLINKSERVICE_H

#include <Services.cpp>
#ifdef WIN32

#define HIGHLIGHT(__O__) std::cout<<__O__<<std::endl
#define EM(__O__) std::cout<<__O__<<std::endl
#include <stdio.h>
#include <tchar.h>

#else

#define HIGHLIGHT(__O__) std::cout<<"\e[1;31m"<<__O__<<"\e[0m"<<std::endl
#define EM(__O__) std::cout<<"\e[1;30;1m"<<__O__<<"\e[0m"<<std::endl

#endif
using namespace std;

class BlinkService: public Service{

public:
    
    BlinkService():Service(){}
    
    BlinkService(string name, list<string> params, bool interruptible, bool service) :
        Service(name, params, interruptible, service) {
        Service::registerClass(name, &BlinkService::create);
    }
    static Service* create() { 
        return new BlinkService; 
    }
    
    bool execute(json params, double modulationValue, string &msg,ModuleInfo* moduleInfo){
        HIGHLIGHT("BLINK: ");
        cout << modulationValue << endl;
        msg = "TERMINEE";
        return true;
    }
};

#endif