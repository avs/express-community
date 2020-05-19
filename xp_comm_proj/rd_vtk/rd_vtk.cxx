//
// read vtk module
//     reads the vtk triangle list format
//     Extended format code by Sabreen Vig 
//     Extended, revision by I. Curington
//     Removed Compression
//
#define XP_WIDE_API

#include "xp_comm_proj/rd_vtk/gen.h"

#include "avs/gd_def.h"
#include "avs/f_utils.h"
 
#include <stdlib.h>
#include <stdio.h>

//
// Uncomment the following line to get debug messages
//
// #define DEBUG 1


#ifdef DEBUG
   #define PRINT(a)  ERRverror("",ERR_PRINT|ERR_NO_HEADER,(a));
#else
   #define PRINT(a)  {};
#endif

#define PRINTERR(a)  ERRverror("",ERR_PRINT|ERR_NO_HEADER,(a));

int ReadVtk_ReadVtkMods_ReadVtkCore::Read( OMevent_mask, int )
{
  //
  // int dummy;
  // int npoints;
  // coords (OMXfloat_array write)
  //
  xp_long coords_size;
  float *coords_arr;
  
  // connect (OMXlong_array write)
  //
  xp_long connect_size;
  xp_long *connect_arr;
  
  // num_points (OMXint write)
  //

  FILE *infile; 

  char h[256]; // header.
  char dummy1[256]; // dummy array
  char dummy2[256]; // dummy array
  xp_long lines_to_read;
  xp_long npoints;
  xp_long triangles;
  xp_long storage;
  int actual = 0;
  xp_long real_ncoords;
  xp_long i, j;
  int flag;
  int interrupt;


  PRINT(" ** Read VTK **\n");


  // Make sure that we have a valid filename
  //
  if (!filename.valid_obj()) {

    PRINTERR(" invalid filename\n");
    return 0;
  }
  PRINT((char*)filename);
  
  // Open the input file
  //
  if( (infile = (FILE *)FILEfopen((char*)filename,SIO_R_TXT)) == (FILE *) NULL  ) {
                PRINTERR(" could not open file\n");
                return(0);
  }     
 
  // Read First Header line from the file, # vtk DataFile....
  fgets( h, 128, infile ); PRINT(h);

  // Read Second Header line from the file - user description line
  fgets( h, 128, infile ); PRINT(h);

  // Read Third Header line from the file, assume type
  fgets( h, 128, infile ); PRINT(h);

  //
  // check that is says ASCII format
  //
  if (strncmp(h,"ASCII",5)==0) {
          PRINT("- got ASCII keyword\n");
  } else {
          PRINTERR("- failed to find ASCII keyword\n");
          return(0);
  }


  //
  // check for dataset polydata
  fgets( h, 128, infile ); PRINT(h);
  if (strncmp(h,"DATASET POLYDATA",16)==0) {
          PRINT("- got POLYDATA keyword\n");
  } else {
          PRINTERR("- failed to find POLYDATA keyword\n");
          return(0);
  }

  npoints = -1;
  fscanf( infile, "%s%"XP_L_WIDTH"d%s", dummy1, &npoints, dummy2 );
  if (strncmp(dummy1,"POINTS",6)==0) {
          PRINT("- got POINTS keyword\n");
  } else {
          PRINTERR("- failed to find POINTS keyword\n");
          return(0);
  }
  if ( npoints < 0 ) {
          PRINTERR("- npoints out of range\n");
          return(0);
  }
  sprintf(dummy2,"npoints = %"XP_L_WIDTH"d\n",npoints);
  PRINT(dummy2);

  lines_to_read = npoints;

  real_ncoords = 0;

  float *tmp_coords = (float *)malloc( lines_to_read * 3 * sizeof(float) );

  num_coords = npoints;
  coords_arr = (float *)coords.ret_array_ptr(OM_GET_ARRAY_WR,&coords_size);

  if( npoints && tmp_coords )
  {
      // Read in coordinate and put in array
      
      for( i = 0; i < lines_to_read; i++ )
      {
          // sprintf(dummy2,"node %d\n",i); PRINT(dummy2);

          // Read in next coordinate
          
          fscanf( infile, "%f%f%f",
                          &(coords_arr[i*3+0]),
                          &(coords_arr[i*3+1]),
                          &(coords_arr[i*3+2]) );

          real_ncoords += 1;
          if ( (i%500)==0) {
             OMstatus_check((50*i/lines_to_read),"read vtk",&interrupt);
             if (interrupt) return(1);
          }
 
        } // end of for loop
  }// end of first pass.
  
  // 
  // Now parse the polygon section of the file
  //  look for keywords and count values
  //
  fscanf( infile, "%s%"XP_L_WIDTH"d%"XP_L_WIDTH"d", h, &triangles, &storage );
  if (strncmp(h,"POLYGONS",8)==0) {
          PRINT("- got POLYGONS keyword\n");
  } else {
          PRINTERR("- failed to find POLYGONS keyword\n");
          return(0);
  }
  if ( triangles < 0 || triangles > 1e8 ) {
          PRINTERR("- polygon count out of range\n");
          return(0);
  }
  sprintf(dummy2,"triangles = %"XP_L_WIDTH"d\n",triangles);
  PRINT(dummy2);
  if ( storage < 0 || storage > 1e8 ) {
          PRINTERR("- connectivity storage count out of range\n");
          return(0);
  }
  sprintf(dummy2,"storage = %"XP_L_WIDTH"d\n",storage);
  PRINT(dummy2);

  num_connect = triangles * 3;
  connect_arr = (xp_long *)connect.ret_array_ptr(OM_GET_ARRAY_WR,&connect_size);
  
  //
  // Now Read in Polygon connectivity data
  //
  if( triangles && connect_arr )
    {
      // Read in triangle connectivity and put in array
      
      for( i = 0; i < triangles; i++ )
      {
          // sprintf(dummy2,"tri = %d\n",i); PRINT(dummy2);

          j = 0;
		  fscanf(infile,"%"XP_L_WIDTH"d %"XP_L_WIDTH"d %"XP_L_WIDTH"d %"XP_L_WIDTH"d", &j, &(connect_arr[i*3+0]),
                                           &(connect_arr[i*3+1]),
                                           &(connect_arr[i*3+2]) );
          if ( j != 3 )
          {
              PRINTERR(" don't support anything but triangles right now...\n");
              fclose(infile);
              return(0);
          }
          actual += 1;

          if ( (i%500)==0) {
             OMstatus_check((50*i/triangles)+50,"read vtk",&interrupt);
             if (interrupt) return(1);
          }
 
      } // end of for loop
    } // end of second pass
 
    ARRfree((char *)coords_arr);
    ARRfree((char *)connect_arr);
  
    // Close File
    fclose(infile);


    PRINT(" ** End of Read VTK **\n");

   return(1);

} // end of main.

// end of file

