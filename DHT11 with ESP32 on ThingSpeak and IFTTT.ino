//------style guard ----
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

// ------header files----
#include <WiFi.h>
#include "DHT.h"
#include "ThingSpeak.h"
#include <Adafruit_Sensor.h>
#include <HTTPClient.h>

//-----netwrok credentials

char* ssid = "DESKTOP-9VFO795 4515"; //enter SSID
char* passphrase = "abcd1234"; // enter the password
WiFiServer server(80);
WiFiClient client;

String server = "http://maker.ifttt.com";
String eventname = "readings_detected";
String IFTTT_Key = "h_5fLAHPICd4WiWj6pXRoHYHKXrc5AmeVUds9ypxt4X";
String IFTTTUrl = "https://maker.ifttt.com/trigger/readings_detected/json/with/key/h_5fLAHPICd4WiWj6pXRoHYHKXrc5AmeVUds9ypxt4X";

int value1;
int value2;

//-----ThingSpeak channel details
unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "GYSKGXEG8WOVIUOL";

//----- Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

//----DHT declarations
#define DHTPIN 27 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

// Initializing the DHT11 sensor.
DHT dht(DHTPIN, DHTTYPE);
 
void setup()
{
Serial.begin(115200); //Initialize serial
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, passphrase);
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

//----nitialize dht11
dht.begin();
ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop()
{

if ((millis() - lastTime) > timerDelay)
{
delay(15000);

// Reading temperature or humidity takes about 250 milliseconds!

float h = dht.readHumidity();

// Read temperature as Celsius (the default)

float t = dht.readTemperature();
float f = dht.readTemperature(true);
if (isnan(h) || isnan(t) || isnan(f)) {
Serial.println(F("Failed to read from DHT sensor!"));
return;

}

Serial.print("Temperature (ºC): ");
Serial.print(t);
Serial.println("ºC");
Serial.print("Humidity");
Serial.println(h);
ThingSpeak.setField(1, h);
ThingSpeak.setField(2, t);

// Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different

// pieces of information in a channel. Here, we write to field 1.

int x = ThingSpeak.writeFields(myChannelNumber,

myWriteAPIKey);

if(x == 200){
Serial.println("Channel update successful.");
}

else{
Serial.println("Problem updating channel. HTTP error code " + String(x));
}

lastTime = millis();
}

}