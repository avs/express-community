
#include "gen.h"
#include <iostream>
using namespace std;


#ifndef XP_HAS_LONG
#define XP_HAS_LONG
typedef int xp_long;
typedef unsigned int xp_ulong;
#endif
#ifndef XP_HAS_BYTE
#define XP_HAS_BYTE
typedef unsigned char xp_byte;
#endif



#define ERR_RETURN(A) ERRerror("polycut",1,ERR_ORIG,A);return(1);


int check_polyconnect(xp_long numb,xp_long poly_index,xp_long *polyconnect,xp_long nnodes);
int check_node_in_line(xp_long numb,xp_long poly_index,xp_long *polyconnect);




int
CutPolylines_CutPolylinesCore::update(OMevent_mask event_mask, int seq_num)
{
   int test=0;
   // level (OMXfloat read req notify)



   // in_dists (Node_Data read req notify)

   // in_dists.nnodes (int)
   // in_dists.nnode_data (int)
 
   int  in_dists_data_size, in_dists_data_type;

   if(in_dists.nnode_data != 1) 
      {test=1;}


   // in_dists.node_data[in_dists_data_comp].veclen (int) 
   if(in_dists.node_data[0].veclen != 1) 
      {test=1;}

   
   // in_dists.node_data[in_dists_data_comp].values (char [])
   char *in_dists_node_data = (char *)in_dists.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD, &in_dists_data_size, &in_dists_data_type);
   
      float *in_dists_float;

      switch (in_dists_data_type)
       {
        case DTYPE_FLOAT: in_dists_float = (float *)in_dists_node_data;
        break;
        case DTYPE_DOUBLE:
        case DTYPE_INT:
        case DTYPE_SHORT:
        case DTYPE_CHAR:
        case DTYPE_BYTE: ERR_RETURN("polycut: wrong data type");
        break;
        case 12: test = 1;
       }
    








   // in_poly (Mesh read req notify)


   // in_poly.nspace (int) 
   // in_poly.nnodes (xp_long)
   // in_poly.coordinates.values (float [])
   float *in_poly_coordinates = (float *)in_poly.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
   // in_poly.ncell_sets (int)
   int in_poly_ncell_sets = in_poly.ncell_sets;
   if(in_poly_ncell_sets != 1) 
     {ERR_RETURN("polycut: wrong number of cell sets");}

    // in_poly.cell_set[in_poly_set].ncells (xp_long)
    // in_poly.cell_set[in_poly_set]. cell_nnodes (int)
    // in_poly.cell_set[in_poly_set]. cell_ndim (int)
    // in_poly.cell_set[in_poly_set]. cell_order (int)
    // in_poly.cell_set[in_poly_set]. cell_name (char *)
    // in_poly.cell_set[in_poly_set].node_connect_list (xp_long *)


   xp_long *in_polylist = (xp_long *)in_poly.cell_set[0].poly_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);






   // out (Mesh write)

   // out.nspace (int) 
   // out.nnodes (xp_long)
   // out.coordinates.values (float [])
   //float *out_coordinates = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
   // out.cell_set[0].ncells (xp_long)
   // out.cell_set[0]. cell_nnodes (int)
   // out.cell_set[0]. cell_ndim (int)
   // out.cell_set[0]. cell_order (int)
   // out.cell_set[0]. cell_name (char *)
   // out.cell_set[0].node_connect_list (xp_long *)
   //xp_long *out_node_connect_list = (xp_long *)out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);

   if(in_poly.nnodes == (xp_long)0)
     { test=1;}

   if(test==1)
     { 

       /*set the values for the output*/
       // out (Mesh write)

       // out.nspace (int)
       out.nspace=3;
 
       // out.nnodes (xp_long)
       out.nnodes=(xp_long)0;

       // out.coordinates.values (float [])
       float *out_coordinates = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);

      // out.cell_set[0].ncells (int)
      //out.cell_set[0].ncells=new_node_index;

      // out.cell_set[0]. cell_nnodes (int)
      // out.cell_set[0]. cell_ndim (int)
      // out.cell_set[0]. cell_order (int)
      // out.cell_set[0]. cell_name (char *)
      // out.cell_set[0].node_connect_list (int *)
      xp_long *out_node_connect_list = (xp_long*)out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);

      out.cell_set[0].npolys=0;

      xp_long *out_polylist;

      out_polylist= (xp_long *)out.cell_set[0].poly_connect_list.ret_array_ptr( OM_GET_ARRAY_WR );
      if (in_dists_node_data)
         ARRfree(in_dists_node_data);


      if (in_poly_coordinates)
         ARRfree(in_poly_coordinates);
      if (in_polylist)
         ARRfree(in_polylist);

      if (out_coordinates)
        ARRfree(out_coordinates);
      if (out_polylist)
        ARRfree(out_polylist);


       return(1); 
     }   


   xp_long n;

   xp_long next_node,this_node,last_node,new_line;

   int next_node_valid,this_node_valid,last_node_valid;

   xp_long new_node_index,no_of_new_lines,new_poly_connect_index,coord_index;

   
   xp_long *list_info;

   list_info = (xp_long *)malloc(sizeof(xp_long)*in_dists.nnodes);

   if (list_info==NULL)
     {ERR_RETURN("polycut: unable to meet malloc request.");}

   xp_long start_node;




   /***********************/
   /* Function's Body     */
   /***********************/


   xp_long no_of_lines_in=in_poly.cell_set[0].npolys;
   xp_long no_of_poly_connect_in=2*no_of_lines_in;

   /*This program goes through each point of the polyline and puts a    */
   /*value in a corresponding array, list_info, that tracks whether the */
   /*node is the start finish or middle of the new line. The index value*/
   /*of list_info is the coord node index of the input.                 */


   /*parameters to keep track of nodes in polyline                       */
   /*value of 0 node is start of a polyline                              */
   /*value of 1 node is middle of a polyline in_poly_node_connect_list[0]*/
   /*value of 2 node is end of a polyline                                */
   /*value of 3 no value is set                                          */  
   /*These are the values stored in list_info.                           */


   /*must set initial values*/
   this_node=3;
   last_node=3;
   new_line=3;

   /*toggle if the node is above the set level, zero is not valid, one is valid*/
   next_node_valid=0;
   this_node_valid=0;
   last_node_valid=0;

   /*keep track of idecies for the new polyline*/
   new_node_index=0;
   no_of_new_lines=0;
   new_poly_connect_index=0; 

   /*set up values for the last node, this is the first, start_node*/
   start_node=in_polylist[0];
   last_node=0;
    
   if(last_node==3)  //this is an error, lets exit
     {ERR_RETURN("polycut: the node is outside range");}



   /*check if last_node point has a valid value, we know it is in the poly connect list, we read it from there*/
   if(in_dists_float[0]>=level)
     {last_node_valid=1;}
   else
     {last_node_valid=0;}


   /*set up values for the this node, this node is the next one, the first in the main loop*/
   /*check if this_node point has a valid value*/
   if((in_dists_float[1]>=level) && (check_node_in_line(start_node+1,no_of_poly_connect_in,in_polylist)==1))
     {this_node_valid=1;}
   else
     {this_node_valid=0;}



   /*Let's see what the we do with the first node*/
   /*start filling in the list_info array*/
   if(start_node>0)
   {
     for(n=0;n<start_node;n++)
     {list_info[n]=3;}
   }
   if((last_node==0) && (last_node_valid==1))
      {list_info[start_node]=0;}
   else
      {list_info[start_node]=3;}

   
   if(list_info[0]==0)
      {
       new_node_index++;
       no_of_new_lines++;
       new_poly_connect_index++;
      }


   float value=level;


    for (n=start_node+1;n<(in_dists.nnodes-1);n++)  //go through each node
     {
       /*check to see if index n is in polyconnect list*/
       /* and if it marks the start or end of polyline*/

       this_node=check_polyconnect(n,no_of_poly_connect_in,in_polylist,in_poly.nnodes);


       if(this_node==3)  //this is an error, lets exit
         {ERR_RETURN("polycut: the node is outside range");}



       /*check if next_node point has a valid value*/
       if((in_dists_float[n+1]>=level) && (check_node_in_line(n+1,no_of_poly_connect_in,in_polylist)==1))
        {next_node_valid=1;}
      else
        {next_node_valid=0;}


      new_line=3;


      //if the new line is the same as the old one
      if((last_node_valid==1) && (this_node_valid==1) && (next_node_valid==1))
        {
         new_line=this_node;
        }

      //if the new line is forced to end no matter what the old line did
      if((last_node_valid==1) && (this_node_valid==1) && (next_node_valid==0))
        {
         last_node=check_polyconnect((n-1),no_of_poly_connect_in,in_polylist,in_poly.nnodes);
         if(last_node==3)  //this is an error, lets exit
           {ERR_RETURN("polycut: the node is outside range");}

         if(last_node==2)
           {new_line=3;}
         else
           {new_line=2;}

         //could add new nodes if this_node != 2
        }

      //if the new line is forced to start no matter what the old line did
      if((last_node_valid==0) && (this_node_valid==1) && (next_node_valid==1))
        {
         next_node=check_polyconnect((n+1),no_of_poly_connect_in,in_polylist,in_poly.nnodes);
         if(next_node==3)  //this is an error, lets exit
           {ERR_RETURN("polycut: the node is outside range");}

         if(next_node==0) //can not have a line of one node
           {new_line=3;}
         else
           {new_line=0;}


         //could add new nodes if this_node != 0
        }


      //could add two new nodes if had a line of one node!!


      list_info[n]=new_line;

   
      if(new_line==0)
        {
         new_node_index++;
         no_of_new_lines++;
         new_poly_connect_index++;
        }

      if(new_line==1)
        {
          new_node_index++;
          //   last_valid_index=n-1;
        }

      if(new_line==2)
        {
         new_node_index++;
         new_poly_connect_index++;
         //   last_valid_index=n;
        }

   
   /*get ready for the next time around the loop*/
   last_node_valid=this_node_valid;
   this_node_valid=next_node_valid;
   this_node=3;
   new_line=3;

  }

  next_node=check_polyconnect(in_dists.nnodes,no_of_poly_connect_in,in_polylist,in_poly.nnodes);

  /*must sort out the last value*/
  if ((this_node_valid==1) && (next_node_valid==1) && (1<=next_node<=2))
    {list_info[in_dists.nnodes-1]=2;new_node_index++;new_poly_connect_index++;}
  else
    {list_info[in_dists.nnodes-1]=3;}





 /*set the values for the output*/
   // out (Mesh write)

   // out.nspace (int)
   out.nspace=in_poly.nspace;
 
   // out.nnodes (int)
   out.nnodes=new_node_index;

   // out.coordinates.values (float [])
   float *out_coordinates = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
   // out.cell_set[0].ncells (int)
   //out.cell_set[0].ncells=new_node_index;

   // out.cell_set[0]. cell_nnodes (int)
   // out.cell_set[0]. cell_ndim (int)
   // out.cell_set[0]. cell_order (int)
   // out.cell_set[0]. cell_name (char *)
   // out.cell_set[0].node_connect_list (int *)
   // int *out_node_connect_list = (int *)out.cell_set[0].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);

   out.cell_set[0].npolys=no_of_new_lines;

   xp_long *out_polylist;
   out_polylist= (xp_long *)out.cell_set[0].poly_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);





   /*now put the values in the arrays for out*/

   xp_long m=0,node_index=0,line_index=0;

   for (n=0;n<in_dists.nnodes;n++)  //go through each node
      {

       m=n*3;
       coord_index=node_index*3;

       if(list_info[n]==0)
         {
          out_coordinates[coord_index]=in_poly_coordinates[m];
          out_coordinates[coord_index+1]=in_poly_coordinates[m+1];
          out_coordinates[coord_index+2]=in_poly_coordinates[m+2];
          out_polylist[line_index]=node_index;
          node_index++;
          line_index++;
         }

       if(list_info[n]==1)
         {
          out_coordinates[coord_index]=in_poly_coordinates[m];
          out_coordinates[coord_index+1]=in_poly_coordinates[m+1];
          out_coordinates[coord_index+2]=in_poly_coordinates[m+2];
          node_index++;
         }

       if(list_info[n]==2)
         {
          out_coordinates[coord_index]=in_poly_coordinates[m];
          out_coordinates[coord_index+1]=in_poly_coordinates[m+1];
          out_coordinates[coord_index+2]=in_poly_coordinates[m+2];
          out_polylist[line_index]=node_index;
          node_index++;
          line_index++;
         }

      }



