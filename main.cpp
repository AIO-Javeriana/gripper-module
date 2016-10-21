#include <Module.cpp>
#include <CommunicationChannel.cpp>
#include <string>
#include <queue>
#include <map>
//-----------------------
#include <stdio.h>
#include <wiringPi.h>


#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>

//using json = nlohmann::json;
using namespace std;

#define	LED	24
#define NUM_SENSORS 5
#define PORT_ADC1 0
#define M1DIR 3
#define M2DIR 21
// This is the maximum speed the motors will be allowed to turn.
// (400 lets the motors go at top speed; decrease to impose a speed limit)
#define MAX_SPEED  150
void testADC(){

	int data;
	while(1){
	data=analogRead(PORT_ADC1);
		cout<<" DATA "<<data<<endl;
		
	}


}


int loop(ZumoReflectanceSensorArray reflectanceSensors,int lastError)
{
  unsigned int sensorValues[NUM_SENSORS];
  // Get the position of the line.  Note that we *must* provide the "sensors"
  // argument to readLine() here, even though we are not interested in the
  // individual sensor readings
  int position = reflectanceSensors.readLine(sensorValues);

  // Our "error" is how far we are away from the center of the line, which
  // corresponds to position 2500.
  int error = position - 2500;

  // Get motor speed difference using proportional and derivative PID terms
  // (the integral term is generally not very useful for line following).
  // Here we are using a proportional constant of 1/4 and a derivative
  // constant of 6, which should work decently for many Zumo motor choices.
  // You probably want to use trial and error to tune these constants for
  // your particular Zumo and line course.
  int speedDifference = error / 4 + 6 * (error - lastError);

  lastError = error;

  // Get individual motor speeds.  The sign of speedDifference
  // determines if the robot turns left or right.
  int m1Speed = MAX_SPEED + speedDifference;
  int m2Speed = MAX_SPEED - speedDifference;

  // Here we constrain our motor speeds to be between 0 and MAX_SPEED.
  // Generally speaking, one motor will always be turning at MAX_SPEED
  // and the other will be at MAX_SPEED-|speedDifference| if that is positive,
  // else it will be stationary.  For some applications, you might want to
  // allow the motor speed to go negative so that it can spin in reverse.
  if (m1Speed < 0)
    m1Speed = 0;
  if (m2Speed < 0)
    m2Speed = 0;
  if (m1Speed > MAX_SPEED)
    m1Speed = MAX_SPEED;
  if (m2Speed > MAX_SPEED)
    m2Speed = MAX_SPEED;

  ZumoMotors::setSpeeds(m1Speed, m2Speed);
  return lastError;
}



void setup(ZumoReflectanceSensorArray reflectanceSensors)
{
  // Play a little welcome song
  //buzzer.play(">g32>>c32");
  ZumoMotors::init();

  // Turn on LED to indicate we are in calibration mode
  pinMode (LED, OUTPUT) ;
  digitalWrite (LED, HIGH) ;	// On
  // Initialize the reflectance sensors module
  reflectanceSensors.init();

  // Wait for the user button to be pressed and released
  //button.waitForButton();

  

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      ZumoMotors::setSpeeds(-200, 200);
    else
      ZumoMotors::setSpeeds(200, -200);
    reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  ZumoMotors::setSpeeds(0,0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite (LED, LOW) ;	// Off
  //buzzer.play(">g32>>c32");

  // Wait for the user button to be pressed and released
  //button.waitForButton();

  // Play music and wait for it to finish before we start driving.
  //buzzer.play("L16 cdegreg4");
  //while(buzzer.isPlaying());
  int lastError = 0;
  while(1){
  	lastError=loop(reflectanceSensors,lastError);
  }
}


void test(){

  wiringPiSetup () ;
  printf ("ZumoReflectanceSensorArray\n") ;
  ZumoReflectanceSensorArray reflectanceSensors;
  ZumoMotors::init(); 
  delay(2000); 
  ZumoMotors::setSpeeds(100, 100);
  //setup(reflectanceSensors);

}


void start();
int main(int argc ,const char* args[])
{
	cout<<"Strated Module Moblitity"<<endl;
	test();
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
