
#include "xp_comm_proj/rd_aiff/gen.h"


int
readAIFFfile(FILE *audioFile ,int *nChannels,int *nSamples,int *sSize,double *sRate,char **sData);

char* getAIFFerr();

int
ReadAiff_ReadAiffMods_ReadAiffData::update(OMevent_mask event_mask, int seq_num)
{
  // file_name (OMXstr read req notify)
  // Num_channels (int write)
  // Num_samples (int write)
  // Sample_size (int write)
  // Sample_rate ( double write)



  FILE*  audiofile;
  int    nchan, nsamp, ssize;
  char*  sdata;
  double srate;
  short* Sound_data;
  int    count;
  int    aiffErrNum = 0;
  char   buffer[50];


  if (!(audiofile = fopen(file_name,"rb")))
  {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Can't open input file.\n");
     return(0);
  }


  aiffErrNum = readAIFFfile(audiofile,&nchan,&nsamp,&ssize,&srate,&sdata);

    // Don't need file open any longer so close it.
  fclose(audiofile);

	if ( aiffErrNum != 1 )
	{
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Error while reading AIFF file.\n");
     sprintf(buffer, "readAIFFfile Error Code: %d\n", aiffErrNum );
     ERRverror("",ERR_NO_HEADER | ERR_PRINT, buffer);
     sprintf(buffer, "readAIFFfile Error Type: %s\n", getAIFFerr() );
     ERRverror("",ERR_NO_HEADER | ERR_PRINT, buffer);

     return(0);
	}

  Num_channels=nchan;
  Num_samples=nsamp;
  Sample_size=ssize;
  Sample_rate=srate;


  if( (ssize != 8) && (ssize !=16) )
  {
     ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Don't understand sample sizes that are not 8 or 16 bits.\n");
     return(0);
  }


  Sound_data = (short *) out_data.ret_array_ptr(OM_GET_ARRAY_WR);


  /*
    Loop through sample data and place it into Sound_data short array.
    If 8 bit data then expand it to fill 16 bits.
  */
  switch (ssize)
  {
    case 8:   for (count=0;count<nsamp;count++)
                Sound_data[count] = (sdata[count] << 8);
              break;

    case 16:  for (count=0;count<nsamp;count++)
                Sound_data[count] = ((sdata[count*2] << 8) | sdata[(count*2)+1]);
              break;
  }


  if (Sound_data)
    ARRfree(Sound_data);



  // return 1 for success
  return(1);
}

