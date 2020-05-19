/*
PURPOSE

o To convert a scattered field into a uniform field without interpolation

ASSUMPTIONS

o Input field has ndim = 1 (module verifies this)
o Node-data types char, int, & short can be manipulated as ints ?


OTHER NOTES

o As much as possible of the o/p field is set up by the module's V
  code (because it's simpler & can use Express built-in array functions

Output field is created with following properties:-
o ndim = (nspace of Input field)
o nspace = (nspace of Input field)
o coordinate dims extending beyond those of i/p field (after truncation)
  by one unit in all directions
o Integer coords spaced by one unit along each axis


o Input field coordinates are rounded to integer values


CHANGE HISTORY
#001 08/02/99 delete array out_points, ARRfree array max_extent
#002 12/02/99 scatter data validation - return error val changed to 0
Validate ret_array_ptr calls, remove checks b4 ArrFrees, remove code
setting to NULL for arrays:- min_extent, max_extent, out_dims, in_coords,
in_node_data
#003 Declare & free in_node_data out of loop - ptr can be re-used
#004 Omit allocating new memory for in_data arrays of each type.
Using ptr of correct type to Express in_node_data array is OK (JES see
JEScopy_nd4.cpp)
#005 replace "delete ptr" with "delete[] ptr" where ptr points to an
array (ref: Practical C++ Prog. - Steve Oualline)


FUTURE WORK

o More input validation?

*/
#include <iostream.h>
#include "xp_comm_proj/sct2unif/gen.hxx"


#define ROUND(val) (val < 0 ? (int)(val - 0.5) : (int)(val + 0.5))

#define DEL_ARRAYS \
  delete[] dims_mult;\
  delete[] node_index;\
  delete[] out_points;\
  delete[] out_dims;

