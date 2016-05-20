//#ifndef __AVR__

#include "TrueNum.h"
#include "TestSensor.h"
#include <SPI.h>
#include <Adafruit_WINC1500.h>

//TrueNum myNum("test@adomain.abc", "condor", "public-iot", "arduino1");
TrueNum myNum("ollie@truenum.com", "Randal9?", "IoT-Project", "ard123");

#define WINC_CS   8
#define WINC_IRQ  7
#define WINC_RST  4
#define WINC_EN   2 

Adafruit_WINC1500 WiFi(WINC_CS, WINC_IRQ, WINC_RST);


char ssid[] = "Samsung Galaxy S6 1532";     
char pass[] = "randal99";    
int keyIndex = 0;
int status = WL_IDLE_STATUS;

Adafruit_WINC1500Client client;
TestSensor mySensor;

void setup() {
#ifdef WINC_EN
  pinMode(WINC_EN, OUTPUT);
  digitalWrite(WINC_EN, HIGH);
#endif  

Serial.begin(9600);
while(!Serial); 
//sketch will start when serial monitor is opened
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    uint8_t timeout = 10;
    while (timeout && (WiFi.status() != WL_CONNECTED)) {
      timeout--;
      delay(1000);
    }
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
  
//myNum.getQuery(client);
}

void loop() {
  
//myNum.setVal("$sensor-temp", mySensor.getTemperature());
//myNum.setVal("$sensor-hum", mySensor.getHumidity() ); 

myNum.setVal("$DHT-temp", mySensor.getTemperature());
myNum.setVal("$thing1f", mySensor.getHumidity());

Serial.println(myNum.getVal("$DHT-temp"));

myNum.getQuery(client);
myNum.makeCall(client);

//myNum.makeCall( "temperature of myArduino = $DHT-temp < 70 F", client );

if(myNum.getReturnBool()){
  digitalWrite(13,HIGH);
} else { digitalWrite(13,LOW);}

}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
/*
//#else

#include "TrueNum.h"
#include "TestSensor.h"
#include <Bridge.h>
#include <YunClient.h>

//TrueNum myNum("test@adomain.abc", "condor", "public-iot", "arduino1");
TrueNum myNum("ollie@truenum.com", "Randal9?", "IoT-Project", "ard123");

YunClient client;
TestSensor mySensor;

void setup() {
  
Bridge.begin();
Serial.begin(9600);
while(!Serial); 
//sketch will start when serial monitor is opened

//myNum.getQuery(client);

}

void loop() {
  
//myNum.setVal("$sensor-temp", mySensor.getTemperature());
//myNum.setVal("$sensor-hum", mySensor.getHumidity() ); 

myNum.setVal("$DHT-temp", mySensor.getTemperature());
myNum.setVal("$thing1f", mySensor.getHumidity());

Serial.println(myNum.getVal("$DHT-temp"));

myNum.getQuery(client);
myNum.makeCall(client);

//myNum.makeCall( "temperature of myArduino = $DHT-temp < 70 F", client );

if(myNum.getReturnBool()){
  digitalWrite(13,HIGH);
} else { digitalWrite(13,LOW);}

}

#endif
*/


