
#ifndef MOBILITYMODULE_H
#define MOBILITYMODULE_H 

#include <Module.cpp>
#include <MobilityModuleInfo.cpp>
#include <CalibratingSensorsService.cpp>
#include <MoveService.cpp>
#include <wiringPi.h>

class MobilityModule: public Module{

    public:
        MobilityModule(std::string host, int port,MobilityModuleInfo *moduleInfo) :
            Module(host, port,moduleInfo) {
			    wiringPiSetup () ;
               this->addService(new CalibratingSensorsService());
               this->addService(new MoveService()); 
        }

};

#endif
