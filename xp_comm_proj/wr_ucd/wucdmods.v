
flibrary WriteUCDMods <build_dir="xp_comm_proj/wr_ucd",
                       out_src_file="gen.cxx",
                       out_hdr_file="gen.hxx">
{

   module WriteUCDCore <src_file="wr_ucd.cxx",
                        libdeps="FLD",
                        cxx_hdr_files="fld/Xfld.h">
   {

      string+IPort2         filename<export=2>;
      Mesh+Node_Data+Cell_Data+IPort2 &in<export=2>[];
      string+IPort2         top_name<export=2> = "default_avs_field";
      int+IPort2            nobjs<export=2>;
      int+IPort2            binary<export=2>;

      cxxmethod+req update<status=1> (
         filename+read+notify+req,
         in+read+req,
         top_name+read,
         nobjs+read,
         binary+read,
         .err+write,
         .err_str+write
      );

      int+OPort2    err<export=2> = 0;
      string+OPort2 err_str<export=2> = "";
   };

};

