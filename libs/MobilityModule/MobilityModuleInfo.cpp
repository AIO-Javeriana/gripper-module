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

#define NUM_SENSORS 6

class MobilityModuleInfo: public ModuleInfo{
    private:
        ZumoReflectanceSensorArray reflectanceSensors;
        unsigned int sensorValues[NUM_SENSORS];
    public:
        MobilityModuleInfo(std::string module_id):ModuleInfo(module_id){
            
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
