
flibrary FastGlyphMods <build_dir="xp_comm_proj/fglyph",
                        out_src_file="gen.cxx",
                        out_hdr_file="gen.h">
{

   group+OPort FastGlyph2DParams {
      float+Port2 scale;
      float+Port2 a_offset;
      float+Port2 a_multiplier;
   };


   module FastGlyph2DCore <src_file="fglyph2d.cxx",
                           libdeps="FLD",
                           cxx_hdr_files="fld/Xfld.h">
   {
      cxxmethod+req update(
         .coords+read+req+notify,
         .ncoords+read+req,
         .angles+read+notify,
         .glyph_fld+read+req+notify,
         .scale+read+notify,
         .a_multiplier+read+notify,
         .a_offset+read+notify,
         .ncoordOut+write,
         .coordOut+write,
         .ncells+write,
         .cell_ndim+write,
         .cell_nnodes+write,
         .node_connect_list+write
      );

      float+IPort2 coords[][2];
      int          ncoords => array_size(coords);
      float+IPort2 angles[];
      Mesh+IPort2  &glyph_fld;

      FastGlyph2DParams+IPort2 &params;

      float scale        => params.scale;
      float a_offset     => params.a_offset;
      float a_multiplier => params.a_multiplier;

      int+OPort2   ncoordOut;
      float+OPort2 coordOut[ncoordOut][2];
      int+OPort2   ncells;
      int+OPort2   cell_ndim;
      int+OPort2   cell_nnodes;
      int+OPort2   node_connect_list[ncells*cell_nnodes];
   };



   module FastGlyph3DCore <src_file="fglyph3d.cxx",
                           libdeps="FLD",
                           cxx_hdr_files="fld/Xfld.h">
   {
      cxxmethod+req update(
         .coords+read+req+notify,
         .ncoords+read+req,
         .glyph_fld+read+req+notify,
         .scale+read+notify,
         .ncoordOut+write,
         .coordOut+write,
         .ncells+write,
         .cell_ndim+write,
         .cell_nnodes+write,
         .node_connect_list+write
      );

      float+IPort2 coords[][3];
      int          ncoords => array_size(coords);
      Mesh+IPort2  &glyph_fld;

      float+IPort2 scale;

      int+OPort2   ncoordOut;
      float+OPort2 coordOut[ncoordOut][3];
      int+OPort2   ncells;
      int+OPort2   cell_ndim;
      int+OPort2   cell_nnodes;
      int+OPort2   node_connect_list[ncells*cell_nnodes];
   };

}; // end of lib

