// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
//#define BLYNK_TEMPLATE_ID           "TMPLcPyxjGyC"
#define BLYNK_TEMPLATE_ID           "TMPLpsuHbeGS"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "7oPEZpbHU3VK2lQXM4FP3Bf4aiQvgqRU"
//#define BLYNK_AUTH_TOKEN            "fOfMGdBFeEsUJmYUUCcTdjEVwVkNclV1"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int sensorValue = 0;  // value read from the moisture sensor
int ledPin = 13; // GPIO13----D7 of NodeMCU
bool state = 0; // LED flag
bool waterlevel = 0;  // water level float digital input
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "username";
char pass[] = "password";

BlynkTimer timer;
//**********************************************************************
// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  //digitalWrite(ledPin, HIGH);
  //sensorValue = analogRead(analogInPin);
  // Update state
  Blynk.virtualWrite(V1, value);
  Blynk.virtualWrite(V3, sensorValue);
}

//************************************************************************************
// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

//*********************************************************************************
// This function sends Arduino's uptime every 2 seconds to Virtual Pin 2.
void myTimerEvent()
{
  sensorValue = analogRead(analogInPin);  //read moisture sensor every second
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 2000);
  Blynk.virtualWrite(V1, waterlevel);
  Blynk.virtualWrite(V3, sensorValue);
  Serial.print(waterlevel);
  Serial.print("/");
  Serial.println(sensorValue);
}

//*******************************************************************************
void setup()
{
  pinMode(12, INPUT);        // gpio12=D6 level sensor
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(115200);  // Debug console

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();

  waterlevel = digitalRead(12); // gpio12=D6 on=water present off=empty
  
  if((sensorValue > 600) && waterlevel){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
}