int
Sct2Unif_Sct2UnifMods_sct2unif::update(OMevent_mask event_mask, int seq_num) {

  //************************************************************
  //Function entry message
  //************************************************************
  //  ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: Sct2Unif_Sct2UnifMods_sct2unif::update\n");


  //************************************************************
  //Perform some validation
  //************************************************************
  // Check we have scattered data input (ndim = 1) 
  if (in_fld.ndim != 1)
  {
    ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Sct2Unif : Not scatter data - ndim > 1\n"); return(0);
  }


  //************************************************************
  // Declare variables
  //************************************************************
  int cmpnt;            // node data component
  int in_data_size;     // size of in_fld node_data array for each comp
  int data_type;        // type of node_data array for each comp
  int i,j,k,m,n;        //counters
  int shift_coord;      //standardised coord after subtracting min coord

  // Pointers to arrays returned from AVS. Have to use ARRFree after use
  float * in_coords;    //coords of in_fld (defined in V code)
  float * min_extent;   //ptr to min_coord (defined in V code)
  float * max_extent;   //ptr to max_coord (defined in V code)

  int out_data_size;    //size of out node_data array for each component
  int points_size;      //size of points array

  //************************************************************
  // Testing vbles
  //************************************************************  
  //  int ndim = out_fld.ndim;


  //************************************************************
  // Set the "points" array in the o/p field
  // - this is a concat of min_coord & max_coord arrays
  // "points" can't be set right in V - casting probs  
  //************************************************************ 
  min_extent = (float *) min_coord.ret_array_ptr(OM_GET_ARRAY_RD);  // get min_coord
  if (!min_extent)
  {
    ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Sct2Unif : Can't get min_extent ptr\n");
    return(1);
  }
  max_extent = (float *) max_coord.ret_array_ptr(OM_GET_ARRAY_RD);  // get max_coord
  if (!max_extent)
  {
    ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Sct2Unif : Can't get max_extent ptr\n");
    ARRfree((char *)min_extent);
    return(1);
  }

  points_size =  out_fld.nspace * 2;
  float * out_points = new float [points_size];   //ptr to out_fld's "points" array
  float * out_dims = new float [out_fld.nspace];   //ptr to out_fld's "dims" array

  for (i=0;i < out_fld.nspace;i++)
  {
    out_points[i] = ROUND(min_extent[i]);
    out_points[i + out_fld.nspace] = ROUND(max_extent[i]);
  }
  out_fld.points.set_array(OM_TYPE_FLOAT,(char *)out_points,points_size,OM_SET_ARRAY_COPY);   //set Express "points" arr of out_fld

  for (i=0;i < out_fld.nspace;i++)
  {
    out_dims[i] = out_points[i + out_fld.nspace] - out_points[i] + 1;
  }
  out_fld.dims.set_array(OM_TYPE_FLOAT,(char *)out_dims,out_fld.nspace,OM_SET_ARRAY_COPY);   //set Express "points" arr of out_fld




  //************************************************************
  //Generate mapping from each in_fld node -> node index of out_fld ...
  //dims_mult[i]=num nodes to skip for every unit increase in dim i
  //************************************************************
  int * dims_mult = new int [out_fld.ndim];
  int * node_index = new int [in_fld.nnodes];   // mapped value for each in_fld node

  in_coords = (float *) coords.ret_array_ptr(OM_GET_ARRAY_RD);// get in_fld coords
  if (!in_coords)
  {
    ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Sct2Unif : Can't get in_coords ptr\n");
    DEL_ARRAYS;
    ARRfree((char *)min_extent);
    ARRfree((char *)max_extent);
    return(1);
  }

  dims_mult[0]=1;
  for (i=1;i<out_fld.ndim;i++)
    dims_mult[i]= dims_mult[i-1] * out_dims[i-1];

  for (i=0;i<in_fld.nnodes;i++)
  {
    //for each in_fld node
    node_index[i]=0; //initialise
    j = i * (in_fld.nspace); // start posn of node in the in_coords array
    for (k=0;k<in_fld.nspace;k++)
    {
      //for each dim of node's coords
      shift_coord = ROUND(in_coords[j+k]) - out_points[k];   //account for extents
      node_index[i] = node_index[i] + (shift_coord * dims_mult[k]); 
    }
  }



  //************************************************************
  // For each component of node data ...
  // o copy relevant items from in_fld to out_fld
  //   - (only copy id if set in in_fld - see DVK 2-51)
  // o fill all node_data values with fill_value
  // o insert in_fld node data into the out_fld node data
  //************************************************************
  for (cmpnt = 0; cmpnt < in_fld.nnode_data; cmpnt++)
  {
    // copy across some settings from in to out field 
    out_fld.node_data[cmpnt].veclen = in_fld.node_data[cmpnt].veclen;

    if  (in_fld.node_data[cmpnt].id.valid_obj())
      out_fld.node_data[cmpnt].id = in_fld.node_data[cmpnt].id; 

    out_fld.node_data[cmpnt].null_flag = in_fld.node_data[cmpnt].null_flag;
    if (in_fld.node_data[cmpnt].null_flag == 1)
      out_fld.node_data[cmpnt].null_value = in_fld.node_data[cmpnt].null_value;

    out_fld.node_data[cmpnt].labels = in_fld.node_data[cmpnt].labels;
    out_fld.node_data[cmpnt].units = in_fld.node_data[cmpnt].units;

    void * in_node_data = NULL;     //ptr to i/p node data - type unknown initially
    in_node_data = in_fld.node_data[cmpnt].values.ret_array_ptr(OM_GET_ARRAY_RD,&in_data_size, &data_type);   // get in_node_data for cmpnt

    if (!in_node_data)
    {
      ERRverror("",ERR_NO_HEADER | ERR_PRINT,"Sct2Unif : Can't get in_node_data ptr\n");
      DEL_ARRAYS;
      ARRfree((char *)min_extent);
      ARRfree((char *)max_extent);
      ARRfree((char *)in_coords);
      return(1);
    }



    //************************************************************
    // Execute one of the following "case" code blocks according 
    // to type of node_data. Blocks are identical except for
    // array data type.
    // If type = "unset" then treated as "double"
    // Types "byte", "short" & "char" are manipulated
    // as int arrays then written out in their appropriate types
    //************************************************************
    switch(data_type)
    {
    case OM_TYPE_BYTE:
    {
      char *in_data_b;//#004 used to point to same array as in_fld_node_data
      in_data_b = (char *) in_node_data; /* cast Express node data array */
      out_data_size = out_fld.node_data[cmpnt].veclen * out_fld.nnodes;
      char *out_data_b = new char [out_data_size];
      int *out_data_count = new int [out_fld.nnodes];

      char c_fill_value = (char)( (int)fill_value );  // Can only cast from OMXprim to int, float or double
                                                      // Therefore cast to int first, before casting to char

      for (i=0;i<out_fld.nnodes;i++)
        out_data_count[i] = 0;

      for (i=0;i<out_data_size;i++)
        out_data_b[i] = c_fill_value; //fill all out node_data with fill_value 

      for (i=0;i<in_fld.nnodes;i++)
      {
        n = node_index[i];
        j = node_index[i]*out_fld.node_data[cmpnt].veclen;
        k = i*in_fld.node_data[cmpnt].veclen;

        if (out_data_count[n] == 0) {
          //No previous values so put data straight in
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_b[j+m]=in_data_b[k+m];
        }
        else
        {
          //average data with previous values
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_b[j+m]=( (out_data_b[j+m]*out_data_count[n]) + in_data_b[k+m] ) / (out_data_count[n] + 1);
        }

        out_data_count[n]++;

      } //End "for each node"
      
      out_fld.node_data[cmpnt].values.set_array(OM_TYPE_BYTE,(char *)out_data_b,out_data_size,OM_SET_ARRAY_COPY); //set Express out node data arr  for cmpnt
      delete[] out_data_b; //#005
      delete[] out_data_count;
    }
    break;

    case OM_TYPE_INT:
    {
      int *in_data_i; //#004 used to point to same array as in_fld_node_data
      in_data_i = (int *) in_node_data; // cast Express node data array
      out_data_size = out_fld.node_data[cmpnt].veclen * out_fld.nnodes;
      int *out_data_i = new int [out_data_size];
      int *out_data_count = new int [out_fld.nnodes];

      int i_fill_value = (int)fill_value;

      for (i=0;i<out_fld.nnodes;i++)
        out_data_count[i] = 0;

      for (i=0;i<out_data_size;i++)
        out_data_i[i] = i_fill_value; //fill all out node_data with fill_value 

      for (i=0;i<in_fld.nnodes;i++)
      {
        n = node_index[i];
        j = node_index[i]*out_fld.node_data[cmpnt].veclen;
        k = i*in_fld.node_data[cmpnt].veclen;

        if (out_data_count[n] == 0) {
          //No previous values so put data straight in
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_i[j+m]=in_data_i[k+m];
        }
        else
        {
          //average data with previous values
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_i[j+m]=( (out_data_i[j+m]*out_data_count[n]) + in_data_i[k+m] ) / (out_data_count[n] + 1);
        }

        out_data_count[n]++;

      } //End "for each node"
      
      out_fld.node_data[cmpnt].values.set_array(OM_TYPE_INT,(char *)out_data_i,out_data_size,OM_SET_ARRAY_COPY);  //set Express out node data arr for cmpnt
      delete[] out_data_i; //#005
      delete[] out_data_count;
    }
    break;

    case OM_TYPE_FLOAT:
    {
      float *in_data_f; //#004
      in_data_f = (float *) in_node_data;
      out_data_size = out_fld.node_data[cmpnt].veclen * out_fld.nnodes;
      float *out_data_f = new float [out_data_size];
      int *out_data_count = new int [out_fld.nnodes];

      float f_fill_value = (float)fill_value;

      for (i=0;i<out_fld.nnodes;i++)
        out_data_count[i] = 0;

      for (i=0;i<out_data_size;i++)
        out_data_f[i] = f_fill_value; 

      for (i=0;i<in_fld.nnodes;i++)
      {
        n = node_index[i];
        j = node_index[i]*out_fld.node_data[cmpnt].veclen;
        k = i*in_fld.node_data[cmpnt].veclen;

        if (out_data_count[n] == 0) {
          //No previous values so put data straight in
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_f[j+m]=in_data_f[k+m];
        }
        else
        {
          //average data with previous values
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_f[j+m]=( (out_data_f[j+m]*out_data_count[n]) + in_data_f[k+m] ) / (out_data_count[n] + 1);
        }

        out_data_count[n]++;

      } //End "for each node"
      
      out_fld.node_data[cmpnt].values.set_array(OM_TYPE_FLOAT,(char *)out_data_f,out_data_size,OM_SET_ARRAY_COPY);
      delete[] out_data_f; //#005
      delete[] out_data_count;
    }
    break;


    case OM_TYPE_SHORT:
    {
      short *in_data_s; //#004
      in_data_s = (short *) in_node_data;
      out_data_size = out_fld.node_data[cmpnt].veclen * out_fld.nnodes;
      short *out_data_s = new short [out_data_size];
      int *out_data_count = new int [out_fld.nnodes];

      short s_fill_value = (short)( (int)fill_value );

      for (i=0;i<out_fld.nnodes;i++)
        out_data_count[i] = 0;

      for (i=0;i<out_data_size;i++)
        out_data_s[i] = s_fill_value; 

      for (i=0;i<in_fld.nnodes;i++)
      {
        n = node_index[i];
        j = node_index[i]*out_fld.node_data[cmpnt].veclen;
        k = i*in_fld.node_data[cmpnt].veclen;

        if (out_data_count[n] == 0) {
          //No previous values so put data straight in
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_s[j+m]=in_data_s[k+m];
        }
        else
        {
          //average data with previous values
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_s[j+m]=( (out_data_s[j+m]*out_data_count[n]) + in_data_s[k+m] ) / (out_data_count[n] + 1);
        }

        out_data_count[n]++;

      } //End "for each node"
      
      out_fld.node_data[cmpnt].values.set_array(OM_TYPE_SHORT,(char *)out_data_s,out_data_size,OM_SET_ARRAY_COPY);
      delete[] out_data_s; //#005
      delete[] out_data_count;
    }
    break;

    case OM_TYPE_CHAR:
    {
      char *in_data_c; //#004
      in_data_c = (char *) in_node_data;
      out_data_size = out_fld.node_data[cmpnt].veclen * out_fld.nnodes;
      char *out_data_c = new char [out_data_size];
      int *out_data_count = new int [out_fld.nnodes];

      char c_fill_value = (char)( (int)fill_value );

      for (i=0;i<out_fld.nnodes;i++)
        out_data_count[i] = 0;

      for (i=0;i<out_data_size;i++)
        out_data_c[i] = c_fill_value; 

      for (i=0;i<in_fld.nnodes;i++)
      {
        n = node_index[i];
        j = node_index[i]*out_fld.node_data[cmpnt].veclen;
        k = i*in_fld.node_data[cmpnt].veclen;

        if (out_data_count[n] == 0) {
          //No previous values so put data straight in
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_c[j+m]=in_data_c[k+m];
        }
        else
        {
          //average data with previous values
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_c[j+m]=( (out_data_c[j+m]*out_data_count[n]) + in_data_c[k+m] ) / (out_data_count[n] + 1);
        }

        out_data_count[n]++;

      } //End "for each node"

      out_fld.node_data[cmpnt].values.set_array(OM_TYPE_CHAR,(char *)out_data_c,out_data_size,OM_SET_ARRAY_COPY);
      delete[] out_data_c; //#005
      delete[] out_data_count;
    }
    break;

    case OM_TYPE_UNSET:
    case OM_TYPE_DOUBLE:
    {
      double *in_data_d; //#004
      in_data_d = (double *) in_node_data;
      out_data_size = out_fld.node_data[cmpnt].veclen * out_fld.nnodes;
      double *out_data_d = new double [out_data_size];
      int *out_data_count = new int [out_fld.nnodes];

      double d_fill_value = (double)fill_value;

      for (i=0;i<out_fld.nnodes;i++)
        out_data_count[i] = 0;

      for (i=0;i<out_data_size;i++)
        out_data_d[i] = d_fill_value;

      for (i=0;i<in_fld.nnodes;i++)
      {
        n = node_index[i];
        j = node_index[i]*out_fld.node_data[cmpnt].veclen;
        k = i*in_fld.node_data[cmpnt].veclen;

        if (out_data_count[n] == 0) {
          //No previous values so put data straight in
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_d[j+m]=in_data_d[k+m];
        }
        else
        {
          //average data with previous values
          for (m=0;m<out_fld.node_data[cmpnt].veclen;m++)
            out_data_d[j+m]=( (out_data_d[j+m]*out_data_count[n]) + in_data_d[k+m] ) / (out_data_count[n] + 1);
        }

        out_data_count[n]++;

      } //End "for each node"

      out_fld.node_data[cmpnt].values.set_array(OM_TYPE_DOUBLE,(char *)out_data_d,out_data_size,OM_SET_ARRAY_COPY);
      delete[] out_data_d; //#005
      delete[] out_data_count;
    }
    break;

    }//End switch

    ARRfree((char *)in_node_data);

  }//End "for each component"

  DEL_ARRAYS;
  ARRfree((char *)in_coords);
  ARRfree((char *)min_extent);
  ARRfree((char *)max_extent);

  // return 1 for success
  return(1);

}

