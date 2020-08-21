/* Original Code by DJ Walsh
 * NUBaja IMU Blackbox
 * V 0.0.2
 * 08/16/2020
 * 
 * A large amount of this code is inspired by the Sparkfun 9DoF Razor IMU sample firmware, availible here:
 * https://learn.sparkfun.com/tutorials/9dof-razor-imu-m0-hookup-guide?_ga=2.180723815.1742731065.1594684509-2129211768.1594259654
 * Worth a look to get some bearings and an idea of where to improve
 */

#include <SparkFunMPU9250-DMP.h>            //MPU-9250 Library
#include <SD.h>                             //File and Library Control
#include <SPI.h>


/***********************************LOGGING CONTROL GLOBALS***********************************/

//Log Rates
const int SERIAL_BAUD_RATE = 9600;


/***********************************PIN LOCATION GLOBALS***********************************/

const int LED_PIN = 13;
const int SD_PIN = 38;



MPU9250_DMP imu;                  //Initialize MPU9250 object

void setup() {

  //Pin modes
  pinMode(SD_PIN,OUTPUT);

  //SD Card initializaiton
  SerialUSB.begin(SERIAL_BAUD_RATE);

  while(!SerialUSB);
  SerialUSB.println("initializing SD card...");
  
  SD.begin(SD_PIN);
  
  if(!SD.begin(SD_PIN)){
    SerialUSB.println("initialization failed. things to check:");
    SerialUSB.println("1. is a card inserted? Try removing and inserting again");
    SerialUSB.println("2. is the wiring correct?");
    SerialUSB.println("3. is the SD_PIN correct? Amtel ARM should be 38");
    SerialUSB.println("Note: Must be restarted for any changes to take effect");
    while(true);
  }

  SerialUSB.println("initialization done.");

  //IMU initialization
  if (imu.begin() != INV_SUCCESS){
    SerialUSB.println("IMU not initialized");
    while(1){
      //Fails to initialize IMU, holds forever
    }
  }
  
  //IMU setup
  SerialUSB.println("IMU initialization success");
    //Enable all sensors
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
    //Full scale range
  imu.setGyroFSR(500);                //+/- 250, 500, 1000, 2000 dps
  imu.setAccelFSR(2);                 //+/- 2, 4, 8, 16 g
    //Frequency Setup
  imu.setLPF(10);                     //188, 98, 42, 20, 10, 5 Hz
  imu.setSampleRate(10);              //4 thru 1000 Hz
  imu.setCompassSampleRate(10);       //1 thru 100 Hz
  
}

void loop() {

    //Update IMU values
  imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
    //Calculate signed, unitized values
  float accelX = imu.calcAccel(imu.ax); // accelX is x-axis acceleration in g's
  float accelY = imu.calcAccel(imu.ay); // accelY is y-axis acceleration in g's
  float accelZ = imu.calcAccel(imu.az); // accelZ is z-axis acceleration in g's

  float gyroX = imu.calcGyro(imu.gx); // gyroX is x-axis rotation in dps
  float gyroY = imu.calcGyro(imu.gy); // gyroY is y-axis rotation in dps
  float gyroZ = imu.calcGyro(imu.gz); // gyroZ is z-axis rotation in dps

  float magX = imu.calcMag(imu.mx); // magX is x-axis magnetic field in uT
  float magY = imu.calcMag(imu.my); // magY is y-axis magnetic field in uT
  float magZ = imu.calcMag(imu.mz); // magZ is z-axis magnetic field in uT


  //Write to serial
  SerialUSB.print(accelX);
  SerialUSB.print(", ");
  SerialUSB.print(accelY);
  SerialUSB.print(", ");
  SerialUSB.print(accelZ);
  SerialUSB.print(", ");
  SerialUSB.print(gyroX);
  SerialUSB.print(", ");
  SerialUSB.print(gyroY);
  SerialUSB.print(", ");
  SerialUSB.print(gyroZ);
  SerialUSB.print(", ");
  SerialUSB.print(magX);
  SerialUSB.print(", ");
  SerialUSB.print(magY);
  SerialUSB.print(", ");
  SerialUSB.println(magZ);
  
}



/***********************************FUNCTIONS***********************************/
