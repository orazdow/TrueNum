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
//myNum.getQuery(client);
//myNum.setVal("$hello", 4.7);
//Serial.println(myNum.getVal("$hello"));
//myNum.setVal("$sensor-temp", mySensor.getTemperature());
//myNum.setVal("$sensor-hum", mySensor.getHumidity() ); 
//myNum.makeCall("temperature of myNiceRoom = $sensor-temp < 71 F",client);
/*
myNum.setVal("$sensor-temp", mySensor.getTemperature());
myNum.setVal("$sensor-hum", mySensor.getHumidity() ); 
myNum.getQuery(client);
//myNum.makeCall(client);
myNum.getQuery(client);
//myNum.makeCall(client);
myNum.getQuery(client);
//myNum.makeCall(client);
myNum.getQuery(client);
//myNum.makeCall(client); */
}

void loop() {
  
//myNum.setVal("$sensor-temp", mySensor.getTemperature());
//myNum.setVal("$sensor-hum", mySensor.getHumidity() ); 

myNum.setVal("$DHT-temp", mySensor.getTemperature());
myNum.setVal("$thing1f", mySensor.getHumidity());

myNum.getQuery(client);
myNum.makeCall(client);
//Serial.println(myNum.getVal("$DHT-temp"));
//myNum.makeCall( "temperature of myArduino = $DHT-temp < 70 F", client );

if(myNum.getReturnBool()){
  digitalWrite(13,HIGH);
} else { digitalWrite(13,LOW);}

}

