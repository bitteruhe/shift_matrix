#include "Arduino.h"
#include "shift_matrix.h"

/*
Define the three pins for data, clock and latch clock.
Have a look at the Readme if you do not know what these pins are for.
*/
Matrix::Matrix(int dataPin, int clockPin, int latchPin)
{
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  _dataPin = dataPin;
  _clockPin = clockPin;
  _latchPin = latchPin;

  _pause = 1;
}

/*
The multiplex function overwrites the current pattern. Notice that it does not
display the pattern. For that, use the "tick" function
*/
void Matrix::multiplex(String data[]){
  _data = data;
}

/*
By default, the multiplexing pace is set to one.
A pattern is displayed row by row. Because of its speed, it looks like all rows
are shining at the same time.
The input of this function puts a delay (the unit is milliseconds) after each row.
*/
void Matrix::setPace(int pause = 1){
  _pause = pause;
}

/*
This function inverts the pattern.
*/
void Matrix::setInverted(boolean inv){
  _inv = inv;
}

/*
Because displaying a pattern on an 8 by 8 LED matrix takes 16 bytes of output,
quickly shutting it of is rather slow. This method only sends out 2 bytes to
shut it down and therefore it is rather fast.
*/
void Matrix::clearAll(){
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clockPin, LOW);
  digitalWrite(_latchPin, LOW);

  for(byte j = 0;j<16;j++){
    digitalWrite(_clockPin, LOW);
    digitalWrite(_dataPin, LOW);
    digitalWrite(_clockPin, HIGH);
  }
  digitalWrite(_latchPin, HIGH);
  digitalWrite(_clockPin, LOW);
}

/*
This function is the heart of the program. Its algorithm sends 16 bytes to the
shift registers and thereby displays the current pattern on the matrix.

Notice that this works without the use of any built in timers of the Arduino.
This means that you need to call this function regularly so that the user does
not notice any flickering. I recommend you to call it at least 30 times a second.

I decided to code it that way so that it does not come to any complications with,
e.g., the Servo library or the VirtualWire library.
Both libraries use one of the Arduino's timers and therefore could not be used
with this library.

The code for this function originates from the "ShiftOut" tutorial by Carly Maw and Tom Igoe (2006, https://www.arduino.cc/en/Tutorial/ShiftOut).
It was heavily modified since then, as you may notice.
*/
void Matrix::tick(){
  int pinState;

  //Prepare sending out data by pulling all pins to low
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clockPin, LOW);
  // Iterate over all eight rows
  for(byte i = 0;i<8;i++){
    //Pull the latch pin to low to signal that new data is coming in
    digitalWrite(_latchPin, LOW);
    String output = _data[i];
    // Send out row _i_
    for(byte j = 0;j<8;j++){

      // Data must be sent to the register on the rising edge of the clock,
      // therefore the clock must be first pulled to low
      digitalWrite(_clockPin, LOW);

      // Depending on the _inv variable, the pin is either set to 1 or 0.
      if(output.charAt(j)=='1'){
        pinState = _inv;
      } else {
        pinState = !_inv;
      }

      // Set the dataPin accordingly
      digitalWrite(_dataPin, pinState);
      // and then create the rising edge for the clock
      digitalWrite(_clockPin, HIGH);

      digitalWrite(_dataPin, LOW);
    }
    /*
      This is the loop for the second shift register.
      It selects which row should be activated and sends out one byte in "one hot encoding".
      The selected row matches the row pulled from the data array above.
    */
    for(byte j = 0;j<8;j++){
      digitalWrite(_clockPin, LOW);
      if(j==i){
        pinState = 1;
      } else {
        pinState = 0;
      }
      digitalWrite(_dataPin, pinState);
      digitalWrite(_clockPin, HIGH);
      digitalWrite(_dataPin, LOW);
    }

    // To tell the shift registers that all bytes were sent, the latch pin is now pushed to high.
    digitalWrite(_latchPin, HIGH);
    delay(_pause);
  }
  digitalWrite(_clockPin, LOW);
  /* Because it could take some time until the tick function is called again,
     the matrix would display the last row a bit longer than the other ones.
     This results in one brighter row. This is prevented by shutting the matrix off after
     all rows were displayed
  */
  clearAll();
}
