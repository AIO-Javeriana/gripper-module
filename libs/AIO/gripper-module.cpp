#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <memory>
#include <stdexcept>
#include <cstdio>


#define MAX_ADC_bits 1024
#define MAX_ADC_voltage 1.8
using namespace std;

string  PWM_PATH= "sys/devices/platform/pwm-ctrl/";
///sys/devices/platform/pwm-ctrl
//g++ -std=c++11 -o out gripper-module.cpp

void execute(string s){
	system(s.c_str());
}

string exec(const char* cmd){
	char buffer[128];
	string result="";
	//shared_pdr<FILE> pipe(popen(cmd,"r"),pclose);
	FILE *pipe=popen(cmd,"r");	
	if(!pipe) throw runtime_error("popen() failed !");
	while(!feof(pipe)){
		if(fgets(buffer,128,pipe) != NULL){
			result+=buffer;		
		}
	}
	pclose(pipe);
	return result;
}

float getADC(){
		string nn=exec("cat /sys/class/saradc/ch0");
		string::size_type sz;
		float d=(std::stoi(nn,&sz));
		return (d/(float)MAX_ADC_bits)*MAX_ADC_voltage;
}


int main(){
	cout<<"Strared Gripper  Module"<<PWM_PATH<<endl;

	while(true){
		
		cout<<getADC()<<endl;
	}
/*
	cout<<"Strared Gripper Module"<<PWM_PATH<<endl;
	execute("sudo modprobe pwm-meson");
	execute("sudo modprobe pwm-ctrl");
	
	execute("echo 102 > /sys/devices/platform/pwm-ctrl/duty0");
	execute("echo 50 > /sys/devices/platform/pwm-ctrl/freq0");
	execute("echo 1 > /sys/devices/platform/pwm-ctrl/enable0");
	*/
	return 0;
}
