/*
  SD card file dump
 
 This example shows how to read a file from the SD card using the
 SD library and send it over the serial port.
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11, pin 7 on Teensy with audio board
 ** MISO - pin 12
 ** CLK - pin 13, pin 14 on Teensy with audio board
 ** CS - pin 4, pin 10 on Teensy with audio board
 
 created  22 December 2010
 by Limor Fried
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */

#include <SD.h>
#include <SPI.h>
#include <TFLI2C.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

TFLI2C tflI2C;
int16_t  tfDist;    // distance in centimeters
int16_t  tfAddr = TFL_DEF_ADR;  // Use this default I2C address

Adafruit_AlphaNum4 afficheur1 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 afficheur2 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 afficheur3 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 afficheur4 = Adafruit_AlphaNum4();


const int chipSelect = 4;

char data[200];
char mot[16];
int compteur=0;
int separator=0;
int compteur_start=0;
int longueur_mot=0;
File dataFile;
int data_size;
int temps=0;
boolean capteur = false;
boolean capteur_check=false;


void setup()
{

  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect.
  }

Wire.begin();

pinMode(6,INPUT);
  Serial.print("Initializing SD card...");
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  // open the file.
  dataFile = SD.open("DATA.TXT");
  data_size=dataFile.size();

  // if the file is available, write to it:

  if (dataFile) {
    for( int x=0;x<data_size;x++){
      data[x]=dataFile.read();
      Serial.println(int(data[x]));
    }
    dataFile.close();
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

afficheur1.begin(0x70);
afficheur1.clear();
afficheur2.begin(0x71);
afficheur2.clear();
afficheur3.begin(0x72);
afficheur3.clear();
afficheur4.begin(0x73);
afficheur4.clear();

for (int x=0;x<16;x++){
  mot[x]= ' ';
}


if (digitalRead(6)==HIGH){
  capteur=true;
}
   
}

