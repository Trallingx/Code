            if(blockCombine[i] == 0x2a && blockCombine[i+1] == 0x0a && blockCombine[i+2] == 0x58 )//&& i < 512-19
            {printf("Xfound ");
                xloc= i+2;
                for(int k =0; k<12;k++)
                {
                    if(blockCombine[xloc+k] == 0x59)
                    {printf("yfound %x %x %x %x %x %x\n",blockCombine[xloc+1],blockCombine[xloc+2],blockCombine[xloc+3],blockCombine[xloc+4],blockCombine[xloc+5],blockCombine[xloc+6]);
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
                if(xsize==5)
                {
                    sprintf(first.x[aloc],"%c%c%c%c%c",blockCombine[xloc+1],blockCombine[xloc+2],blockCombine[xloc+3],blockCombine[xloc+4],blockCombine[xloc+5]);
                    first.xx[aloc] = atof(first.x[aloc]);
                }
                
                printf("%f and %d and %x %x %x %x %x %x\n\n",first.xx[aloc],xsize,blockCombine[xloc+1],blockCombine[xloc+2],blockCombine[xloc+3],blockCombine[xloc+4],blockCombine[xloc+5],blockCombine[xloc+6]);
                for(int i=0;i<decimal;i++)
---------------
    int blocklenght;

    blocklenght= sectors_per_file*512;

    uint8_t blockA[512],blockB[512],blockCombine[blocklenght];
    int pos1,pos2;

    

    
    res[0] = SD_readSingleBlock(GTL_location, blockA, &token);
    res[0] = SD_readSingleBlock(GTL_location+1, blockB, &token);

    for(pos1=0; pos1<512;pos1++)
    {
        blockCombine[pos1+pos2] = blockA[pos1];
    }
    pos2=pos1;