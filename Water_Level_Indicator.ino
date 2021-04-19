/* Code Written by Rishi Tiwari
 *  Website:- https://tricksumo.com
 *  
 *  Libraries used:- 
                       1. Firebase arduino master => https://github.com/FirebaseExtended/firebase-arduino
                       2. Arduino JSON => https://github.com/bblanchon/ArduinoJson/tree/5.x 
                       
    Tutorials:-
                       
                                                                              => https://tricksumo.com/use-firebase-with-mit-app-inventor/
                     Connect NodeMCU to firebase                              => https://youtu.be/dLvSGPsV344
                     Interface HCSR04 Ultrasonic sensor With NodeMCU ESP8266  => https://youtu.be/li1mtgCEDwQ */


#include <ESP8266WiFi.h>
#include<FirebaseArduino.h>

#define FIREBASE_HOST "xxxxxxxxxx"           // Enter database HOST without "https:// "  and "/" at the end of URL             
#define FIREBASE_AUTH "xxxxxxxxxx"   // Real-time database secret key here

#define WIFI_SSID "xxxxxxx"            // WIFI SSID here                                   
#define WIFI_PASSWORD "xxxxxxx"        // WIFI password here

 
int level=0, motor=0, count = 0, val=0, man=0, tmp;


// define pin numbers for ultrasonic sensor

const int trigPin = D7;  
const int echoPin = D8;
const int motorPin = D5;

// defines variables for ultrasonic sensor
long duration;
int distance;

int lvlmax = 2; // maximum desired water level i.e.(tank height in cm - 2 cm) {ex. 10 - 2 = 8 cm}
int lvlmin = 8; // minimum desired water level i.e.(tank height in cm - 8 cm) {ex. 10 - 8 = 2 cm}

int lvlone = 6; // margin of level 1
int lvltwo = 5; // margin of level 2
int lvlthree = 3; // margin of level 3





void setup() {

     
  Serial.begin(9600); Serial.println("Communication Started \n\n");  

 pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT);// Sets the echoPin as an Input
pinMode(motorPin, OUTPUT);//  Sets the motorPin as an Output
 


           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());    //print local IP address

  delay(30);
   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); delay(300);       // connect to firebase
                                                            
}


  

void loop() { 

// Firebase Error Handling ************************************************
  if (Firebase.failed())
  {
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Serial.println(Firebase.error());
    delay(10);
    Serial.println("Error connecting firebase!");
    count++;
    if (count == 10) {
      count = 0;
      ESP.reset();
    }
    return;
  }

  else {
    Serial.println("Everything is ready!");
    delay(300); Serial.println("Everything is ready!");
    delay(300); Serial.println("Everything is ready!");
    delay(300);
  }


// ULTRASONIC *****************************************

// Clears the trigPin 
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 
digitalWrite(trigPin, HIGH);
delayMicroseconds(100);
digitalWrite(trigPin, LOW);

Serial.println("Trigger for 10 \n\n");  
           

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

Serial.println("duration calculated \n\n");  

Serial.println("assembling sonar\n\n");  
           

// Calculating the distance
distance= duration*0.034/2;  

// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance); delay(100);


if (distance>=lvlmin) {level=0; motor=1; Firebase.setFloat("/S_HO_C_K/level",level);  Serial.println("Water below desired level. Motor turned ON.\n\n\n"); 

motor=1; digitalWrite(D5, HIGH);  Firebase.setFloat("/S_HO_C_K/motorstatus",motor);} delay(500);

if (distance>= lvlone && distance<lvlmin) {
  if (motor==0) {level=11;}  if (motor==1) {level=31;} // checking ON - OFF at mid levels
  Firebase.setFloat("/S_HO_C_K/level",level);Serial.println("Water level 1.\n\n\n"); 
} 
delay(5);

if (distance==lvltwo) { 
   if (motor==0) {level=12;}  if (motor==1) {level=32;} 
   Firebase.setFloat("/S_HO_C_K/level",level);Serial.println("Water level 2.\n\n\n"); } 
delay(5);

if (distance>=lvlthree && distance<lvltwo) {
 if (motor==0) {level=13;}  if (motor==1) {level=33;} 
 Firebase.setFloat("/S_HO_C_K/level",level);Serial.println("Water level 3.\n\n\n"); } 
delay(5);

if (distance<=lvlmax) {level=4; Firebase.setFloat("/S_HO_C_K/level",level); 
motor=0; digitalWrite(D5, LOW);  Firebase.setFloat("/S_HO_C_K/motorstatus",motor); Serial.println("Water above desired level. Motor turned OFF.\n\n\n"); } delay(5); 

 val = Firebase.getString("/S_HO_C_K/man").toInt();

 man=val;

  // Manual option *******************************************


if (man == 1) { 
  
  while (1) { 

  
 Serial.println("Welcome to Manual Mode. Motor turned ON.\n\n\n");  delay(500);

 digitalWrite(D5, HIGH); level=2; motor=1; Firebase.setFloat("/S_HO_C_K/level",level); // turned on motor

if (man==0) {digitalWrite(D5, LOW); motor=0; Firebase.setFloat("/S_HO_C_K/man",man); delay(100); Serial.println("Exit Manual Mode. Motor turned OFF.\n\n\n"); return;}

// Calculateing distance in manual mode


// Clears the trigPin 
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 
digitalWrite(trigPin, HIGH);
delayMicroseconds(100);
digitalWrite(trigPin, LOW);

Serial.println("Trigger for 10 \n\n");  
           

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

Serial.println("Manual mode. duration calculated \n\n");  

Serial.println("Manual mode. assembling sonar\n\n");  
           

// Calculating the distance
distance= duration*0.034/2;  


if (distance>= lvlone && distance<lvlmin) {
  if (motor==0) {level=11;}  if (motor==1) {level=31;} // checking ON - OFF at mid levels
  Firebase.setFloat("/S_HO_C_K/level",level);Serial.println("Manual Mode. Water level 1.\n\n\n"); 
} 
delay(5);

if (distance==lvltwo) { 
   if (motor==0) {level=12;}  if (motor==1) {level=32;} 
   Firebase.setFloat("/S_HO_C_K/level",level);Serial.println("Manual Mode. Water level 2.\n\n\n"); } 
delay(5);

if (distance>=lvlthree && distance<lvltwo) {
 if (motor==0) {level=13;}  if (motor==1) {level=33;} 
 Firebase.setFloat("/S_HO_C_K/level",level);Serial.println("Manual Mode. Water level 3.\n\n\n"); } 
delay(5);


if (distance<=lvlmax) {
  level=4; Firebase.setFloat("/S_HO_C_K/level",level); man=0;  Firebase.setFloat("/S_HO_C_K/man",man);
motor=0; digitalWrite(D5, LOW);  Firebase.setFloat("/S_HO_C_K/motorstatus",motor); Serial.println("Manual Mode. Water above desired level. Motor turned OFF.\n\n\n");
}

tmp = Firebase.getString("/S_HO_C_K/man").toInt();
man = tmp;
  
  }
}
  
 
 }
