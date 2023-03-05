 if(sdBuf[i] == 0x2a && sdBuf[i+1] == 0x0a && sdBuf[i+2] == 0x58)
        {
            k++;
            xloc= i+2;
            for(int k =0; k<12;k++)
            {
                if(sdBuf[xloc+k] == 0x59)
                {
                    xsize= k-1;
                    yloc= xloc+k;
                    for(int l=0;l<12;l++)
                    {
                        if(sdBuf[xloc+k+l]==0x44)
                        {
                            dloc=xloc+k+l;
                            sprintf(first.command[aloc],"%c%c",sdBuf[dloc+1],sdBuf[dloc+2]);//store d command
                            ysize=l-1; 
                        }
                    }
                    
                }
            }
        }