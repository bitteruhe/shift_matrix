#ifndef Binary64_h
#define Binary64_h
#ifndef Pattern_h
#define Pattern_h
#ifndef Matrix_h
#define Matrix_h


#include "Arduino.h"

/*
This class is used to send all sorts of output to the LED matrix.
It is the center of this library.
Use Pattern and Binary64 to make use of some predefined output.
*/
class Matrix
{
  public:
    Matrix(int dataPin, int clockPin, int latchPin);
    void multiplex(String data[]);
    void clearAll();
    void tick();
    void setPace(int pause = 1);
    void setInverted(boolean inv);
  private:
    int _dataPin;
    int _clockPin;
    int _latchPin;
    int _pause;
    boolean _inv;
    String* _data;
};
/*
This class offers some predefined patterns for the matrix.
Functions that have "percentage" input create some sort of progress bar.
All public functions can directly be placed into Matrix.multiplex().
*/
class Pattern
{
  public:
    Pattern();
    void resetPattern();
    String* quadPat(float percentage);
    String* circlePat(float percentage);
    String* snakePat(float percentage);
    String* barsPat(float percentage);
    String* randomPat(float percentage);
    String* binaryPat(unsigned int number);
  private:
    String* _pat;
    char quadHelper(float x, float y, float percentage);
    char circleHelper(float x, float y, float percentage);
    char snakeHelper(float x, float y, float percentage);
    char barsHelper(float x, float y, float percentage);
    char binaryHelper(float x, float y, unsigned int number);
};

/*
As Arduino only supports 16 bit numbers, this class can be of good use when it
comes to displaying 64 bit numbers. Notice that you can not calculate with such
numbers and Binary64's functions can only increase and decrease one step at a time.
*/
class Binary64
{
  public:
    Binary64(String data[] = NULL, boolean startAtMax = false);
    String* inc();
    String* inc(unsigned long by);
    String* dec();
    String* dec(unsigned long by);

    String* _pat;
};

#endif
#endif
#endif
