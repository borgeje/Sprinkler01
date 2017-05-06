
void receive(const MyMessage &message) {
  // We only expect one type of message from controller. But we better check anyway.
  if (message.isAck()) {
     Serial.println("This is an ack from gateway");
  }
  state = message.getBool();      // Change relay state
  if (message.type == V_LIGHT && message.sensor == CHILD_ID_Garage_Motor_1 && state == true) {
       Serial.println("Drive Motor 1");
//       digitalWrite(Garage_Motor_1, ON);
       wait(DoorActivationPeriod);
//       digitalWrite(Garage_Motor_1, OFF);
       wait(500);
       send(msgMotor1.set(false), true);    // After activating the door, move indicator to FALSE again

       // Write some debug info
       Serial.print("Incoming change for sensor:");
       Serial.print(message.sensor);
       Serial.print(", New status: ");
       Serial.println(state);
   } 
   else if (message.type == V_LIGHT && message.sensor == CHILD_ID_Garage_Motor_2 && state == true) {
       Serial.println("Drive Motor 2");
//       digitalWrite(Garage_Motor_2, ON);
       wait(DoorActivationPeriod);
//       digitalWrite(Garage_Motor_2, OFF);
       wait(500);
       send(msgMotor2.set(false), true);    // After activating the door, move indicator to FALSE again
       // Write some debug info
       Serial.print("Incoming change for sensor:");
       Serial.print(message.sensor);
       Serial.print(", New status: ");
       Serial.println(state);
        }
     else if (message.type == V_DIMMER && message.sensor == CHILD_ID_PWM) {
          PWMvar = message.getInt();
           }
        else {};

}




void receiveTime(time_t newTime)
{
  DEBUG_PRINTLN(F("Time value received and updated..."));
  int lastSecond = second();
  int lastMinute = minute();
  int lastHour = hour();
  setTime(newTime);
  if (((second() != lastSecond) || (minute() != lastMinute) || (hour() != lastHour)) || showTime)
  {
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
    DEBUG_PRINTLN(dayOfWeek[weekday()]);
    showTime = false;
  }
  else
  {
    DEBUG_PRINTLN(F("Sensor's time did NOT need adjustment greater than 1 second."));
  }
  clockUpdating = false;
}

