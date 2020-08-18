/* Original Code by DJ Walsh
 * NUBaja IMU Blackbox
 * V 0.0.1
 * 08/16/2020
 * 
 * A large amount of this code is inspired by the Sparkfun 9DoF Razor IMU sample firmware, availible here:
 * https://learn.sparkfun.com/tutorials/9dof-razor-imu-m0-hookup-guide?_ga=2.180723815.1742731065.1594684509-2129211768.1594259654
 * Worth a look to get some bearings and an idea of where to improve
 */

//#include <SparkFunMPU9250-DMP.h>            //MPU-9250 Library
#include <SD.h>                             //File and Library Control
#include <SPI.h>


/***********************************LOGGING CONTROL GLOBALS***********************************/

//Log Rates
const int SERIAL_BAUD_RATE = 9600;


/***********************************PIN LOCATION GLOBALS***********************************/

const int LED_PIN = 13;
const int SD_PIN = 38;



void setup() {

  //Pin modes

  pinMode(SD_PIN,OUTPUT);

  SerialUSB.begin(SERIAL_BAUD_RATE);

  while(!SerialUSB);

  SerialUSB.println("initializing SD card...");
  
  SD.begin(SD_PIN);
  
  if(!SD.begin(SD_PIN)){
    SerialUSB.println("initialization failed. things to check:");
    SerialUSB.println("1. is a card inserted? Try removing and inserting again.");
    SerialUSB.println("2. is the wiring correct?");
    SerialUSB.println("3. is the SD_PIN correct? Amtel ARM should be 38.");
    SerialUSB.println("Note: Must be restarted for any changes to take effect");
    while(true);     
  }

  SerialUSB.println("initialization done.");

}

void loop() {

  

}



/***********************************FUNCTIONS***********************************/
