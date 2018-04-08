#include "Arduino.h"
#include "shift_matrix.h"

Binary64::Binary64(String data[] = NULL, boolean startAtMax = false){
  if(data != NULL){
    _pat = data;
  } else {

    _pat = new String[8];
    for(byte i = 0; i< 8;i++){
      if(!startAtMax)
        _pat[i] = "00000000";
      else
        _pat[i] = "11111111";
    }
  }
}

/*
  The Arduino can not calculate with 64 bit integers. But we can display them anyway.
  For this, a pattern is created which can be updated with some little logic.
  This function iterates over an 64 bit little-endian array.

  All bits that are set to 1 will be set to 0 until there is a bit that is already set to 0.
  This one will be replaced by 1 and the function stops.
*/
String * Binary64::inc(){
  boolean done = false;
  for( byte b = 0; b < 8 && !done; b++){
      for( byte c = 0; c < 8; c++){
        if(_pat[b].charAt(c) == '0'){
          _pat[b].setCharAt(c, '1');
          done = true;
          break;
        } else {
          _pat[b].setCharAt(c, '0');
        }
      }
    }
  return _pat;
}

/*
  Have a look at inc(). It is essentially the same, only the break is located differently.

*/
String * Binary64::dec(){
  boolean done = false;
  for( byte b = 0; b < 8 && !done; b++){
      for( byte c = 0; c < 8; c++){
        if(_pat[b].charAt(c) == '0'){
          _pat[b].setCharAt(c, '1');

        } else {
          _pat[b].setCharAt(c, '0');
          done = true;
          break;
        }
      }
    }
  return _pat;
}

String * Binary64::inc(unsigned long by){
  for(unsigned long i = 0;i<by;i++){
    this->inc();
  }
  return _pat;
}

String * Binary64::dec(unsigned long by){
  for(unsigned long i = 0;i<by;i++){
    this->dec();
  }
  return _pat;
}
