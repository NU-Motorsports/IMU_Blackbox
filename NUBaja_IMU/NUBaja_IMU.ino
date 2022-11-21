/* Original Code by DJ Walsh
 * NUBaja IMU Blackbox
 * V 1.1.2
 * 08/22/2020
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
const int SerialUSB_BAUD_RATE = 9600;
int logfile_number=1;

//IMU Variables
  float accelX = 0; // accelX is x-axis acceleration in g's
  float accelY = 0; // accelY is y-axis acceleration in g's
  float accelZ = 0; // accelZ is z-axis acceleration in g's

  float gyroX = 0; // gyroX is x-axis rotation in dps
  float gyroY = 0; // gyroY is y-axis rotation in dps
  float gyroZ = 0; // gyroZ is z-axis rotation in dps

  float magX = 0; // magX is x-axis magnetic field in uT
  float magY = 0; // magY is y-axis magnetic field in uT
  float magZ = 0; // magZ is z-axis magnetic field in uT

  int imuTime = 0;  //imuTime is the time in ms

//Pot Variables
  float frontl = 0;
  float frontr = 0;
  float rearl = 0;
  float rearr = 0;


//File Setup
  File dataFile;


/***********************************PIN LOCATION***********************************/

#define BLUE_PIN 13
#define RED_PIN 12
#define GREEN_PIN 11
#define SD_PIN 38
#define BUTTON_PIN 9
#define FL A3
#define FR A2
#define RR A1
#define RL A0


bool button_state = 0;




MPU9250_DMP imu;                  //Initialize MPU9250 object

void setup() {

  SerialUSB.begin(9600);

  //Pin modes
  pinMode(SD_PIN,OUTPUT);
  pinMode(BLUE_PIN,OUTPUT);
  pinMode(RED_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(BUTTON_PIN,INPUT);
  
  if(!SD.begin(SD_PIN)){
    digitalWrite(BLUE_PIN,LOW);
    while(true);
  }

  digitalWrite(BLUE_PIN,HIGH);

  //IMU initialization
  if (imu.begin() != INV_SUCCESS){
    digitalWrite(GREEN_PIN,HIGH);
    delay(300);
    digitalWrite(GREEN_PIN,LOW);
    delay(300);
    while(1){
      //Fails to initialize IMU, holds forever
    }
  }

  
  //IMU setup
  digitalWrite(GREEN_PIN,HIGH);
    //Enable all sensors
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
    //Full scale range
  imu.setGyroFSR(1000);                //+/- 250, 500, 1000, 2000 dps
  imu.setAccelFSR(8);                 //+/- 2, 4, 8, 16 g
    //Frequency Setup
  imu.setLPF(10);                     //188, 98, 42, 20, 10, 5 Hz
  imu.setSampleRate(50);              //4 thru 1000 Hz
  imu.setCompassSampleRate(10);       //1 thru 100 Hz


  //File directory finder
  bool ex = true;         //File exists variable
  do{
    if(SD.exists(String(logfile_number)+".txt")){
      logfile_number++;
    } else{
      ex = false;
    }
  } while(ex==true);
 SerialUSB.println("Setup Complete");   
}


void loop() {
    //LED Loop start Condition
  
  button_state = digitalRead(BUTTON_PIN);

  if(button_state == HIGH){
    digitalWrite(RED_PIN,HIGH);
    delay(500);
    do  {
    IMU_Update();
    Pot_Update();
    SerialUSB_write();

    dataFile = SD.open(String(logfile_number)+".txt", FILE_WRITE);

    dataFile_write();

    dataFile.close();

    

    button_state = digitalRead(BUTTON_PIN);

  } while(button_state==LOW);

  digitalWrite(RED_PIN,LOW);
  delay(2000);
}
}




/***********************************FUNCTIONS***********************************/

void IMU_Update() {

     //Update IMU values
  imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
    //Calculate signed, unitized values
  accelX = imu.calcAccel(imu.ax); // accelX is x-axis acceleration in g's
  accelY = imu.calcAccel(imu.ay); // accelY is y-axis acceleration in g's
  accelZ = imu.calcAccel(imu.az); // accelZ is z-axis acceleration in g's

  gyroX = imu.calcGyro(imu.gx); // gyroX is x-axis rotation in dps
  gyroY = imu.calcGyro(imu.gy); // gyroY is y-axis rotation in dps
  gyroZ = imu.calcGyro(imu.gz); // gyroZ is z-axis rotation in dps

  magX = imu.calcMag(imu.mx); // magX is x-axis magnetic field in uT
  magY = imu.calcMag(imu.my); // magY is y-axis magnetic field in uT
  magZ = imu.calcMag(imu.mz); // magZ is z-axis magnetic field in uT

  imuTime = imu.time;  //imuTime is the time in ms
  
}

void Pot_Update() {

  //Update IMU Values
  frontl = (analogRead(FL))*0.196;
  frontr = (analogRead(FR))*0.196;
  rearl = (analogRead(RL))*0.147;
  rearr = (analogRead(RR))*0.147;
}

void dataFile_write() {
  
    //Write to SD
  dataFile.print(imuTime);
  dataFile.print(", ");
  dataFile.print(accelX);
  dataFile.print(", ");
  dataFile.print(accelY);
  dataFile.print(", ");
  dataFile.print(accelZ);
  dataFile.print(", ");
  dataFile.print(gyroX);
  dataFile.print(", ");
  dataFile.print(gyroY);
  dataFile.print(", ");
  dataFile.print(gyroZ);
  dataFile.print(", ");
  dataFile.print(magX);
  dataFile.print(", ");
  dataFile.print(magY);
  dataFile.print(", ");
  dataFile.print(magZ);
  dataFile.print(", ");
  dataFile.print(frontl);
  dataFile.print(", ");
  dataFile.print(frontr);
  dataFile.print(", ");
  dataFile.print(rearl);
  dataFile.print(", ");
  dataFile.println(rearr);

}

void SerialUSB_write() {
  SerialUSB.print(imuTime);
  SerialUSB.print(", ");
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
  SerialUSB.print(magZ);
  SerialUSB.print(", ");
  SerialUSB.print(frontl);
  SerialUSB.print(", ");
  SerialUSB.print(frontr);
  SerialUSB.print(", ");
  SerialUSB.print(rearl);
  SerialUSB.print(", ");
  SerialUSB.println(rearr);
}
