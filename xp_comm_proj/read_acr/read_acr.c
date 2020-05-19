

/****************************************************************************/

/** This project was orriginally submitted as as AVS5 module   **/
/** but has been converted to an AVS/Express project by J Leng **/
/** of the Manchester Visualization Centre in 1998             **/

/* 
 * Module Name   read ACR_NEMA SPI (Input) (Subroutine) 
 * Author:       MARCHETTI Andrea
 * Date Created  Wed May 10 10:18:09 1995    
 *                       
 * The original outputs were                                                  
 *
 * output 0 "image" field 2D 2-space 1-vector uniform short 
 * param  0 "filename" browser "" "" ":"                     
 * End of Module Description Comments 
 *
 *
 *NOW THE OUPUTS ARE
 *dims_out[] an array of int holding the image dimensions
 *short_array[] an array of shorts holding the data
 *
 *These two components are combined into a field in the final module/macro
 */


#include "xp_comm_proj/read_acr/gen.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

  /* Macro per accedere i singoli byte di una word (16 bit) */
    #define LOWBYTE(w)  (  (w)    &0x00FF )
    #define HIGHBYTE(w) ( ((w)>>8)&0x00FF )

  /* b1 b0 -> b0 b1: Nota l'operatore "+" ha precedenza su "<<"  */
    #define SWAPWORD(w) ( (LOWBYTE(w)<<8) + HIGHBYTE(w) )

  /* Macro per accedere i singoli byte di una double word (32 bit) */
    #define FIRST(dw)   ( (dw)     &0x000000FF)
    #define SECOND(dw)  (((dw)>> 8)&0x000000FF)
    #define THIRD(dw)   (((dw)>>16)&0x000000FF)
    #define FORTH(dw)   (((dw)>>24)&0x000000FF)
    #define SWAPDOUBLEWORD(dw) ( FORTH(dw) + (THIRD(dw)<<8) + (SECOND(dw)<<16) + (FIRST(dw)<<24) )

  /*
   * Variabili Globali
   */
    FILE *input;  

int
read_acr(OMobj_id read_acr_nema_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   char  *filename = NULL;

   
   
     long  length;
     int   n;
     short ngrp,ndata;

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get filename's value */
   if (OMget_name_str_val(read_acr_nema_id, OMstr_to_name("filename"), &filename, 0) != 1)
      filename = NULL;
   

   /***********************/
   /* Function's Body     */
   /***********************/
   

   

      
  /* *****************************************/
  /* Module Compute Routine                  */
   /* *****************************************/
   

     /* No data file name */
     if (!filename)
           return(1);
   
     input=fopen(filename,"r");
     if (input==NULL)
       {
	    ERRverror("read_ACR_NEMA", ERR_WARNING, "Can't open data file %s",filename);
       /*AVSerror("Can't open data file %s",filename);*/
            return(0);
       }
     

     
     /* Scansione Elementi */
     n=1;
     while(!(feof(input)) && n>0)
     {
        n=fread(&ngrp,1,sizeof(short),input);
        n=fread(&ndata,1,sizeof(short),input);
        ngrp  = SWAPWORD(ngrp);
        ndata = SWAPWORD(ndata);

        n=fread(&length,1,sizeof(long int),input);
        length = SWAPDOUBLEWORD( length );

        dump_data(ngrp,ndata,length,read_acr_nema_id);
     }

     fclose(input);   
   


   /*************************/
   /*  Free input variables */
   /*************************/
   if (filename)
      free(filename);

   return(1);
}





/* ***********************************************************************/
/* Initialization for modules contained in this file.                    */
/* ***********************************************************************/



dump_byte(length,stringa)
         int length;
         char *stringa;
{
   unsigned char buffer[1024];
   fread(buffer,1,length,input);
   buffer[length]='\0';
   fprintf(stderr,"%s=%s\n",stringa,buffer);
}

