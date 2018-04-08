# Shift Matrix
This is a library for easier control of LED matrices. Using shift registers, only three pins of an Arduino are required to display all possible patterns and animations.  
This library only supports 8 by 8 matrices at the moment.

## Purpose
This library was mainly developed to give quick progress feedback in some projects of mine.
There exist several functions that animate transitions to display a number between 0% and 100%.

## Structure
### Matrix
The matrix class is responsible for sending data to the matrix. The datatype must be a String array of length 8, where each String has 8 characters.
```C
Matrix matrix(5,6,7);
String* pattern = new String[8];
// Each String represents one row in the matrix.
// The char '0' means that this LED is turned off and '1' means it is turned on.
pattern[0] = "01010101";
pattern[1] = "10101010";
pattern[2] = "01010101";
pattern[3] = "10101010";
pattern[4] = "01010101";
pattern[5] = "10101010";
pattern[6] = "01010101";
pattern[7] = "10101010";
// Send this pattern to the matrix class.
matrix.multiplex(pattern);
// And flush it to the shift registers.
matrix.tick();
```
_multiplex()_ and _tick()_ are the main functions of the matrix class. _tick()_ must be called
very regularly, at least 30 times a second, to prevent to high flickering.
You can extend the behaviour of the matrix class with the following functions:
```C
Matrix matrix(5,6,7);
// This turns every LED on if its corresponding char is set to '0' and off if its set to '1'.
matrix.setInverted(true);
// Define a delay (in microseconds) that should pass after each row is sent to the registers.
// Higher values result in higher flickering.
matrix.setPace(2);
// This function quickly pulls all parallel output pins of the registers to GND, resulting in a
// deactivated matrix.
matrix.clearAll();
```
### Pattern
The pattern class provides several functions for easier creation of patterns.
Most functions are used to display some sort of progress. All such functions are listed below, each one takes a float between _0.0f_ and _1.0f_ as input.
```C
String* quadPat(float percentage);
String* circlePat(float percentage);
String* snakePat(float percentage);
String* barsPat(float percentage);
String* randomPat(float percentage);
// Unlike the other, this function displays a 16 bit number in binary code:
String* binaryPat(unsigned int number);
```
Here is an example for using those functions:
```C
Matrix matrix(5,6,7);
Pattern pattern;

for(float f = 0.0f; f<PI*2;f+=0.02f){
  float y = cos(PI+f)/2+0.5f;
  matrix.multiplex(pattern.quadPat(y));
  matrix.tick();
}
```

### Binary64
As the Arduino is restricted in its size for integers and longs, you can not make use of big numbers for calculations.  
With this class you can, at least, display those and increment or decrement them.  
As the matrix has 64 LEDs, you can display numbers between zero and _2^64_ in binary.

You can play around with its functions ``String * inc();`` and ``String * dec();`` like so:
```C
Matrix matrix(5,6,7);
Binary64 bin;

while(true){
  matrix.multiplex(bin.inc());
  matrix.tick();
}
```

## Examples
You find some example sketches in _examples/_.  
You will also find a mini version of the [Game Of Life by John Conway](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life), fully working with the LED matrix as display. It is a good example for the variety of things you can use such matrix for.

## Hardware setup
This library was developed and tested on the Arduino Nano. The shift registers in use are two 74HC595N ICs. Those receive data over a serial connection to the Arduino.  

If you do not know how to wire all things up, have a look at [this example](https://www.arduino.cc/en/Tutorial/ShiftOut). The wiring I use is exactly the same as in that tutorial, I just connect the registers to a matrix instead of the 16 LEDs.

There exist other solutions for controlling LED matrices, such as [Adafruit's matrix](https://www.adafruit.com/product/870) which communicates through the I²C protocol.
