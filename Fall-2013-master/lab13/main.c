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
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <f3d_dac.h>
#include <f3d_timer2.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <diskio.h>
#include <ff.h>
#include <fcntl.h>
#include <queue.h>


#define TIMER 20000
#define AUDIOBUFSIZE 128

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

FATFS Fatfs;		/* File system object */
FIL fid;		/* File object */
BYTE Buff[512];		/* File read buffer */
int ret;

struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;      
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};
void initfunc(void);
void gamee(void);
void gamemenu(void);
void gamestart(void);
int shake(void);
void cursor(int x, int y, uint16_t color);

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

int colored[16][20];

/*
ckhd
fmtck
readckhd
die
globals
pmenu
audi
init
colorchange
gamee
gamestart
main
 */

void pmenu(void){
  nunchuk_t DataN;

 f3d_lcd_fillScreen(WHITE);
 f3d_lcd_drawString(20, 10, "Welcome", 0x000, 0xFFFF); 
 f3d_lcd_drawString(20, 20, "To the Awesome", 0x000, 0xFFFF); 
 f3d_lcd_drawString(20, 30, "World of the", 0x000, 0xFFFF); 
 f3d_lcd_drawString(20, 40, "Etch-a-sketch!", 0x000, 0xFFFF); 
 f3d_lcd_drawString(20, 60, "Play!", 0x000, 0x07FF); 
 
 
 while (1){
    f3d_nunchuk_read(&DataN);
    //screen modes
    //nunchuk up
 if (DataN.jy==0){
      f3d_lcd_drawString(20, 10, "Welcome", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 20, "To the Awesome", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 30, "World of the", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 40, "Etch-a-sketch!", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 60, "Play!", 0x000, 0xFFFF); 
      f3d_nunchuk_read(&DataN);    
      if(DataN.c == 0 && DataN.z==1){
	delay(100);
	gamestart();
      } 
 }
 //nunchuk middle
 else {
   f3d_lcd_drawString(20, 10, "Welcome", 0x000, 0xFFFF); 
   f3d_lcd_drawString(20, 20, "To the Awesome", 0x000, 0xFFFF); 
   f3d_lcd_drawString(20, 30, "World of the", 0x000, 0xFFFF); 
   f3d_lcd_drawString(20, 40, "Etch-a-sketch!", 0x000, 0xFFFF); 
   f3d_lcd_drawString(20, 60, "Play!", 0x000, 0x07FF); 
   f3d_nunchuk_read(&DataN);
   if(DataN.c == 0 && DataN.z==1){
     delay(100);
     gamestart();
   }
 }
 }
}


int audi(const char *track){ //audio 
  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  int bytesread;
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

 f_mount(0, &Fatfs);
 rc = f_open(&fid, track, FA_READ); 

 if (!rc) {
   struct ckhd hd;
   uint32_t  waveid;
   struct fmtck fck;
    
   readckhd(&fid, &hd, 'FFIR');
    
   f_read(&fid, &waveid, sizeof(waveid), &ret);
   if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
     return -1;
    
   readckhd(&fid, &hd, ' tmf');
    
   f_read(&fid, &fck, sizeof(fck), &ret);
    
   // skip over extra info
    
   if (hd.cksize != 16) {
     printf("extra header info %d\n", hd.cksize - 16);
     f_lseek(&fid, hd.cksize - 16);
   }
    
   printf("audio format 0x%x\n", fck.wFormatTag);
   printf("channels %d\n", fck.nChannels);
   printf("sample rate %d\n", fck.nSamplesPerSec);
   printf("data rate %d\n", fck.nAvgBytesPerSec);
   printf("block alignment %d\n", fck.nBlockAlign);
   printf("bits per sample %d\n", fck.wBitsPerSample);
    
   // now skip all non-data chunks !
    
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }
    
    printf("Samples %d\n", hd.cksize);
    
    // Play it !
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
  printf("\nClose the file.\n"); 
  rc = f_close(&fid);
  if (rc) die(rc);
}


void initfunc(void) {
  f3d_i2c1_init();
  delay(10);
  f3d_gyro_init();
  f3d_led_init();
  f3d_uart_init();
  delay(10); 
  f3d_timer2_init();
  delay(10);
  f3d_dac_init();
  delay(10); 
  f3d_delay_init();
  delay(10); 
  f3d_rtc_init();
  delay(10);
  f3d_systick_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);
}

