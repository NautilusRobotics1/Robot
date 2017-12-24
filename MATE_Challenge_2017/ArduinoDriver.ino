
/*
 * MOTE_Challenge_2017 
 * 
 * Created as part of a local competition to create a remotely operated robot that satisfies three criteria {Aircraft, Earthquakes, and Energy}
 * Team "Nautilus"
 * 
 * David Chau
 * Caleb Yang
 * 
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
void sendInfo() {

  Accelerometer *accelerometer = Sensors::getAccelerometer();
  if(accelerometer) {

      Vector3 a = accelerometer->getAcceleration();
      Serial.print("Acceleration (m/s^2)  ");
      Serial.print(a.x);
      Serial.print(", ");
      Serial.print(a.y);
      Serial.print(", ");
      Serial.println(a.z);
  }

  Barometer *barometer = Sensors::getBarometer();
  if(barometer) {

      float p = barometer->getPressure();
      Serial.print("Pressure (hPa)        ");
      Serial.println(p);

      float a = barometer->getAltitude();
      Serial.print("Altitude (m)          ");
      Serial.println(a);
  }

  Gyroscope *gyroscope = Sensors::getGyroscope();
  if(gyroscope) {

      Vector3 g = gyroscope->getRotation();
      Serial.print("Rotation (rad/s)      ");
      Serial.print(g.x);
      Serial.print(", ");
      Serial.print(g.y);
      Serial.print(", ");
      Serial.println(g.z);
  }

  Magnetometer *magnetometer = Sensors::getMagnetometer();
  if(magnetometer) {

      Vector3 m = magnetometer->getMagneticField();
      Serial.print("Magnetic Field (uT)   ");
      Serial.print(m.x);
      Serial.print(", ");
      Serial.print(m.y);
      Serial.print(", ");
      Serial.println(m.z);

      float azimuth = magnetometer->getAzimuth();
      Serial.print("Azimuth (deg)         ");
      Serial.println(azimuth);
  }

  Thermometer *thermometer = Sensors::getThermometer();
  if(thermometer) {

      float t = thermometer->getTemperature();
      Serial.print("Temperature (C)       ");
      Serial.println(t);
  }

  delay(50);
}

// Baseline code recieved from: http://www.instructables.com/id/Connect-Your-Raspberry-Pi-and-Arduino-Uno/
// Sensor code recieved from: Sensor example




