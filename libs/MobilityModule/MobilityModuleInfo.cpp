/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#ifndef MOBILITYMODULEINFO_H
#define MOBILITYMODULEINFO_H


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
#define THRESHOLD 800

class MobilityModuleInfo: public ModuleInfo{
    private:
        ZumoReflectanceSensorArray reflectanceSensors;
        unsigned int sensorValues[NUM_SENSORS];
    public:
        MobilityModuleInfo(std::string module_id):ModuleInfo(module_id){
             pinMode (LED, OUTPUT) ;
             wiringPiSetup () ;
             ZumoMotors::init(); 
             wiringPiSetup () ;
             reflectanceSensors.init();
            // */
            
        }
		
		ZumoReflectanceSensorArray& getReflectanceSensorArray(){
			return reflectanceSensors;
		} 
		//*/
};
#endif
