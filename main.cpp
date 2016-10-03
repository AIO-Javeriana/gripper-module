#include "Module.cpp"
#include "CommunicationChannel.cpp"
#include <string>
#include <queue>
#include <map>

using json = nlohmann::json;
using namespace std;

void start();
int main(int argc ,const char* args[])
{
  start();
	return 0;
}


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
}
