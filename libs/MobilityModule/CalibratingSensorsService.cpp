#ifndef CALIBRATINGSENSORSSERVICE_H
#define CALIBRATINGSENSORSSERVICE_H

#include <Services.cpp>
/*
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
//*/
#ifdef WIN32

#define HIGHLIGHT(__O__) std::cout<<__O__<<std::endl
#define EM(__O__) std::cout<<__O__<<std::endl
#include <stdio.h>
#include <tchar.h>

#else

#define HIGHLIGHT(__O__) std::cout<<"\e[1;31m"<<__O__<<"\e[0m"<<std::endl
#define EM(__O__) std::cout<<"\e[1;30;1m"<<__O__<<"\e[0m"<<std::endl

#endif
using namespace std;

class CalibratingSensorsService: public Service{

public:
    
    CalibratingSensorsService():Service(){
        this->name="CALIBRATING-SENDORS";
        this->interruptible=false;
        this->service=false;
        this->params.push_back("MODE");//true is automatic or false is manual 
        Service::registerClass(name, &CalibratingSensorsService::create);
    }
    
    CalibratingSensorsService(string name, list<string> params, bool interruptible, bool service) :
        Service(name, params, interruptible, service) {
        Service::registerClass(name, &CalibratingSensorsService::create);
    }
    static Service* create() { 
        return new CalibratingSensorsService; 
    }
    
    bool execute(json params, string &msg,ModuleInfo* moduleInfo){
        HIGHLIGHT("CALIBRATING-SENDORS: ");
        cout << params["EMOTIONAL_VALUE"] <<" "<<params["MODE"] << endl;
        string mode=params["MODE"];
        if((mode).compare("AUTOMATIC")==0 ){
            
        }else if((mode).compare("MANUAL")==0 ){
        
        }
        /*    
        ZumoMotors::init();
        // Turn on LED to indicate we are in calibration mode
        pinMode (LED, OUTPUT) ;
        digitalWrite (LED, HIGH) ;	// On
        //*//*
        if(params["AUTOMATIC"]){
            automaticSensorCalibration();
        }else {
            manualSensorCalibration();
        }
        //digitalWrite (LED, LOW) ;	// Off
        //*/
        msg = "TERMINEE";
        return true;
    }
    
    void automaticSensorCalibration(/*ZumoReflectanceSensorArray &reflectanceSensors*/){
        cout<<"AUTOMATIC CALIBRATION"<<endl;
        /*
            ZumoMotors::init();
            // Initialize the reflectance sensors module
            reflectanceSensors.init();

            // Wait for the user button to be pressed and released
            //button.waitForButton();
            // Wait 1 second and then begin automatic sensor calibration
            // by rotating in place to sweep the sensors over the line
            delay(1000);

            int i; 
            unsigned long startTime = millis();
            unsigned long timeLimit=800;
            for (i = 0; i < 2; i++){
                startTime = millis();
                ZumoMotors::setSpeeds(-CALIBRATION_SPEED , CALIBRATION_SPEED );	
                while (millis() - startTime < timeLimit)   // make the calibration take 10 seconds
                {
                    reflectanceSensors.calibrate();
                    //delay(10);
                }
                ZumoMotors::setSpeeds(0, 0);
                startTime = millis();
                ZumoMotors::setSpeeds(CALIBRATION_SPEED , -CALIBRATION_SPEED );
                while (millis() - startTime < timeLimit)   // make the calibration take 10 seconds
                {
                    reflectanceSensors.calibrate();
                }
                ZumoMotors::setSpeeds(0, 0);
                startTime = millis();
                ZumoMotors::setSpeeds(CALIBRATION_SPEED , -CALIBRATION_SPEED );	
                while (millis() - startTime < timeLimit)   // make the calibration take 10 seconds
                {
                    reflectanceSensors.calibrate();
                }
                ZumoMotors::setSpeeds(0, 0);
                startTime = millis();
                ZumoMotors::setSpeeds(-CALIBRATION_SPEED , CALIBRATION_SPEED );
                while(millis() - startTime < timeLimit)   // make the calibration take 10 seconds
                {
                    reflectanceSensors.calibrate();
                }
                // Since our counter runs to 80, the total delay will be
                // 80*20 = 1600 ms.

            }
            ZumoMotors::setSpeeds(0,0);
         
         
               //*/
    }
    
    void manualSensorCalibration(/*ZumoReflectanceSensorArray &reflectanceSensors*/){
        cout<<"MANUAL CALIBRATION"<<endl;
        
            /*
            reflectanceSensors.init();
            unsigned long startTime = millis();
            while(millis() - startTime < 10000)   // make the calibration take 10 seconds
            {
                reflectanceSensors.calibrate();
            }
                
               */
    }
};

#endif