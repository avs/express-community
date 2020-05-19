
flibrary CylinderCutMods {

   module CylinderCutDist < build_dir="xp_comm_proj/cylcut",
                            src_file="cylcut.c",
                            out_src_file="gen.cxx",
                            out_hdr_file="gen.h"
                	        > {
      /* compute distance to a plane */

	   Mesh+Iparam  &in {
   		nnodes+req;
	   };
   	Mesh+Xform+Iparam &plane;

	   method+notify_val+notify_inst cylinder_cut_update<status=1> = "cylinder_cut_update";

	   Node_Data+Scalar+Float+Oparam out;
   	float+Oparam dist;
   }; 

};

