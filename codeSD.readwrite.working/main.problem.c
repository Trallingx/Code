#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "spi.h"
#include "sdcard.h"
#include "sdprint.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


#define BAUD_RATE 57600

int main(void)
{
    uint8_t sdBuf[512],token;


    uart_init();
    io_redirect();
    SPI_init(SPI_MASTER | SPI_FOSC_128 | SPI_MODE_0);

    // initialize sd card
    if(SD_init() != SD_SUCCESS)
    {
        printf("Error initializaing SD CARD\r\n"); while(1);
    }
    else
    {
        printf("SD Card initialized\r\n\n");
    }
    
     double array[10];

    for(int i=0;i<10;i++){
        array[i]=1.1;
    }
    
    for(int i=0;i<10;i++){
        printf("number %d = %f\n",i,array[i]);
    }

    // read Bootsector
    SD_readSingleBlock(0, sdBuf, &token);
    printf("\n\n");
    for(int i=0;i<10;i++){
        printf("number %d = %f\n",i,array[i]);
    }

    return 1;
}
