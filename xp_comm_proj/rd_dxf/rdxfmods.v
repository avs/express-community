
flibrary ReadDXFMods<build_dir="xp_comm_proj/rd_dxf",
                     out_src_file="gen.cxx",
                     out_hdr_file="gen.hxx">

{

   group+OPort ReadDXFParams {
      string+Port2 file_name;
      int+Port2    reverse_normals;
      int+Port2    fill_polygons;
      int+Port2    layers[];
   };


   module ReadDXFCore <libdeps="FLD",
                       cxx_hdr_files="fld/Xfld.h",
                       cxx_src_files="src/avsutils.cxx  src/blistcpp.cxx
                                      src/dxfprim.cxx   src/dxffile.cxx
                                      src/dxfcir.cxx    src/dxfray.cxx 
                                      src/dxfline.cxx   src/dxfface.cxx 
                                      src/dxfpline.cxx  src/dxfarc.cxx 
                                      src/dxfpoint.cxx  src/dxfsolid.cxx
                                      src/dxfbody.cxx   src/dxfrecordblock.cxx
                                      src/dxfleader.cxx src/dxftext.cxx
                                      src/dxfblock.cxx  src/dxfdimension.cxx
                                      src/dxfinsert.cxx src/dxfellipse.cxx
                                      src/dxfmtext.cxx  src/dxfmline.cxx
                                      src/dxftrace.cxx  src/dxfheader.cxx
                                      src/dxfxline.cxx",
                       src_file="rd_dxf.cxx">
   {
      ReadDXFParams+IPort2 &params;

      string+read+notify+req file_name => params.file_name;
      int+read+notify reverse_normals => params.reverse_normals;
      int+read+notify fill_polygons => params.fill_polygons;

      cxxmethod+req+notify_inst update;

      //ptr+read+write dxf_file_ptr;

      int+write          nOut=0;
      string+write       layernames[nOut];
      Field+write+OPort2 out[nOut];

      int+write    nText=0;
      Xform+OPort2 Text[nText] {
         int align_horiz;
         int align_vert;
         int drop_shadow;
         int background;
         int bounds;
         int underline;
         int lead_line;
         int radial;
         int do_offset;
         float offset[3];
         int xform_mode;
         string str;
         int nspace;
         float position[nspace];
         int stroke = 0;

         group StrokeTextAttribs {
            int font_type;
            int style;
            int plane;
            int orient;
            int path;
            int space_mode;
            float spacing;
            float angle;
            float height;
            float expansion;
            float width;
         };

         float+write min_vec[nspace];
         float+write max_vec[nspace];
      };

      float+write+OPort2 urc[3];
      float+write+OPort2 llc[3];
   };

};

