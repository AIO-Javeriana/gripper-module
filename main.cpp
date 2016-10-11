
#include <string>
#include <queue>
#include <map>
//-----------------------
#include <stdio.h>
#include <MobilityModule.cpp>
#include <BlinkService.cpp>
#include <TellService.cpp>
#include <MoveService.cpp>
//#include <wiringPi.h>


//#include <ZumoReflectanceSensorArray.h>

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
  TellService* tellService = new TellService("DECIR", params, false, false);
  mobilityModule->addService(tellService);
  MoveService* moveService = new MoveService("MOVER", params, false, false);
  mobilityModule->addService(moveService);
  mobilityModule->start();
  while(true);
}
