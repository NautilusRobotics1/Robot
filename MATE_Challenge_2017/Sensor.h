#include "Driver.h"

class Sensor : Driver {
  public:
    Sensor(int&* pins, int sizeOfPins) : Driver(int&* pins, int sizeOfPins) { };

    virtual void init(int INTERVAL);
    virtual void update( );
}
