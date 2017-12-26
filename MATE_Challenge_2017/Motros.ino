
#include <Servo.h>

Servo esc;
int s = 1500;
int c;
int pin = 10;
void setup() {
  esc.attach(pin); 
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
}

void loop(){
  //esc.writeMicroseconds(s);
  if (Serial.available() > 0) {
    //Serial.println(Serial.read());
    c = Serial.read();
    if(s > 1000 && c == 49){s -= 50;}
    if(s < 2000 && c == 50){s += 50;}
    if(c == 51){s=1500;}
    Serial.println(s);
    esc.writeMicroseconds(s);    
  }
  //analogWrite(pin, s);
}

