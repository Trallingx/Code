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

void print_sector();


typedef struct
{
    char x[1024][10];
    double xx[1024];

}coordinates;


//watch out do reaed outside of the array/buffer!!!!


int main(void)
{
    
    // array to hold responses
    uint8_t res[5],sdBuf[512],token;
    //localisation
    unsigned int rootdirectory;
    unsigned int FAT1;
    int sectors_per_cluster;
    //files
    unsigned int GTL_location;
    unsigned int filesize;
    int sectors_per_file;
    int N;
    //reading
    int decimal=5; // decimal number of gerberfile//standart 5 later changed
    coordinates first;
    int xloc,aloc=0;//dsize
    int xsize;
    int linesize;
    char storebuffer[50];
    int sbloc=0;





    // initialize UART
    uart_init();

    //redirect communication to pc
    io_redirect();

    // initialize SPI
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
   
    // read Bootsector
    SD_readSingleBlock(0, sdBuf, &token);
    //print_sector(res,sdBuf,token);


    //calculate the root directory
        //0-511 in little endian
        
        //12+11 = Bytes per Sector
        //13 = Sectors per cluster 
        //15+14 = Start for FAT1 including Reserved
        //21 = Media Type
        //31+30+29+28 = hidden Sectors
        //39+38+37+36 = fat file in sectors
        //47+46+45+44 = Root Directory Sectors
    
        //rootdirectory = Start for FAT1 including reserved + (fat file in sectors )*2 
        //rootdirectory = 0x(15)(14)+2*(0x(39)(38)(37)(36))

    sectors_per_cluster=sdBuf[13]; 
    FAT1 =((sdBuf[15]<<8)|(sdBuf[14]));
    rootdirectory = FAT1+2*((sdBuf[39]<<32)|(sdBuf[38]<<16)|(sdBuf[37]<<8)|sdBuf[36]);

    printf("\nThe Root directory is located at Sector %u\n\n",rootdirectory);

    // read rootdirectory
    SD_readSingleBlock(rootdirectory, sdBuf, &token);
    //print_sector(res,sdBuf,token);
  

    for(int i=0;i<512;i++)
        {                 // G                      T                   L
            if(sdBuf[i] == 0x47 && sdBuf[i+1] == 0x54 && sdBuf[i+2] == 0x4c){
                printf("\nGTL has been found");
                N= sdBuf[i+18];
                filesize= (sdBuf[i+21]<<8|sdBuf[i+20]);
                printf("\nFilesize: %u",filesize);
                sectors_per_file = (filesize/512)+1;
                printf("\nSectors per file: %d\n\n",sectors_per_file);
                break;
            }
        }

    //find file location
    // (N-2)*sectors_per_cluster+rootdirectory

    GTL_location = (N-2)*sectors_per_cluster+rootdirectory;
    
    

    // read GTL
    int tf =0;
    int st =0;//state
    int kl =0; // if 1 print storebuffer

    for(int i=0;i<sectors_per_file;i++)
    {
    res[0] = SD_readSingleBlock(GTL_location+i, sdBuf, &token);
    //print_sector(res,sdBuf,token);
        //store x values into string array and convert to float;    
        
        for(int i=0;i<512;i++)
        {

                if(/*(tf==0) &&*/ sdBuf[i] == 0x2a && sdBuf[i+1] == 0x0a && sdBuf[i+2] == 0x58 && i < 512-19) // smaller than 493 read normal
                {//printf("Xfound ");
                    xloc= i+2;
                    for(int k=1;k<30;k++)
                    {
                        if(sdBuf[xloc+k]==0x2A)
                        {
                            linesize=k;  
                        // printf("Line %d = %d\n",aloc,linesize);
                            break;
                        }
                    }
                /*  for(int i=0;i<linesize;i++)
                    {
                        printf("%c",sdBuf[xloc+i]);
                    }
                    printf("\n\n");*/
                    aloc++;
                    st=0;
                    kl=1;
                } /*
                if(sdBuf[i] == 0x58 && i > 512-19) //bigger than 493 read into storebuffer
                {   
                    for(int k=i;k<512;k++)
                    {
                        storebuffer[sbloc]=sdBuf[k];
                        sbloc++;
                    }
                    tf=1;
                    st=0;
                    kl =1;
                
                }
                if(tf!=0)
                {
                    storebuffer[sbloc+i]=sdBuf[i];
                    if(sdBuf[i]==0x2a) 
                    {
                        tf=0;
                        aloc++;
                    }
                    for(int k=1;k<30;k++)
                    {
                        if(sdBuf[xloc+k]==0x2A)
                        {
                            linesize=k;  

                            break;
                        }
                    }
                
                } */
                if(st==0)
                {
                    printf("%d\n",aloc);
                    
                    if(kl==2&&kl==1){
                        if(linesize==19){
                            sprintf(first.x[aloc],"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",sdBuf[xloc],sdBuf[xloc+1],sdBuf[xloc+2],sdBuf[xloc+3],sdBuf[xloc+4],sdBuf[xloc+5]
                            ,sdBuf[xloc+6],sdBuf[xloc+7],sdBuf[xloc+8],sdBuf[xloc+9],sdBuf[xloc+10],sdBuf[xloc+11],sdBuf[xloc+12],sdBuf[xloc+13],sdBuf[xloc+14],sdBuf[xloc+15],sdBuf[xloc+16],sdBuf[xloc+17],sdBuf[xloc+18]);
                        }
                        if(linesize==18){
                            sprintf(first.x[aloc],"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",sdBuf[xloc],sdBuf[xloc+1],sdBuf[xloc+2],sdBuf[xloc+3],sdBuf[xloc+4],sdBuf[xloc+5]
                            ,sdBuf[xloc+6],sdBuf[xloc+7],sdBuf[xloc+8],sdBuf[xloc+9],sdBuf[xloc+10],sdBuf[xloc+11],sdBuf[xloc+12],sdBuf[xloc+13],sdBuf[xloc+14],sdBuf[xloc+15],sdBuf[xloc+16],sdBuf[xloc+17]);
                        }
                    }else{/*
                         if(linesize==19){
                            sprintf(first.x[aloc],"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",storebuffer[0],storebuffer[1],storebuffer[2],storebuffer[3],storebuffer[4],storebuffer[5],storebuffer[6],storebuffer[7],storebuffer[8],storebuffer[9]
                            ,storebuffer[10],storebuffer[11],storebuffer[12],storebuffer[13],storebuffer[14],storebuffer[15],storebuffer[16],storebuffer[17],storebuffer[18]);
                        }
                        if(linesize==18){
                            sprintf(first.x[aloc],"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",storebuffer[0],storebuffer[1],storebuffer[2],storebuffer[3],storebuffer[4],storebuffer[5],storebuffer[6],storebuffer[7],storebuffer[8],storebuffer[9]
                            ,storebuffer[10],storebuffer[11],storebuffer[12],storebuffer[13],storebuffer[14],storebuffer[15],storebuffer[16],storebuffer[17]);
                        }
                       */ kl=0;
                    }
                    
                    st=1;
                    aloc++;
                }
        }
    } 
    for(int i=0;i<aloc;i++)
    {
    printf("\n%s\n",first.x[i]);   
    }
    return 1;

}