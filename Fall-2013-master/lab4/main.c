/* main.c --- 
 * 
 * Filename: main.c
 * Description: Lab4 main.c
 * Author: Kevin Kapoun & Robert Peng
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 9/21/2013
 *           By: kdkapoun & rcpeng
 *     Update #: 1
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

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) { 
  int button_state;
  int i=1;    
  f3d_led_init();
  while (1)
    {
      button_state = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
      
      if (button_state == 0){    
	//  GPIOE->BSRR = GPIO_Pin_9;
	//      delay();
	// f3d_led_on(2);
	
	for(i=1; i<=7; i++){
	  while( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){ f3d_led_on(i);
	    delay();}
	  f3d_led_on(i);
	  while( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){ delay();}
	  delay();
	  f3d_led_off(i);
	}
	f3d_led_on(0);
	delay();
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) {
	  f3d_led_on;}
	f3d_led_off(0);
	delay();
	f3d_led_all_on();
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
	  f3d_led_all_on;}
	delay();
	f3d_led_all_off();
      }
    }
  f3d_led_on(i);
}
    
  

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
