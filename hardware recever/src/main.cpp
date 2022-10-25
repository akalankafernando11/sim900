#include <Arduino.h>
/*HardwareSerial Unit2(1);
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  Unit2.begin(115200,SERIAL_8N1,17,16);
}

void loop() {
  // put your main code here, to run repeatedly:
   while (Unit2.available())
  {
    char RxdChar = Unit2.read();
    Serial.print(RxdChar);
  }
}*/

#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// Declare the "link" serial port
// Please see SoftwareSerial library for detail
SoftwareSerial linkSerial(16, 17); // RX, TX

void setup() {
  // Initialize "debug" serial port
  // The data rate must be much higher than the "link" serial port
  Serial.begin(115200);
  while (!Serial) continue;

  // Initialize the "link" serial port
  // Use the lowest possible data rate to reduce error ratio
  linkSerial.begin(4800);
}
 
void loop() {
  // Check if the other Arduino is transmitting
  if (linkSerial.available()) 
  {
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    StaticJsonDocument<5000> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, linkSerial);

    if (err == DeserializationError::Ok) 
    {
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)
      Serial.print("meg = ");
      Serial.println(doc["meg"].as<String>());
     // Serial.print("value = ");
      //Serial.println(doc["value"].as<int>());
    } 
    else 
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
  
      // Flush all bytes in the "link" serial port buffer
      while (linkSerial.available() > 0)
        linkSerial.read();
    }
  }
}
