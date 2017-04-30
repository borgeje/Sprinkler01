/**
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Joao E Borges
 * 
 * DESCRIPTION
 *    MySensor Node working to control SPRINKLERS
 *     D2 - Zone 1 relay
 *     D3 - Zone 2 relay
 *     D4 - Zone 3 relay
 *     D5 - Zone 4 relay
 *     
 *     Radio normally connected as per Mysensors.org
 *     
 ********************
 *
 *     The MySensors Armduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
************************************************************
 */ 

// Basic Configuration - alwyas keep
#define MY_DEBUG                  // Enable debug prints to serial monitor
#define MY_RADIO_NRF24            // Enable and select radio type attached
#define MY_REPEATER_FEATURE       // Enabled repeater feature for this node


#//ifndef MY_RF24_PA_LEVEL
 //#define MY_RF24_PA_LEVEL RF24_PA_HIGH
#//endif

//Libraries, some are project specific
#include <SPI.h>
#include <MySensors.h>
#include <Bounce2.h>
#include <DHT.h>                  // library for temp and humidity sensor


// Project Pins
#define Water_Sensor A0
#define Water_Sensor2 A1

// Node Childs (all individual iDs for each IO)
#define CHILD_ID_Water 1         // use this for the BINARY output
#define CHILD_ID_WaterLevel 2    // use this for the analog read
#define CHILD_ID_Water2 3         // use this for the BINARY output
#define CHILD_ID_WaterLevel2 4    // use this for the analog read

// Other Defines
#define ON 0
#define OFF 1
#define SENSOR_TEMP_OFFSET 0      // Set this offset if the sensor has a permanent small offset to the real temperatures

// Some additional definitons and INSTANCES creation
MyMessage msgWater(CHILD_ID_Water, V_STATUS);
MyMessage msgWaterLevel(CHILD_ID_Water, V_FLOW);
MyMessage msgWater2(CHILD_ID_Water2, V_STATUS);
MyMessage msgWaterLevel2(CHILD_ID_Water, V_FLOW);

Bounce Debounce_LRC = Bounce();  // create instance of debounced button
Bounce Debounce_RRC = Bounce();  // create instance of debounced button
Bounce Debounce_FC = Bounce();  // create instance of debounced button
Bounce Debounce_PRES = Bounce();
DHT dht;                          // Creating instance of DHT

//VARIABLES
int oldDoorValue1=0;
int oldDoorValue2=0;
int oldDoorValue3=0;
int oldPres;
int oldWaterValue=1023;
static const uint64_t UPDATE_INTERVAL = 10000; // Sleep time between sensor updates (in milliseconds) // Must be >1000ms for DHT22 and >2000ms for DHT11
static const uint8_t FORCE_UPDATE_N_READS = 3; // Force sending an update of the temperature after n sensor reads
float lastTemp;                   // variable to hold last read temperature
float lastHum;                    // variable to hold last read humidity
uint8_t nNoUpdatesTemp;           // keeping track of # of reqdings 
uint8_t nNoUpdatesHum;
bool metric = true;               // metric or imperial?
bool state;
const uint8_t DoorActivationPeriod = 600; // [ms]




