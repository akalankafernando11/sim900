#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(16,17);

#include <String.h>
//#include <Adafruit_Sensor.h>
#include <DHTesp.h> 
 
#define DHTPIN 15
 
DHTesp dht;


void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000); 
  
}


void setup()
{
  gprsSerial.begin(19200);               // the GPRS baud rate   
  Serial.begin(115200);    // the GPRS baud rate 
 // dht.begin();
 dht.setup(DHTPIN, DHTesp::DHT11);
  delay(1000);
}

void loop()
{
      float h = dht.getHumidity();
      float t = dht.getTemperature(); 
      delay(100);   
         
      Serial.print("Temperature = ");
      Serial.print(t);
      Serial.println(" °C");
      Serial.print("Humidity = ");
      Serial.print(h);
      Serial.println(" %");    
      
   
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());
 
  gprsSerial.println("AT");
  delay(1000);
 
  gprsSerial.println("AT+CPIN?");
  delay(1000);
 
  gprsSerial.println("AT+CREG?");
  delay(1000);
 
  gprsSerial.println("AT+CGATT?");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CSTT=\"www.mobitel.lk\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=XZO4XBJCNON1CIYW&field1=" + String(t) +"&field2="+String(h);
  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 
