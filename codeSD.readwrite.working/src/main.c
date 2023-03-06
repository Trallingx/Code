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
    char x[1024][10]; //2D array of numbers converted to hex string
    double xx[1024];  // final coordinates

}coordinates;





int main(void)
{
    
    // array to hold responses
    uint8_t sdBuf[512],token; //watch out to not read outside of the array/buffer!!!!
    //--------------------------------|
    // Find FAT= file allocation table|
    //--------------------------------|
    unsigned int rootdirectory;
    unsigned int FAT1;
    int sectors_per_cluster;
    //------------------------|
    // Find GerberToplayer.GTL|
    //------------------------|
    unsigned int GTL_location;
    unsigned int filesize;
    int sectors_per_file;
    int N;
    //--------------------------------------|
    // Variable used to read the gerber file|
    //--------------------------------------|
    uint8_t blockA[512],blockB[512];
    char blockCombine[2048];
    coordinates first;
    int xloc,xsize,aloc=0,pos1,pos2=0;
    
    
    //----------------------------------------------------------------------------------------------------|
    int decimal=5; // decimal number of gerberfile//  Task: Extend code to check for decimal places needed|
    //----------------------------------------------------------------------------------------------------|
    
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
   
    

    //--------------------------------------|
    // Calculate the Rootdirectory          |
    // "-"stores file informations          |
    //--------------------------------------|
    /*  

    0-511 Important positions in the Bootsector of FAT32, (does not work for FAT16)
        
    12+11 = Bytes per Sector
    13 = Sectors per cluster 
    15+14 = Start for FAT1 including Reserved
    21 = Media Type
    31+30+29+28 = hidden Sectors
    39+38+37+36 = fat file in sectors
    47+46+45+44 = Root Directory Sectors
    
    rootdirectory = Start for FAT1 including reserved + (fat file in sectors )*2 
    rootdirectory = 0x(15)(14)+2*(0x(39)(38)(37)(36))
    */


   
    // read bootsector
    SD_readSingleBlock(0, sdBuf, &token);  //sector 0 is always the bootsector

    FAT1 =((sdBuf[15]<<8)|(sdBuf[14]));
    rootdirectory = FAT1+2*((sdBuf[39]<<32)|(sdBuf[38]<<16)|(sdBuf[37]<<8)|sdBuf[36]);
    sectors_per_cluster=sdBuf[13]; //needed to find filelocation

    //printf("\nThe Root directory is located at Sector %u\n\n",rootdirectory);

    
    //-----------------------------------------------|
    // read rootdirectory and find GerberToplayer.GTL|
    //-----------------------------------------------|

    SD_readSingleBlock(rootdirectory, sdBuf, &token);  

    for(int i=0;i<512;i++)
        {                 // G                      T                   L
            if(sdBuf[i] == 0x47 && sdBuf[i+1] == 0x54 && sdBuf[i+2] == 0x4c){
                //printf("\nGTL has been found");
                N= sdBuf[i+18];
                filesize= (sdBuf[i+21]<<8|sdBuf[i+20]);
                //printf("\nFilesize: %u",filesize);
                sectors_per_file = (filesize/512)+1;
                //printf("\nSectors per file: %d\n\n",sectors_per_file);
                break;
            }
        }

    //find file location
    GTL_location = (N-2)*sectors_per_cluster+rootdirectory;// N is the first cluster
    
    

    //---------|
    // read GTL|
    //---------|
    SD_readSingleBlock(GTL_location, blockA, &token); 
    SD_readSingleBlock(GTL_location+1, blockB, &token); 

    for(pos1=0;pos1<512;pos1++)
    {
        blockCombine[pos1+pos2]=blockA[pos1];
        
    }
    pos2=pos1;
      for(pos1=0; pos1<512;pos1++)
    {
        blockCombine[pos1+pos2] = blockB[pos1];
    }

    
      

    //---------------------------------------------------|
    //store values into string array and convert to float|
    //---------------------------------------------------|  
    //Logic: Scan through the array until *LSX is found which represents the end of a line, Line Space and start of x coordinates;
    //       Find X store location in array and find next y and store size of x coordinates.
    //       Combine Size of X coordinates into string, convert to float, set decimal point and increase arraylocation.
    //-----------------------------------------------------------------------------------------------------------------------------|
    //       Task: Look into findxyd.c and implement finding size of Y coordinates and storing each command at the end of line 

      for(int i=0;i<1024;i++)
    {//                         *                        LineSpace                         X       
        if(blockCombine[i] == 0x2a && blockCombine[i+1] == 0x0a && blockCombine[i+2] == 0x58 )//&& i < 512-19
        {
            xloc= i+2;
            for(int k =0; k<12;k++)
            {//                               Y
                if(blockCombine[xloc+k] == 0x59)
                    {
                        if(blockCombine[xloc+k-1]!=0x00)
                        xsize= (k-1); 
                        else xsize= k-2;
                        break;
                    }
                }
            
                if(xsize==7)
                {
                    sprintf(first.x[aloc],"%c%c%c%c%c%c%c",blockCombine[xloc+1],blockCombine[xloc+2],blockCombine[xloc+3],blockCombine[xloc+4],blockCombine[xloc+5],blockCombine[xloc+6],blockCombine[xloc+7]);
                    first.xx[aloc] = atof(first.x[aloc]); 
                }
                if(xsize==6)
                {
                    sprintf(first.x[aloc],"%c%c%c%c%c%c",blockCombine[xloc+1],blockCombine[xloc+2],blockCombine[xloc+3],blockCombine[xloc+4],blockCombine[xloc+5],blockCombine[xloc+6]);
                    first.xx[aloc] = atof(first.x[aloc]);
                }

                for(int i=0;i<decimal;i++)
                {
                    first.xx[aloc]=first.xx[aloc]/10;
                } 
                aloc++;
               
        } 
    } 

    for(int i=0; i<aloc;i++)
    {
        printf("X:%f\n",first.xx[i]);   
    }

    return 1;
}
