class TestSensor{

float r[3] = {68.3, 72.4, 70.3};
unsigned char a = 0;
public:

float getTemperature(){
  a = (a+1)%3;
  return r[a] ;
}

float getHumidity(){
  a = (a+1)%3;
  return r[a]/2;
}

};

