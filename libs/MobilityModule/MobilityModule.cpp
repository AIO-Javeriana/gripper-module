
#ifndef MOBILITYMODULE_H
#define MOBILITYMODULE_H 

#include <Module.cpp>
#include <MobilityModuleInfo.cpp>
#include <CalibratingSensorsService.cpp>
#include <MoveService.cpp>
#include <BatterySensorService.cpp>
#include <wiringPi.h>

class MobilityModule: public Module{

    public:
        MobilityModule(std::string host, int port,MobilityModuleInfo *moduleInfo) :
            Module(host, port,moduleInfo) {
			    wiringPiSetup () ;
				CalibratingSensorsService *css=new CalibratingSensorsService();
               this->addService(css);
               this->addService(new MoveService());
			   this->addSensorService(new BatterySensorService(this->moduleInfo->getModule_id()));
				//css->automaticSensorCalibration(moduleInfo->getReflectanceSensorArray());
        }

};

#endif
