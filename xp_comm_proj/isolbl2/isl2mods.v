
flibrary Isolabel2Mods <build_dir="xp_comm_proj/isolbl2",
                        out_src_file="gen.cxx",
                        out_hdr_file="gen.hxx"> {

   group+OPort Isolabel2Params {
      int+Port2    Active;
      int+Port2    ContourFrequency;
      int+Port2    NodeFrequency;
      int+Port2    Decimals;
      string+Port2 Format;
   };


   module Isolabel2Core <src_file="isolbl2.cxx",
                         libdeps="FLD",
                         cxx_hdr_files="fld/Xfld.h"> {

      Mesh+Node_Data &in_fld<NEportLevels={2,1}>;
      Isolabel2Params &params<NEportLevels={2,1}>;

      int Active => params.Active;
      int ContourFrequency => params.ContourFrequency;
      int NodeFrequency => params.NodeFrequency;

      cxxmethod update (
         in_fld+read+notify+req,
         Active+read+notify+req,
         ContourFrequency+read+notify+req,
         NodeFrequency+read+notify+req,
         out+write
      );

      Mesh+Node_Data out<NEportLevels={1,2}> {
         &xform => in_fld.xform;
      };
   };

};