dump_data(ngrp,ndata,length,read_acr_nema_id)
         short ngrp,ndata;
         long int length;
	 OMobj_id read_acr_nema_id;
{
 short rows,cols,bit_allocated,bit_stored,high_bit; 
 
   fprintf(stderr,"(%.4x-%.4x) ",ngrp,ndata);

   switch(ngrp)
   {
         case 0x0000:
      dump_byte(length,"Command (not supported)");
      break;
         case 0x0008:
      switch(ndata)
          {
                case 0x0010:
                 dump_byte(length,"Recognition Code");
                 break;
                case 0x0020:
                 dump_byte(length,"Date");
                 break;
                case 0x0030:
                 dump_byte(length,"Study Time");
                 break;
                case 0x0031:
                 dump_byte(length,"Series Time");
                 break;
                case 0x0032:
                 dump_byte(length,"Acquisition Time");
                 break;
                case 0x0033:
                 dump_byte(length,"Image Time");
                 break;
                case 0x0060:
                 dump_byte(length,"Modality");
                 break;
                case 0x0070:
                 dump_byte(length,"Manufacturer");
                 break;
                case 0x1090:
                 dump_byte(length,"Manufacturer Model Name");
                 break;
                default:
                 dump_byte(length,"Identifying (not supported)");
                 break;
          }
      break;
         case 0x0009:
         case 0x0019:
      switch(ndata)
          {
                case 0x0010:
                 dump_byte(length,"SPI Comments");
                 break;
                default:
                 dump_byte(length,"SPI Private Group (not supported)");
                 break;
          }
      break;
         case 0x0010:
      switch(ndata)
          {
                case 0x0010:
                 dump_byte(length,"Patient Name");
                 break;
                case 0x0020:
                 dump_byte(length,"Patient Id");
                 break;
                case 0x0030:
                 dump_byte(length,"Patient Birthdate");
                 break;
                case 0x0040:
                 dump_byte(length,"Patient Sex");
                 break;
                case 0x1010:
                 dump_byte(length,"Patient Age");
                 break;
                default:
                 dump_byte(length,"Patient: (not supported)");
                 break;
          }
      break;
         case 0x0018:
      switch(ndata)
          {
                case 0x0010:
                 dump_byte(length,"Contrast/Bolus Agent");
                 break;
                case 0x0030:
                 dump_byte(length,"Radionuclide");
                 break;
                case 0x0050:
                 dump_byte(length,"Slice Thickness");
                 break;
                case 0x0080:
                 dump_byte(length,"Repetition Time");
                 break;
                case 0x0081:
                 dump_byte(length,"Echo Time");
                 break;
                case 0x0082:
                 dump_byte(length,"Inversion Time");
                 break;
                case 0x1120:
                 dump_byte(length,"Gantry Tilt");
                 break;
                default:
                 dump_byte(length,"Acquisition (not supported)");
                 break;
          }
      break;
         case 0x0020:
      switch(ndata)
          {
                case 0x1040:
                 dump_byte(length,"Position Reference");
                 break;
                default:
                 dump_byte(length,"Relationship (not supported)");
                 break;
          }
      break;
         case 0x0028:
      switch(ndata)
          {
                case 0x0010:
                 fread(&rows,1,sizeof(short),input);
                 if( sizeof(short) != length )
                 {
                        /* si corregge la posizione del file puntatore */
                        fseek(input,length-sizeof(short),SEEK_CUR);
                        fprintf(stderr,"Length data Rows not supported\n");
                 }
                 else
                 {
                        rows=SWAPWORD(rows);
                        fprintf(stderr,"Rows=%d\n",rows);
                 }
                 break;
                case 0x0011:
                 fread(&cols,1,sizeof(short),input);
                 if( sizeof(short) != length )
                 {
                        /* si corregge la posizione del file puntatore */
                        fseek(input,length-sizeof(short),SEEK_CUR);
                        fprintf(stderr,"Length data Cols not supported\n");
                 }
                 else
                 {
                        cols=SWAPWORD(cols);
                        fprintf(stderr,"Cols=%d\n",cols);
                 }
                 break;
                case 0x0030:
                 dump_byte(length,"Pixel Size");
                 break;
                case 0x0100:
                 fread(&bit_allocated,1,sizeof(short),input);
                 if( sizeof(short) != length )
                 {
                        /* si corregge la posizione del file puntatore */
                        fseek(input,length-sizeof(short),SEEK_CUR);
                        fprintf(stderr,"Length data Bit_Allocated not supported\n");
                 }
                 else
                 {
                        bit_allocated=SWAPWORD(bit_allocated);
                        fprintf(stderr,"Bit_Allocated=%d\n",bit_allocated);
                 }
                 break;
                case 0x0101:
                 fread(&bit_stored,1,sizeof(short),input);
                 if( sizeof(short) != length )
                 {
                        /* si corregge la posizione del file puntatore */
                        fseek(input,length-sizeof(short),SEEK_CUR);
                        fprintf(stderr,"Length data Bit_Stored not supported\n");
                 }
                 else
                 {
                        bit_stored=SWAPWORD(bit_stored);
                        fprintf(stderr,"Bit_Stored=%d\n",bit_stored);
                 }
                 break;
                case 0x0102:
                 fread(&high_bit,1,sizeof(short),input);
                 if( sizeof(short) != length )
                 {
                        /* si corregge la posizione del file puntatore */
                        fseek(input,length-sizeof(short),SEEK_CUR);
                        fprintf(stderr,"Length data High_Bit not supported\n");
                 }
                 else
                 {
                        high_bit = SWAPWORD(high_bit);
                        fprintf(stderr,"High_Bit=%d\n",high_bit);
                 }
                 break;
                default:
                 dump_byte(length,"Image Presentation (not supported)");
                 break;
          }
      break;
         case 0x4000:
      dump_byte(length,"Text (not supported)");
      break;
         case 0x6000:
      dump_byte(length,"Overlay (not supported)");
      break;
         case 0x7FE0:
      switch(ndata)
          {
                case 0x0010:
                 fprintf(stderr,"Data\n");
                 read_data(rows,cols,bit_allocated,bit_stored,high_bit,read_acr_nema_id);
                 break;
                default:
                 dump_byte(length,"Pixel Data (not supported)");
                 break;
          }
      break;
         default:
      if((ngrp%2)==0)
                dump_byte(length,"Standard Group (not supported)");
      else
                dump_byte(length,"Private Group (not supported)");
      break;
   }

   return;
}


