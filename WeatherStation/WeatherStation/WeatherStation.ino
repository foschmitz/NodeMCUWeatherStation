/*
 Name:		WeatherStation.ino
 Created:	16.03.2018 10:56:18
 Author:	Frank Schmitz
*/

int CurrentFilter = 0;

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
		if (cmd == "GETFILTER") {
			Serial.print(CurrentFilter); 
			Serial.println("#");
		}
		else if (cmd == "FILTER0") {
			MoveFilter(0);
		}
		else if (cmd == "FILTER1") {
			MoveFilter(1);
		}
		else if (cmd == "FILTER2") {
			MoveFilter(2);
		}
		else if (cmd == "FILTER3") {
			MoveFilter(3);
		}
	}
  
}

void MoveFilter(int pos) {

	// do stepper motor stuff
	for (int i; i < pos; i++) {
		digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
		delay(500);                       // wait for a second
		digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
		delay(500);
	}
	
	CurrentFilter = pos;
	Serial.println("0#");
}