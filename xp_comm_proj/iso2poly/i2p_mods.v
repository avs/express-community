
flibrary Isoline2PolylineMods <build_dir="xp_comm_proj/iso2poly",
                               out_src_file="gen.cxx",
                               out_hdr_file="gen.hxx"> {

   group+OPort Isoline2PolylineParams {
      int+Port2 Active;
      int+Port2 Downsize;
      int+Port2 UseNodeData;
      int+Port2 AutoClose;
   };


   module Isoline2PolylineCore <src_file="iso2poly.cxx",
                                libdeps="FLD",
                                cxx_hdr_files="fld/Xfld.h">
   {
      Mesh+Node_Data &in_fld<NEportLevels={2,1}>;
      Isoline2PolylineParams &params<NEportLevels={2,1}>;

      int Active      => params.Active;
      int Downsize    => params.Downsize;
      int UseNodeData => params.UseNodeData;
      int AutoClose   => params.AutoClose;

      cxxmethod update (
         Active+read+notify+req,
         Downsize+read+notify+req,
         UseNodeData+read+notify+req,
         AutoClose+read+notify+req,
         in_fld+read+notify+req,
         out+write
      );

      Mesh+Node_Data out<NEportLevels={1,2}> {
         &xform => in_fld.xform;
         ncell_sets = 0;
         Polyline cell_set[ncell_sets];
      };
   };

};

