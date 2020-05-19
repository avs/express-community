

/*
  Copyright / Disclaimer :

  This software/documentation was produced as part of the INDEX project
  (Intelligent Data Extraction) which is funded under contract ESPRIT EP22745 of 
  the European Community. For further details see
  http://www.man.ac.uk/MVC/research/INDEX/Public/. 

  Copyright (c) June 1998, Manchester Visualisation Centre, UK. All Rights Reserved.

  Permission to use, copy, modify and distribute this software and its documentation
  is hereby granted without fee, provided that the above copyright notice and this
  permission notice appear in all copies of this software/documentation.

  This software/documentation is provided with no warranty, express or implied, 
  including, without limitation, warrant of merchantability or fitness for a 
  particular purpose.
*/


/*
  AVS/Express field examination/info tool.

  Amardeep S Bhattal.
  March 1998.
  a.bhattal@mcc.ac.uk

  Looks at the input field, and prints out the number of arrays, and 
  information about each array - name, type, length, size.
*/


#include <iomanip.h>
#include <iostream.h>
#ifdef MSDOS
  #include <strstrea.h>
#else
  #include <strstream.h>
#endif
#include "out_hdr.h"


void n_arrays_info
  (OMobj_id field_id, strstream& info);

void array_info
  (OMobj_id array_id, strstream& info);


/* -------------------------------------------------------------------------- */

void n_arrays_info
  (OMobj_id field_id, strstream& info)
{
  info
   << n_arrays (field_id) << " arrays" << endl 
   << "(" 
   << n_grid_arrays (field_id) << " gr, " 
   << n_node_data_arrays (field_id) << " nd, "
   << n_cell_data_arrays (field_id) << " cd, " 
   << n_connectivity_arrays (field_id) << " co"
   << ")." << endl
   << endl;
}

/* -------------------------------------------------------------------------- */

void array_info
  (OMobj_id array_id, strstream& info)
{
  char *label_p, *type_p;
  int  size;

  info.setf(ios::fixed,ios::floatfield);
  info.precision (0);
  array_label (array_id, &label_p, OM_OBJ_RD);
  if (label_p==NULL)
    info << "?";
  else
    info << label_p;
  info << endl;
  type_to_string (array_type(array_id), &type_p);
  if (type_p==NULL)
    info << "?";
  else
    info << type_p;
  info << " [" << array_length(array_id) << "], ";
  size = array_size (array_id);
  if (size < 1024)
    info << size << " bytes.";
  else
    info << size/1024.0 << "kb";
  info << endl << endl;
  if (label_p)
    free (label_p);
  if (type_p)
    free (type_p);
  info.setf(0,ios::floatfield);
  info.precision (6);
}

/* -------------------------------------------------------------------------- */

/*
void print_array_info
  (OMobj_id array_id)
{
  char *label_p, *type_p;

  cout << "  " << setfill('-') << setw(70) << "" << setfill(' ') << endl;
  array_label (array_id, &label_p, OM_OBJ_RD);
  if (label_p==NULL)
    cout << setw(14) << "?";
  else
    cout << setw(14) << label_p;
  cout << " [" << setw(7) << array_length(array_id) << "], ";
  type_to_string (array_type(array_id), &type_p);
  if (type_p==NULL)
    cout << setw(6) << "?" << ", ";
  else
    cout << setw(6) << type_p << ", ";
  cout << setw(8) << array_size(array_id) << " bytes." << endl;
  if (label_p)
    free (label_p);
  if (type_p)
    free (type_p);
  cout << "  " << setfill('-') << setw(70) << "" << setfill(' ') << endl;
}
*/

/* -------------------------------------------------------------------------- */

int
Examine_Field_Examine_Field_Prim::update(OMevent_mask event_mask, int seq_num)
{
  int       i;
  strstream info;
  char      *string_p;

  if (this_is_a_field((OMobj_id)data)) {
    text_rows = 3*n_arrays ((OMobj_id)data) + 2;
    n_arrays_info ((OMobj_id)data, info);
    for (i=0; i<n_arrays((OMobj_id)data); i++) {
      info << i+1 << " -  ";
      array_info (ith_array_id ((OMobj_id)data, i, OM_OBJ_RD), info);
    }
    info << ends;
    string_p = info.str();
    text_string = string_p;
    delete string_p;
  }
  else
    cout << "  this is not a field." << endl << endl;
  return(1);
}

/* -------------------------------------------------------------------------- */
