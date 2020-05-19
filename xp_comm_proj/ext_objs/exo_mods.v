
flibrary ExtractObjectsMods <build_dir="xp_comm_proj/ext_objs",
                             out_src_file="gen.cxx",
                             out_hdr_file="gen.hxx">
{

   module ExtractObjectsCore <src_file="ext_objs.cxx">
   {
      imlink       in_objs;
      int+IPort2   inc_objs[];

      cxxmethod+req+notify_inst update (
         in_objs+read+notify+req,
         inc_objs+read+notify+req,
         out_objs+write
      );

      omlink out_objs;
   };

};

