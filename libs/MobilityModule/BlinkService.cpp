
#include <Services.cpp>

using namespace std;

class BlinkService: public Service{

public:
    
    BlinkService():Service(){}
    
    BlinkService(string name, list<string> params, bool interruptible, bool service) :
        Service(name, params, interruptible, service) {
        Service::registerClass("BlinkService", &BlinkService::create);
    }
    static Service* create() { 
        return new BlinkService; 
    }
    
    void execute(json params){
        HIGHLIGHT("Blink");
    }
};
