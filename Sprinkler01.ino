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
 *     D7 - DHT sensor data pin
 *     D6 - Switch1 = Menu/back
 *     D8 - Switch2 = Arrow down
 *     A6 - ALARM door for basement external door i have
 *     A0 - Switch3 = ENTER
 *     A4 - SDA line for LCD display I2C
 *     A5 - SCL line for LCD display I2C
 *     D9 - 
 *     D10- 
 *     D11- 
 *     D12- 
 *     D13- 
 *     Radio normally connected as per Mysensors.org
 *     
 ********************
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

// Basic Configuration - always keep for MYSENSOR projects
#define MY_DEBUG                  // Enable debug prints to serial monitor
#define MY_RADIO_NRF24            // Enable and select radio type attached
#define MY_REPEATER_FEATURE       // Enabled repeater feature for this node
#define DEBUG_ON   // comment out to supress serial monitor output
#ifdef DEBUG_ON
#define DEBUG_PRINT(x)   Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define SERIAL_START(x)
#endif
//




//Libraries, some are project specific
#include <SPI.h>
#include <Wire.h>
#include <Time.h>
#include <TimeLib.h>
#include <MySensors.h>
#include <Bounce2.h>
#include <DHT.h>                          // library for temp and humidity sensor
#include <LiquidCrystal_I2C.h>


// Project Pins
#define Zone1 2                           // Relay setting for Zone 1 to 4
#define Zone2 3
#define Zone3 4
#define Zone4 5
#define Menu_key 6
#define DHT_DATA_PIN 7                    // Set this to the pin you connected the DHT's data pin to
#define Arrow_key 8
#define Enter_Key A0
#define External_Door A6



// Project DEFINITIONS
#define SKETCH_NAME "MySprinkler - Joao Borges"
#define SKETCH_VERSION "1.0"
#define NUMBER_OF_VALVES 3            // Change this to set your valve count up to 16.
#define VALVE_RESET_TIME 7500UL       // Change this (in milliseconds) for the time you need your valves to hydraulically reset and change state
#define VALVE_TIMES_RELOAD 300000UL   // Change this (in milliseconds) for how often to update all valves data from the controller (Loops at value/number valves)
                                      // ie: 300000 for 8 valves produces requests 37.5seconds with all valves updated every 5mins 
                                      // Node Childs (all individual iDs for each IO)
#define CHILD_ID_SPRINKLER  0
#define CHILD_ID_Zone_1     1
#define CHILD_ID_Zone_2     2
#define CHILD_ID_Zone_3     3
#define CHILD_ID_TEMP       4            // iD for temperature reporting
#define CHILD_ID_HUM        5            // iD for humidity reporting
#define CHILD_ID_Door       6

// Other Defines
#define ON 0
#define OFF 1


//LCD RELATED
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
byte Bar1[8] = {
  B10000,
  B01000,
  B01000,
  B00100,
  B00100,
  B00010,
  B00010,
  B00001
};
byte Bar2[8] = {
  B00001,
  B00010,
  B00010,
  B00100,
  B00100,
  B01000,
  B01000,
  B10000
};
byte ParBar[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000
};
byte VertBar[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};
byte face[8] = {  0x01, 0x03, 0x02, 0x06, 0x16, 0x1C, 0x0C, 0x08};
byte FifteenMin[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0}; // fetching time indicator
byte raindrop[8] = {0x4, 0x4, 0xA, 0xA, 0x11, 0xE, 0x0,}; // fetching Valve Data indicator
typedef enum {
  STAND_BY_ALL_OFF, RUN_SINGLE_ZONE, RUN_ALL_ZONES, CYCLE_COMPLETE, ZONE_SELECT_MENU, CANCELLING
}  SprinklerStates;
SprinklerStates state = STAND_BY_ALL_OFF;   //define variable called state, of the type SprinklerStates
SprinklerStates lastState;                  //define variable called lastState, of the type SprinklerStates
byte MainMenu_Current_State = 0;       // 0 is Run all, 1= zone1, 2=zone2, 3= zone3, 4 = config



