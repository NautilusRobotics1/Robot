
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


char receivedChar;
boolean newData = false;

void setup() {

  Serial.begin(9600);

  pinMode(3, OUTPUT); // Corresponding to R led
  pinMode(5, OUTPUT); // Corresponding to G led
  pinMode(6, OUTPUT); // Corresponding to B led
  
}

void loop() {

  recvInfo();
  lightLED();
  
}

// Used to interface with motors
void recvInfo() {

  if (Serial.available() > 0) {

    receivedChar = Serial.read();
    newData = true;
  }
  
}

// Used to interface with sensors
void sendInfo() {

  e
}

void lightLED() {

  int led = (receivedChar - '0');

  while(newData == true) {

    digitalWrite(led, HIGH);
    delay(2000);
    digitalWrite(led, LOW);

    newData = false;    
  }
  
}

// Baseline code recieved from: http://www.instructables.com/id/Connect-Your-Raspberry-Pi-and-Arduino-Uno/