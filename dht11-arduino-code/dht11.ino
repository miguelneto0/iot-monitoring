#include <DHT.h>
#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temp

void setup(){
  Serial.begin(9600);
  dht.begin();
  hum = dht.readHumidity();
  temp= dht.readTemperature();
}

void loop(){    
    Serial.print(hum);
    Serial.print(" ");
    Serial.print(temp);
    
    delay(2000);
}
