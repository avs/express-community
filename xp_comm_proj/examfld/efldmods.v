

// Copyright / Disclaimer :

// This software/documentation was produced as part of the INDEX project
// (Intelligent Data Extraction) which is funded under contract ESPRIT EP22745 of 
// the European Community. For further details see
// http://www.man.ac.uk/MVC/research/INDEX/Public/. 

// Copyright (c) June 1998, Manchester Visualisation Centre, UK. 
// All Rights Reserved.

// Permission to use, copy, modify and distribute this software and its 
// documentation is hereby granted without fee, provided that the above copyright
// notice and this permission notice appear in all copies of this software / 
// documentation.

// This software/documentation is provided with no warranty, express or implied, 
// including, without limitation, warrant of merchantability or fitness for a 
// particular purpose.


flibrary Examine_FieldMods
<
  build_dir    = "xp_comm_proj/examfld",
  out_hdr_file = "out_hdr.h",
  out_src_file = "out_src.cxx",
  cxx_name     = ""
>
{
  group Examine_Field_Param {
    int    text_rows   <NEportLevels={2,2}>;
    string text_string <NEportLevels={2,2}>;
  };
  module Examine_Field_Prim
  <
    libdeps       = "FLD",
    hdr_dirs      = "../utils_xp",
    src_file      = "examfld.cxx",
    cxx_hdr_files = "fld/Xfld.h array_fn.h field_fn.h om_fn.h",
    cxx_src_files = "../utils_xp/array_fn.cxx ../utils_xp/field_fn.cxx ../utils_xp/om_fn.cxx"
  > 
  {
    int    text_rows   <NEportLevels={2,1}>;
    string text_string <NEportLevels={2,1}>;
    group  &data       
    <
      NEportLevels={2,0},NEx=429.,NEy=286.
    >
    {
      int nnodes;
      int nspace;
      Data_Array coordinates {
         nvals => <-.nnodes;
         veclen => <-.nspace;
         float values[.nvals][.veclen];
         float min => cache(min_array(magnitude(.values),0,0));
         float max => cache(max_array(magnitude(.values),0,0));
         float min_vec[.veclen] => cache(min_array(.values,0,0));
         float max_vec[.veclen] => cache(max_array(.values,0,0));
      };
      int+opt   ndim;
      int+opt   dims[.ndim];
      int+opt   npoints;
      float+opt points[.npoints][.nspace];
      int+opt   grid_type;
      int+opt   ncell_sets;
      Cell_Set+opt cell_set[.ncell_sets] {
        int ncell_data;
        Data_Array cell_data[.ncell_data] {
          nvals => <-.ncells;
        };
      };
      DefaultXform+opt xform;
      int+opt          nnode_data;
      Data_Array+opt   node_data[.nnode_data] {
        nvals => <-.nnodes;
      };
    };
    cxxmethod+req update (data+notify+req, data.nnodes+req);
  };
};
