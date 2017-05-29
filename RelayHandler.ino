void RelayHandler()
{

  if (Zone1_Request==true && digitalRead(Zone1)==HIGH)      // IF request is ON and pin is OFF , start timer
    {
          digitalWrite(Zone1, ON);
          startMillis1=millis(); 
          Serial.print("Request true, turn true");
    } else if (Zone1_Request==true && digitalRead(Zone1)==LOW)
        {
          nowMillis1 = millis();
          Serial.print("Request true, COUNT TIME");
          if ((nowMillis1-startMillis1)>SINGLEVALVETIMERELATIMEOUT)
            {
              digitalWrite(Zone1, OFF);
              Zone1_Request=false;
              }
              else {
                  Serial.print("aqui");
              }
        
        } else 
            {
              digitalWrite(Zone1, OFF);
              Serial.print("    OFFFFFF .....");
            }

  if (Zone2_Request==true && digitalRead(Zone2)==HIGH)      // IF request is ON and pin is OFF , start timer
    {
          digitalWrite(Zone2, ON);
          startMillis2=millis(); 
    } else if (Zone2_Request==true && digitalRead(Zone2)==LOW)
        {
          nowMillis2 = millis();
          if ((nowMillis2-startMillis2)>SINGLEVALVETIMERELATIMEOUT)
            {
              digitalWrite(Zone2, OFF);
              Zone2_Request=false;
              }
              else {
                  
              }
        
        } else digitalWrite(Zone2, OFF);

  if (Zone3_Request==true && digitalRead(Zone3)==HIGH)      // IF request is ON and pin is OFF , start timer
    {
          digitalWrite(Zone3, ON);
          startMillis3=millis(); 
    } else if (Zone3_Request==true && digitalRead(Zone3)==LOW)
        {
          nowMillis3 = millis();
          if ((nowMillis3-startMillis3)>SINGLEVALVETIMERELATIMEOUT)
            {
              digitalWrite(Zone3, OFF);
              Zone3_Request=false;
              }
              else {
                  
              }
        
        } else digitalWrite(Zone3, OFF);



}

