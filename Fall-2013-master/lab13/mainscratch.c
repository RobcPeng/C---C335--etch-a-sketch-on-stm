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
//#include <time.h>


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

struct bmpfile_magic {
  unsigned char magic [2];
};


struct bmpfile_header {
  uint32_t filesz ;
  uint16_t creator1 ;
  uint16_t creator2 ;
  uint32_t bmp_offset ;
};

typedef struct {
  uint32_t header_sz ;
  int32_t width ;
  int32_t height ;
  uint16_t nplanes ;
  uint16_t bitspp ;
  uint32_t compress_type ;
  uint32_t bmp_bytesz ;
  int32_t hres;
  int32_t vres;
  uint32_t ncolors ;
  uint32_t nimpcolors ;
} BITMAPINFOHEADER ;

struct bmppixel { // little endian byte order
  uint8_t b;
  uint8_t g;
  uint8_t r;
};

struct bmppixel pixel;
struct bmpfile_magic magic;
struct bmpfile_header header ;
BITMAPINFOHEADER info;


void gamee(void);
void gamestart(void);
void shake(void);

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

FATFS Fatfs;		/* File system object */
FIL Fil;		/* File object */
BYTE Buff[128];		/* File read buffer */

int colored[16][20];
const char *b[1];
int mode=0;

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
gameemenu
gamee
gamestart
main
 */
int drawpic(void) {
  char footer[20];
  int argc;
  char *argv[];
  int count=0;
  int i;
  int f;
  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  uint16_t color;
  uint8_t bpix;
  uint8_t gpix;
  uint8_t rpix;
  int x = 0;
  int y = 159;
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  int carr[128];
  int j;
  j = get_fattime();


 f_mount(0, &Fatfs);		/* Register volume work area (never fails) */
  f = f_open(&Fil, "BEAR.BMP", FA_READ);
  if (argc > 1){
    if ((f = f_open(&Fil, "BEAR.BMP", FA_READ)) == -1)
      {
	perror ("problem opening file");
	return 1;
      }
    
   
    f3d_lcd_setAddrWindow(0,0,100,100,0x02);
    
    f_read(&Fil, (void *)&pixel, 3, &br); 


    for(;;){
    
      if( rc||!br){break;}

      f_read(&Fil, &pixel, sizeof(pixel), &br);
      pixel.r&=0xF8;
      pixel.g&=0xFC;
      pixel.b&=0xF8;
      
      
      pixel.r = pixel.r >> 3;
      color |= pixel.r;
      color = color <<  6;
      pixel.g = pixel.g >> 2;
      color |= pixel.g;
      color = color << 5;
      pixel.b = pixel.b >> 3;
      color |= pixel.b;
      
    

      f3d_lcd_drawPixel(x,y,color);
      x+=1;      
      
      if(x>=128){
	x=0;
	y-=1;
      }  
    }
    printf("\nClose the file.\n");
    rc = f_close(&Fil);
    if (rc) die(rc);
    
  }
  
  while (1);
}

  



