#include "Arduino.h"
#include "shift_matrix.h"

/*
  This class provides example patterns for the matrix.
  All functions that take "percentage" as input can be used as some sort of progress bar.
  The percentage shall be a float between zero and one.



*/
Pattern::Pattern()
{
  
  _pat = new String[8];
  for(byte i = 0; i< 8;i++){
    _pat[i] = "00000000";
  }
}

void Pattern::resetPattern(){
	free(_pat);
  _pat = new String[8];
  for(byte i = 0; i< 8;i++){
    _pat[i] = "00000000";
  }
}

String* Pattern::quadPat(float percentage){

  for(byte i = 0;i<8;i++){
      for(byte j = 0;j<8;j++){
         _pat[i].setCharAt(j, quadHelper( j, i, percentage));
      }
    }
  return _pat;

}
char Pattern::quadHelper(float x, float y, float percentage){
  if(x<3.5 && y<3.5){
     x = abs(x - 3.5)/8;
     y = abs(y - 3.5)/8;

     if(pow(x,2)+pow(y,2)>=pow(percentage*4,2)){
       return '0';
     } else {
       return '1';
     }
   } else if(x<3.5 && y>3.5){
     x = abs(x - 3.5)/8;
     y = abs(y - 3.5)/8;

     if(pow(x,2)+pow(y,2)>=pow((percentage-0.5)*4,2) || percentage<=0.5){
       return '0';
     } else {
       return '1';
     }
   } else if(x>3.5 && y>3.5){
     x = abs(x - 3.5)/8;
     y = abs(y - 3.5)/8;
     if(pow(x,2)+pow(y,2)>=pow((percentage-0.25)*4,2) || percentage<=0.25){
       return '0';
     } else {
       return '1';
     }
   } else if(x>3.5 && y<3.5){
     x = abs(x - 3.5)/8;
     y = abs(y - 3.5)/8;

     if(pow(x,2)+pow(y,2)>=pow((percentage-0.75)*4,2) || percentage<=0.75){
       return '0';
     } else {
       return '1';
     }
   }
   return '0';
}


String* Pattern::circlePat(float percentage){
  for(byte i = 0;i<8;i++){
      for(byte j = 0;j<8;j++){
         _pat[i].setCharAt(j, circleHelper( j, i, percentage));
      }
    }
  return _pat;

}
char Pattern::circleHelper(float x, float y, float percentage){
   x = abs(x - 3.5)/5;
   y = abs(y - 3.5)/5;

   if(pow(x,2)+pow(y,2)>=pow(percentage,2)){
     return '0';
   } else {
     return '1';
   }
}

String* Pattern::snakePat(float percentage){
  for(byte i = 0;i<8;i++){
      for(byte j = 0;j<8;j++){
         _pat[i].setCharAt(j, snakeHelper( j, i, percentage));
      }
    }
  return _pat;

}
char Pattern::snakeHelper(float x, float y, float percentage){
  if((int)y%2==0){
      x = 7 - x;
  }
  if(percentage * 64 <= y * 8 + x){
   return '0';
  } else {
   return '1';
  }
}

String* Pattern::barsPat(float percentage){
  for(byte i = 0;i<8;i++){
      for(byte j = 0;j<8;j++){
         _pat[i].setCharAt(j, barsHelper( j, i, percentage));
      }
    }
  return _pat;

}
char Pattern::barsHelper(float x, float y, float percentage){
  if(percentage * 64 <= y * 8 + x){
   return '0';
  } else {
   return '1';
  }
}


String* Pattern::binaryPat(unsigned int number){
  for(byte i = 0;i<8;i++){
      for(byte j = 0;j<8;j++){
         _pat[i].setCharAt(j, binaryHelper( j, i, number));
      }
    }
  return _pat;

}
char Pattern::binaryHelper(float x, float y, unsigned int number){
  int k = (int) x + 8*y;
  if((number & ( 1 << k )) >> k == 1){
    return '1';
  } else {
    return '0';
  }
}

String* Pattern::randomPat(float percentage){
  for(byte i = 0;i<8;i++){
      for(byte j = 0;j<8;j++){
        if(random(0,99)<=percentage*99)
          _pat[i].setCharAt(j, '1');
         else
          _pat[i].setCharAt(j, '0');
      }
    }
  return _pat;
}


String* Pattern::doubleDecPat(int number){
	// -
	if (number < 0) {
		number = 0 - number;
		_pat[7].setCharAt(2, '1');
	}
	else {
		_pat[7].setCharAt(2, '0');
	}
	// first digit
	if (number >= 0) {
		doubleDecHelper((number / 10)%10);
		for (byte i = 1; i<4; i++) {
			for (byte j = 0; j<5; j++) {
				_pat[7-i].setCharAt(j, _digit[j].charAt(i-1));
			}
		}
	}
	 //second digit
	doubleDecHelper(number % 10);
	for (byte i = 5; i<8; i++) {
		for (byte j = 0; j<5; j++) {
			_pat[7-i].setCharAt(j, _digit[j].charAt(i-5));
		}
	}
	
  return _pat;

}
void Pattern::doubleDecHelper(int number){
	if (_digit == nullptr) {
		_digit = new String[5];
	}
	switch (number) {
	case 0:
		_digit[0] = "111";
		_digit[1] = "101";
		_digit[2] = "101";
		_digit[3] = "101";
		_digit[4] = "111";
		break;
	case 1:
		_digit[0] = "001";
		_digit[1] = "011";
		_digit[2] = "001";
		_digit[3] = "001";
		_digit[4] = "001";
		break;
	case 2:
		_digit[0] = "111";
		_digit[1] = "001";
		_digit[2] = "111";
		_digit[3] = "100";
		_digit[4] = "111";
		break;
	case 3:
		_digit[0] = "111";
		_digit[1] = "001";
		_digit[2] = "011";
		_digit[3] = "001";
		_digit[4] = "111";
		break;
	case 4:
		_digit[0] = "100";
		_digit[1] = "101";
		_digit[2] = "111";
		_digit[3] = "001";
		_digit[4] = "001";
		break;
	case 5:
		_digit[0] = "111";
		_digit[1] = "100";
		_digit[2] = "111";
		_digit[3] = "001";
		_digit[4] = "111";
		break;
	case 6:
		_digit[0] = "111";
		_digit[1] = "100";
		_digit[2] = "111";
		_digit[3] = "101";
		_digit[4] = "111";
		break;
	case 7:
		_digit[0] = "111";
		_digit[1] = "001";
		_digit[2] = "001";
		_digit[3] = "001";
		_digit[4] = "001";
		break;
	case 8:
		_digit[0] = "111";
		_digit[1] = "101";
		_digit[2] = "111";
		_digit[3] = "101";
		_digit[4] = "111";
		break;
	case 9:
		_digit[0] = "111";
		_digit[1] = "101";
		_digit[2] = "111";
		_digit[3] = "001";
		_digit[4] = "111";
		break;
	default:
		_digit[0] = "000";
		_digit[1] = "000";
		_digit[2] = "000";
		_digit[3] = "000";
		_digit[4] = "000";
		break;
	}
}