//   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: Cut_Polylines_Cut_Polylines::update\n");


        
   if (in_dists_node_data)
      ARRfree(in_dists_node_data);


   if (in_poly_coordinates)
      ARRfree(in_poly_coordinates);
   if (in_polylist)
      ARRfree(in_polylist);


   if (out_coordinates)
      ARRfree(out_coordinates);
   if (out_polylist)
      ARRfree(out_polylist);

   if (list_info)
      free(list_info);


   // return 1 for success
   return(1);
}







/*this checks to see if the value of numb appears in the poly connect list*/
/*if it does it means that this node is a start or end node for the polyline*/
/*if numb appears in the poly connect list and has an odd index it is a start*/
/*if numb appers in the poly connect list and has an even index it is a end*/
/*N.B. polyconnect[0] is not checked for here*/
int check_polyconnect(xp_long numb,xp_long poly_index,xp_long *polyconnect,xp_long nnodes)
{
  xp_long i;


  for (i=1;i<poly_index;i++)
     {

       if(polyconnect[i]==numb)
         {
           if (i%2==0)
             {return 0;}
           if (i%2==1)
             {return 2;}

         }
     }

  if(numb>nnodes)
     {return 3;}

  else return 1;

}



/*checks that the lines on the polyconnect list do not miss some nodes out */
/*this would make the node not valid, returns 1 for valid and returns      */
/*zero for not valid.                                                      */
/*N.B. polyconnect[0] is not checked for here                              */
int check_node_in_line(xp_long numb,xp_long poly_index,xp_long *polyconnect)
{
  xp_long i;

  for (i=1;i<poly_index;i=i+2)
    {

      if((polyconnect[i]<numb) && (numb<polyconnect[i+1]))
        {
	  return 0;
        }
    }


  return 1;

}
