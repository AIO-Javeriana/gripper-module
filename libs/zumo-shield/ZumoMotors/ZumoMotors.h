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
#include <thread>
#include <mutex>

using namespace std;
class ZumoMotors
{
  private:
	static string PWM_PATH;
	static bool flipLeft;
	static bool flipRight;
	static bool enableLeft;
	static bool enableRight;
	static bool calibrateRight;
	static bool calibrateLeft;
	static int maxPulsesRight;
	static int maxPulsesLeft;
	static bool wasInit;
   	static void execute(string s);
    static int left_velocity;
    static int right_velocity;
    static int duty_cycle_right;
    static int duty_cycle_left;
	static float last_error_right;
	static float last_error_left;
	static float integral_right;
	static float integral_left;
    static thread* mobility_thread;
	static thread* left_velocity_thread;
    static thread* right_velocity_thread;
    static mutex m;
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
    static bool calibrationFinished();
    static void setDuty(int duty,int motor);
    static void setFrequency(int freq,int motor);
    static void setEnable(int enable,int motor);
    static void enableCalibration();    
    static void enableCalibrationLeft();    
    static void enableCalibrationRight();    
	static void mobilityThreadFunction();
    static void leftThread();
    static void rightThread();
	static void velocityController(int motor, int encoder,int referenceVelocity, int maxPulses,  int &duty_cycle, int time_window, float &lastError, float &integral);
	static int calibrateMotor(int motor, int encoder,int calibration_time, int time_window);
	static int getPulsesEach(int time, int encoder);
	static void controlDutyCicleValues(int &dutyCicle);
	static int getPulseFromSpeed(int speed, int maxPulses);
	
};

	string ZumoMotors::PWM_PATH= "/sys/devices/platform/pwm-ctrl/";
	bool ZumoMotors::flipLeft = false;
	bool ZumoMotors::flipRight = false;	
	bool ZumoMotors::enableLeft =false;
	bool ZumoMotors::enableRight = false;
	bool ZumoMotors::wasInit=false;
	bool ZumoMotors::calibrateRight=false;
	bool ZumoMotors::calibrateLeft=false;
	int ZumoMotors::maxPulsesRight = 0;
	int ZumoMotors::maxPulsesLeft = 0;	
    mutex ZumoMotors::m;
    thread* ZumoMotors::mobility_thread = NULL;
	thread* ZumoMotors::left_velocity_thread = NULL;
    thread* ZumoMotors::right_velocity_thread = NULL;
    int ZumoMotors::right_velocity = 0;
	int ZumoMotors::left_velocity = 0;
	int ZumoMotors::duty_cycle_right = 0;
	int ZumoMotors::duty_cycle_left = 0;
	float ZumoMotors::last_error_right = 0;
	float ZumoMotors::last_error_left = 0;
	float ZumoMotors::integral_right = 0;
	float ZumoMotors::integral_left = 0;
		
#include "ZumoMotors.cpp"

#endif

