/*
 Name:		WeatherStation.ino
 Created:	16.03.2018 10:56:18
 Author:	Frank Schmitz
*/
#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

double AmbientTemp = 12.3421;
double AmbientHumidity = 67.3421;
double RainState = 0;
int ScaleMaxMM = 80;

DHT dht(DHTPIN, DHTTYPE);

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(57600);
	Serial.flush();
	pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {

  delay(2000);
  
  // Read Rainstate
  int val = analogRead(A0);
  RainState = (1-val/1024)*ScaleMaxMM;
  AmbientTemp = dht.readTemperature();
  AmbientHumidity = dht.readHumidity();

  Serial.print("Humidity: "); 
  Serial.print(AmbientHumidity);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(AmbientTemp);
  Serial.print(" %\t");
  Serial.print("RainState: "); 
  Serial.print(RainState);
  Serial.println(" *C ");
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(AmbientHumidity) || isnan(AmbientTemp) || isnan(RainState)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  String cmd;
	if (Serial.available() > 0) {
		cmd = Serial.readStringUntil('#');
		if (cmd == "GETHUMIDITY") {
			char TempString[10];
			dtostrf(AmbientHumidity, 2, 2, TempString);
			Serial.print(String(TempString));
			Serial.println("#");
		}
		else if (cmd == "GETTEMPERATURE") {
			char TempString[10];  //  Hold The Convert Data
			dtostrf(AmbientTemp, 2, 2, TempString);
			Serial.print(String(TempString));
			Serial.println("#");
		}
		else if (cmd == "GETRAINSTATE") {
      char TempString[10];  //  Hold The Convert Data
      dtostrf(RainState, 2, 2, TempString);
      Serial.print(String(TempString));
      Serial.println("#");
    }
	}
  
  
  

  
}
