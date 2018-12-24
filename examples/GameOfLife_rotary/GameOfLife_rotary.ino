#include <RotaryEncoder.h>

#include <OneButton.h>

#include <shift_matrix.h>
/*
  This is a small, fun program about the Game Of Life by John Conway (https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

  Setup:
  Connect data to pin 9,
  clock to pin 10,
  and latch to pin 11.
  Power the shift registers by connecting 5V and GND to the corresponding pins
  Connect a rotary encoder to pin A2 and A3 and a button (GND) to A1
*/
Matrix matrix(9,10,11);
OneButton button(A1, true);
RotaryEncoder encoder(A2, A3);

String* pat;
String* start_pat;
String* new_pat;
String* old_pat;

int interval = 0;
byte x = 0;
byte y = 0;
bool orientation = false;
bool started = false;
int rotPos = 0;

void setup() {
  matrix.setPace(0);

  button.attachClick(singleClickSetup);
  button.attachDoubleClick(doubleClickSetup);
  button.attachLongPressStart(longPressSetup);
  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT9);
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);
  pat = new String[8];
  start_pat = new String[8];
  new_pat = new String[8];
  old_pat = new String[8];
  for(byte b = 0;b<8;b++){
    pat[b]       = "00000000";
    start_pat[b] ="00000000";
    new_pat[b]   = "11111111";
    old_pat[b]   = "11111111";
  }
  prestart();
}

void prestart(){
  for(byte i = 0;i<8;i++){
    pat[i] = start_pat[i].substring(0,8);
  }
  while(!started){
    int newPos = encoder.getPosition();
    if (rotPos != newPos) {
      pat[x].setCharAt(y, start_pat[x].charAt(y));
      int inc_ = 1;
      if(newPos<rotPos){
        inc_ = -1;
      }
      rotPos = newPos;
      if(orientation){
        x = (x+inc_)%8;
      } else {
        y = (y+inc_)%8;
      }
    }
    matrix.multiplex(pat);
    matrix.tick();
    delay(20);
    if(pat[x].charAt(y)=='1'){
      pat[x].setCharAt(y, '0');
    } else {
      pat[x].setCharAt(y, '1');
    }
    matrix.multiplex(pat);
    matrix.tick();
  }
}

void restart(){
  delay(1000);
  for(byte i = 0;i<8;i++){
    pat[i] = start_pat[i].substring(0,8);
  }
}

void loop() {
  if(started){
    int pace = 2*encoder.getPosition()+1;
    if(pace<0){
      pace = 0;
      encoder.setPosition(pace);
    }
    if(interval >=pace){
      if(gameOfLife() || isSame()){
        restart();
      }
    }
    interval = ++interval % pace+1;

    matrix.multiplex(pat);
    matrix.tick();
  } else {
    started = false;
    prestart();
  }
}

boolean gameOfLife(){
  for(byte i = 0;i<8;i++){
    for(byte j = 0;j<8;j++){
      old_pat[i].setCharAt(j, pat[i].charAt(j));
    }
  }
  boolean emp = true;
  for(byte i = 0;i<8;i++){
    for(byte j = 0;j<8;j++){

      int n = getNeighbours(i,j);

      // The game rules:
      if(n<=1 || n>=4){
        new_pat[i].setCharAt(j, '0');
      } else if(n==3){
        new_pat[i].setCharAt(j, '1');
        emp = false;
      } else {
        char c = pat[i].charAt(j);
        if(c=='1'){
          emp = false;
        }
        new_pat[i].setCharAt(j, c);
      }
    }
  }
  for(byte i = 0;i<8;i++){
    for(byte j = 0;j<8;j++){
      pat[i].setCharAt(j, new_pat[i].charAt(j));
    }
  }
  return emp;
}

// This function counts all alive neighbours of a given cell
int getNeighbours(byte i, byte j){
  int counter  = 0;
  if(pat[(i+7)%8].charAt((j+7)%8) == '1'){
    counter++;
  }
  if(pat[(i+7)%8].charAt(j) == '1'){
    counter++;
  }
  if(pat[(i+7)%8].charAt((j+9)%8) == '1'){
    counter++;
  }
  if(pat[i].charAt((j+7)%8) == '1'){
    counter++;
  }
  if(pat[i].charAt((j+9)%8) == '1'){
    counter++;
  }
  if(pat[(i+9)%8].charAt((j+7)%8) == '1'){
    counter++;
  }
  if(pat[(i+9)%8].charAt(j) == '1'){
    counter++;
  }
  if(pat[(i+9)%8].charAt((j+9)%8) == '1'){
    counter++;
  }
  return counter;
}

bool isSame(){
  for(byte i = 0;i<8;i++){
    for(byte j = 0;j<8;j++){
      if(pat[i].charAt(j)!=old_pat[i].charAt(j)){
        return false;
      }
    }
  }
  return true;
}

void singleClickSetup(){
  orientation = !orientation;
}

void doubleClickSetup(){
  if(!started){
    if(start_pat[x].charAt(y)=='1'){
      start_pat[x].setCharAt(y,'0');
    } else {
      start_pat[x].setCharAt(y,'1');
    }
    pat[x].setCharAt(y, start_pat[x].charAt(y));
    matrix.multiplex(pat);
  }
}

void longPressSetup(){
  started = !started;
}


ISR(PCINT1_vect) {
  encoder.tick();
  button.tick();
}
