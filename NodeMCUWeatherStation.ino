/*
 Name:		WeatherStation.ino
 Created:	16.03.2018 10:56:18
 Author:	Frank Schmitz
*/
#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

#define WRITE Serial
#define READ Serial

double AmbientTemp = 12.3421;
double AmbientHumidity = 67.3421;
double RainState = 0;
int ScaleMaxMM = 80;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
	Serial.begin(57600);
	Serial.flush();
	pinMode(LED_BUILTIN, OUTPUT);
}


void Humidity()
{
  WRITE.println(dht.readHumidity());
}

void RainDetect()
{
  WRITE.println((1-analogRead(A0)/1024.0f)*ScaleMaxMM);
}

void Temperature()
{
  if (dht.readTemperature()) { 
    WRITE.println(dht.readTemperature());
  } else {
    WRITE.println("");
  }
}

void loop() {


  String command = "";
  if (READ.available() > 0) {
    command = READ.readStringUntil('\n');

		if (command.equalsIgnoreCase("H")) {
			Humidity();
		}
		else if (command.equalsIgnoreCase("T")) {
			Temperature();
		}
		else if (command.equalsIgnoreCase("RR")) {
      RainDetect();
    }
    else if (command.equalsIgnoreCase("PRINTWEATHER")) {
      WRITE.println("*********************");
      
      WRITE.print("H: ");
      Humidity();
      
      WRITE.print("T: ");
      Temperature();
      
      WRITE.print("RR: ");
      RainDetect();
      
      WRITE.println("*********************");

    }
  }
  
}
