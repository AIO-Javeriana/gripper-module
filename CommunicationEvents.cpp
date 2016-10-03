

enum CommunicationEvents
{
    REGISTRATION,
    REGISTRATION_REPLY,
    WORK_ASSIGNATION,
    WORK_ASSIGNATION_REPLY,
    WORK_STATUS,
    WORK_STATUS_REPLY,
    ALL_BEGINS,
    ACTION_FINISHED,
    GET_SENSOR_SERVICES,
    SUBSCRIPTION_SENSOR_SERVICE,
    SENSOR_SERVICE,
    EMOTIONAL_EVENT,
    PARTICIPATION_EVENT,
    SUBSCRIPTION_SENSOR_SERVICE_REPLY,
    NECESSITY_FOR_MODULE,
    FINISHED_EXECUTE_COMMAND,
    SENSOR_SERVICE_REPLY,
    GET_SENSOR_SERVICES_REPLY,
    COMMANDS_ASSIGNATION,
    NOT_DEFINED
};

std::ostream& operator<<(std::ostream& os, CommunicationEvents c)
{
    switch(c)
    {
      case REGISTRATION: os << ("REGISTRATION");    break;
      case REGISTRATION_REPLY: os <<("REGISTRATION_REPLY");    break;
      case WORK_ASSIGNATION: os <<("WORK_ASSIGNATION");    break;
      case WORK_ASSIGNATION_REPLY: os <<("WORK_ASSIGNATION-REPLY");    break;
      case WORK_STATUS: os <<("WORK_STATUS");    break;
      case WORK_STATUS_REPLY: os <<("WORK_STATUS-REPLY");    break;
      case ALL_BEGINS: os <<("ALL_BEGINS");    break;
      case ACTION_FINISHED: os <<("ACTION_FINISHED");    break;
      case GET_SENSOR_SERVICES: os <<("GET_SENSOR-SERVICES");    break;
      case SUBSCRIPTION_SENSOR_SERVICE: os <<("SUBSCRIPTION_SENSOR-SERVICE");    break;
      case SENSOR_SERVICE: os <<("SENSOR_SERVICE");    break;
      case EMOTIONAL_EVENT: os <<("EMOTIONAL_EVENT");    break;
      case PARTICIPATION_EVENT: os <<("PARTICIPATION_EVENT");    break;
      case SUBSCRIPTION_SENSOR_SERVICE_REPLY: os <<("SUBSCRIPTION_SENSOR-SERVICE-REPLY");    break;
      case NECESSITY_FOR_MODULE: os <<("NECESSITY_FOR-MODULE");    break;
      case FINISHED_EXECUTE_COMMAND: os <<("FINISHED_EXECUTE_COMMAND");    break;
      case SENSOR_SERVICE_REPLY: os <<("SENSOR_SERVICE_REPLY");    break;
      case GET_SENSOR_SERVICES_REPLY: os <<("GET_SENSOR_SERVICES-REPLY");    break;
      case COMMANDS_ASSIGNATION: os <<("COMMANDS_ASSIGNATION");    break;
      default : os << "NOT_DEFINED";  break;
    }
    return os;
}

string toString(CommunicationEvents c)
{
    switch(c)
    {
      case REGISTRATION: return ("REGISTRATION");
      case REGISTRATION_REPLY: return("REGISTRATION_REPLY");
      case WORK_ASSIGNATION:  return("WORK_ASSIGNATION");
      case WORK_ASSIGNATION_REPLY: return ("WORK_ASSIGNATION-REPLY");
      case WORK_STATUS: return ("WORK_STATUS");
      case WORK_STATUS_REPLY: return ("WORK_STATUS-REPLY");
      case ALL_BEGINS: return ("ALL_BEGINS");
      case ACTION_FINISHED: return ("ACTION_FINISHED");
      case GET_SENSOR_SERVICES: return ("GET_SENSOR-SERVICES");
      case SUBSCRIPTION_SENSOR_SERVICE: return ("SUBSCRIPTION_SENSOR-SERVICE");
      case SENSOR_SERVICE: return ("SENSOR_SERVICE");
      case EMOTIONAL_EVENT: return ("EMOTIONAL_EVENT");
      case PARTICIPATION_EVENT: return ("PARTICIPATION_EVENT");
      case SUBSCRIPTION_SENSOR_SERVICE_REPLY: return ("SUBSCRIPTION_SENSOR-SERVICE-REPLY");
      case NECESSITY_FOR_MODULE: return ("NECESSITY_FOR-MODULE");
      case FINISHED_EXECUTE_COMMAND:  return ("FINISHED_EXECUTE_COMMAND");
      case SENSOR_SERVICE_REPLY: return ("SENSOR_SERVICE_REPLY");
      case GET_SENSOR_SERVICES_REPLY: return ("GET_SENSOR_SERVICES-REPLY");
      case COMMANDS_ASSIGNATION: return ("COMMANDS_ASSIGNATION");
      default : return  "NOT_DEFINED";
    }
    return "";
}
