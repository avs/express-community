
flibrary Scalar2Texture2Mods <build_dir="xp_comm_proj/scl2tex2",
                      out_hdr_file="gen.hxx",
                      out_src_file="gen.cxx">
{

   // define the low-level module

   module Scalar2Texture2Core <src_file="scl2tex2.cxx",
                              libdeps="FLD",
                              cxx_hdr_files="fld/Xfld.h">
   {
      Node_Data+IPort2 &in;

      cxxmethod+notify_inst+req update (
         in+read+notify+req,
         out+write
      );

      Node_Data+OPort2 out;
   };

};

