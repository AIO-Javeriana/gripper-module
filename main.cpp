#include <Module.cpp>
#include <CommunicationChannel.cpp>
#include <string>
#include <queue>
#include <map>
//-----------------------
#include <stdio.h>
#include <wiringPi.h>


#include <ZumoReflectanceSensorArray.h>

//using json = nlohmann::json;
using namespace std;


string  PWM_PATH= "/sys/devices/platform/pwm-ctrl/";
#define	LED	24
#define NUM_SENSORS 6
#define PORT_ADC1 0
#define M1DIR 3
#define M2DIR 21
void testADC(){

	int data;
	while(1){
	data=analogRead(PORT_ADC1);
		cout<<" DATA "<<data<<endl;
		
	}


}


void execute(string s){
	int r=system(s.c_str());
}

void wheelsInitialization (){
	pinMode (M1DIR, OUTPUT) ;
	pinMode (M2DIR, OUTPUT) ;
	digitalWrite (M1DIR, LOW) ;	// On
	digitalWrite (M2DIR, LOW) ;	// On
	cout<<"Strared Gripper Module"<<PWM_PATH<<endl;
	execute("sudo modprobe pwm-meson npwm=2");
	execute("sudo modprobe pwm-ctrl");
	int freq=20000;
	int duty=800;
	int enbale=0;
	execute("echo "+to_string(duty)+" > "+PWM_PATH+"duty"+to_string(0));
	execute("echo "+to_string(freq)+" > "+PWM_PATH+"freq"+to_string(0));
	execute("echo "+to_string(enbale)+" > "+PWM_PATH+"enable"+to_string(0));
	execute("echo "+to_string(duty)+" > "+PWM_PATH+"duty"+to_string(1));
	execute("echo "+to_string(freq)+" > "+PWM_PATH+"freq"+to_string(1));
	execute("echo "+to_string(enbale)+" > "+PWM_PATH+"enable"+to_string(1));
	
}

void test(){

  printf ("ZumoReflectanceSensorArray\n") ;
	
  wiringPiSetup () ;
  wheelsInitialization ();
  pinMode (LED, OUTPUT) ;
  ZumoReflectanceSensorArray reflectanceSensors;
  unsigned int sensorValues[NUM_SENSORS];
  reflectanceSensors.init();
  digitalWrite (LED, HIGH) ;	// On
  unsigned long startTime = millis();
  while(millis() - startTime < 10000)   // make the calibration take 10 seconds
  {
    reflectanceSensors.calibrate();
  }
  digitalWrite (LED, LOW) ;	// Off



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
