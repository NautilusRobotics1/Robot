
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

using namespace std;

// include necessary drivers
// Autonomous
//#include "SimpleNavigator.cpp"
//#include "SimplePilot.cpp
//#include "SimplePlanner.cpp"
#include "MotorDriver.cpp"
#include "CompassDriver.cpp"
#include "GyroscopeDriver.cpp"
#include "LeakDriver.cpp"
#include "ProximityDriver.cpp"
#include "TemperatureDriver.cpp"
#include "AccelerometerDriver.cpp"

#define DEBUG_MODE false

// component update intervals
#define MOTOR_UPDATE_INTERVAL 100
#define COMPASS_UPDATE_INTERVAL 0
#define GYROSCOPE_UPDATE_INTERVAL 0
#define LEAK_UPDATE_INTERVAL 0
#define PROXIMITY_UPDATE_INTERVAL 0
#define TEMPERATURE_UPDATE_INTERVAL 0
#define ACCELEROMETER_UPDATE_INTERVAL 0

// with a prescale of 1024, and preload of 100, a
// timer with 8-bit counter will overflow every 10 ms
// and update all components
#define UPDATE_TIMER_PRELOAD 100

// instantiate components (drivers, controllers, etc.)
Motor* motorDriver = new MotorDriver( );
Sensor* compass = new CompassDriver( );
Sensor* gyroscope = new GyroscopeDriver( );
Sensor* leak = new LeakDriver( );
Sensor* proximity = new ProximityDriver( );
Sensor* temp = new TemperatureDriver( );
Sensor* accelerometer = new AccelerometerDriver( );
// In SimpleNavigator, utilize any sensor information that affects navigation
Navigator* navigator = new SimpleNavigator( *compass );
Pilot* pilot = new SimplePilot( *navigator, *motorDriver );
Planner* planner = new SimplePlanner( *navigator, *pilot );

// toggle this boolean to enable/disable timer interrupt update calls
volatile bool isUpdateEnabled = true;

unsigned long startingMillis;

void setup() {

  // utilize the largest data rate
  if ( DEBUG_MODE ) Serial.begin(115200);

  // reminder Serial.print takes in BIN/OCT/DEC normally, F("**") is for strings 
  if ( DEBUG_MODE ) Serial.println( F( "Initializing components..." ) );

  // start I2C communications to allow those devices to init
  Wire.begin( );

  // init all components
  motorDriver->init( MOTOR_UPDATE_INTERVAL );
  compass->init( COMPASS_UPDATE_INTERVAL );
  gyroscope->init( GYROSCOPE_UPDATE_INTERVAL );
  leak->init( LEAK_UPDATE_INTERVAL );
  proximity->init( PROXIMITY_UPDATE_INTERVAL );
  temp->init( TEMPERATURE_UPDATE_INTERVAL );
  accelerometer->init( ACCELEROMETER_UPDATE_INTERVAL );

  if ( DEBUG_MODE ) Serial.println( F("Initializing interrupts...") );

  // configure Timer 2 interrupts to call updateAll()
  // based on example from: https://arduinodiy.wordpress.com/2012/02/28/timer-interrupts/
  noInterrupts();        // disable global interrupts while configuring timers

  // reset Timer 2 registers to known state
  TCCR2A = 0;
  TCCR2B = 0; 

  // enable Timer2 overflow interrupt
  TIMSK2 |= (1 << TOIE1);

  // Set CS10 and CS12 bits so timer is prescaled by 1024
  TCCR2B |= (1 << CS10);
  TCCR2B |= (1 << CS12);

  // preload counter to interrupt based on count
  TCNT2 = UPDATE_TIMER_PRELOAD;
  
  // re-enable global interrupts
  interrupts();

  startingMillis = millis();

  if ( DEBUG_MODE ) Serial.println( F( "Running!" ) );

}

void loop() {

  motorDriver->update( );

}

void updateAll() {

  // re-enable global interrupts
  interrupts();

  // prevent calling this method before it completes
  if ( !isUpdateEnabled ) return;
  isUpdateEnabled = false;

  // get current time for components to check last update
  unsigned long timeMillis = millis();

  // update all components, in order of priority
  motorDriver->update( );
  compass->update( );
  gyroscope->update( );
  leak->update( );
  proximity->update( );
  temp->update( );
  accelerometer->update( );
  
  // updates complete, re-enable entry
  isUpdateEnabled = true;
  
}

// this interrupt routine triggers update on all components
ISR( TIMER2_OVF_vect ) {

  updateAll();

  // reset timer preload
  TCNT2 = UPDATE_TIMER_PRELOAD;

}
