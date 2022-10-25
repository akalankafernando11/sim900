#include <Arduino.h>
#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial SIM900(16, 17); 

void sendSMS() {
  Serial.println ("Sending Message"); 
  SIM900.print("AT+CMGF=1\r"); //Sets the GSM Module in Text Mode 
  delay(1000);

  Serial.println ("Set SMS Number");   
  SIM900.println("AT+CMGS=\"+94717055267\"");   // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  delay(1000);
  
  Serial.println ("Set SMS Content"); 
  SIM900.println("Welcome to sim900A."); // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  delay(100);

  Serial.println ("Finish");   
  SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
  delay(1000);
  SIM900.println();
  Serial.println ("Message has been sent ->SMS"); 
  // Give module time to send SMS
  delay(5000); 
}

void RecieveMessage()

{

  Serial.println ("SIM900A Membaca SMS");   
  delay (1000);   
  SIM900.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS   
  delay(1000);   
  Serial.write ("Unread Message done"); 

}

  void DialCall()

 {

  Serial.println ("SIM900A is Calling");   
  delay (1000);  
  SIM900.println("ATD0717055267;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  delay(100);  
  //SIM900.println("ATH");
  //SIM900.println("ATA"); 

 }

void setup() {
  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  SIM900.begin(19200);
   Serial.begin(115200);
  // Give time to your GSM shield log on to network
  delay(20000);   
  
  // Send the SMS
  //sendSMS();
}

void loop() { 
 if (Serial.available()>0)

   switch(Serial.read())

  {

    case 's':

      sendSMS();

      break;

    case 'r':

      RecieveMessage();

      break;

       case 'c':

      DialCall();

      break;

  }
 if (SIM900.available()>0)

   Serial.write(SIM900.read());

}

