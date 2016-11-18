//#include "ZumoMotors.h"

#define PWM_L 1
#define PWM_R 0
#define DIR_L 6
#define DIR_R 10
#define DUTY_CYCLE_MAX 1023
#define DUTY_CYCLE_MIN 0
#define Kp 5
#define Ki 0
#define Kd 0
#define ENCODER_R 22
#define ENCODER_L 21
#define CALIBRATION_TIME 5000
#define SPEED_MAX 400
#define SPEED_MIN 0
#define TIME_WINDOW 100
#define PIN_STROBE 24


s



// constructor (doesn't do anything)
ZumoMotors::ZumoMotors(void)
{
	init();
	/*
	pinMode(PWM_L,  OUTPUT);
	pinMode(PWM_R,  OUTPUT);
	pinMode(DIR_L, OUTPUT);
	pinMode(DIR_R, OUTPUT);
	*/

}

void ZumoMotors::init(){
	if (!ZumoMotors::wasInit){
		ZumoMotors::wasInit=true;
		ZumoMotors::execute("sudo modprobe pwm-meson npwm=2");
		ZumoMotors::execute("sudo modprobe pwm-ctrl");	
		pinMode (DIR_R, OUTPUT) ;
		pinMode (DIR_L, OUTPUT) ;
		pinMode (PIN_STROBE, OUTPUT) ;
		digitalWrite (DIR_R, LOW) ;	// LOW - FORWARD / HIGH -BACKWARD
		digitalWrite (DIR_L, LOW);	// LOW - FORWARD / HIGH -BACKWARD
		digitalWrite (PIN_STROBE, LOW);	
		int freq=20000;
		int duty=500;
		int enable=0;
		ZumoMotors::setFrequency(freq,(int)PWM_R);
		ZumoMotors::setFrequency(freq,(int)PWM_L);
		ZumoMotors::setDuty(duty,(int)PWM_R);
		ZumoMotors::setDuty(duty,(int)PWM_L);	
		ZumoMotors::setEnable(enable,(int)PWM_R);
		ZumoMotors::setEnable(enable,(int)PWM_L);
        // Launch velocity control threads
        //ZumoMotors::mobility_thread = new thread(ZumoMotors::mobilityThreadFunction);
		ZumoMotors::left_velocity_thread = new thread(ZumoMotors::leftThread);
     	ZumoMotors::right_velocity_thread=new thread(ZumoMotors::rightThread);
	}

}


void ZumoMotors::setFrequency(int freq,int motor)
{
  execute("echo "+to_string(freq)+" > "+ZumoMotors::PWM_PATH+"freq"+to_string(motor));
}

void ZumoMotors::setEnable(int enable,int motor)
{  
	execute("echo "+to_string(1)+" > "+ZumoMotors::PWM_PATH+"enable"+to_string(motor));
	bool enable_=false;	
	if (enable == 1){	
		ZumoMotors::setDuty(DUTY_CYCLE_MAX ,(int)motor);
		enable_=true;
	}else{
		ZumoMotors::setDuty(DUTY_CYCLE_MIN,(int)motor);
	}
	
	ZumoMotors::m.lock();
		switch(motor){
			case(PWM_R): ZumoMotors::enableRight = enable_; break;
			case(PWM_L): ZumoMotors::enableLeft = enable_; break;
		}
	ZumoMotors::m.unlock();
	
}	