// Some additional definitons and INSTANCES creation
MyMessage msgTemp(CHILD_ID_TEMP,    V_TEMP);
MyMessage msgHum(CHILD_ID_HUM,      V_HUM);
MyMessage msgZone1(CHILD_ID_Zone_1, V_LIGHT);
MyMessage msgZone2(CHILD_ID_Zone_2, V_LIGHT);
MyMessage msgZone3(CHILD_ID_Zone_3, V_LIGHT);
MyMessage msgDoor1(CHILD_ID_Door,   V_TRIPPED);

Bounce Debounce_Door1 = Bounce();  // create instance of debounced button
Bounce Debounce_Arrow  = Bounce();  // create instance of debounced button
Bounce Debounce_Enter = Bounce();  // create instance of debounced button
DHT dht;                          // Creating instance of DHT


// ************** VARIABLES *****************
// TEMP, DOOR and SWITCHES RELATED<<<<<<<<<<<
int oldDoorValue1=0;
int oldDoorValue2=0;
int oldDoorValue3=0;
static const uint8_t FORCE_UPDATE_N_READS = 3; // Force sending an update of the temperature after n sensor reads
float lastTemp;                   // variable to hold last read temperature
float lastHum;                    // variable to hold last read humidity
uint8_t nNoUpdatesTemp;           // keeping track of # of readings 
uint8_t nNoUpdatesHum;
bool metric = true;               // metric or imperial?
bool Rstate;                      // RELAY STATE 
//const long DoorActivationPeriod = 600; // [ms]

// LCD RELATED & SPRINKLER OPERATION<<<<<<<<<<<<<<<<<<<
int Clock_Animation_time=200;
unsigned long nowMillis;
unsigned long startMillis;
unsigned long timeRemaining;          // Time remaining for a given valve
unsigned long timeRemainingMinutes;   // Time remaining in minutes
unsigned long timeRemainingSeconds;   // Time remaining seconds
bool DOWNbuttonPushed = false;
bool ENTERbuttonPushed = false;


//Flashmap
unsigned long SINGLEVALVETIME=10000;   // 10 minutes * 60 *1000 time in milliseconds
unsigned long Zone1TimeAuto = 11000;
unsigned long Zone2TimeAuto = 9000;
unsigned long Zone3TimeAuto = 5000;
int RunValve = 1;

//others
#define VALVE_RESET_TIME 7500UL 







/***********************************************************/
/***********************************************************/
/***********************************************************/
/***********************************************************/
void setup()  
{  
  DEBUG_PRINTLN(F("Running Setup.."));          //...................
  // INPUTS //
  pinMode(External_Door,INPUT);                 // Setup Doors as INPUTS
  digitalWrite(External_Door,HIGH);             // Activate internal pull-up
  Debounce_Door1.attach(External_Door);         // After setting up the button, setup debouncer
  Debounce_Door1.interval(1);
  pinMode(Enter_Key,INPUT);                 // Setup Doors as INputs
  digitalWrite(Enter_Key,HIGH);             // Activate internal pull-up
  Debounce_Enter.attach(Enter_Key);         // After setting up the button, setup debouncer
  Debounce_Enter.interval(1);
  pinMode(Arrow_key,INPUT);                 // Setup Doors as INputs
  digitalWrite(Arrow_key,HIGH);             // Activate internal pull-up
  Debounce_Arrow.attach(Arrow_key);         // After setting up the button, setup debouncer
  Debounce_Arrow.interval(1);

  // OUTPUTS  //
  digitalWrite(Zone1, OFF);                 // Make sure Motor is off at startup
  pinMode(Zone1, OUTPUT);                   // Then set Motor pins in output mode
  digitalWrite(Zone2, OFF);                 // Make sure Motor is off at startup
  pinMode(Zone2, OUTPUT);                   // Then set Motor pins in output mode
  digitalWrite(Zone3, OFF);                 // Make sure Motor is off at startup
  pinMode(Zone3, OUTPUT);                   // Then set Motor pins in output mode
  dht.setup(DHT_DATA_PIN);                  // set data pin of DHT sensors
  
  //LCD  //
  lcd.init();                       // initialize the LCD
  lcd.createChar(0, Bar1);          // Create custom character in LCD
  lcd.createChar(1, Bar2);          // Create custom character in LCD
  lcd.createChar(2, ParBar);        // Create custom character in LCD
  lcd.createChar(3, VertBar);       // Create custom character in LCD
  lcd.createChar(4, FifteenMin);    // Create custom character in LCD
  lcd.createChar(5, raindrop);      // Create custom character in LCD
  lcd.createChar(6, face);
  lcd.backlight();                  // Turn on Back light (to tur off is lcd.noBacklight()

  // GET CLOCK FROM GATEWAY
  lcd.setCursor(0, 0);
  lcd.print(F(" Syncing Time  "));
  lcd.setCursor(15, 0);
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  int clockCounter = 0;
  while (timeStatus() == timeNotSet && clockCounter < 21)
  {
    requestTime();
    DEBUG_PRINTLN(F("Requesting time from Gateway:"));
    wait(1000);
    lcd.print(".");
    clockCounter++;
    if (clockCounter > 16)
    {
      DEBUG_PRINTLN(F("Failed initial clock synchronization!"));
      lcd.clear();
      lcd.print(F("  Failed Clock  "));
      lcd.setCursor(0, 1);
      lcd.print(F(" Syncronization "));
      wait(2000);
      break;
    }
  }
  //////// MAIN INTRO SCREEN, print and stay 2s /////////
  fastClear();
  lcd.setCursor(0,0);
  lcd.print("Sprinkler    ");
  lcd.print(SKETCH_VERSION);
  lcd.setCursor(2,1);
  lcd.print("My Sensors System");
  lcd.setCursor(0,2);
  lcd.print("              ");
  lcd.setCursor(2,3);
  lcd.print("Joao Borges");
  delay(2000);                //////// REPLACE WITH WAIT WHEN INTEGRATING WITH MYSENSORS
  lcd.init();
  // TURN RELAYS OFF
  MainMenu(MainMenu_Current_State);
}