read_data(rows,cols,bit_allocated,bit_stored,high_bit,read_acr_nema_id)
         short rows,cols,bit_allocated,bit_stored,high_bit;
	 OMobj_id read_acr_nema_id;
{
   short pixel[2];        /*Qui sono messi i due pixel estratti da tre bytes */
   register int i,j;
   int dim;
   unsigned char *buffer;
   int   dims0[2];
   
   int  short_array_size = 0;
   short *short_array = NULL; 
   int  dims_out_size = 0;
   int *dims_out = NULL;    

   /***********************/
   /*  Set output values  */
   /***********************/
  

   
   dims_out_size = 2;
   dims_out = (int *)ARRalloc(NULL, DTYPE_INT, dims_out_size, NULL);
   
   dims_out[0]=cols;
   dims_out[1]=rows;
   
   OMset_name_array(read_acr_nema_id, OMstr_to_name("dims_out"), DTYPE_INT, (void *)dims_out, 
                    dims_out_size, OM_SET_ARRAY_FREE);
     
   
   
 
   short_array_size = cols*rows;
       
   short_array = (short *)ARRalloc(NULL, DTYPE_SHORT, short_array_size, NULL);
   
             /*  FILL IN THE OUTPUT FIELD DATA  Here!    */
   if( (bit_allocated==12) && (bit_stored==12) && (high_bit==11))
   {
         /* 
          * I pixel sono impacchettati: due pixel in tre bytes 
          * Si esegue uno spacchettamento ed una lettura dei pixel
          * in formato LSMS
          */
         dim = (rows * cols * 3)/2;
         buffer = (unsigned char *)malloc(dim);
         if(buffer==NULL)
	   {
	     ERRverror ("read_ACR_NEMA",ERR_WARNING,"Allocation of internal data failed"); 
                return(0);
         }
         fread(buffer,dim,sizeof(unsigned char),input);
	 	 

         for(i=0,j=0;i<dim;i+=3,j+=2)
         {
                 short_array[j]  = buffer[i]       + (((int)(buffer[i+1]&0x0F))<<8);
                 short_array[j+1] = (buffer[i+1]>>4) + (((int) buffer[i+2]      )<<4);
#ifdef DEBUG
                 fprintf(stderr,"Char1=%x Char2=%x Char3=%x\n",
                                 buffer[i],buffer[i+1],buffer[i+2]);
                 fprintf(stderr,"Pixel 1 = Char1(%d) + Char2&0x0f<<8(%d) =%d\n",
                                 buffer[i],((int)(buffer[i+1]&0x0F))<<8,pixel[0]);
                 fprintf(stderr,"Pixel 2 = Char2>>4(%d) + Char3<<4(%d) = %d\n\n",
                                 buffer[i+1]>>4,((int) buffer[i+2])<<4,pixel[1]); 
#endif
          }

/* IAC CODE CHANGE :       free(buffer); */
	 free(buffer);
	 
   }
   
   
   else if( (bit_allocated==16) && (bit_stored==12) && (high_bit==11))
   {
         /* 
          * I pixel non sono impacchettati: ogni pixel sta su due byte
          * Si esegue una lettura dei pixel
          * in formato LSMS
          */
      dim = rows * cols * 2;
      buffer = (unsigned char *)malloc(dim);
      if(buffer==NULL)
	{
	  ERRverror ("read_ACR_NEMA", ERR_WARNING,"Allocation of internal data failed");
	    /*AVSerror("Allocation of internal data failed");*/ 
                return(0);
          }
      fread(buffer,dim,sizeof(unsigned char),input);

      for(i=0,j=0;i<dim;i+=4,j+=2)
          {
                 short_array[j]  = buffer[i]   + (((int) buffer[i+1])<<8);
                 short_array[j+1]= buffer[i+1] + (((int) buffer[i+2])<<8);
#ifdef DEBUG
                 fprintf(stderr,"Char1=%x Char2=%x Char3=%x\n",
                                 buffer[i],buffer[i+1],buffer[i+2]);
                 fprintf(stderr,"Pixel 1 = Char1(%d) + Char2&0x0f<<8(%d) =%d\n",
                                 buffer[i],((int)(buffer[i+1]&0x0F))<<8,pixel[0]);
                 fprintf(stderr,"Pixel 2 = Char2>>4(%d) + Char3<<4(%d) = %d\n\n",
                                 buffer[i+1]>>4,((int) buffer[i+2])<<4,pixel[1]); 
#endif
          }

/* IAC CODE CHANGE :       free(buffer); */
       free(buffer);
   }
   else
     {
          ERRverror("read_ACR_NEMA", ERR_WARNING,"DATA not readable");
     /*AVSerror("DATA not readable");*/
          return(0);
#ifdef DEBUG
      fprintf(stderr,"bit allocated = %d", bit_allocated);
      fprintf(stderr,"bit stored    = %d", bit_stored);
      fprintf(stderr,"high bit      = %d", high_bit);
#endif
   }	 
	 
	 
   OMset_name_array(read_acr_nema_id, OMstr_to_name("short_array"), DTYPE_SHORT, (void *)short_array, 
                    short_array_size, OM_SET_ARRAY_FREE);
   
   

 
   return;
}

