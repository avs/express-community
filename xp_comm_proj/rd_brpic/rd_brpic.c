
/*************************************************************************/
/**** THIS PROGRAM IS AN AVS/EXPRESS MODULE PROGRAMMED IN C.        ******/
/**** THE MODULE IS BASED ON THE MODULE WRITTEN BY C. PUDNEY IN     ******/
/**** 1997, THIS MODULE HOWEVER BYTE SWAPS SO IT CAN BE USED BY     ******/
/**** BY PEOPLE ON PLATFORMS OTHER THAN THE PC AND DEC ALPHA.       ******/ 
/**** THE MODULE READS THE BIORAD-PIC FILE TO GET THE DIMENSIONS    ******/
/**** SO THAT THE FILE CAN BE READ IN BY A FILE ACCESS MODULE.      ******/
/****                                                               ******/
/****                                                               ******/
/**** THIS WAS DEVELOPED BY JO LENG IN 1999.                        ******/
/****                                                               ******/
/*************************************************************************/


#include "xp_comm_proj/rd_brpic/gen.h"

  /* The single bytes of a word (16 bit) */
    #define LOWBYTE(w)  (  (w)    &0x00FF )
    #define HIGHBYTE(w) ( ((w)>>8)&0x00FF )

  /* b1 b0 -> b0 b1: Note the "+" has a higher precedence to the"<<"  */
    #define SWAPWORD(w) ( (LOWBYTE(w)<<8) + HIGHBYTE(w) )


int
read_pic_update(OMobj_id Read_Pic_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   char  *filename = NULL;
   int  dims_size = 0;
   int *dims = NULL;

   /*char word;*/
   short datapoint;
   int i;
   FILE *fp_img;


   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get filename's value */
   if (OMget_name_str_val(Read_Pic_id, OMstr_to_name("filename"), &filename, 0) != 1)
      filename = NULL;


   /***********************/
   /* Function's Body     */
   /***********************/


  /*OPEN THE FILE*/ 


    if((fp_img= fopen(filename,"rb")) == NULL) {
       char buf[200];
       sprintf( buf, "cannot open file %s for reading\n", filename);
       
	   ERRerror("ReadPicData", 1, ERR_ORIG, buf);
       if (filename)
          free(filename);

       return 0;
    }




   /**********************************************/
   /*EXPORT FILE  DIMS                 ***********/
   /**********************************************/

   /*set number of elements in array dims with dims = ....*/
 
   dims_size = 3;
   dims = (int *)ARRalloc(NULL, DTYPE_INT, dims_size, NULL);

   for(i=0; i<dims_size; i++)
    {
      dims[i]=0;  /*fill in array dims*/

      fread(&datapoint, 2, 1, fp_img);

#ifdef AVS_BIGENDIAN
      datapoint = SWAPWORD(datapoint);
#endif

      dims[i] = datapoint;
    }



   OMset_name_array(Read_Pic_id, OMstr_to_name("dims"), DTYPE_INT, (void *)dims, 
                    dims_size, OM_SET_ARRAY_FREE);





   /*CLOSE THE FILE*/

   fclose(fp_img);  /*close the image file*/



   /*************************/
   /*  Free input variables */
   /*************************/
   if (filename)
      free(filename);

   return(1);
}

