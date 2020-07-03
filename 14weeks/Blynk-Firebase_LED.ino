
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "kbd-41615007.firebaseio.com/" //Without http:// or https:// schemes
#define FIREBASE_AUTH "9wVHI1BUvke8YqD2jZ6WmpCT1Gb9r7i1f2KjtI0A"
#define WIFI_SSID "SK_WiFi86A9"
#define WIFI_PASSWORD "1506031342"

//Debug Blynk to serial port
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Auth token for your Blynk app project
#define BLYNK_AUTH "1ktXQMVZ7WjcL6qLTWM0CdT9tbPUkbMN"

//Define FirebaseESP8266 data objects
FirebaseData firebaseData1;
FirebaseData firebaseData2;


String path = "/LED_Control/LED1";
String path2 = "/LED_Control/LED2";

//D4 or GPIO2 on Wemos D1 mini
uint8_t BuiltIn_LED = 2;

int LED2 = D5;


//WidgetLED led(V2);

void setup()
{

  Serial.begin(115200);

  pinMode(BuiltIn_LED, OUTPUT);
  pinMode(LED2, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData1.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData1.setResponseSize(1024);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData2.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData2.setResponseSize(1024);


  if (!Firebase.beginStream(firebaseData1, path))
  {
    Serial.println("------------------------------------");
    Serial.println("Can't begin stream connection...");
    Serial.println("REASON: " + firebaseData1.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_PASSWORD);
}

void loop()
{
  Blynk.run();

  if (!Firebase.readStream(firebaseData1))
  {
    Serial.println("------------------------------------");
    Serial.println("Can't read stream data...");
    Serial.println("REASON: " + firebaseData1.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (firebaseData1.streamTimeout())
  {
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }

  if (firebaseData1.streamAvailable())
  {
    Serial.println("------------------------------------");
    Serial.println("Stream Data available...");
    Serial.println("STREAM PATH: " + firebaseData1.streamPath());
    Serial.println("EVENT PATH: " + firebaseData1.dataPath());
    Serial.println("DATA TYPE: " + firebaseData1.dataType());
    Serial.println("EVENT TYPE: " + firebaseData1.eventType());
    Serial.print("VALUE: ");
    if (firebaseData1.dataType() == "int")
    {

      Serial.println(firebaseData1.intData());
      if (firebaseData1.intData() == 0)
      {
        digitalWrite(BuiltIn_LED, HIGH);
        //led.off();
      }
      else if (firebaseData1.intData() == 1)
      {
        digitalWrite(BuiltIn_LED, LOW);
        //led.on();
      }
    }
    Serial.println("------------------------------------");
    Serial.println();
  }
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  Serial.println("------------------------------------");
  Serial.println("Set integer...");
  //Also can use Firebase.set instead of Firebase.setInt
  if (Firebase.setInt(firebaseData2, path, pinValue))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData2.dataPath());
    Serial.println("TYPE: " + firebaseData2.dataType());
    Serial.print("VALUE: ");
    if (firebaseData2.dataType() == "int")
      Serial.println(firebaseData2.intData());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData2.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable


  Serial.println("------------------------------------");
  Serial.println("Set integer...");
  //Also can use Firebase.set instead of Firebase.setInt
  if (Firebase.setInt(firebaseData2, path2, pinValue))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData2.dataPath());
    Serial.println("TYPE: " + firebaseData2.dataType());
    Serial.print("VALUE: ");
    if (firebaseData2.dataType() == "int")
    {

      Serial.println(firebaseData2.intData());
      if (firebaseData2.intData() == 0)
      {
        digitalWrite(LED2, LOW);
        //led.off();
      }
      else if (firebaseData2.intData() == 1)
      {
        digitalWrite(LED2, HIGH);
        //led.on();
      }
    }
      Serial.println(firebaseData2.intData());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData2.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}

