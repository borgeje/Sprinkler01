
void receive(const MyMessage &message) {
  // We only expect one type of message from controller. But we better check anyway.
  if (message.isAck()) {
     //DEBUG_PRINTLN("This is an ack from gateway");
  }
  Rstate = message.getBool();      // Change relay state 


  
  if (message.type == V_LIGHT && message.sensor == CHILD_ID_Zone_1) {
       DEBUG_PRINTLN("Drive Relay on Zone 1");
       if (Rstate)
            {
                  MainMenu_Current_State=1;
                  DOWNbuttonPushed=false;
                  ENTERbuttonPushed=true;
                  state=STAND_BY_ALL_OFF;
            } else
                {
                  MainMenu_Current_State=0;
                  DOWNbuttonPushed=false;
                  ENTERbuttonPushed=false;
                  state=STAND_BY_ALL_OFF;
                  MainMenu_Current_State=0;
                  Zone1_Request=0;
                  fastClear();
                  MainMenu(MainMenu_Current_State);
                }

       // Write some debug info
       DEBUG_PRINT("                  ........ Incoming Change for Sensor: ");                  // "Incoming change for sensor:"
       DEBUG_PRINT(message.sensor);
       DEBUG_PRINT(NS);
       DEBUG_PRINT(Rstate);

   } 
   else if (message.type == V_LIGHT && message.sensor == CHILD_ID_Zone_2 ) {
       DEBUG_PRINTLN("Drive Zone 2");
       if (Rstate)
            {
                  MainMenu_Current_State=2;
                  DOWNbuttonPushed=false;
                  ENTERbuttonPushed=true;
                  state=STAND_BY_ALL_OFF;
            } else
                {
                  MainMenu_Current_State=0;
                  DOWNbuttonPushed=false;
                  ENTERbuttonPushed=false;
                  state=STAND_BY_ALL_OFF;
                  MainMenu_Current_State=0;
                  Zone2_Request=0;
                  fastClear();
                  MainMenu(MainMenu_Current_State);
                } 
 
       // Write some debug info
       DEBUG_PRINT("                  ........ Incoming Change for Sensor: ");  // "Incoming change for sensor:"
       DEBUG_PRINT(message.sensor);
       DEBUG_PRINT(NS);
       DEBUG_PRINTLN(Rstate);
        }     
      else if (message.type == V_LIGHT && message.sensor == CHILD_ID_Zone_3 ) {
       DEBUG_PRINTLN("Drive Zone 3");
       if (Rstate)
            {
                  MainMenu_Current_State=3;
                  DOWNbuttonPushed=false;
                  ENTERbuttonPushed=true;
                  state=STAND_BY_ALL_OFF;
            } else
                {
                  MainMenu_Current_State=0;
                  DOWNbuttonPushed=false;
                  ENTERbuttonPushed=false;
                  state=STAND_BY_ALL_OFF;
                  MainMenu_Current_State=0;
                  Zone3_Request=0;
                  fastClear();
                  MainMenu(MainMenu_Current_State);
                } 
 
       // Write some debug info
       DEBUG_PRINT(ICS); //"Incoming change for sensor:"
       DEBUG_PRINT(message.sensor);
       DEBUG_PRINT(NS);
       DEBUG_PRINTLN(Rstate);
        }  

        else if (message.type == V_LIGHT && message.sensor == CHILD_ID_SPRINKLER ) 
            {
                DEBUG_PRINTLN("AUTOZONE ********");
                DEBUG_PRINTLN("AUTOZONE ********");
                DEBUG_PRINTLN("AUTOZONE ********");
                if (Rstate)
                    {
                        MainMenu_Current_State=0;
                        DOWNbuttonPushed=false;
                        ENTERbuttonPushed=true;
                        state=STAND_BY_ALL_OFF;
                    } else
                          {
                          MainMenu_Current_State=0;
                          DOWNbuttonPushed=false;
                          ENTERbuttonPushed=false;
                          state=STAND_BY_ALL_OFF;
                          MainMenu_Current_State=0;
                          Zone1_Request=0;
                          Zone2_Request=0;
                          Zone3_Request=0;
                          fastClear();
                          MainMenu(MainMenu_Current_State);
                          } 
 
       // Write some debug info
       DEBUG_PRINT("Incoming change for sensor:");
       DEBUG_PRINT(message.sensor);
       DEBUG_PRINT(", New status: ");
       DEBUG_PRINTLN(state);
        }
 

}






void receiveTime(time_t newTime)
{
  DEBUG_PRINTLN(F("Time value received and updated..."));
  int lastSecond = second();
  int lastMinute = minute();
  int lastHour = hour();
  setTime(newTime);
 // if (((second() != lastSecond) || (minute() != lastMinute) || (hour() != lastHour)) || showTime)
//  {
    DEBUG_PRINTLN(F("Clock updated...."));
    DEBUG_PRINT(F("Sensor's time currently set to:"));
    DEBUG_PRINT(hourFormat12() < 10 ? F(" 0") : F(" "));
    DEBUG_PRINT(hourFormat12());
    DEBUG_PRINT(minute() < 10 ? F(":0") : F(":"));
    DEBUG_PRINT(minute());
    DEBUG_PRINTLN(isAM() ? F("am") : F("pm"));
    DEBUG_PRINT(month());
    DEBUG_PRINT(F("/"));
    DEBUG_PRINT(day());
    DEBUG_PRINT(F("/"));
    DEBUG_PRINTLN(year());
//    DEBUG_PRINTLN(dayOfWeek[weekday()]);
 //   showTime = false;
  //}
  //else
  //{
    DEBUG_PRINTLN(F("Sensor's time did NOT need adjustment greater than 1 second."));
 // }
//  clockUpdating = false;
}