void presentation()  
{
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);   // Send the sketch version information to the gateway and Controller
  Serial.println("Presentation function..");
  present(CHILD_ID_TEMP, S_TEMP);         // Registar Temperature to gw
  present(CHILD_ID_HUM, S_HUM);           // Register Humidity to gw
  present(CHILD_ID_Door, S_DOOR);      // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_Zone_1, S_BINARY);      // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_Zone_2, S_BINARY);      // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_Zone_3, S_BINARY);              // Register PWM output
//  metric = getConfig().isMetric;          // get configuration from the controller on UNIT system

}








/***************
 * MAIN LOOP
 * 
 * 
 * Asynchronously check for new messages from gw
*****************/
void loop() 
{
//  DEBUG_PRINT("Main loop at node: ");
//  DEBUG_PRINTLN(getNodeId());

  Debounce_Door1.update();
  Debounce_Enter.update();
  Debounce_Arrow.update();

  LCDMainLoop();
  ReadTemp(); 
  int value = Debounce_Door1.read();   //Get the update value
  if (value != oldDoorValue1) {
     send(msgDoor1.set(value?true:false), true); // Send new state and request ack back
     Serial.print("Door 1:");
     Serial.println(value);
    }
  oldDoorValue1 = value;

//  int value2 = Debounce_Door2.read();   //Get the update value
/*  if (value2 != oldDoorValue2) {
     send(msgDoor2.set(value2?true:false), true); // Send new state and request ack back
     Serial.print("Door 2:");
     Serial.println(value2);
    }
  oldDoorValue2 = value2;

  int value3 = Debounce_Door3.read();   //Get the update value
  if (value3 != oldDoorValue3) {
     send(msgDoor3.set(value3?true:false), true); // Send new state and request ack back
     Serial.print("Door 3:");
     Serial.println(value3);
    }
  oldDoorValue3 = value3;

// bool Tripped = digitalRead(Presence_Input);   //Get the update value
 // if (value4 != oldPres && value4 == true) {
 //    send(msgMotion.set((Tripped?"1":"0"))); // Send new state and request ack back
     Serial.print("Motion Detected in Garage: ");
//     Serial.println(Tripped);
 //    analogWrite(PWM, PWMvar);
  //  }
  //  else if (value4 != oldPres && value4 == false) {
  //    send(msgMotion.set(false)); // Send new state and request ack back
 //     analogWrite(PWM, 0);
//    } else{};
//    oldPresence = value4;*/
  ReadTemp(); 
  wait(2000);
} 








