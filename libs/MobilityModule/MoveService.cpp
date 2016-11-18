
#ifndef MOVESERVICE_H
#define MOVESERVICE_H 
        
#include <json.hpp>
#include <iostream>
#include <MobilityModuleInfo.cpp>
#include <Services.cpp>
#include <string>

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
using json = nlohmann::json;
using namespace std;


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

class MoveService: public Service{

public:
    
    MoveService():Service(){
        this->name="MOVE";
        this->interruptible=false;
        this->service=false;
        this->params.push_back("DIRECTION");//NONE, TURN-RIGHT,TURN-LEFT 
        Service::registerClass(name, &MoveService::create);
    
    }
    
    MoveService(string name, list<string> params, bool interruptible, bool service) :
        Service(name, params, interruptible, service) {
        Service::registerClass(name, &MoveService::create);
    }
    static Service* create() { 
        return new MoveService; 
    }
    
    bool execute(json params, string &msg,ModuleInfo* moduleInfo){
        MobilityModuleInfo* mobilityModuleInfo=dynamic_cast<MobilityModuleInfo*>(moduleInfo); 
        HIGHLIGHT("MOVE: ");
        cout << params["EMOTIONAL_VALUE"] << endl;
        int direction=0;
        bool isIntersection=false;
        string direction_parameter=params["DIRECTION"];
        cout<<direction_parameter<<endl;
        if((direction_parameter).compare("NONE")==0 ){
            direction=0;
        }else if((direction_parameter).compare("TURN-RIGHT")==0 ){
            direction=1;
        }else if((direction_parameter).compare("TURN-LEFT")==0 ){
            direction=2;
        }
        
        int lastError = 0,integral=0;
        while(!isIntersection){
              move(mobilityModuleInfo->getReflectanceSensorArray(),lastError,integral,isIntersection,direction);
        }
        ZumoMotors::setSpeeds(0,0 );
        ZumoMotors::init(); 
        //*/
        return true;
    }
    
    bool isLine(unsigned int sensorValue){
	return sensorValue > THRESHOLD;
    }

    bool isThereIntersection(unsigned int sensorValues[]){
		int sensors=0;
		int threshold=800;
	
		for (int i = 0; i < NUM_SENSORS; i++){
				cout<<sensorValues[i]<<endl;
			 if (isLine(sensorValues[i])){
					sensors++;
				}else if (i+1 < NUM_SENSORS && isLine(sensorValues[i+1]) && i-1>=0 && !isLine(sensorValues[i-1]) && sensors >0){
						//return true;				
					}
			}
		//*/
		cout<<" ->>>>>>>>> "<< sensors<<endl;
		if (sensors==6){
			return true;
		}
		return false;
    }

    void followLine(int position ,int &lastError,int &integral){
        int error = position - 2500;
        // Get motor speed difference using proportional and derivative PID terms
        // (the integral term is generally not very useful for line following).
        // Here we are using a proportional constant of 1/4 and a derivative
        // constant of 6, which should work decently for many Zumo motor choices.
        // You probably want to use trial and error to tune these constants for
        // your particular Zumo and line course.
        //int speedDifference = error / 4 + 6 * (error - lastError);
        int speedDifference = error/4  ;+ 6 * (error - lastError)+(integral+error)/8;	
        integral=integral+error;
        lastError = error;
        // Get individual motor speeds.  The sign of speedDifference
        // determines if the robot turns left or right.

        int m1Speed = MAX_SPEED + speedDifference;
        int m2Speed = MAX_SPEED - speedDifference;
        //*/
        /*
        int m1Speed =  speedDifference;
        int m2Speed =  -speedDifference;
        //*/
        cout<<" m1Speed "<<m1Speed<<" m2Speed "<<m2Speed<<" error "<< error <<" speedDifference "<<speedDifference<<" integral "<<integral <<endl;
        // Here we constrain our motor speeds to be between 0 and MAX_SPEED.
        // Generally speaking, one motor will always be turning at MAX_SPEED
        // and the other will be at MAX_SPEED-|speedDifference| if that is positive,
        // else it will be stationary.  For some applications, you might want to
        // allow the motor speed to go negative so that it can spin in reverse.
        if (m1Speed < MIN_SPEED  )
          m1Speed = MIN_SPEED ;
        if (m2Speed < MIN_SPEED)
          m2Speed = MIN_SPEED;
        if (m1Speed > MAX_SPEED)
          m1Speed = MAX_SPEED;
        if (m2Speed > MAX_SPEED)
          m2Speed = MAX_SPEED;
         ZumoMotors::setSpeeds(m1Speed, m2Speed);

    }

    
    void move(ZumoReflectanceSensorArray &reflectanceSensors,int &lastError,int &integral,bool &isIntersection,int &direction){
        unsigned int sensorValues[NUM_SENSORS];
        // Get the position of the line.  Note that we *must* provide the "sensors"
        // argument to readLine() here, even though we are not interested in the
        // individual sensor readings
        int position = reflectanceSensors.readLine(sensorValues);
        isIntersection=isThereIntersection(sensorValues);
        if (isIntersection && direction==0){
            cout<<"New Values position "<<position<<"    "<<direction <<endl;
            for (int i = 0; i < NUM_SENSORS; i++){
                            cout<<sensorValues[i]<<endl;
                    }
            return;																															
            }else{
                   isIntersection=false;	
            }

        if (position>2000 && position<3000 &&isLine(sensorValues[2]) && isLine(sensorValues[3])&& !isLine(sensorValues[1])&& !isLine(sensorValues[4])){
            cout<<sensorValues[1]<<" ++ "<<sensorValues[4]<<endl;		
            direction=0;
        }

        switch (direction){
                  case (1): 
                            position=5000;
                            cout<< position;									
                            for(int i=NUM_SENSORS-1;i>=0;i--){
                                    if(isLine(sensorValues[i])){
                                            position=i*1000;
                                            break;						
                                    }
                            }
                            cout<<"   --------------------- "<<position<<endl;
                            break;
                  case (2): 
                            position=0;
                            cout<< position;									
                            for(int i=0;i<NUM_SENSORS;i++){
                                    if(isLine(sensorValues[i])){
                                            position=i*1000;
                                            break;						
                                    }
                            }
                            cout<<"   --------------------- "<<position<<endl;
                            break;
            };
          followLine(position ,lastError,integral);
        }
        //*/
    
};
#endif
