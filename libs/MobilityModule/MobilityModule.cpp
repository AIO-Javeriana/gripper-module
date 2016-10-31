
#include <Module.cpp>

class MobilityModule: public Module{

    public:
        MobilityModule(string ID, string host, int port) :
            Module(ID, host, port) {
        }

};