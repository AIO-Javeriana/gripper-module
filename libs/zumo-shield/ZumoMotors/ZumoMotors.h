/*! \file ZumoMotors.h
 *
 * See the ZumoMotors class reference for more information about this library.
 *
 * \class ZumoMotors ZumoMotors.h
 * \brief Control motor speed and direction
 * 
 */

#ifndef ZumoMotors_h
#define ZumoMotors_h

#include <wiringPi.h>

using namespace std;
class ZumoMotors
{
  private:
	static string PWM_PATH;
	static bool flipLeft ;
	static bool flipRight;
	static bool wasInit;
   	static void execute(string s);

  public:  
  	
    // constructor (doesn't do anything)
    ZumoMotors();
    static void init();
    // enable/disable flipping of motors
    static void flipLeftMotor(bool flip);
    static void flipRightMotor(bool flip);
    
    // set speed for left, right, or both motors
    static void setLeftSpeed(int speed);
    static void setRightSpeed(int speed);
    static void setSpeeds(int leftSpeed, int rightSpeed);
    
	static void setDuty(int duty,int motor);
	static void setFrequency(int freq,int motor);
	static void setEnable(int enable,int motor);
	

	
};

	string ZumoMotors::PWM_PATH= "/sys/devices/platform/pwm-ctrl/";
	bool ZumoMotors::flipLeft = false;
	bool ZumoMotors::flipRight = false;	
	bool ZumoMotors::wasInit=false;
#include "ZumoMotors.cpp"

#endif

