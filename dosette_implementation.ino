#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

const char* ssid = "";//use your connection information here
const char* password = "";

#define PIN            5
#define NUMPIXELS      30

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int blinkRate = 1000;

const int buttonPin = 25;     // the number of the pushbutton pin
const int ledPin =  5;


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("Connected to: ");
  Serial.println(ssid);

//LED initialization
   Serial.begin(115200);

  // initialize the NeoPixel Library
  pixels.begin();

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

}

////////////////////////////////////
// main loop
////////////////////////////////////
void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    

    HTTPClient http;  
    http.begin("http://worldtimeapi.org/api/timezone/America/Denver");
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
      
 const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonDocument doc(capacity);

      char* json; 
      json = new char[capacity];
      payload.toCharArray(json, capacity);
      deserializeJson(doc, payload);

const char* abbreviation = doc["abbreviation"]; 
const char* client_ip = doc["client_ip"];
const char* datetime = doc["datetime"];
int day_of_week = doc["day_of_week"];
int day_of_year = doc["day_of_year"];
bool dst = doc["dst"]; 
const char* dst_from = doc["dst_from"];
int dst_offset = doc["dst_offset"];
const char* dst_until = doc["dst_until"];
int raw_offset = doc["raw_offset"];
const char* timezone = doc["timezone"];
long unixtime = doc["unixtime"];
const char* utc_datetime = doc["utc_datetime"];
const char* utc_offset = doc["utc_offset"];
String week_number = doc["week_number"];


//would have been displayed on a mini lcd screen 
String time = datetime;
Serial.println("Day and time:");
Serial.println(datetime);
Serial.println("Day of the Week:");
Serial.println(day_of_week);
Serial.println("Time of day:");
Serial.println(time.substring(11, 19));

//set time for user's medication schedule; more times can be added to this statement for multiple doses in a day
if (time.substring(11,19) == "12:00:30") {
//lets user know it's time to take medication through LED lights
bool activateLight = true;
int stateButton = digitalRead(buttonPin);
while(activateLight == true){
  pixels.setPixelColor(29, pixels.Color(210, 90, 90));//represents the motor activating to push out pill
  pixels.show();
for (int i = 0; i < NUMPIXELS-1; i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 250,200));
    
    pixels.show();

    delay(10);
  }


  // pause for a moment
  delay(blinkRate);

  // turn off:
  for (int i = 0; i < NUMPIXELS-1; i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));

  }
  pixels.show(); // This sends the updated pixel color to the hardware.

  // pause for a moment
  delay(blinkRate);


// ***could not get LED to turn off after pressing button, maybe needed a switch
//if(stateButton == HIGH){
//  bool activateLight = false;
//  Serial.println(stateButton);
//
//      }
//
    }
  }

    http.end();   //Close connection
  }
  // Delay
  delay(500);
}
}
