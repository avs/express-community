
flibrary Sct2UnifMods <build_dir="xp_comm_proj/sct2unif",
                       out_hdr_file="gen.hxx",
                       out_src_file="gen.cxx">
{
   // Define the low-level module
   // To set up Mesh_Unif need to specify (IX 4-27) ...
   //       nspace, ndim, dims, points
   // fill_value is cast to correct type in the module's method
   // out_fld's "points" array can't be set correctly in V because Express defines it as float array 

   module sct2unif <src_file="sct2unif.cxx",
                    cxx_hdr_files="fld/Xfld.h">
   {
      Grid_Struct+Node_Data+read+req+notify+IPort2 &in_fld;
      prim+read+req+notify+IPort2                  &fill_value;
      float+read+nres                              min_coord[]=>in_fld.coordinates.min_vec - 1;
      float+read+nres                              max_coord[]=>in_fld.coordinates.max_vec + 1;
      float+read+nres                              coords[]=>in_fld.coordinates.values;

      cxxmethod+req+notify_inst update;

      Mesh_Unif+Node_Data+write+OPort2 out_fld
      {
		   ndim=>in_fld.nspace;
			nspace=>in_fld.nspace;
			nnode_data=>in_fld.nnode_data;
		};
   };



   // Utility Macro:
   // Takes a field and copies data into a format that can be used by sct2unif.
   // Can be used to reformat data from Read_UCD.

   Grid_Struct+Node_Data+OPort field2scat {
      Field+IPort2 &in_fld<export=1>;

      &coordinates => in_fld.coordinates;
      dims => {in_fld.nnodes};
      ndim = 1;
      nspace => in_fld.nspace;
      &node_data => in_fld.node_data;
      nnode_data => in_fld.nnode_data;
   };


};

