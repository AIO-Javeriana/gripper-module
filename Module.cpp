

#include "./libs/json.hpp"
#include <string>
#include <queue>
#include <map>


using json = nlohmann::json;
using namespace std;

class CommandInfo{
    json command;
    bool completed;
    bool error;

  public:
      CommandInfo(json command){
        this->command=command;
        this->completed=false;
        this->error=false;

      }
      void setCommand(json command){
        this->command=command;
      }
      json getCommand(){
        return this->command;
      }
      void completedWork(){
          this->completed=true;
      }
      void errorWork(){
          this->completed=true;
      }
      bool isCompletedTask(){
          return this->completed;
      }
      bool isError(){
          return this->error;
      }
};

class Services{
    map<long,queue<CommandInfo>> commandsInfo;
    bool working;

  public:
      Services(){
          this->working=false;
      }

      bool assessWork(json command){
          if (command["COMMAND"]=="BLINK") {
              this->commandsInfo[command["GROUP_ID"]].push(CommandInfo(command));
              return true;
          }else if (command["COMMAND"]=="DECIR") {
              this->commandsInfo[command["GROUP_ID"]].push(CommandInfo(command));
              return true;
          }
          return false;
      }

      CommandInfo makeWorkFront(long group_id){
            CommandInfo command=this->commandsInfo[group_id].front();
            this->commandsInfo[group_id].pop();
            command.completedWork();
            return command;
      }

      bool isEmptyCommands(long group_id){
            return this->commandsInfo[group_id].empty();
      }

      void setWorking(bool working){
          this->working=working;
      }

      bool isWorking(){
          return this->working;
      }

};
