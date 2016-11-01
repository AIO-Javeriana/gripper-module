
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
  MobilityModuleInfo moduleInfo(module_id);
  MobilityModule* mobilityModule = new MobilityModule("ws://localhost", 9090,&moduleInfo);
  mobilityModule->start();
  while(true);
}
