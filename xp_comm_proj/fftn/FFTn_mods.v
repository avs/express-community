
//
// Copyright (C) Chris Pudney, The University of Western Australia, 1998.
// All rights reserved.
// 
// Permission to use, copy, modify and distribute this software and its
// documentation only for the purposes of teaching and research is hereby
// granted without fee, provided that the above copyright notice and this
// permission notice appear in all copies of this software/documentation and
// that you do not sell the software.  No commercial use or distribution
// of the software is permitted without the consent of the copyright
// owners.  Commercial licensing is available by contacting the author(s).
//
// THIS SOFTWARE/DOCUMENTATION IS PROVIDED WITH NO WARRANTY, EXPRESS OR
// IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTY OF MERCHANTABILITY OR
// FITNESS FOR A PARTICULAR PURPOSE.
//
flibrary FFTnMods <build_dir="xp_comm_proj",
  link_files="fftn/fftn/fftn.o",
  out_hdr_file="fftn/gen.h",
  out_src_file="fftn/gen.cxx">
{
  // FFTn_params
  // Parameter group
  //
  group FFTn_params<NEportLevels={0,1}>
    {
      int direction<NEportLevels={2,2}>;
      int rescale<NEportLevels={2,2}>;
    };


  // FFTn_raw
  // Low-level module - Contains Parameter group FFTn_params
  // 
  module FFTn_raw <c_src_files="cp_utils/avs_err.c cp_utils/data_field.c",
    hdr_dirs="cp_utils fftn/fftn",
    src_file="fftn/FFTn_vxp.c">
    {
      Mesh_Unif+Node_Data &in_field<export=2,NEportLevels={2,0}>;
      Mesh_Unif+Node_Data out_field<export=2,NEportLevels={0,2}>;
      FFTn_params &FFTn_params;
      boolean direction => FFTn_params.direction;
      int rescale => FFTn_params.rescale;
      omethod+notify_inst+req FFTupdate
	(direction+read+notify+req,
	 in_field+read+notify+req,
	 rescale+read+notify+req,
	 out_field+write ) = "FFTn_vxp";
    };
};
