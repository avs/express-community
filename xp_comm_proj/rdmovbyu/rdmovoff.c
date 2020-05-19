/*
 * Movie-BYU ASCII file reader for AVS/Express
 * Adapted from the AVS geometry filter, in
 * Offset files only
 *
 * Adapted by I. Curington
 * Revision:
 * 28 June 1997  First adaption pass as omodule
 *  7 July 1997  polyhedron optimization
 *  8 July 1997  read_off
 *
 */

/*
			Copyright (c) 1997 by
			Advanced Visual Systems Inc.
			All Rights Reserved

	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

*/
/*-----------------------------------------------------*
 *                                                     *
 *               ****  read offset files  ****         *
 *                                                     *
 * read a file (ascii) in the Movie-BYU offset format. *
 *                                                     *
 *  June 1997  I. Curington  <ianc@acm.org>            *
 *-----------------------------------------------------*/

#include "xp_comm_proj/rdmovbyu/gen.h"

#include <stdio.h>

#include <avs/util.h>
#include <avs/err.h>
#include <avs/om.h>
#include <avs/math.h>
#include <avs/dtype.h>
#include <avs/fld.h>

#define ERR_RETURN(A) {ERRerror("read_off",1,ERR_ORIG, A); return(0);}
#define ABS(D)	((D)<0 ? -(D) : (D))
#define BUFFER_SIZE 200
#define BUF_START   10000
/*
 * Set Debug Flag (uncomment next line)
 * #define DEBUG 1
 * #define MAKE_CELL_DATA 1
 */
/*#define DEBUG 0*/

#ifdef DEBUG
   #define DEBUG_PRINT(a) {printf(a);}
#else
   #define DEBUG_PRINT(a) {}
#endif

/*--------------------------------------------------------------------
 *      OM module Entry Point, Resolve Parameters
 *--------------------------------------------------------------------*/

int read_off_update(OMobj_id elem_id, OMevent_mask event_mask, int seq_num)
{
    OMobj_id filename, out, size, index;
    char *str;
    int num;
    int Result;

    /*char buffer[50];*/

    str = NULL;
    filename = OMfind_subobj(elem_id, OMstr_to_name("filename"),
			      OM_OBJ_RD);
    OMget_str_val(filename, &str, 0);

    /*index = OMfind_subobj(elem_id, OMstr_to_name("index"),
               OM_OBJ_RD);
    OMget_int_val(index, &num);
    sprintf(buffer, "Index Value: %d\n", num );
    ERRverror("",ERR_NO_HEADER | ERR_PRINT, buffer);*/
    
    out = OMfind_subobj(elem_id, OMstr_to_name("out"), OM_OBJ_RW);

    size = OMfind_str_subobj(elem_id, "size", OM_OBJ_RD);
    if (OMis_null_obj(size)) {
      ERR_RETURN("Error searching for size");
    }

    Result = FUNCread_off(str, out, size);

    if (str) free(str);

    return(Result);
}

/*--------------------------------------------------------------------
 *      Read MOVIE BYU offset module
 *--------------------------------------------------------------------*/
int FUNCread_off( char *filename, OMobj_id out, OMobj_id size)
{

   float *array, val;
   float *store;
   int i, j, k, n, d;
   FILE *pf;
   FILE  *FILEfopen();
   char  line[BUFFER_SIZE];
   int veclen;
   int interrupt;
   int ending;
   int dtype, ndims, dims[OM_ARRAY_MAXDIM];

   /*char buffer[50];*/

   /**************
    ***  body  ***
    **************/
   DEBUG_PRINT(" start body\n");

   interrupt = 0;

   if (!filename) {
        DEBUG_PRINT(" no filename\n");
	return(1); /* silent exit */
   }

   /*************************
    * Open File             *
    *************************/
   if (!(pf = (FILE *)FILEfopen(filename, SIO_R_TXT))) {
	ERRerror("read offset",1,ERR_ORIG,"Error opening file %s", filename);
	/*fclose(pf);*/
	return(0);
   }

   DEBUG_PRINT(" file opened\n");
   /*sprintf(buffer, "File opened: %s\n", filename );*/
   /*ERRverror("",ERR_NO_HEADER | ERR_PRINT, buffer);*/
   	

   /*************************
    * Allocate Temp Arrays  *
    * Scan in Offset Data   *
    *************************/
   array = (float *) malloc(BUF_START*sizeof(float));
   if ( array == NULL ) ERR_RETURN("Could not allocate memory for array\n");

   n = 0;
   ending = 0;
   do {
	 if (fscanf(pf,"%f", &val) != 1) {
            ending = 1;
	 }
         else
         {
        
	     n++;

         /*array = (float *) realloc((float *)array, n*sizeof(int));*/  /*should be - n*sizeof(float)*/
	     array = (float *) realloc((float *)array, n*sizeof(float));    /*changed to allocate space for floats*/

	     array[n-1] = val;

             if ( (n%2000)==0) {
               OMstatus_check(((n/2000)%100),"read offset",&interrupt);
               if (interrupt) return(1);
             }
         }

   } while (ending==0);


   /*************************
    * Close File            *
    *************************/
   fclose(pf);


#ifdef DEBUG
   printf("   file closed, n vals = %d\n", n );
#endif

   /*************************
    * Build Output Structure *
    *************************/
   if (OMset_int_val(size, n) != OM_STAT_SUCCESS) {
      ERR_RETURN("Error setting size");
   }
   DEBUG_PRINT(" set int val ok\n");

   if (OMset_array_size(out, n) != OM_STAT_SUCCESS) {
      ERR_RETURN("Error setting array size");
   }

   dtype = OM_TYPE_FLOAT;
   ndims = 0;
   store = NULL;
   if (OMget_array(out, &dtype, (char **) &store, &ndims, dims,
		   OM_GET_ARRAY_RW) != OM_STAT_SUCCESS) {
      ERR_RETURN("Error getting store");
   }

   DEBUG_PRINT(" got output array pointer\n");

   if (n > 0) {
      for (i=0; i < n; i++ )
             store[i] = array[i];
   }

   OMstatus_check(99,"read offset",&interrupt);

   DEBUG_PRINT(" out field finished building\n");

   /*************************
    * Free Memory           *
    *************************/
   if (array) free (array);
   ARRfree( (char *)store );

   DEBUG_PRINT(" memory freed\n");

   return(1);
}


