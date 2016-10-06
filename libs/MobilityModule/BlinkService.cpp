
#include <Services.cpp>

using namespace std;

class BlinkService: public Service{

public:
    BlinkService(string name, list<string> params, bool interruptible, bool service) :
        Service(name, params, interruptible, service) {
    }

    void execute(json params){
        HIGHLIGHT("Blink");
    }
};
