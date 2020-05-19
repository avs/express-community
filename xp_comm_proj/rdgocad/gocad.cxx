

/*
         loadFile.cxx

         Reads ASCII "GOCAD" Geological Structure Files,
         and converts them into AVS Geometry for use
         by Uviewer3D and Uviewer2D.

         18/05/95 converted from AVS5 to AVS/Express by J.Greenhalgh
 

*/


#include "gen.h"
#include <string.h>

int
ReadGocad_GocadMods_ReadGOCAD::update(OMevent_mask event_mask, int seq_num)
{
   // Grid (Grid write)
   // x (OMXint read write req notify)
   // y (OMXint read write req notify)
   // z (OMXint read write req notify)
   // scale (OMXint read write req notify)
   // file_name (OMXstr read write req notify)

   FILE *fp;
   char line[128],useless[16];
   int vertId;
   float vertX,vertY,vertZ;
   int triPoint1,triPoint2,triPoint3;
   float xMin,yMin,zMin;
   float xMax,yMax,zMax;  
   int xCent,yCent,zCent;

   /***********************/
   /* Function's Body     */
   /***********************/


   ////////////////////////////////////////////////////////////////////////////////////////
   //   Count number of triangles and calculate max/min x,y and z. Also note max vertId  //
   ////////////////////////////////////////////////////////////////////////////////////////

   if(!(fp = fopen(file_name,"r")))     // open data file for reading
   {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Unable to open input file\n");
      return(0);
   }


   xMax = yMax = zMax = -1e+20;
   xMin = yMin = zMin = 1e+20;

   int nTri=0;
   int maxVert=0;
   while(fgets(line,128,fp) != NULL)    // scan through file to count triangles and verts
   {

//    if (line[0]=='T') nTri++;
//    if (line[0]=='V')

      if((strncmp(line , "TRGL" ,4)) ==0) nTri++;

      if((strncmp(line , "VRTX" ,4)) ==0)
      {
          sscanf(line,"%s %i %f %f %f",useless,&vertId,&vertX,&vertY,&vertZ);

          if (vertId>maxVert) maxVert=vertId;
       
          if (vertX>xMax) xMax=vertX;
          else if (vertX<xMin) xMin=vertX;
             
          if (vertY>yMax) yMax=vertY;
          else if (vertY<yMin) yMin=vertY;
              
          if (vertZ>zMax) zMax=vertZ;
          else if (vertZ<zMin) zMin=vertZ;     
      }
   } 
   fclose(fp);

   
   xCent = (xMax + xMin) / 2.;
   yCent = (yMax + yMin) / 2.;
   zCent = (zMax + zMin) / 2.;

   
   
   //////////////////////////////////////
   //    Set arrays to required size   //
   //////////////////////////////////////


   Grid.nnodes=maxVert+1;
   float * coords = (float*) Grid.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

   Cells.cell_set[0].ncells=nTri;
   int * connectList = (int*) Cells.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);


   //////////////////////////////////////////////////
   //  open file again then read data into arrays  //
   //////////////////////////////////////////////////


   if(!(fp = fopen(file_name,"r")))     // open data file for reading
   {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Unable to open input file\n");
      return(0);
   }
   
   coords[0]=xCent;    // fills first vert in the array with neutral values 
   coords[1]=yCent;    // incase it gets left blank.
   coords[2]=zCent;

   int count=0;
   while(fgets(line,128,fp) != NULL)    // scan through file to count triangles and verts
   {

      //if (line[0]=='T')

      if((strncmp(line , "TRGL" ,4)) ==0)
      {
          sscanf(line,"%s %i %i %i",useless,&triPoint1,&triPoint2,&triPoint3);
          connectList[count*3]=triPoint1;
          connectList[count*3+1]=triPoint2;
          connectList[count*3+2]=triPoint3;
          count++;
      }
       //if (line[0]=='V')

      if((strncmp(line , "VRTX" ,4)) ==0)
       {
           sscanf(line,"%s %i %f %f %f",useless,&vertId,&vertX,&vertY,&vertZ);
      
           if (scale)
           {                                       // scales image to parameter
              coords[vertId*3]=(vertX-xCent) * x + xCent;  // settings.
              coords[vertId*3+1]=(vertY-yCent) * y + yCent;
              coords[vertId*3+2]=(vertZ-zCent) * z + zCent;
           }
           else
           {
              coords[vertId*3]=vertX;
              coords[vertId*3+1]=vertY;
              coords[vertId*3+2]=vertZ;
           }
       }
   }


   /////////////////////////////////////
   //  close arrays and release file  //
   /////////////////////////////////////


   
   if(coords) ARRfree(coords);
   if(connectList) ARRfree(connectList);
   fclose(fp);



   // return 1 for success
   return(1);
}

