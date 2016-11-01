
#ifndef MOBILITYMODULE_H
#define MOBILITYMODULE_H 

#include <Module.cpp>
#include <CalibratingSensorsService.cpp>
#include <MoveService.cpp>
/*
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
//*/
#define	LED	24
#define NUM_SENSORS 6
#define PORT_ADC1 0
#define M1DIR 3
#define M2DIR 21
// This is the maximum speed the motors will be allowed to turn.
// (400 lets the motors go at top speed; decrease to impose a speed limit)
#define MAX_SPEED  200
#define MIN_SPEED  -200
#define CALIBRATION_SPEED 150
#define THRESHOLD 800

class MobilityModuleInfo: public ModuleInfo{
    private:
        //ZumoReflectanceSensorArray reflectanceSensors;
        unsigned int sensorValues[NUM_SENSORS];
    public:
        MobilityModuleInfo(std::string module_id):ModuleInfo(module_id){
            /*
             pinMode (LED, OUTPUT) ;
             wiringPiSetup () ;
             ZumoMotors::init(); 
             wiringPiSetup () ;
             reflectanceSensors.init();
            // */
            
        }
};

class MobilityModule: public Module{

    public:
        MobilityModule(std::string host, int port,MobilityModuleInfo *moduleInfo) :
            Module(host, port,moduleInfo) {
               this->addService(new CalibratingSensorsService());
               this->addService(new MoveService()); 
        }

};

#endif