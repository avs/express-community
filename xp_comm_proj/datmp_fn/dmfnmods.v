
flibrary DatamapFunctionMods <build_dir="xp_comm_proj/datmp_fn",
                              out_hdr_file="gen.hxx",
                              out_src_file="gen.cxx">
{

   // define the parameter block group

   group+OPort DatamapFunctionParams {
      int+Port2 datamap_function;
   };



   // define the low-level module

   module DatamapFunctionCore <src_file="datmp_fn.cxx",
                               libdeps="FLD",
                               cxx_hdr_files="fld/Xfld.h">
   {
      Node_Data+IPort2 &in;
      DatamapFunctionParams+IPort2 &params;

      int datamap_function => params.datamap_function;

      cxxmethod+notify_inst+req update (
         in+read+notify+req,
         datamap_function+read+notify+req,
         out+write
      );

      Node_Data+OPort2 out;
   };

};

