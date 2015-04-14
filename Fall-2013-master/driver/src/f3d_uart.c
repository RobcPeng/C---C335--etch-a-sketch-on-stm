/* f3d_uart.c --- 
 * 
 * Filename: f3d_uart.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Sep 26 07:05:26 2013
 * Last-Updated: 9/26/2013 
 *           By: kdkapoun & rcpeng
 *     Update #: 1
 * Last-Updated: 
 *           By: 
 *     Update #: 0
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

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

#include <stm32f30x.h>
#include <f3d_led.h>
#include <stm32f30x_misc.h>
#include <f3d_uart.h>
#include <queue.h>

//int putchar(queue_t *, int);
int getchar(void);

int putchar(int);
//int getchar(void);


static queue_t rxbuf;
static queue_t txbuf;

void f3d_uart_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
  
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC , &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOC,4,GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC,5,GPIO_AF_7);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1 ,&USART_InitStructure);
  USART_Cmd(USART1 , ENABLE);

  // Initialize the rx and tx queues

  init_queue(&rxbuf);
  init_queue(&txbuf);
  
  // Setup the NVIC priority and subpriority
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // Enable the RX interrupt 
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}


/*
int putchar(int c) {
  while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == (uint16_t)RESET);
  USART_SendData(USART1, c); 
  return c;
} 

int getchar(void) {
  int c;
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == (uint16_t)RESET);
  c = USART_ReceiveData(USART1);
  return c;
}
*/

int putchar(int c) {
  /*  while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == (uint16_t)RESET);
    enqueue(&txbuf, c);
    return 1;
  */
  //while(!enqueue(&txbuf, c));
  
  enqueue(&txbuf, c);
  USART_ITConfig(USART1,USART_IT_TXE,ENABLE); 
  return 0;
  
}

int getchar(void){
  //int c;
  // while (!(c=dequeue(&rxbuf)));
  //return c;
  //USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == (uint16_t)RESET);
  return(dequeue(&rxbuf));
  
}



void USART1_IRQHandler(void) {
  int ch; 

  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) {
    ch = USART_ReceiveData(USART1);
    while (!enqueue(&rxbuf,ch));
  }
  if (USART_GetFlagStatus(USART1,USART_FLAG_TXE)) {
    ch = dequeue(&txbuf);
    if (ch) {
      USART_SendData(USART1,ch);
    }
    else {
      USART_ITConfig(USART1,USART_IT_TXE,DISABLE); 
    }
    if (!queue_empty(&txbuf)) {
      flush_uart();
    }
  }
}


void flush_uart(void) {
  USART_ITConfig(USART1,USART_IT_TXE,ENABLE); 
}