uint8_t x = 64;
uint8_t y = 80;
uint16_t color = 0x0000;
int choice=0;
int button_state;

    
void gamemenu(void){
  nunchuk_t DataN;
  int select;
  while (1){
    f3d_nunchuk_read(&DataN);
    //screen modes
    //nunchuk up
    if(DataN.jx<=15){
      select++;
      delay(20);    
    }
    else if (DataN.jx>=230){
      select--;
      delay(20);
      }

    if(select%3==0){  
      f3d_lcd_drawString(10, 152, "Menu", 0x000, 0x07FF); 
      f3d_lcd_drawString(50, 152, "New", 0x000, 0xFFFF);
      f3d_lcd_drawString(90, 152, "Draw", 0x000, 0xFFFF);
      f3d_nunchuk_read(&DataN);    
      if(DataN.c==0 && DataN.z==1){
	delay(100);
	f3d_lcd_fillScreen(WHITE);
	pmenu();	
      }
    }
    //mode 1=back
    if (select%3==1){
      f3d_lcd_drawString(10, 152, "Menu", 0x000, 0xFFFF); 
      f3d_lcd_drawString(50, 152, "New", 0x000, 0xFFFF); 
      f3d_lcd_drawString(90, 152, "Draw", 0x000, 0x07FF); 
      f3d_nunchuk_read(&DataN);    
      if(DataN.c==0 && DataN.z==1){
	f3d_lcd_drawString(90, 152, "Draw", 0x000, 0xFFFF); 
	delay(100);   
	break;
	shake();
      }
   }
   //nunchuk middle
   //mode 2=save
   if(select%3==2) {
      f3d_lcd_drawString(10, 152, "Menu", 0x000, 0xFFFF); 
      f3d_lcd_drawString(50, 152, "New", 0x000, 0x07FF); 
      f3d_lcd_drawString(90, 152, "Draw", 0x000, 0xFFFF);
      f3d_nunchuk_read(&DataN);
      f3d_nunchuk_read(&DataN);    
      if(DataN.c==0 && DataN.z==1){
	gamestart();
	delay(50);
      }
   }
  }
}

void cursor(int x, int y, uint16_t color) {
  /*allows us to erase in chunks, 
  also written for the head of the
  drawing if time permits*/
  int i, j;
  for (i=-5; i<5; i+=1){
    for(j=-5; j<5; j+=1){
      f3d_lcd_drawPixel(x+i,y+j,color);
    }
  } 
}

int shake(void){
  int i, ex, why;  
  audi("eraseout.wav");
  for (i=0;i<=100; i+=1){
    why = rand()%144;
    ex = rand()%125;
    cursor(ex,why,WHITE);
  }
  return 1;
}


void gamestart(void){
  audi("wooshout.wav");
  nunchuk_t DataN;
  float DataB[3]={0};
  f3d_lcd_fillScreen(WHITE);
  //menu print
  int i=0;
  
  for(i;i<126;i++){
    f3d_lcd_drawPixel(i,148,color);
  }

  f3d_lcd_drawString(10, 152, "Menu", 0x000, 0xFFFF); 
  f3d_lcd_drawString(50, 152, "New", 0x000, 0xFFFF); 
  f3d_lcd_drawString(90, 152, "Draw", 0x000, 0xFFFF);
  
  if (DataN.z == 1) {
    gamestart();
  }

  while(1){
     f3d_nunchuk_read(&DataN);
     f3d_gyro_getdata(DataB);
     if ((DataB[0]>5) || (DataB[1]>=5) || (DataB[2]>5)){
       shake();
     }  
     
     if(DataN.c==1){
       gamemenu();
     }

    //downright
     if ((DataN.jx==255) && (DataN.jy<=30)) {
       x++;
       y++;
       delay(10);
     } 
     
     //upright 
     else if ((DataN.jx>=230) && (DataN.jy>=230) && (DataN.jx<=255) && (DataN.jy<=255) ) {
       x++;
       y--;
       delay(10);
     }
     //downleft
     else if((DataN.jx>=0) && (DataN.jy>=0) && (DataN.jx<=20) && (DataN.jy<=10)){
	x--;
	y++;
	delay(10);
     }
     
     //upleft
     else if  ((DataN.jx>=0) && (DataN.jy>=230) && (DataN.jx<=10) && (DataN.jy<=255) ) { 
       y--;
       x--;
       delay(10);
     }
     //down 
     else if  (DataN.jy==0) {
       y++;
       delay(10);
     }
     //right
     else if (DataN.jx>=230){
       x++;
       delay(10);
     }
     //up 
     else if (DataN.jy==255) {
       y--;
       delay(10);
     }
     //left
     else if (DataN.jx==0) {
       x--;
       delay(10);
     }
     
     //edge checkers
     if (x>126) {
       x=126;
     }
     if (x<1) {
       x=1;
     } 
     if (y>148) {
       y=148;
     }
     if (y<1) {
       y=1;
     }
     f3d_lcd_drawPixel(x,y,color);
  }
}


int main(void) { 
  initfunc();
  f3d_lcd_fillScreen(WHITE);
  while(1){    
    pmenu();
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
