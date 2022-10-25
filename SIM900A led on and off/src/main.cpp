#include <Arduino.h>

#include <SoftwareSerial.h>

SoftwareSerial SIM900(16, 17); // configure software serial port
int REDLed= 19;
int GREENLed= 18;
int BLUELed= 22;

void ShowSerialData()
{
  while(SIM900.available()!=0)
    Serial.write(char (SIM900.read()));
}

void changeLed()
{
 String content = "";
// String RedState = content.substring();
 while(SIM900.available()!=0)
 {  
    //Serial.write(SIM900.read());
    content = content + String(char (SIM900.read()));
 }
    Serial.println(content);
    
String R = content.substring(28,29);
String G = content.substring(29,30);
String B = content.substring(30,31);
Serial.println(R);
Serial.println(G);
Serial.println(B);

 if(R == "1")
 {
   digitalWrite(REDLed, HIGH);
 }
 else if (R== "0")
 {
   digitalWrite(REDLed, LOW);
 }
 
 if(G == "1")
 {
   digitalWrite(GREENLed, HIGH);
 }
 else if (G == "0")
 {
   digitalWrite(GREENLed, LOW);
 }

 if(B == "1")
 {
   digitalWrite(BLUELed, HIGH);
 }
 else if (B == "0")
 {
   digitalWrite(BLUELed, LOW);
 }
 content = "";
}


void SubmitHttpRequest()
{
 
  SIM900.println("AT+CSQ"); // Signal quality check

  delay(100);
 
  ShowSerialData();// this code is to show the data from gprs shield, in order to easily see the process of how the gprs shield submit a http request, and the following is for this purpose too.
  
  SIM900.println("AT+CGATT?"); //Attach or Detach from GPRS Support
  delay(100);
 
  ShowSerialData();
  SIM900.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r");//setting the SAPBR, the connection type is using gprs
  delay(1000);
 
  ShowSerialData();
 
  SIM900.println("AT+SAPBR=3,1,\"APN\",\"www.mobitel.lk\"\r");//setting the APN, Access point name string
  delay(4000);
 
  ShowSerialData();
 
  SIM900.println("AT+SAPBR=1,1");//setting the SAPBR
  delay(2000);
ShowSerialData();

  /*SIM900.println("AT+CMEE=2"); //
  delay(2000); 
  ShowSerialData();*/

  SIM900.println("AT+HTTPTERM"); //
  delay(2000); 
  ShowSerialData();
  

  SIM900.println("AT+HTTPINIT"); //init the HTTP request
 
  delay(2000); 
  ShowSerialData();

  SIM900.println("AT+HTTPPARA=\"URL\",\"shaniakalanka.000webhostapp.com/getstate.php?color=All\"\r");// setting the httppara, the second parameter is the website you want to access
  delay(1000);
 
  ShowSerialData();
 
  SIM900.println("AT+HTTPACTION=0");//submit the request 
  delay(10000);//the delay is very important, the delay time is base on the return from the website, if the return datas are very large, the time required longer.
  //while(!SIM900.available());
 
  ShowSerialData();
 
  SIM900.println("AT+HTTPREAD");// read the data from the website you access
  delay(300);
  changeLed();
  ShowSerialData();
 
  SIM900.println("");
  delay(100);
}

void setup() {     
  
  pinMode(REDLed, OUTPUT);
  pinMode(GREENLed, OUTPUT);
  pinMode(BLUELed, OUTPUT);
  SIM900.begin(19200);
  Serial.begin(115200); 
  Serial.print("power up" );
  delay(30000); 

}


 
void loop()
{
 if (Serial.available()>0)
  Serial.println("SubmitHttpRequest - started" );
  SubmitHttpRequest();
  Serial.println("SubmitHttpRequest - finished" );
  delay(10000);

}
