#include "SoftwareSerial.h"
#include<Servo.h>

Servo S1;

int pirPin = 2;         //Define pir pin         
int pirStat = 0;

const int EchoPin = 10;   //Define ultrasonic pin
const int TrigPin = 8;
 
int led1 = 11;      // Define leds
int led2 = 12;
int led3 = 13;

SoftwareSerial mySerial(7,6);

bool once = false;

void setup()
{
  S1.attach(9);
  
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  
  mySerial.begin(9600);
  Serial.begin(9600);
  Serial.println("Initializing...");
  delay(500);

  mySerial.println("AT");                 // Sends an ATTENTION command, reply should be OK
  maincode();
  mySerial.println("AT+CMGF=1");          // Configuration for sending SMS
  maincode();
  mySerial.println("AT+CNMI=1,2,0,0,0");  // Configuration for receiving SMS
  maincode();
}
long Duration, Distance;

void loop()
{
  maincode();
  
}

void maincode()
{

 pirStat = digitalRead(pirPin); 
 if (pirStat == HIGH) {              //Human detection
  S1.attach(9);
  Serial.println("Human Detected, Dustbin open...");
  S1.write(0);
 }
 if(pirStat == LOW)
 {
  S1.write(500);
  Serial.println("Dustbin close...");
 }

   //Ultrasonic value measuring
  digitalWrite(TrigPin, LOW);  
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH); 
  delayMicroseconds(5);
  digitalWrite(TrigPin, LOW);
  Duration = pulseIn(EchoPin, HIGH);  
  Distance = ((Duration*0.034)/2);


  if ( Distance>=0 && Distance <= 5){
      
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      Serial.println("Dustbin is full");
      // Sending SMS
      if (once == false)
      {
      Serial.println("Sending SMS.....");
      sendSMS();
      Serial.println("SMS sent....");
      once = true;
      }
      
   }

   if(Distance>6 && Distance <=15){
      
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      Serial.println("Dustbin is 50% full");
      
  } 
 
   if(Distance>15 && Distance <=30){
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      Serial.println("Dustbin is Empty");
      
  }
     
}

  void sendSMS(){ //SMS sending function
  S1.detach();
  mySerial.println("AT+CMGF=1");
  delay(500);
  
  mySerial.println("AT+CMGS=\"01793192906\"\r"); // Number where SMS will send
  
  delay(500);
  mySerial.print("Dustbin is full!");
  delay(500);
  mySerial.write(26);
  S1.attach(9);
  }
