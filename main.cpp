
#include <string>
#include <queue>
#include <map>
//-----------------------
#include <stdio.h>
#include <MobilityModule.cpp>
#include <BlinkService.cpp>

//#include <wiringPi.h>


//#include <ZumoReflectanceSensorArray.h>

//using json = nlohmann::json;
using namespace std;

void start();

int main(int argc ,const char* args[])
{
  start();
	return 0;
}


void start(){
  string module_id="mobility_module";
  MobilityModule* mobilityModule = new MobilityModule(module_id, "ws://localhost", 9090);
  list<string> params;
  BlinkService* service = new BlinkService("BLINK", params, false, false);
  mobilityModule->addService(service);
  mobilityModule->start();
  while(true);
}
