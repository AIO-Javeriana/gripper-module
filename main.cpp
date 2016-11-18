
#include <Module.cpp>
#include <CommunicationChannel.cpp>
#include <string>
#include <queue>
#include <map>
//-----------------------
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>


#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <LSM303.h>

//using json = nlohmann::json;
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
#define CALIBRATION_SPEED 150
#define THRESHOLD 800
void testADC(){

	int data;
	while(1){
	data=analogRead(PORT_ADC1);
		cout<<" DATA "<<data<<endl;
		
	}


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

void followLine(int position ,int &lastError,int &integral)
{
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



void move(ZumoReflectanceSensorArray &reflectanceSensors,int &lastError,int &integral,bool &isIntersection,int &direction)
{
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

void automaticSensorCalibration(ZumoReflectanceSensorArray &reflectanceSensors){
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
  for(i = 0; i < 2; i++)
  {
	startTime = millis();
 	ZumoMotors::setSpeeds(-CALIBRATION_SPEED , CALIBRATION_SPEED );	
	while(millis() - startTime < timeLimit)   // make the calibration take 10 seconds
	  {
		reflectanceSensors.calibrate();
		//delay(10);
	  }
        ZumoMotors::setSpeeds(0, 0);
        startTime = millis();
        ZumoMotors::setSpeeds(CALIBRATION_SPEED , -CALIBRATION_SPEED );
	while(millis() - startTime < timeLimit)   // make the calibration take 10 seconds
	  {
		reflectanceSensors.calibrate();
	  }
	ZumoMotors::setSpeeds(0, 0);
	startTime = millis();
 	ZumoMotors::setSpeeds(CALIBRATION_SPEED , -CALIBRATION_SPEED );	
	while(millis() - startTime < timeLimit)   // make the calibration take 10 seconds
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
}


void manualSensorCalibration(ZumoReflectanceSensorArray &reflectanceSensors){
 
  reflectanceSensors.init();
  unsigned long startTime = millis();
  while(millis() - startTime < 10000)   // make the calibration take 10 seconds
  {
    reflectanceSensors.calibrate();
  }
}



void setup(ZumoReflectanceSensorArray &reflectanceSensors)
{
  // Play a little welcome song
  //buzzer.play(">g32>>c32");
  ZumoMotors::init();

  // Turn on LED to indicate we are in calibration mode
  pinMode (LED, OUTPUT) ;
  digitalWrite (LED, HIGH) ;	// On
 
  //manualSensorCalibration(reflectanceSensors);
  automaticSensorCalibration(reflectanceSensors);
  // Turn off LED to indicate we are through with calibration
  digitalWrite (LED, LOW) ;	// Off
  //buzzer.play(">g32>>c32");

  // Wait for the user button to be pressed and released
  //button.waitForButton();

  // Play music and wait for it to finish before we start driving.
  //buzzer.play("L16 cdegreg4");
  //while(buzzer.isPlaying());
  int lastError = 0,integral=0,direction=0,opc;
  bool isIntersection=false;
  do{
	  cout<<" 0 follow line "<<endl;
	  cout<<" 1 follow line turn right"<<endl;
	  cout<<" 2 follow line turn left"<<endl;
	  cout<<" 3 exit "<<endl;
	  cin>>direction;
      isIntersection=false;
	  if(direction==3){
		isIntersection=true;
	  }	
	  while(!isIntersection){
	  	move(reflectanceSensors,lastError,integral,isIntersection,direction);
	  }
	  ZumoMotors::setSpeeds(0,0 );
  	  ZumoMotors::init(); 
    
    }while(direction!=3);
}

void testReflectanceSensorArray(ZumoReflectanceSensorArray reflectanceSensors){

  //pinMode (LED, OUTPUT) ;
	ZumoMotors::init(); 
	ZumoMotors::setSpeeds(0,0 );
  
  unsigned int sensorValues[NUM_SENSORS];
  reflectanceSensors.init();
  //digitalWrite (LED, HIGH) ;	// On
  unsigned long startTime = millis();
  
 while(millis() - startTime < 10000)   // make the calibration take 10 seconds
  {
    reflectanceSensors.calibrate();
  }
  //digitalWrite (LED, LOW) ;	// Off

//*/

  for (;;){
   // delay (500) ;		// mS
   // delay (500) ;
		unsigned int position = reflectanceSensors.readLine(sensorValues);
		// To get raw sensor values instead, call:  
		//reflectanceSensors.read(sensorValues);
		cout<<"New Values position "<<position<<endl;
		for (int i = 0; i < NUM_SENSORS; i++){
			cout<<sensorValues[i]<<endl;
		}
	}

}

void test(){

  wiringPiSetup () ;
  printf ("ZumoReflectanceSensorArray\n") ;
  ZumoReflectanceSensorArray reflectanceSensors;
  ZumoMotors::init(); 
  testReflectanceSensorArray(reflectanceSensors);
  //delay(2000); 
  //ZumoMotors::setSpeeds(100, 100);
  //setup(reflectanceSensors);
  ZumoMotors::setSpeeds(0,0 );
  ZumoMotors::init(); 
  
 
}


void start();
int main(int argc ,const char* args[])
{
	cout<<"Strated Module Moblitity"<<endl;
	test();
	//hoint16_t a;
  //start();
	return 0;
}

/*
void start(){
  string module_id="mobility_module";
  json module_info={
    {"MODULE_ID",module_id},
    {"COMMANDS",json::array({
          {
            {"COMMAND","BLINK"},
            {"PARAMS",json::array({""})},
            {"INTERRUPTIBLE",false},
            {"SERVICE",false}

          },
          {
            {"COMMAND","DECIR"},
            {"PARAMS",json::array({"TEXTO","TONO"})},
            {"INTERRUPTIBLE",false},
            {"SERVICE",false}
          }
        })
    }
  };
  Services services;
  CommunicationChannel st("ws://localhost",9090,services);
  st.start();
  string info=module_info.dump();
  st.subscription(info);
  HIGHLIGHT("Start to Mobility Module\n'$exit' : Shut Down Module\n");
  for (std::string line; std::getline(std::cin, line);) {
      if(line.length()>0){
          if(line == "$exit"){
              break;
          }
      st.getLock()->lock();
      st.getLock()->unlock();
      }
  }
  HIGHLIGHT("Closing...");
  st.endConnection();
}*/
