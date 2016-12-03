#ifndef BATTERYSERVICE_H
#define BATTERYSERVICE_H

#include <Services.cpp>
#ifdef WIN32

#define HIGHLIGHT(__O__) std::cout<<__O__<<std::endl
#define EM(__O__) std::cout<<__O__<<std::endl
#include <stdio.h>
#include <tchar.h>

#else

#define HIGHLIGHT(__O__) std::cout<<"\e[1;31m"<<__O__<<"\e[0m"<<std::endl
#define EM(__O__) std::cout<<"\e[1;30;1m"<<__O__<<"\e[0m"<<std::endl

#endif

#define PORT_ADC1 1
using namespace std;


class BatterySensorService: public SensorService{

public:
    
	static string service_name;

    BatterySensorService(string module_ID):SensorService(module_ID){
		pinMode(PORT_ADC1, INPUT) ;
		this->name = BatterySensorService::service_name = "SENSOR_SERVICE";
		this->execute_function = &execute;
		cout << " BATTERY SENSOR SERVICE STARTED " << endl;
	}
    
    void static execute(){
		int data;
		cout << " BATTERY SENSOR SERVICE RUNNING ";
		string module_ID = SensorService::module_ID;
        while(1){
			data=analogRead(PORT_ADC1);
			json data_JSON = {"BATTERY_LVL",data};
			json sensor_data={
                            {"MODULE_ID",module_ID},
                            {"EVENT_NAME", toString(CommunicationEvents::SENSOR_SERVICE)},
                            {"SENSOR-SERVICE-NAME",BatterySensorService::service_name},
                            {"DATA", data_JSON}
                };		
			SensorService::_lock->lock();
				SensorService::current_socket->emit(toString(CommunicationEvents::SENSOR_SERVICE), sensor_data.dump() );			
            SensorService::_lock->unlock();

			delay(1000);
		}
    }
};

string BatterySensorService::service_name = "";

#endif
