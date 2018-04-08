#include <shift_matrix.h>

/*
  Connect data to pin 9,
  clock to pin 10,
  and latch to pin 11.
  Power the shift registers by connecting 5V and GND to the corresponding pins
*/
Matrix matrix(9,10,11);
Pattern pattern;

void setup() {
  /*
    Make a pause of 1 millisecond after each row is displayed
  */
  matrix.setPace(1);
}

void loop() {
  for(float f = 0.0f; f<PI*2;f+=0.02f){
    // Increment the pattern's progress and overwrite the pattern of the matrix with the new pattern
    float y = cos(PI+f)/2+0.5f;
    /*
      Besides "quadPat", also try out the other functions:
      - circlePat
      - snakePat
      - barsPat
      - randomPat
     */
    matrix.multiplex(pattern.quadPat(y));
    // This function displays the current pattern. Make sure that you call .tick regularly to prevent any flickering
    matrix.tick();
  }

  delay(1000);

}
