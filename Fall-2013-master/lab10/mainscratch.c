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
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_delay.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <ff.h>
#include <math.h>
#include <diskio.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
//#include <bmp.h>


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

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}
struct bmppixel pixel;
struct bmpfile_magic magic;
struct bmpfile_header header ;
BITMAPINFOHEADER info;


FATFS Fatfs;		/* File system object */
FIL Fil;		/* File object */
BYTE Buff[128];		/* File read buffer */

int main(int argc, char *argv[]) { 
  char footer[20];
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

  f3d_uart_init();
  f3d_lcd_init();
  f3d_delay_init();
  f3d_rtc_init();
  f3d_led_init();
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);
  f3d_lcd_fillScreen(0xFFFF);


  f_mount(0, &Fatfs);		/* Register volume work area (never fails) */
  f = f_open(&Fil, "BEAR.BMP", FA_READ);
  if (argc > 1){
    if ((f = f_open(&Fil, "BEAR.BMP", FA_READ)) == -1)
      {
	perror ("problem opening file");
	return 1;
      }
    
    
    
    /* f_read(&Fil, (void *)&magic, 2,&br);
    printf ("Magic %c%c\n", magic.magic[0], magic.magic[1]);
    f_read(&Fil, (void *)&header, sizeof(header), &br);
    printf ("file size %d offset %d\n", header.filesz, header.bmp_offset);
    f_read(&Fil, (void *)&info, sizeof(info), &br);
    printf ("Width %d Height %d, bitspp %d\n", info.width, info.height, info.bitspp);
    */
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


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
