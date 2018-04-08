#include <shift_matrix.h>

/*
  Connect data to pin 9,
  clock to pin 10,
  and latch to pin 11.
  Power the shift registers by connecting 5V and GND to the corresponding pins
*/
Matrix matrix(9,10,11);
Binary64 bin;

void setup() {
  /*
    Make a pause of 1 millisecond after each row is displayed
  */
  matrix.setPace(1);
}

void loop() {

    // Increment the counter and overwrite the pattern of the matrix with inc()'s output
    matrix.multiplex(bin.inc());
    // This function displays the current pattern. Make sure that you call .tick regularly to prevent any flickering
    matrix.tick();

}
