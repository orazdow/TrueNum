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

}

void loop() {
  
//myNum.setVal("$sensor-temp", mySensor.getTemperature());
//myNum.setVal("$sensor-hum", mySensor.getHumidity() ); 

myNum.setVal("$DHT-temp", mySensor.getTemperature());
myNum.setVal("$thing1f", mySensor.getHumidity());

myNum.getQuery(client);
myNum.makeCall(client);

if(myNum.getReturnBool()){
  digitalWrite(13,HIGH);
} else { digitalWrite(13,LOW);}

}