void pmenu(void){
  nunchuk_t DataN;

 f3d_lcd_fillScreen(WHITE);
 f3d_lcd_drawString(20, 10, "Welcome", 0x000, 0xFFFF); 
 f3d_lcd_drawString(20, 20, "To the Awesome", 0x000, 0xFFFF); 
 f3d_lcd_drawString(20, 30, "World of the", 0x000, 0xFFFF); 
 f3d_lcd_drawString(20, 40, "Etchasketch!", 0x000, 0xFFFF); 
 f3d_lcd_drawString(20, 60, "Play!", 0x000, 0x07FF); 
 f3d_lcd_drawString(20, 70, "Credits!!!", 0x000, 0xFFFF); 
 
 
 while (1){//change rc  
    f3d_nunchuk_read(&DataN);
    //screen modes
    //nunchuk up
 if (DataN.jy==0){
      //      f3d_lcd_fillScreen(0xFFFF);
      f3d_lcd_drawString(20, 10, "Welcome", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 20, "To the Awesome", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 30, "World of the", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 40, "Etchasketch!", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 60, "Play!", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 70, "Credits!!!", 0x000, 0x07FF);
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
      f3d_lcd_drawString(20, 40, "Etchasketch!", 0x000, 0xFFFF); 
      f3d_lcd_drawString(20, 60, "Play!", 0x000, 0x07FF); 
      f3d_lcd_drawString(20, 70, "Credits!!!", 0x000, 0xFFFF); 
      f3d_nunchuk_read(&DataN);
      if(DataN.c == 0 && DataN.z==1){
	delay(100);
	drawpic();
	//	gamestart();
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
    
    //      audioplayerInit(fck.nSamplesPerSec);
    
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
  // while(1);
}


void initfunc(void) {
  f3d_i2c1_init();
  delay(10);
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
int choice;

uint16_t colorchange(int z, uint16_t color) {
  if (z == 1) {
    if (color == 0x0000) {
      return color=0x000F;
    }
    else if (color == 0x000F) {
      return color=0x00F0; 
    }
    else if (color == 0x00F0) {
      return color=0x0F00;
    }
    else {
      return color=0x0000;
    }
  }
}
    
    


/*
void curser(nunchuk_t DataN) {
  f3d_lcd_drawPixel(x+1,y,color);
  f3d_lcd_drawPixel(x+1,y+1,color);
  f3d_lcd_drawPixel(x+1,y,color);
  f3d_lcd_drawPixel(x,y,color);
  f3d_lcd_drawPixel(x,y+1,color);
  f3d_lcd_drawPixel(x,y,color);
  f3d_lcd_drawPixel(x,y,color);
  f3d_lcd_drawPixel(x,y,color);
  f3d_lcd_drawPixel(x,y,color);
  }*/

int gamemode;

void gameemenu(void){
  nunchuk_t DataN;
  int select;
  while (1){//change rc  
    f3d_nunchuk_read(&DataN);
    //screen modes
    //nunchuk up
    if(DataN.jx<=15){
      select++;
      delay(25);    
    }
    else if (DataN.jx>=230){
      select--;
      delay(25);
    }

    //    if (DataN.jx==0){ //mode 0=menu
    if(select%3==0){  
      f3d_lcd_drawString(10, 152, "Menu", 0x000, 0x07FF); 
      f3d_lcd_drawString(50, 152, "New", 0x000, 0xFFFF);
      f3d_lcd_drawString(90, 152, "Draw", 0x000, 0xFFFF);
      f3d_nunchuk_read(&DataN);    
      if(DataN.c==0 && DataN.z==1){
	delay(100);
	//audi(woosh);
	f3d_lcd_fillScreen(WHITE);
	pmenu();	
      }
    }
   if (select%3==1){ //mode 1=back
      f3d_lcd_drawString(10, 152, "Menu", 0x000, 0xFFFF); 
      f3d_lcd_drawString(50, 152, "New", 0x000, 0xFFFF); 
      f3d_lcd_drawString(90, 152, "Draw", 0x000, 0x07FF); 
      f3d_nunchuk_read(&DataN);    
      if(DataN.c==0 && DataN.z==1){
	//  f3d_lcd_fillScreen(WHITE);
	//audi(woosh);
	f3d_lcd_drawString(90, 152, "Draw", 0x000, 0xFFFF); 
	delay(100);   
	break;
      }
   }
    //nunchuk middle
   if(select%3==2) {//mode 2=save
      f3d_lcd_drawString(10, 152, "Menu", 0x000, 0xFFFF); 
      f3d_lcd_drawString(50, 152, "New", 0x000, 0x07FF); 
      f3d_lcd_drawString(90, 152, "Draw", 0x000, 0xFFFF);
      f3d_nunchuk_read(&DataN);
   if(DataN.c==0 && DataN.z==1){
	delay(100);
	//	f3d_lcd_fillScreen(WHITE);
	//for(i;i<126;i++){
	//f3d_lcd_drawPixel(i,148,color);
	//}
	shake();
      }
      //audi(woosh);
   }
   /* if (DataN.c==1) {
   delay(20);
      gamemode=select%3; 

      if(select%3==0){

      }
      if(select%3==1){
	f3d_lcd_fillScreen(WHITE);
	break;
      }
      }*/
  }
}


void shake(void){
  //  srand(time(NULL));
  int ex, why, cee;
  int r = rand();
  //f3d_lcd_fillScreen(WHITE);
  /*  if(ex==126){
    ex=ex-60;
  }
  if(why==148){
    why=why-70;
  }
  }*/
   //srand(123);
  for (cee=1;cee==100;cee++){
    ex=rand();
    why=rand();
    //f3d_lcd_fillScreen(WHITE);
    f3d_lcd_drawPixel(ex,why,0xFFFF);
  }
}





void gamee(void){
  nunchuk_t DataN;
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
    shake();
  }

  while(1){
     f3d_nunchuk_read(&DataN);
     colorchange(DataN.z, color);


     if(DataN.c==1){
       gameemenu();
     }

    //downright good
     if ((DataN.jx==255) && (DataN.jy<=30)) {
      x++;
      y++;
      delay(10);
    } 
    
    //upright good
      else if ((DataN.jx>=230) && (DataN.jy>=230) && (DataN.jx<=255) && (DataN.jy<=255) ) {
      x++;
      y--;
      delay(10);
    }
     //downleft good
      else if((DataN.jx>=0) && (DataN.jy>=0) && (DataN.jx<=20) && (DataN.jy<=10)){
	x--;
	y++;
	delay(10);
      }
     
     //upleft good
      else if  ((DataN.jx>=0) && (DataN.jy>=230) && (DataN.jx<=10) && (DataN.jy<=255) ) { 
	y--;
	x--;
	delay(10);
      }
          //down good
      else if  (DataN.jy==0) {
	y++;
	delay(10);
      }
          //right good
      else if (DataN.jx>=230){
	x++;
	delay(10);
      }
         //up good
      else if (DataN.jy==255) {
	y--;
	delay(10);
      }
     //left good
      else if (DataN.jx==0) {
	x--;
	delay(10);
      }
     
     
     //edge checkers
    if (x>126) {
      x=126;
    }
   if (x<1) { //doesn't work
      x=1;
    } 
   if (y>148) {
      y=148;
    }
   if (y<1) {
      y=1;
    }
   //edge checkers put a 1px border around the edge, but corners break it, so we need special cases for those suckers 
    f3d_lcd_drawPixel(x,y,color);
  }
}




void gamestart(void){

  // audi("wooshout.wav");
  gamee();
  //pmenu();
}

int main(void) { 

  initfunc(); 
  
  const char *a[4];
  a[1]="thermo.wav";
  a[2]="wooshout.wav";
  a[3]="eraseout.wav"; 
  f3d_lcd_fillScreen(WHITE);
  // gamestart();
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
