/*  
*  -TrueNumber IoT Client-
*
*  This sketch uses the TrueNumber client to create truenumbers 
*  using sensor data. The client queries the user's numberspace 
*  for truenumbers to use as parameters.
*
*  access the dashboard for this numberspace at:
*  http://pub.truenumbers.com/Numberflow/login.jsp
*  credentials: 'test@adomain.abc', 'condor'
*  
*  TrueNumbers tagged with 'parameter' and a child tag of the board's ID
*  (ex/ "parameter:arduino1") will be queried for parameters to use.
*  
*  try these searches in the dashboard:
*  - 'parameter:arduino1' or 'parameter'
*  - 'subj:my*'
*
*  For a full guide to using the TrueNumber dashboard and client libraries.   
*  visit: http://truenum.com/iot
*/

#include "TrueNum.h"
#include "TestSensor.h"
#include <SPI.h>
#include <Ethernet.h>

EthernetClient client;
TestSensor mySensor;

//enter mac address 
byte mac[] = { 0x90, 0xA2, 0xDA, 0xF3, 0x13, 0xEC };

// username, password, numberspace, boardID:
TrueNum myNum("test@adomain.abc", "condor", "public-iot", "arduino1");

void setup() { 
// Sketch will start when serial monitor is opened:  
Serial.begin(9600);
while(!Serial); 

if (Ethernet.begin(mac) == 0){
Serial.println("Failed to configure Ethernet using DHCP");
} 
delay(1000);
}

void loop() {

// associate new sensor values to token strings
// tokens must begin with a '$' symbol
myNum.setVal("$sensor-temp", mySensor.getTemperature());
myNum.setVal("$sensor-hum", mySensor.getHumidity());

// query numberspace for templates or parameters:
myNum.getQuery(client);

// create new Truenumbers in the numberspace:
myNum.makeCall(client);

// special case for use with a 'return-bool' truenumber:
// led will turn on when the satement evaluates to true
if(myNum.getReturnBool()){
  digitalWrite(13,HIGH);
} else { digitalWrite(13,LOW);}

}
