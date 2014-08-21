//Master Script for SPPB Balance Test 
//Authors: Nicholas Marco, Yicheng Bai
//Last Updated: 7/7/2014

#include "Wire.h"
#include "SoftwareSerial.h"

SoftwareSerial WiFlySerial(5,4); //RX  TX
SoftwareSerial portTwo(9,8);
int buttonPin = 2; 
int buzzer = 7;  
int RFIDResetPin = 13;

boolean flag_RFID = true;
boolean first_time = true;
boolean flag_check = false;
boolean flag_test = false;


int i,num,curtime,stime,ttime = 0;
int threshold = 10000;


void setup(void)
{
  Wire.begin();
  Serial.begin(9600);
  portTwo.begin(9600);
  WiFlySerial.begin(9600);
  portTwo.listen();
  pinMode(RFIDResetPin, OUTPUT);
  digitalWrite(RFIDResetPin, HIGH);
  
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,0);
}



void WiFiSend(String data_send)
{
  WiFlySerial.print(data_send);
  WiFlySerial.println("");
  WiFlySerial.println("");
}

void WiFiSend(unsigned int data_send)
{ 
  WiFlySerial.print(data_send);
  WiFlySerial.println("");
  WiFlySerial.println("");
}

//Function to read RFID and send through WIFI
void new_rfid()
{
  char tagString[13];
  int index = 0;
  boolean reading = false;
  while(portTwo.available()){
    int readByte = portTwo.read(); //read next available byte

    if(readByte == 2) reading = true; //begining of tag
    if(readByte == 3) reading = false; //end of tag

    if(reading && readByte != 2 && readByte != 10 && readByte != 13){
      //store the tag
      tagString[index] = readByte;
      tagString[12] = '\0';
      index ++;
       if(index ==12){
        Serial.println(tagString);
 
        WiFiSend(tagString);       
        delay(400);
        flag_RFID = false;
        }
     }
  }
  clearTag(tagString);
  resetReader(); //reset the RFID reader
}

void clearTag(char one[]){
///////////////////////////////////
//clear the char array by filling with null - ASCII 0
//Will think same tag has been read otherwise
///////////////////////////////////
  for(int i = 0; i < strlen(one); i++){
    one[i] = 0;
  }
}
void resetReader(){
///////////////////////////////////
//Reset the RFID reader to read again.
///////////////////////////////////
  digitalWrite(RFIDResetPin, LOW);
  digitalWrite(RFIDResetPin, HIGH);
  delay(150);
}


void loop(void)
{
 flag_RFID = true;
 
 //Checks for RFID tag and transmits
 while (flag_RFID == true)
 {
   new_rfid();  
 }
  
  
  delay(1000);
  

}
