#include <Arduino.h>
int led = 13;
unsigned int Counter = 0;
unsigned long datalength, CheckSum, RLength;
unsigned short topiclength;
unsigned char topic[30];
char str[250];
unsigned char encodedByte;
int X;
unsigned short MQTTProtocolNameLength;
unsigned short MQTTClientIDLength;
unsigned short MQTTUsernameLength;
unsigned short MQTTPasswordLength;

const char MQTTHost[30] = "io.adafruit.com";
const char MQTTPort[10] = "1883";
const char MQTTClientID[25] = "89940102186202946542";
const char MQTTTopic[45] = "AkalankaShani/feeds/switchfeed";
const char MQTTProtocolName[10] = "MQTT";//"MQIsdp";
const char MQTTLVL = 0x03;//0x02;
const char MQTTFlags = 0xC2;
const unsigned int MQTTKeepAlive = 60;
const char MQTTUsername[30] = "AkalankaShani";
const char MQTTPassword[35] = "aio_eNnE58KWFeyE1erpxwvPht3jxheC";
const char MQTTQOS = 0x00;
const char MQTTPacketID = 0x0001;

#include <SoftwareSerial.h>
SoftwareSerial mySerial(16, 17); // RX, TX

char sim800lreset = 15;


void updateSerial(int Try)
{
  int wait_tick=Try;
  delay(1000);
 /* while(Serial.available()&& wait_tick != 0) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
    wait_tick--;
    delay(5);
  }*/
  while(wait_tick != 0) 
  {
    if(mySerial.available()){
      Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
    }
    else{
        wait_tick--;
        delay(50);
    }
    
  }
}

void SendConnectPacket(void) {
  mySerial.print("\r\nAT+CIPSEND\r\n");  delay(3000);
  updateSerial(5);
  mySerial.write(0x10); //identification cod of the Packet
  MQTTProtocolNameLength = strlen(MQTTProtocolName);
  MQTTClientIDLength = strlen(MQTTClientID);
  MQTTUsernameLength = strlen(MQTTUsername);
  MQTTPasswordLength = strlen(MQTTPassword);
  datalength = MQTTProtocolNameLength + 2 + 4 + MQTTClientIDLength + 2 + MQTTUsernameLength + 2 + MQTTPasswordLength + 2;
  X = datalength;
  do {
    encodedByte = X % 128;
    X = X / 128;
    if (X > 0) {
      encodedByte |= 128;
    }
    mySerial.write(encodedByte);
  }
  while (X > 0);
  mySerial.write(MQTTProtocolNameLength >> 8);
  mySerial.write(MQTTProtocolNameLength & 0xFF);
  mySerial.print(MQTTProtocolName);
  
  mySerial.write(MQTTLVL);// LVL
  mySerial.write(MQTTFlags);// Flags
  mySerial.write((byte)(MQTTKeepAlive >> 8));
  mySerial.write(MQTTKeepAlive & 0xFF);
  
  mySerial.write(MQTTClientIDLength >> 8);
  mySerial.write(MQTTClientIDLength & 0xFF);
  mySerial.print(MQTTClientID);
  
  mySerial.write(MQTTUsernameLength >> 8);
  mySerial.write(MQTTUsernameLength & 0xFF);
  mySerial.print(MQTTUsername);

  mySerial.write(MQTTPasswordLength >> 8);
  mySerial.write(MQTTPasswordLength & 0xFF);
  mySerial.print(MQTTPassword);
  
  mySerial.write(0x1A);//Start Sending Packet to the Server
}

void SendPublishPacket(void) {
  mySerial.print("\r\nAT+CIPSEND\r\n");  delay(3000);
  updateSerial(5);
  memset(str, 0, 250);
  topiclength = sprintf((char * ) topic, MQTTTopic);
  datalength = sprintf((char * ) str, "%s%u", topic, Counter);
  delay(1000);
  mySerial.write(0x30);
  X = datalength + 2;
  do {
    encodedByte = X % 128;
    X = X / 128;
    if (X > 0) {
      encodedByte |= 128;
    }
    mySerial.write(encodedByte);
  }
  while (X > 0);
  mySerial.write(topiclength >> 8);
  mySerial.write(topiclength & 0xFF);
  mySerial.print(str);
  mySerial.write(0x1A);
}
void SendSubscribePacket(void) {
  mySerial.print("\r\nAT+CIPSEND\r\n");  delay(3000);
  updateSerial(5);
  memset(str, 0, 250);
  topiclength = strlen(MQTTTopic);
  datalength = 2 + 2 + topiclength + 1;
  delay(1000);
  mySerial.write(0x82);
  X = datalength;
  do {
    encodedByte = X % 128;
    X = X / 128;
    if (X > 0) {
      encodedByte |= 128;
    }
    mySerial.write(encodedByte);
  }
  while (X > 0);
  mySerial.write((byte)(MQTTPacketID >> 8));
  mySerial.write(MQTTPacketID & 0xFF);
  mySerial.write(topiclength >> 8);
  mySerial.write(topiclength & 0xFF);
  mySerial.print(MQTTTopic);
  mySerial.write(MQTTQOS);
  mySerial.write(0x1A);
}

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("Starting Arduino web client.");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(19200);
  pinMode(sim800lreset, OUTPUT);
  digitalWrite(sim800lreset, LOW);
  delay(10000);
  digitalWrite(sim800lreset, HIGH);
  delay(3000);
  updateSerial(5);
  updateSerial(5);
  updateSerial(5);
  updateSerial(5);
  
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial(5);
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial(5);
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial(5);
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial(5);
  mySerial.println("AT+CGATT=1"); //Check GPRS Service's Enable
  updateSerial(5);
  mySerial.println("AT+CGATT?"); //Check GPRS Service's status
  updateSerial(5);

 // mySerial.println("AT+CSTT=\"CMNET\""); delay(6000); //Check GPRS Service's status
 // updateSerial(5);
  mySerial.println("AT+CSTT=\"www.mobitel.lk\",\"\",\"\""); delay(6000); //Check GPRS Service's status
  updateSerial(5);
  mySerial.println("AT+CIPMODE=0"); delay(6000); //
  updateSerial(5);
  
  mySerial.println("AT+CIICR"); delay(10000); //bring up wireless connection
  updateSerial(5);
  mySerial.println("AT+CIFSR"); delay(10000); //get local IP adress
  updateSerial(5);
  mySerial.println("AT+CIPSTART=\"TCP\",\"io.adafruit.com\",\"1883\""); delay(5000);
  updateSerial(5);
  
  Serial.println("Send SendConnectPacket");  SendConnectPacket();  delay(5000); updateSerial(5);
  //Serial.println("Send SendSubscribePacket");  SendSubscribePacket();  delay(5000);  updateSerial(5);
  
}



void loop() // run over and over
{
   
    SendPublishPacket();  
    delay(5000);  
    updateSerial(5);
    if(Counter < 99)Counter++;

}

