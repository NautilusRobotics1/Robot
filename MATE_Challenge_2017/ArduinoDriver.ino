
/*
 * MOTE_Challenge_2017 
 * 
 * Created as part of a local competition to create a remotely operated robot that satisfies three criteria {Aircraft, Earthquakes, and Energy}
 * Team "Nautilus"
 * 
 * David Chau
 * Caleb Yang
 * 
 * Notes:
 *  Baseline code recieved from: http://www.instructables.com/id/Connect-Your-Raspberry-Pi-and-Arduino-Uno/
 *  Sensor code recieved from: Sensor example
 */

#include "Arduino.h"
#include "Wire.h"

#include "Accelerometer.h"
#include "Barometer.h"
#include "Gyroscope.h"
#include "Magnetometer.h"
#include "Sensors.h"
#include "Vector3.h"

char receivedChar;
boolean newData = false;

void setup(){

  // Activate serial port
  Serial.begin(9600);

  // Activate i2c
  Wire.begin();

  // Initialize devices
  Sensors::initialize();
}

void loop() {

  recvInfo();
  sendInfo();
  
}

// Used to interface with motors
void recvInfo() {

  // TODO: Modify for motor controls
  if (Serial.available() > 0) {

      receivedChar = Serial.read();
      newData = true;
  }
  
}

// Used to interface with sensors
/*
 * Serial.print vs Serial.write: http://forum.arduino.cc/index.php?topic=42603.0
 * Serial.print: Prints a string
 * Serial.print(value, BYTE): method will handle non-byte size inputs, sending multiple bytes as required
 * Serial.write(): method will handle only byte size inputs
 */
void sendInfo() {

  Accelerometer *accelerometer = Sensors::getAccelerometer();
  if(accelerometer) {

      Vector3 a = accelerometer->getAcceleration();

      Serial.print(a.x, BYTE);
      Serial.print(a.y, BYTE);
      Serial.print(a.z, BYTE);
  }
  else {
  	sendnNull(3);
  }

  Barometer *barometer = Sensors::getBarometer();
  if(barometer) {

      float p = barometer->getPressure();
      Serial.print(p, BYTE);

      float a = barometer->getAltitude();
      Serial.print(a, BYTE);
  }
  else {
  	sendnNull(3);
  }

  Gyroscope *gyroscope = Sensors::getGyroscope();
  if(gyroscope) {

      Vector3 g = gyroscope->getRotation();
      Serial.print(g.x, BYTE);
      Serial.print(g.y, BYTE);
      Serial.print(g.z, BYTE);
  }
  else {
  	sendnNull(3);
  }


  Magnetometer *magnetometer = Sensors::getMagnetometer();
  if(magnetometer) {

      Vector3 m = magnetometer->getMagneticField();
      Serial.print(m.x, BYTE);
      Serial.print(m.y, BYTE);
      Serial.print(m.z, BYTE);

      float azimuth = magnetometer->getAzimuth();
      Serial.print(azimuth, BYTE);
  }
  else {
  	sendnNull(4);
  }

  Thermometer *thermometer = Sensors::getThermometer();
  if(thermometer) {

      float t = thermometer->getTemperature();
      Serial.print(t, BYTE);
  }
  else {
  	sendnNull(1);
  }

  delay(50);
}

void sendnNull(int &n) {
  while(n-- > 0) Serial.print(float(NULL), BYTE)
}