void ZumoMotors::setDuty(int duty,int motor)
{
  execute("echo "+to_string(DUTY_CYCLE_MAX-duty)+" > "+ZumoMotors::PWM_PATH+"duty"+to_string(motor));
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

  ZumoMotors::setEnable(1,(int)PWM_L);
  ZumoMotors::m.lock();
  ZumoMotors::left_velocity=speed ;
  ZumoMotors::m.unlock();


  //int duty=(double)speed * (double)1023.0 / (double)400.0; // default to using analogWrite, mapping 400 to 1023
  //ZumoMotors::setDuty(duty,(int)PWM_L);    
  //ZumoMotors::setEnable(1,(int)PWM_L);

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
	ZumoMotors::setEnable(1,(int)PWM_R);
	ZumoMotors::m.lock();
	ZumoMotors::right_velocity=speed;
	ZumoMotors::m.unlock();
  //int duty=(double)speed * (double)1023.0 / (double)400.0; // default to using analogWrite, mapping 400 to 1023
  //ZumoMotors::setDuty(duty,(int)PWM_R);  
  //ZumoMotors::setEnable(1,(int)PWM_R);

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


// Velocity control

void ZumoMotors::mobilityThreadFunction(){
	while(true){	
			ZumoMotors::leftThread();	
			ZumoMotors::rightThread();	
	}
}

void ZumoMotors::leftThread(){
		while(true){
				if(ZumoMotors::calibrateLeft){
		cout << "Statrt izq " << endl;
					ZumoMotors::maxPulsesLeft = ZumoMotors::calibrateMotor(PWM_L, ENCODER_L,CALIBRATION_TIME , TIME_WINDOW);
		cout << "pulsos izq: " << ZumoMotors::maxPulsesLeft << endl;
					ZumoMotors::calibrateLeft = false;
				}else if (ZumoMotors::enableLeft){
						 ZumoMotors::velocityController(PWM_L,ENCODER_L,left_velocity, ZumoMotors::maxPulsesLeft,  ZumoMotors::duty_cycle_left, TIME_WINDOW, ZumoMotors::last_error_left, ZumoMotors::integral_left);
				}
		}
}

void ZumoMotors::rightThread(){
    	while(true){
				if(ZumoMotors::calibrateRight){
					ZumoMotors::maxPulsesRight = ZumoMotors::calibrateMotor(PWM_R, ENCODER_R,CALIBRATION_TIME , TIME_WINDOW);
		cout << "pulsos derecha: " << ZumoMotors::maxPulsesRight << endl;
					ZumoMotors::calibrateRight = false;
				}else if (ZumoMotors::enableRight){
						 ZumoMotors::velocityController(PWM_R,ENCODER_R,right_velocity, ZumoMotors::maxPulsesRight,  ZumoMotors::duty_cycle_right, TIME_WINDOW, ZumoMotors::last_error_right, ZumoMotors::integral_right);
	//cout<<ZumoMotors::duty_cycle_right<<" "<<ZumoMotors::last_error_right<<endl;
				}
		}
}


void ZumoMotors::velocityController(int motor, int encoder,int referenceVelocity, int maxPulses,  int &duty_cycle, int time_window, float &lastError, float &integral){
	int reference = getPulseFromSpeed(referenceVelocity, maxPulses);
	//Contando el numero de pulsos
	int pulses = getPulsesEach(time_window, encoder);
	//calculo del error
	float error = reference - pulses;
	//Controlando los dos pwm
	duty_cycle += error*Kp+Kd*(error - lastError)+Ki*(integral+error);
	cout<<motor<<" "<<(error*Kp+Kd*(error - lastError)+Ki*(integral+error))<<" "<<reference<<" "<<error<<endl;
	integral=integral+error;
	lastError=error;
	if(duty_cycle>DUTY_CYCLE_MAX ){
		duty_cycle=DUTY_CYCLE_MAX;	
	}else if(duty_cycle<DUTY_CYCLE_MIN ){
		duty_cycle=DUTY_CYCLE_MIN;	
	}
	ZumoMotors::setDuty(duty_cycle,motor);
	//delay(100);
}

/**
*	Calibrates motors getting the pulses when pwm is at 100% of duty cicle
*	and when pwm is at 1% of duty cicle.
*	@returns cp max and min values of pulses.
	calibration_time >>> time_window
*/
int ZumoMotors::calibrateMotor(int motor, int encoder,int calibration_time, int time_window){
	int enable = 1;
	int count = 0;
	int total_pulses = 0;
	unsigned long calibrationStartTime;

	ZumoMotors::setEnable(enable,motor);
	ZumoMotors::setDuty( DUTY_CYCLE_MAX ,motor);

	calibrationStartTime = millis();
	while( millis() - calibrationStartTime < CALIBRATION_TIME ){
		count++;
		total_pulses += getPulsesEach(time_window, encoder);
	}
	enable=0;
	ZumoMotors::setEnable(enable,motor);
	cout<<total_pulses<<"  "<<count<<" "<<total_pulses/count<<endl;
	return total_pulses/count;
}

/**
*	counts pulses every "time" amount of time measured in miliSeconds.
*	@param time time window to count pulses.
*/
int ZumoMotors::getPulsesEach(int time, int encoder){
	int signal_edge = 0;
	int pulses = 0;
	
	int encoderValue = digitalRead(encoder);
	int savedEncoderValue = encoderValue;
	unsigned long startTime = millis();
	while(millis() - startTime < time){
		encoderValue = digitalRead(encoder);
		//cout<<encoderValue<<endl;
		if( encoderValue != savedEncoderValue ){
			savedEncoderValue = encoderValue;
			signal_edge++;
		}
		if(signal_edge == 2){
			signal_edge = 0;
			pulses++;
		}
	}
	return pulses;
}

/**
*	Checks duty cicle values, preventing the pwm getting out of bounds.
*	@dutyCicle value to be checked.
*/
void ZumoMotors::controlDutyCicleValues(int &dutyCicle){
	if(dutyCicle > 1000)
		dutyCicle = 1000;
	if(dutyCicle < 0)
		dutyCicle = 0;
	dutyCicle = dutyCicle;
}

/**
*	Gets a pulse number according to a given speed.
*	@param speed value of speed. minSpeed < speed < maxSpeed
*	@param cp max and min of pulses obtained from the calibration.
*/
int ZumoMotors::getPulseFromSpeed(int speed, int maxPulses){
	if(speed > SPEED_MAX)
		return maxPulses;
	else if(speed < SPEED_MIN)
		return 2;
	else
		return (float)speed*((float)maxPulses/(float)SPEED_MAX);
}


    
void ZumoMotors::enableCalibration(){
	ZumoMotors::enableCalibrationLeft();    
	ZumoMotors::enableCalibrationRight(); 	
}    
void ZumoMotors::enableCalibrationLeft(){
  ZumoMotors::m.lock();
  ZumoMotors::calibrateLeft=true ;
  ZumoMotors::m.unlock();
}   
void ZumoMotors::enableCalibrationRight(){
 ZumoMotors::m.lock();
  ZumoMotors::calibrateRight=true ;
  ZumoMotors::m.unlock();

}


bool ZumoMotors::calibrationFinished(){
	bool f;
	ZumoMotors::m.lock();
	f=(!ZumoMotors::calibrateLeft&&!ZumoMotors::calibrateRight);
  	ZumoMotors::m.unlock();
	return f;
}

