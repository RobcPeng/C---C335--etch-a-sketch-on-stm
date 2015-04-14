/* main.c --- 
 * 
 * Filename: main.c
 * Description: Lab11  main.c
 * Author:Kevin Kapoun & Robert Peng
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 11/18/2013
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
#include <f3d_delay.h>
#include <f3d_systick.h> 
#include <stm32f30x_misc.h>
#include <queue.h>
#include <f3d_uart.h>


int main(void) { 
  f3d_uart_init();
  f3d_led_init(); 
  f3d_systick_init();

  int button_state;
  int i=1;    

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  while(1) {
    putchar(getchar());
  }
}



#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
