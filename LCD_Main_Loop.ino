void LCDMainLoop()
{
   switch (state) {
    case STAND_BY_ALL_OFF:
              //        TURN ALL RELAYS OFF!!*******************************************************
        if (DOWNbuttonPushed)
        {
           if (MainMenu_Current_State == 4)  MainMenu_Current_State = 0;
              else MainMenu_Current_State++;
           MainMenu(MainMenu_Current_State);
           DOWNbuttonPushed = false;
         }
        else if (ENTERbuttonPushed)
          {
            if (MainMenu_Current_State==0)
              {
                    state=RUN_ALL_ZONES;
                    fastClear(); 
                    startMillis=millis();
                    ENTERbuttonPushed=false;
                    Run_All_Zones_Menu();
                    RunValve=1;
                    Zone1_Request=true;
                    Zone2_Request=false;
                    Zone3_Request=false;
              }
              else if(MainMenu_Current_State==1)             
                {
                    state=RUN_SINGLE_ZONE;
                    RunValve=1;
                    fastClear(); 
                    SingleZoneScreenManagement(RunValve);
                    timeRemaining=SINGLEVALVETIME;
                    startMillis=millis();
                    ENTERbuttonPushed=false;
                    Zone1_Request=true;
                    Zone2_Request=false;
                    Zone3_Request=false;
                }
                else if(MainMenu_Current_State==2)
                    {
                      state=RUN_SINGLE_ZONE;
                      RunValve=2;
                      fastClear();
                      SingleZoneScreenManagement(RunValve);
                      timeRemaining=SINGLEVALVETIME;
                      startMillis=millis();
                      ENTERbuttonPushed=false;
                      Zone1_Request=false;
                      Zone2_Request=true;
                      Zone3_Request=false;
                      
                    }
                  else if(MainMenu_Current_State==3)
                      {
                        state=RUN_SINGLE_ZONE;
                        RunValve=3;
                        fastClear();
                        SingleZoneScreenManagement(RunValve);
                        timeRemaining=SINGLEVALVETIME;
                        startMillis=millis();
                        ENTERbuttonPushed=false;
                        Zone1_Request=false;
                        Zone2_Request=false;
                        Zone3_Request=true;
                      }
                    else {}
                
          }
      break;
    
    case RUN_SINGLE_ZONE:
          nowMillis = millis();
          if ((nowMillis-startMillis)>SINGLEVALVETIME)
            {
              Zone1_Request=false;
              Zone2_Request=false;
              Zone3_Request=false;
              state=STAND_BY_ALL_OFF;
              MainMenu_Current_State=0;
              MainMenu(MainMenu_Current_State);
              }
              else {
                  timeRemaining = (SINGLEVALVETIME-(nowMillis-startMillis));
                  timeRemainingMinutes=timeRemaining/60000;
                  timeRemainingSeconds=(timeRemaining/1000)-(timeRemainingMinutes*60);
                  lcd.setCursor(14,3);
                  lcd.print(timeRemainingMinutes);
                  lcd.print(":");
                  if (timeRemainingSeconds<10) lcd.print("0");
                  lcd.print(timeRemainingSeconds);
                  Serial.print("Time Remaining: ");
                  Serial.print(timeRemaining);
                  Serial.print("    Millis function:  ");
                  Serial.println(nowMillis);
              }
        
      break;
    
    case RUN_ALL_ZONES:
          nowMillis = millis();
          if (RunValve==1)
          {
            if ((nowMillis-startMillis)>Zone1TimeAuto)
            {
              Zone1_Request=false;
              Zone2_Request=true;
              Zone3_Request=false;
              RunValve=2;
              startMillis=millis();
              lcd.setCursor (18,1);
              lcd.print(" ");                                  
            }
            else{
                  timeRemaining = (Zone1TimeAuto-(nowMillis-startMillis));
                  timeRemainingMinutes=timeRemaining/60000;
                  timeRemainingSeconds=(timeRemaining/1000)-(timeRemainingMinutes*60);
                  lcd.setCursor(12,1);
                  if (timeRemainingMinutes<10) lcd.print("0");
                  lcd.print(timeRemainingMinutes);
                  lcd.print(":");
                  if (timeRemainingSeconds<10) lcd.print("0");
                  lcd.print(timeRemainingSeconds);
                  //AnimeateProgressIcon(true, 100, 18, 1);
                  lcd.setCursor (18,1);
                  lcd.write(5);                                   //raindrop icon
                  Serial.print("Time Remaining: ");
                  Serial.print(timeRemaining);
                  Serial.print("    Run Valve  ");
                  Serial.println(RunValve);
            }
          }
          MainMenu_Current_State=0;
          if(RunValve==2) {
            if ((nowMillis-startMillis)>Zone2TimeAuto)
            {
              Zone1_Request=false;
              Zone2_Request=false;
              Zone3_Request=true;
              RunValve=3;
              startMillis=millis();
              lcd.setCursor (18,2);
              lcd.print(" ");                                   //raindrop icon
            }
               else{
                  timeRemaining = (Zone2TimeAuto-(nowMillis-startMillis));
                  timeRemainingMinutes=timeRemaining/60000;
                  timeRemainingSeconds=(timeRemaining/1000)-(timeRemainingMinutes*60);
                  lcd.setCursor(12,2);
                  if (timeRemainingMinutes<10) lcd.print("0");
                  lcd.print(timeRemainingMinutes);
                  lcd.print(":");
                  if (timeRemainingSeconds<10) lcd.print("0");
                  lcd.print(timeRemainingSeconds);
                  lcd.setCursor (18,2);
                  lcd.write(5);                                   //raindrop icon
                  Serial.print("Time Remaining: ");
                  Serial.print(timeRemaining);
                  Serial.print("    Run Valve  ");
                  Serial.println(RunValve);
                   }
          } else if(RunValve==3) {
                if ((nowMillis-startMillis)>Zone3TimeAuto)
                {
                  Zone1_Request=false;
                  Zone2_Request=false;
                  Zone3_Request=false;
                  RunValve=1;
                  startMillis=millis();
                  state=STAND_BY_ALL_OFF;
                  MainMenu_Current_State=0;
                  MainMenu(MainMenu_Current_State);
                  
                 }
                 else{
                  timeRemaining = (Zone3TimeAuto-(nowMillis-startMillis));
                  timeRemainingMinutes=timeRemaining/60000;
                  timeRemainingSeconds=(timeRemaining/1000)-(timeRemainingMinutes*60);
                  lcd.setCursor(12,3);
                  if (timeRemainingMinutes<10) lcd.print("0");
                  lcd.print(timeRemainingMinutes);
                  lcd.print(":");
                  if (timeRemainingSeconds<10) lcd.print("0");
                  lcd.print(timeRemainingSeconds);
                  lcd.setCursor (18,3);
                  lcd.write(5);                                   //raindrop icon
                  Serial.print("Time Remaining: ");
                  Serial.print(timeRemaining);
                  Serial.print("    Run Valve  ");
                  Serial.println(RunValve);
            }
          }
    break;
    
    case CANCELLING:
    break;
  }


  
  if (DOWNbuttonPushed)
  {
    
  }
    else if (0==0)
    {
    }
}


void updateClock()
{
  static unsigned long lastVeraGetTime;
  if (millis() - lastVeraGetTime >= 3600000UL) // updates clock time and gets zone times from vera once every hour
  {
    DEBUG_PRINTLN(F("Requesting time and valve data from Gateway..."));
    lcd.setCursor(15, 0);
    lcd.write(byte(0));
//    clockUpdating = true;
    requestTime();
    lastVeraGetTime = millis();
  }
}

