/*
 Name:		WeatherStation.ino
 Created:	16.03.2018 10:56:18
 Author:	Frank Schmitz
*/

int CurrentFilter = 0;
double AmbientTemp = 12.3421;
double AmbientHumidity = 67.3421;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(57600);
	Serial.flush();
	pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {
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
		
	}
  
}
