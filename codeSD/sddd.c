// SD Card module connections
#define   SDCARD_SPI_HW
#define   SDCARD_PIN_SELECT  PIN_D3
// End SD card module connections
#include <18F4550.h>
#fuses NOMCLR HSPLL PLL2 CPUDIV1
#use delay(clock = 48MHz)
#use fast_io(D)
#use rs232(xmit = PIN_D2, rcv = PIN_D1, baud = 9600)
#include <sdcard.c>                              // SD card diver source code
#include <fat16.c>                               // FAT16 library source file
 
const int8 *txt = "mytext.txt";                  // File name 'mytext.txt'
int8 file_data[512];
void main(){
  set_tris_d(0);                                 // Configure PORTD pins as outputs
  delay_ms(2000);
  printf("\n\r");                                // Start new line
  printf("*** Read text file from FAT16 SD card using PIC18F4550 ***");
  delay_ms(2000);
  printf("\n\r");                                // Start new line
  printf("Initializing FAT16 library ...... ");
  if(fat16_init() == 0){                         // If FAT16 file system and SD card were successfully initialized
    printf("OK!");
    delay_ms(2000);
    printf("\n\r");                              // Start new line
    printf("Opening file: 'mytext.txt'...... ");
    if(fat16_open_file(txt) == 0){
    printf("OK!");
    printf("\n\r");                              // Start new line
    while(fat16_read_data(512, file_data) == 0)  // Read file data
      printf("%s", file_data);                   // Print file data as string
    }
    else {
      printf("\n\r");                            // Start new line
      printf("File opening error!");
    }
  }
  else {                                         // Problem occured while the initialization
    printf("\n\r");                              // Start new line
    printf("Initialization error!");
  }
  printf("\n\r");                                // Start new line
  printf("*** END ***");
  while(TRUE) ;                                  // Endless loop
}