void loop()
{

  int voltage_bat = analogRead(A0);
  //  Serial.println(voltage_bat);


    if((tflI2C.getData(tfDist, tfAddr))&&(capteur==true)){
      if (tfDist<120){
        Serial.println("YA QQN!!!");
        capteur_check=true;
      }
      else{
        Serial.println("sonper..........");
        capteur_check=false;

  afficheur1.writeDisplay();
  afficheur2.writeDisplay();
  afficheur3.writeDisplay();
  afficheur4.writeDisplay();

        
      }
       // Serial.println(String(tfDist)+" cm / " + String(tfDist/2.54)+" inches");
    }



  

if (voltage_bat > 700){
if (capteur==false){
while(compteur< data_size){
while (data[compteur]!=10){
  
  if (data[compteur]=='/'){
    separator=compteur;
    Serial.print("separator ");
    Serial.println(separator);
  }

  compteur++;
}

longueur_mot=separator-compteur_start;
int compteur_mot=0;

for(int x=0;x<int((16-longueur_mot)/2);x++){
  Serial.print(' ');
  mot[compteur_mot]=' ';
  compteur_mot++;
}
for (int x=compteur_start; x< compteur_start+longueur_mot;x++){
  Serial.print(char(data[x]));
  mot[compteur_mot]=char(data[x]);
  compteur_mot++;
}

int compteur_temps=0;
for (int x=compteur-2;x>separator;x--){
  


  int puissance = pow(10,compteur_temps);
  if (puissance>10){
    puissance++;

  }

  
temps += (int((data[x])-48)*puissance);
compteur_temps++;

}



    afficheur1.writeDigitAscii(0, mot[0]);
    afficheur1.writeDigitAscii(1, mot[1]);
    afficheur1.writeDigitAscii(2, mot[2]);
    afficheur1.writeDigitAscii(3, mot[3]);
    afficheur1.writeDisplay();

    afficheur2.writeDigitAscii(0, mot[4]);
    afficheur2.writeDigitAscii(1, mot[5]);
    afficheur2.writeDigitAscii(2, mot[6]);
    afficheur2.writeDigitAscii(3, mot[7]);
    afficheur2.writeDisplay();


    afficheur3.writeDigitAscii(0, mot[8]);
    afficheur3.writeDigitAscii(1, mot[9]);
    afficheur3.writeDigitAscii(2, mot[10]);
    afficheur3.writeDigitAscii(3, mot[11]);
    afficheur3.writeDisplay();


    afficheur4.writeDigitAscii(0, mot[12]);
    afficheur4.writeDigitAscii(1, mot[13]);
    afficheur4.writeDigitAscii(2, mot[14]);
    afficheur4.writeDigitAscii(3, mot[15]);
    afficheur4.writeDisplay();













Serial.println();
delay(temps*1000);
for (int x=0;x<16;x++){
  mot[x]= ' ';
}
afficheur1.clear();
afficheur2.clear();
afficheur3.clear();
afficheur4.clear();


temps=0;
compteur++;
compteur_start=compteur;

}



compteur=0;
compteur_start=0;
}
else{


while((compteur< data_size)&&(capteur_check==true)){
while (data[compteur]!=10){
  
  if (data[compteur]=='/'){
    separator=compteur;
    Serial.print("separator ");
    Serial.println(separator);
  }

  compteur++;
}

longueur_mot=separator-compteur_start;
int compteur_mot=0;

for(int x=0;x<int((16-longueur_mot)/2);x++){
  Serial.print(' ');
  mot[compteur_mot]=' ';
  compteur_mot++;
}
for (int x=compteur_start; x< compteur_start+longueur_mot;x++){
  Serial.print(char(data[x]));
  mot[compteur_mot]=char(data[x]);
  compteur_mot++;
}

int compteur_temps=0;
for (int x=compteur-2;x>separator;x--){
  


  int puissance = pow(10,compteur_temps);
  if (puissance>10){
    puissance++;

  }

  
temps += (int((data[x])-48)*puissance);
compteur_temps++;

}



    afficheur1.writeDigitAscii(0, mot[0]);
    afficheur1.writeDigitAscii(1, mot[1]);
    afficheur1.writeDigitAscii(2, mot[2]);
    afficheur1.writeDigitAscii(3, mot[3]);
    afficheur1.writeDisplay();

    afficheur2.writeDigitAscii(0, mot[4]);
    afficheur2.writeDigitAscii(1, mot[5]);
    afficheur2.writeDigitAscii(2, mot[6]);
    afficheur2.writeDigitAscii(3, mot[7]);
    afficheur2.writeDisplay();


    afficheur3.writeDigitAscii(0, mot[8]);
    afficheur3.writeDigitAscii(1, mot[9]);
    afficheur3.writeDigitAscii(2, mot[10]);
    afficheur3.writeDigitAscii(3, mot[11]);
    afficheur3.writeDisplay();


    afficheur4.writeDigitAscii(0, mot[12]);
    afficheur4.writeDigitAscii(1, mot[13]);
    afficheur4.writeDigitAscii(2, mot[14]);
    afficheur4.writeDigitAscii(3, mot[15]);
    afficheur4.writeDisplay();













Serial.println();
delay(temps*1000);
for (int x=0;x<16;x++){
  mot[x]= ' ';
}
afficheur1.clear();
afficheur2.clear();
afficheur3.clear();
afficheur4.clear();


temps=0;
compteur++;
compteur_start=compteur;

}



compteur=0;
compteur_start=0;







  
}





}
else{



    afficheur1.writeDigitAscii(0, ' ');
    afficheur1.writeDigitAscii(1, ' ');
    afficheur1.writeDigitAscii(2, ' ');
    afficheur1.writeDigitAscii(3, ' ');
    afficheur1.writeDisplay();

    afficheur2.writeDigitAscii(0, ' ');
    afficheur2.writeDigitAscii(1, '.');
    afficheur2.writeDigitAscii(2, '.');
    afficheur2.writeDigitAscii(3, '.');
    afficheur2.writeDisplay();


    afficheur3.writeDigitAscii(0, '.');
    afficheur3.writeDigitAscii(1,'.');
    afficheur3.writeDigitAscii(2, '.');
    afficheur3.writeDigitAscii(3, ' ');
    afficheur3.writeDisplay();


    afficheur4.writeDigitAscii(0, ' ');
    afficheur4.writeDigitAscii(1, ' ');
    afficheur4.writeDigitAscii(2, ' ');
    afficheur4.writeDigitAscii(3,' ');
    afficheur4.writeDisplay();


  
}





}
