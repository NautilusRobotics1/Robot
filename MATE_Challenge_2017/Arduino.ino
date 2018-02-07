
/*
 * MOTE_Challenge_2017 
 * 
 * Created as part of a local competition to create a remotely operated robot that satisfies three criteria {Aircraft, Earthquakes, and Energy}
 * Team "Nautilus"
 * 
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

void setup(){
  // Activate serial port
  Serial.begin(9600);
  // Activate i2c
  Wire.begin();
  // Initialize devices
  Sensors::initialize();
}

/*
 *Accelerometer {x,y,z}
 *  Accelerometer *accelerometer = Sensors::getAccelerometer();
 *  Vector3 a = accelerometer->getAcceleration();
 *Barometer {pressure,altitude}
 *  Barometer *barometer = Sensors::getBarometer();
 *  float p = barometer->getPressure();
 *  float a = barometer->getAltitude();
 *Gyroscope {x,y,z} // Rotation
 *  Gyroscope *gyroscope = Sensors::getGyroscope();
 *  Vector3 g = gyroscope->getRotation();
 *Magnetometer {x,y,z,azimuth}
 *  Magnetometer *magnetometer = Sensors::getMagnetometer();
 *  Vector3 m = magnetometer->getMagneticField();
 *  float azimuth = magnetometer->getAzimuth();
 *Thermometer {temp}
 *  Thermometer *thermometer = Sensors::getThermometer();
 *  float t = thermometer->getTemperature();
*/
void loop() {
    Accelerometer *accelerometer = Sensors::getAccelerometer();
    Vector3 a = (accelerometer) ? accelerometer->getAcceleration() : Vector3();

    Barometer *barometer = Sensors::getBarometer();
    float pres = (barometer) ? barometer->getPressure() : 0;
    float alt = (barometer) ? barometer->getAltitude() : 0;

    Gyroscope *gyroscope = Sensors::getGyroscope();
    Vector3 g = (gyroscope) ? gyroscope->getRotation() : Vector3();

    Magnetometer *magnetometer = Sensors::getMagnetometer();
    Vector3 m = (magnetometer) ? magnetometer->getMagneticField() : Vector3();
    float azimuth = (magnetometer) ? magnetometer->getAzimuth() : 0;

    Thermometer *thermometer = Sensors::getThermometer();
    float temp = (thermometer) ? thermometer->getTemperature() : 0;
    
    float buffer[13] = {a.x,a.y,a.z,pres,alt,g.x,g.y,g.z,m.x,m.y,m.z,azimuth,temp};

    // https://forum.arduino.cc/index.php?topic=142830.0
    Serial.write( (uint8_t*)buffer, sizeof(buffer) );
    delay(100);
}