void setup()  
{  
  Serial.println("Running Setup");
  // Doors setup input pins and debounce function
//  pinMode(Left_RC_Door,INPUT);                 // Setup Doors as INputs
//  digitalWrite(Left_RC_Door,HIGH);             // Activate internal pull-up
//  Debounce_LRC.attach(Left_RC_Door);           // After setting up the button, setup debouncer
//  Debounce_LRC.interval(1);
//  pinMode(Right_RC_Door,INPUT);                 // Setup Doors as INputs
//  digitalWrite(Right_RC_Door,HIGH);             // Activate internal pull-up
//  Debounce_RRC.attach(Right_RC_Door);           // After setting up the button, setup debouncer
//  Debounce_RRC.interval(1);
//  pinMode(Freezer_Door,INPUT);                 // Setup Doors as INputs
//  digitalWrite(Freezer_Door,HIGH);             // Activate internal pull-up
 // Debounce_FC.attach(Freezer_Door);           // After setting up the button, setup debouncer
//  Debounce_FC.interval(1);
//  pinMode(Presence_Input, INPUT);
//  digitalWrite(Presence_Input, HIGH);                     // internal pullups
//  Debounce_PRES.attach(Presence_Input);           // After setting up the button, setup debouncer
//  Debounce_PRES.interval(1);
  
  // Digital outputs pin
//  digitalWrite(Relay_1, OFF);               // Make sure Motor is off at startup
//  pinMode(Relay_1, OUTPUT);                 // Then set Motor pins in output mode
//  digitalWrite(Relay_2, OFF);               // Make sure Motor is off at startup
//  pinMode(Relay_2, OUTPUT);                 // Then set Motor pins in output mode
//  digitalWrite(PWM, OFF);                         // Make sure Motor is off at startup
//  analogWrite(PWM, 0);
//  pinMode(PWM, OUTPUT);                 // Then set Motor pins in output mode
//  digitalWrite(Relay_2, ON);
//  pinMode(Relay_2, OUTPUT);
//  dht.setup(DHT_DATA_PIN); // set data pin of DHT sensos
//  wait(2000);
//  digitalWrite(Relay_2, OFF);
//  send(msgWater.set(false));
}





void presentation()  
{
  sendSketchInfo("Joao_Fridge_Sensors", "1.0");   // Send the sketch version information to the gateway and Controller
  Serial.println("Presentation function..");
  present(CHILD_ID_TEMP, S_TEMP);         // Registar Temperature to gw
  present(CHILD_ID_HUM, S_HUM);           // Register Humidity to gw
  present(CHILD_ID_Left_RC_Door, S_DOOR);      // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_Right_RC_Door, S_DOOR);      // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_Freezer_Door, S_DOOR);      // Register all sensors to gw (they will be created as child devices)
 // present(CHILD_ID_PWM, S_DIMMER);              // Register PWM output
  present(CHILD_ID_Presence, S_MOTION);      // Register Motor doors
  present(CHILD_ID_Relay_1, S_BINARY);      // Register Motor doors
  present(CHILD_ID_Relay_2, S_BINARY);      // Register Motor doors  
  present(CHILD_ID_Water, S_BINARY);
  present(CHILD_ID_WaterLevel, S_WATER);
  metric = getConfig().isMetric;          // get configuration from the controller on UNIT system

}






/*
*  Example on how to asynchronously check for new messages from gw
*/
void loop() 
{

  Serial.print("Main loop at node: ");
  Serial.println(getNodeId());
  Debounce_LRC.update();
  Debounce_RRC.update();
  Debounce_FC.update();
  Debounce_PRES.update();

  int value = Debounce_LRC.read();   //Get the update value
  if (value != oldDoorValue1) {
     send(msgLRC.set(value?true:false), true); // Send new state and request ack back
     Serial.print("Left RC Door:");
     Serial.println(value);
    }
  oldDoorValue1 = value;

  int value2 = Debounce_RRC.read();   //Get the update value
  if (value2 != oldDoorValue2) {
     send(msgRRC.set(value2?true:false), true); // Send new state and request ack back
     Serial.print("Right RC Door :");
     Serial.println(value2);
    }
  oldDoorValue2 = value2;

  int value3 = Debounce_FC.read();   //Get the update value
  if (value3 != oldDoorValue3) {
     send(msgFC.set(value3?true:false), true); // Send new state and request ack back
     Serial.print("Freezer door:");
     Serial.println(value3);
    }
  oldDoorValue3 = value3;

 int value4 = Debounce_PRES.read();   //Get the update value
  if (value4 != oldPres && value4 == true) {
     send(msgMotion.set(true)); // Send new state and request ack back
     Serial.print("Motion not Detected in Garage");
     Serial.println(value4);
    }
    else if (value4 != oldPres && value4 == false) {
      send(msgMotion.set(false)); // Send new state and request ack back
    } else{};
    oldPres = value4;
  ReadTemp(); 
  int WaterLevelReading = ReadWater(Water_Sensor);
  send(msgWaterLevel.set(WaterLevelReading));
  Serial.println(WaterLevelReading);
  wait(3000);
} 








