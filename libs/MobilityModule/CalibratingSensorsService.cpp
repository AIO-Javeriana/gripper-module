#ifndef CALIBRATINGSENSORSSERVICE_H
#define CALIBRATINGSENSORSSERVICE_H

#include <Services.cpp>

//#include <ZumoReflectanceSensorArray.h>
//#include <ZumoMotors.h>

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
        this->params.push_back("AUTOMATIC");//true is automatic or false is manual 
    }
    
    CalibratingSensorsService(string name, list<string> params, bool interruptible, bool service) :
        Service(name, params, interruptible, service) {
        Service::registerClass(name, &CalibratingSensorsService::create);
    }
    static Service* create() { 
        return new CalibratingSensorsService; 
    }
    
    bool execute(json params, double modulationValue, string &msg,ModuleInfo* moduleInfo){
        HIGHLIGHT("CALIBRATING-SENDORS: ");
        cout << modulationValue << endl;
        if(params["AUTOMATIC"]){
            automaticSensorCalibration();
        }else {
            manualSensorCalibration();
        }
        msg = "TERMINEE";
        return true;
    }
    
    void automaticSensorCalibration(/*ZumoReflectanceSensorArray &reflectanceSensors*/){
        cout<<"AUTOMATIC CALIBRATION"<<endl;
    }
    
    void manualSensorCalibration(/*ZumoReflectanceSensorArray &reflectanceSensors*/){
        cout<<"MANUAL CALIBRATION"<<endl;
    }
};

#endif