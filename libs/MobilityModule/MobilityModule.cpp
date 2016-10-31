
#ifndef MOBILITYMODULE_H
#define MOBILITYMODULE_H 

#include <Module.cpp>

class MobilityModule: public Module{

    public:
        MobilityModule(std::string module_id, std::string host, int port) :
            Module(module_id, host, port) {
        }

};

#endif