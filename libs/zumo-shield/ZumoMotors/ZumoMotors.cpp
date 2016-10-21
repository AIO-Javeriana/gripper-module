//#include "ZumoMotors.h"

#define PWM_L 0
#define PWM_R 1
#define DIR_L 21
#define DIR_R 3





// constructor (doesn't do anything)
ZumoMotors::ZumoMotors(void)
{
	//init();
	/*
	pinMode(PWM_L,  OUTPUT);
	pinMode(PWM_R,  OUTPUT);
	pinMode(DIR_L, OUTPUT);
	pinMode(DIR_R, OUTPUT);
	//*/

}

void ZumoMotors::init(){
	if (!ZumoMotors::wasInit){
		ZumoMotors::wasInit=true;
		ZumoMotors::execute("sudo modprobe pwm-meson npwm=2");
		ZumoMotors::execute("sudo modprobe pwm-ctrl");	
		pinMode (DIR_R, OUTPUT) ;
		pinMode (DIR_L, OUTPUT) ;
		digitalWrite (DIR_R, LOW) ;	// LOW - FORWARD / HIGH -BACKWARD
		digitalWrite (DIR_L, LOW);	// LOW - FORWARD / HIGH -BACKWARD
		int freq=20000;
		int duty=500;
		int enable=0;
		ZumoMotors::setFrequency(freq,(int)PWM_R);
		ZumoMotors::setFrequency(freq,(int)PWM_L);
		ZumoMotors::setDuty(duty,(int)PWM_R);
		ZumoMotors::setDuty(duty,(int)PWM_L);	
		ZumoMotors::setEnable(enable,(int)PWM_R);
		ZumoMotors::setEnable(enable,(int)PWM_L);
	}

}

void ZumoMotors::setFrequency(int freq,int motor)
{
  execute("echo "+to_string(freq)+" > "+ZumoMotors::PWM_PATH+"freq"+to_string(motor));
}

void ZumoMotors::setEnable(int enable,int motor)
{
  execute("echo "+to_string(enable)+" > "+ZumoMotors::PWM_PATH+"enable"+to_string(motor));
}

void ZumoMotors::setDuty(int duty,int motor)
{
  execute("echo "+to_string(duty)+" > "+ZumoMotors::PWM_PATH+"duty"+to_string(motor));
}

// enable/disable flipping of left motor
void ZumoMotors::flipLeftMotor(bool flip)
{
  ZumoMotors::flipLeft = flip;
}

// enable/disable flipping of right motor
void ZumoMotors::flipRightMotor(bool flip)
{
  ZumoMotors::flipRight = flip;
}

// set speed for left motor; speed is a number between -400 and 400
void ZumoMotors::setLeftSpeed(int speed)
{
	
  init(); // initialize if necessary
    
  bool reverse = false;
  
  if (speed < 0)  {
    speed = -speed; // make speed a positive quantity
    reverse = true;    // preserve the direction
  }
  if (speed > 400) {  // Max 
    	speed = 400;
  }
  int duty=(double)speed * (double)1023.0 / (double)400.0; // default to using analogWrite, mapping 400 to 1023

  ZumoMotors::setDuty(duty,(int)PWM_L);    
  ZumoMotors::setEnable(1,(int)PWM_L);
  if (reverse ||  ZumoMotors::flipLeft){ // flip if speed was negative or flipLeft setting is active, but not both
    digitalWrite(DIR_L, HIGH);
  }else{
    digitalWrite(DIR_L, LOW);
	}	
//*/
}

// set speed for right motor; speed is a number between -400 and 400
void ZumoMotors::setRightSpeed(int speed)
{

  init(); // initialize if necessary
    
  bool reverse = 0;
  
  if (speed < 0){
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 400){  // Max PWM dutycycle
    speed = 400;
  }

  int duty=(double)speed * (double)1023.0 / (double)400.0; // default to using analogWrite, mapping 400 to 1023
  ZumoMotors::setDuty(duty,(int)PWM_R);  
  ZumoMotors::setEnable(1,(int)PWM_R);

  if (reverse ||  ZumoMotors::flipRight) { // flip if speed was negative or flipRight setting is active, but not both
		digitalWrite(DIR_R, HIGH);
  }else{
    	digitalWrite(DIR_R, LOW);
	}
//*/
}

// set speed for both motors
void ZumoMotors::setSpeeds(int leftSpeed, int rightSpeed)
{
  setLeftSpeed(leftSpeed);
  setRightSpeed(rightSpeed);
}

void ZumoMotors::execute(string s){
	int r=system(s.c_str());
}
