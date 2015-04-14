/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <stdio.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int mywc() /* a smaller word count for optimization of space on an embedded device */
{ //declaring variables to use as counters
int c, line, word, byte;
  line = 0;
  word = 0;
  byte = 0;
  while ((c = getchar()) != 0x1b) {
    //this checks to see if there is a new line, and adds if so   

    if (c == '\n'){
      line = line++;
      word = word++;
    }
    // this checks to see if there are spaces, if there are it assumes that means there is a new word, and adds one to the word count
    if (c == ' '){
      word = word++;
      byte = byte++;
    }
    //if the first two ifs are not met, then we can assume the char is just a char, so it adds one to the byte count
    else { 
      byte = byte++;
    }
  }
  printf("%d %d %d", line, word, byte);
  return (0);
  
}

int main(void) { 
  f3d_uart_init();
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  mywc();
 
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
