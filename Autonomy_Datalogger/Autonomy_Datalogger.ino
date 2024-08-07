/*
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library.

  The circuit:
   analog sensors on analog ins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created  24 Nov 2010
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>
#include <virtuabotixRTC.h>

const int chipSelect = 4;
int lastMinStamp;
int currentMinStamp;

virtuabotixRTC myRTC(6, 7, 8);

int measure0, measure1, measure2, measure3, measure4;
int min0 = 9999, min1 = 9999, min2 = 9999, min3 = 9999, min4 = 9999;
int mins[] = {min0, min1, min2, min3, min4};
int measure[] = {0, 0, 0, 0, 0};

unsigned long lastMillis;
unsigned long currentMillis;

void setup() {
  // Open serial communications and wait for port to open:
  pinMode(A0, INPUT); //Pilas
  pinMode(A1, INPUT); //5V
  pinMode(A2, INPUT); //3V3
  pinMode(A3, INPUT); //Step Up : 5V y 3V3
  pinMode(A4, INPUT); //Step Up : HV y LV
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  myRTC.setDS1302Time(0, 39, 12, 4, 30, 9, 2021);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {

  lastMillis=millis();
  currentMillis = lastMillis;
  while(currentMillis - lastMillis < 60000) {
    delayMicroseconds(300);
    //Medición
    measure[0] = analogRead(A0);
    measure[1] = analogRead(A1);
    measure[2] = analogRead(A2);
    measure[3] = analogRead(A3);
    measure[4] = analogRead(A4);
    //Almacenamiento del mínimo
    for (int i=0; i<5; i++) {
      if (measure[i] - mins[i] < 0) {
        mins[i] = measure[i];
      }
    }
    currentMillis=millis();
  }
  
  // make a string for assembling the data to log:
  String dataString = "";
  dataString += getTimeStamp() + " ";

  // read three sensors and append to the string:
  for (int i = 0; i < 5; i++) {
    dataString += String(mins[i]);
    if (i < 4) {
      dataString += ",";
    }
    mins[i] = 9999;
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

}

String getTimeStamp(void) {
 myRTC.updateTime();
 String timeStamp = String(myRTC.dayofmonth) + "/" + myRTC.month + "/" + myRTC.year + " " + myRTC.hours + ":" + myRTC.minutes + ":" + myRTC.seconds;
 return timeStamp;
}
