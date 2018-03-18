#include <DHTesp.h>
#include <ESP8266WiFi.h>


#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHTesp::DHT22   // DHT 22  (AM2302)

DHTesp dht;

// Replace with your network credentials
const char* ssid     = "JONK";
const char* password = "*********";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current led state
String ledBuiltInState = "off";

double AmbientTemp = 12.3421;
double AmbientHumidity = 67.3421;
double RainState = 0;
int ScaleMaxMM = 80;

void Humidity()
{
  Serial.println(dht.getHumidity());
}

void RainDetect()
{
  Serial.println((1-analogRead(A0)/1024.0f)*ScaleMaxMM);
}

void Temperature()
{
  if (dht.getTemperature()) { 
    Serial.println(dht.getTemperature());
  } else {
    Serial.println("");
  }
}

void setup() {
  Serial.begin(115200);
  
  dht.setup(DHTPIN, DHTTYPE);
  Serial.println("DHT initiated");
  
  // Initialize the led variables as outputs
  pinMode(LED_BUILTIN, OUTPUT);
  // Set outputs to LOW
  digitalWrite(LED_BUILTIN, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  String command = "";
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');

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
      Serial.println("*********************");
      
      Serial.print("H: ");
      Humidity();
      
      Serial.print("T: ");
      Temperature();
      
      Serial.print("RR: ");
      RainDetect();
      
      Serial.println("*********************");

    }
  }
  
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /1/on") >= 0) {
              Serial.println("LED_BUILTIN on");
              ledBuiltInState = "on";
              digitalWrite(LED_BUILTIN, HIGH);
            } else if (header.indexOf("GET /1/off") >= 0) {
              Serial.println("LED_BUILTIN off");
              ledBuiltInState = "off";
              digitalWrite(LED_BUILTIN, LOW);
            } 
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>LEDBUILTIN - State " + ledBuiltInState + "</p>");
            // If the led is off, it displays the ON button       
            if (ledBuiltInState=="off") {
              client.println("<p><a href=\"/1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            Serial.println("Writing page...");
            RainDetect();
            if ((1-analogRead(A0)/1024.0f)*ScaleMaxMM > 1) {
                client.println("<p>It's raining!!!!!</p>");
            } else {
                client.println("<p>It's dry!!!!!</p>");
            }
            
            
         /*   client.print("<p>RAINRATE: ");
            client.print((1-analogRead(A0)/1024.0f)*ScaleMaxMM);
            client.println("</p>"); */
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

}
