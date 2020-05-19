
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
flibrary NStatsMods <build_dir="xp_comm_proj",
                    out_hdr_file="nstats/gen.h",
                    out_src_file="nstats/gen.cxx">
{
  // nstats_raw
  // Low-level module
  //
  module nstats_raw <src_file="nstats/node_stats_vxp.c",
    c_src_files="nstats/node_stats.c cp_utils/avs_err.c cp_utils/data_field.c",
    hdr_dirs="nstats cp_utils">
    {
      Mesh_Unif+Node_Data &input<NEportLevels={2,0},export=2>;
      string info<NEportLevels={1,2}>;
      omethod+notify_inst+req update
	(input+read+notify+req,
	info+write) = "node_stats_vxp";
    };
};
