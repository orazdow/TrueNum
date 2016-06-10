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

#include <SPI.h>
#include <Adafruit_WINC1500.h>
#include "TrueNum.h"
#include "TestSensor.h"

// username, password, numberspace, boardID:
TrueNum myNum("test@adomain.abc", "condor", "public-iot", "arduino1");

//Enter your WIFi SSID and password here:
char ssid[] = "Samsung Galaxy S6 1532";     
char pass[] = "randal99"; 

TestSensor mySensor;

#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2 

Adafruit_WINC1500 WiFi(WINC_CS, WINC_IRQ, WINC_RST); 
int keyIndex = 0;
int status = WL_IDLE_STATUS;
Adafruit_WINC1500Client client;

void setup() { 
// Sketch will start when serial monitor is opened:  
#ifdef WINC_EN
  pinMode(WINC_EN, OUTPUT);
  digitalWrite(WINC_EN, HIGH);
#endif  
Serial.begin(9600);
while(!Serial); 
while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    uint8_t timeout = 10;
    while (timeout && (WiFi.status() != WL_CONNECTED)) {
      timeout--;
      delay(1000);
    }
  }
Serial.println("Connected to wifi");
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
