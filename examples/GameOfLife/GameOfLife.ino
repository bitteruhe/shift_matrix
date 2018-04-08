#include <shift_matrix.h>
/*
  This is a small, fun program about the Game Of Life by John Conway (https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
  Open the serial monitor to enter a starting pattern and then watch as it develops life on the LED matrix.

  Setup:
  Connect data to pin 9,
  clock to pin 10,
  and latch to pin 11.
  Power the shift registers by connecting 5V and GND to the corresponding pins

  Connect a potentiometer to A0 to control the speed of the single generations
*/
Matrix matrix(9,10,11);

String* pat;
String* start_pat;
String* new_pat;

int interval = 0;

byte inputPin = A0;

void setup() {
  matrix.setPace(1);
  pinMode(inputPin, INPUT_PULLUP);


  pat = new String[8];
  start_pat = new String[8];
  new_pat = new String[8];
  for(byte b = 0;b<8;b++){
    pat[b] = "00000000";
    new_pat[b] = "00000000";
  }

  // Some setup script for getting a starting patern from the user

  // Uncomment all Serial output commands to display the matrix on the terminal
  Serial.begin(9600);
  Serial.println("Please define the starting pattern.");
  Serial.println("Enter eight characters for each row.");
  Serial.println("Each character shall be either '1' (alive) or '0' (dead).");
  for(byte i = 0;i<8;i++){
    while(Serial.available() < 8){}

    start_pat[i] = Serial.readString();
    pat[i] = start_pat[i].substring(0,8);

    Serial.println("You entered '" + pat[i] + "' for row " + (i+1));
  }
  Serial.println("Thank you! Your game of life will start now");
}

void loop() {
  int pace = (int)(1.0*analogRead(inputPin)/10);
  if(interval >=pace)
    if(gameOfLife()){
      Serial.println("No cells are left. Enter 'YES' if you want to replay.");
      while(Serial.available() < 3){}
      String answer = Serial.readString();
      answer.toUpperCase();
      if(answer.equals("YES")){
        for(byte i = 0;i<8;i++){
          pat[i] = start_pat[i].substring(0,8);
        }
      }
    }

  interval = ++interval % pace+1;
  // Write the current pattern to the matrix object
  matrix.multiplex(pat);
  // Flush the pattern to the shift registers
  matrix.tick();
}



boolean gameOfLife(){
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
      //Serial.print(new_pat[i].charAt(j));
    }
    //Serial.println();
  }
  for(byte i = 0;i<8;i++){
    for(byte j = 0;j<8;j++){
      pat[i].setCharAt(j, new_pat[i].charAt(j));
    }
  }
  //Serial.println();

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
