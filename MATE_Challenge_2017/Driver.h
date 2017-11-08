// Information needed to transmit
#include "Component.h"

class Driver {
  public:
    Driver(int&* pins, int sizeOfPins) : Component(int&* pins, int sizeOfPins) { };

    // Strictly used for the single thread problem
    virtual void init(int INTERVAL) = 0;
    virtual void update( ) = 0;
}
