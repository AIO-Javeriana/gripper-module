
#include <Services.cpp>

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
    
    bool execute(json params){
        HIGHLIGHT("Blink");
        return true;
    }
};
