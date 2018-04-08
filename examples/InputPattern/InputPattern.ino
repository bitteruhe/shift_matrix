#include <shift_matrix.h>
/*
  Connect data to pin 9,
  clock to pin 10,
  and latch to pin 11.
  Power the shift registers by connecting 5V and GND to the corresponding pins
*/
Matrix matrix(9,10,11);
Pattern pattern;
/*
  Connect a potentiometer to A0 to control the inputPin and one to A1 to control the pace
*/
int inputPin = A0;
int pacePin = A1;

void setup() {
  pinMode(inputPin, INPUT_PULLUP);
  /*
    Make a pause of 1 millisecond after each row is displayed
  */
  matrix.setPace(1);
}

void loop() {
  // The input of quadPat must be between 1 and 0, therefore you need to map the output of analogRead to that field.
  float percentage = (1.0*analogRead(inputPin))/1023.0;
  float pace = ((1.0*analogRead(pacePin))/1023.0) * 1000;
  matrix.setPace(pace);
  matrix.multiplex(pattern.quadPat(percentage));
  matrix.tick();
}
