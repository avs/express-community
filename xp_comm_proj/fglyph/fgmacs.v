
flibrary FastGlyphMacs <compile_subs=0> {

   macro FastGlyph2DUI {
      UImod_panel UImod_panel {
         title => "Fast Glyph 2D";
         parent<NEportLevels={4,0}>;
      };

      XP_COMM_PROJ.FastGlyph.FastGlyphMods.FastGlyph2DParams &FastGlyph2DParams<NEportLevels={2,1}>;

      UIslider scaling_slider {
         parent => <-.UImod_panel;
         value => <-.FastGlyph2DParams.scale;
         title => "Glyph Scale";

         min = 0.;
         max = 10.;
         mode = "real";
         decimalPoints = 2;
         horizontal = 1;
         increment = 1;
      };

      UIslider angle_offset_slider {
         parent => <-.UImod_panel;
         value => <-.FastGlyph2DParams.a_offset;
         title => "Angle Offset";

         min = 0.;
         max = 360.;
         mode = "real";
         decimalPoints = 2;
         horizontal = 1;
         increment = 5;
      };

      UIslider angle_multipler_slider {
         parent => <-.UImod_panel;
         value => <-.FastGlyph2DParams.a_multiplier;
         title => "Angle Scale";

         min = 0.;
         max = 5.;
         mode = "real";
         decimalPoints = 2;
         horizontal = 1;
         increment = 0.5;
      };
   };

   macro FastGlyph3DUI {
      UImod_panel UImod_panel {
         title => "Fast Glyph 3D";
         parent<NEportLevels={4,0}>;
      };

      float &scale<NEportLevels={2,1}>;

      UIslider scaling_slider {
         parent => <-.UImod_panel;
         value => <-.scale;
         title => "Glyph Scale";

         min = 0.;
         max = 10.;
         mode = "real";
         decimalPoints = 2;
         horizontal = 1;
         increment = 1;
      };
   };


   macro FastGlyph2D {
      ilink glyph_fld;
      ilink coords;
      ilink angles;

      XP_COMM_PROJ.FastGlyph.FastGlyphMods.FastGlyph2DParams &FastGlyph2DParams<NEportLevels={2,1}>;

      XP_COMM_PROJ.FastGlyph.FastGlyphMods.FastGlyph2DCore FastGlyph2DCore {
         coords => <-.coords;
         angles => <-.angles;
         glyph_fld => <-.glyph_fld;
         params => <-.FastGlyph2DParams;
      };

      group Mesh_Fld {
         Mesh Mesh<NEportLevels={0,2}> {
            coordinates {
               values<NEportLevels={4,0}> => <-.<-.<-.FastGlyph2DCore.coordOut;
            };
            cell_set = {
               {
                  cell_nnodes => <-.<-.cell_nnodes,
                  node_connect_list => <-.<-.node_connect_list,
                  ncells => <-.<-.ncells,,,,,
                  cell_ndim => <-.<-.cell_ndim,
                  cell_order=1,
                  poly_flag=0,,,,,
               }
            };
            ncell_sets = 1;
            nspace = 2;
            nnodes<NEportLevels={3,0}> => <-.<-.FastGlyph2DCore.ncoordOut;
         };
         int ncells<NEportLevels={2,1}> => <-.FastGlyph2DCore.ncells;
         int cell_ndim<NEportLevels={2,1}> => <-.FastGlyph2DCore.cell_ndim;
         int cell_nnodes<NEportLevels={2,1}> => <-.FastGlyph2DCore.cell_nnodes;
         int node_connect_list<NEportLevels={2,1}>[] => <-.FastGlyph2DCore.node_connect_list;
      };

      olink out_mesh => .Mesh_Fld.Mesh;
   };



   macro FastGlyph3D {
      ilink glyph_fld;
      ilink coords;

      float &scale<NEportLevels={2,1}>;

      XP_COMM_PROJ.FastGlyph.FastGlyphMods.FastGlyph3DCore FastGlyph3DCore {
         coords => <-.coords;
         glyph_fld => <-.glyph_fld;
         scale => <-.scale;
      };

      group Mesh_Fld {
         Mesh Mesh<NEportLevels={0,2}> {
            coordinates {
               values<NEportLevels={4,0}> => <-.<-.<-.FastGlyph3DCore.coordOut;
            };
            cell_set = {
               {
                  cell_nnodes => <-.<-.cell_nnodes,
                  node_connect_list => <-.<-.node_connect_list,
                  ncells => <-.<-.ncells,,,,,
                  cell_ndim => <-.<-.cell_ndim,
                  cell_order=1,
                  poly_flag=0,,,,,
               }
            };
            ncell_sets = 1;
            nspace = 3;
            nnodes<NEportLevels={3,0}> => <-.<-.FastGlyph3DCore.ncoordOut;
         };
         int ncells<NEportLevels={2,1}> => <-.FastGlyph3DCore.ncells;
         int cell_ndim<NEportLevels={2,1}> => <-.FastGlyph3DCore.cell_ndim;
         int cell_nnodes<NEportLevels={2,1}> => <-.FastGlyph3DCore.cell_nnodes;
         int node_connect_list<NEportLevels={2,1}>[] => <-.FastGlyph3DCore.node_connect_list;
      };

      olink out_mesh => .Mesh_Fld.Mesh;
   };



   macro fast_glyph_2D {
      ilink glyph_fld;
      ilink coords;
      ilink angles;

      XP_COMM_PROJ.FastGlyph.FastGlyphMods.FastGlyph2DParams FastGlyph2DParams {
         scale = 1;
         a_multiplier = 1;
         a_offset = 0;
      };

      XP_COMM_PROJ.FastGlyph.FastGlyphMacs.FastGlyph2DUI FastGlyph2DUI {
         FastGlyph2DParams => <-.FastGlyph2DParams;
      };

      XP_COMM_PROJ.FastGlyph.FastGlyphMacs.FastGlyph2D FastGlyph2D {
         coords => <-.coords;
         angles => <-.angles;
         glyph_fld => <-.glyph_fld;
         FastGlyph2DParams => <-.FastGlyph2DParams;
      };

      olink out_mesh => .FastGlyph2D.out_mesh;

#ifdef VIZ_EXPRESS
      GDM.DataObject DataObject {
         in => <-.FastGlyph2D.out_mesh;
         Obj {
            name => name_of(<-.<-.<-);
         };
      };

      olink out_obj => .DataObject.obj;
#else
      DataObjectLite DataObjectLite {
         in => <-.FastGlyph2D.out_mesh;
         Obj {
            name => name_of(<-.<-.<-);
         };
      };

      olink out_obj => .DataObjectLite.obj;
#endif

   };



   macro fast_glyph_3D {
      ilink glyph_fld;
      ilink coords;

      float scale = 1;

      XP_COMM_PROJ.FastGlyph.FastGlyphMacs.FastGlyph3DUI FastGlyph3DUI {
         scale => <-.scale;
      };

      XP_COMM_PROJ.FastGlyph.FastGlyphMacs.FastGlyph3D FastGlyph3D {
         coords => <-.coords;
         glyph_fld => <-.glyph_fld;
         scale => <-.scale;
      };

      olink out_mesh => .FastGlyph3D.out_mesh;

#ifdef VIZ_EXPRESS
      GDM.DataObject DataObject {
         in => <-.FastGlyph3D.out_mesh;
         Obj {
            name => name_of(<-.<-.<-);
         };
      };

      olink out_obj => .DataObject.obj;
#else
      DataObjectLite DataObjectLite {
         in => <-.FastGlyph3D.out_mesh;
         Obj {
            name => name_of(<-.<-.<-);
         };
      };

      olink out_obj => .DataObjectLite.obj;
#endif

   };




   APPS.MultiWindowApp FastGlyph2D_AngleEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.fast_glyph_2D.out_obj};
            };
         };
      };

      float+Port x[] = {1.,2.,1.,2.};
      float+Port y[] = {0.,0.,1.,1.};
      float+Port angles[] = {30.,90.,150.,210.};

      FLD_MAP.interleave_2_arrays interleave_2_arrays {
         in1 => <-.x;
         in2 => <-.y;
      };
      GEOMS.Arrow1 Arrow1;

      XP_COMM_PROJ.FastGlyph.FastGlyphMacs.fast_glyph_2D fast_glyph_2D {
         FastGlyph2DParams {
            scale = 0.5;
         };
         glyph_fld => <-.Arrow1.out_fld;
         coords => <-.interleave_2_arrays.out;
         angles => <-.angles;
      };
   };



   APPS.MultiWindowApp FastGlyph2D_SizeEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.fast_glyph_2D.out_obj};
            };
         };
      };

      GEOMS.Diamond2D Diamond2D;

      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/hydrogen.fld";
         };
      };
      MODS.orthoslice orthoslice {
         in_field => <-.Read_Field.field;
         OrthoSliceParam {
            axis = 2;
            plane => 32;
         };
      };
      FLD_MAP.extract_coordinate_array extract_coordinate_array {
         in => <-.orthoslice.out_fld;
         comp = {0,1};
      };

      XP_COMM_PROJ.FastGlyph.FastGlyphMacs.fast_glyph_2D fast_glyph_2D {
         FastGlyph2DParams {
            scale = 0.3;
         };
         coords => <-.extract_coordinate_array.coord;
         glyph_fld => <-.Diamond2D.out_fld;
      };
   };



   APPS.MultiWindowApp FastGlyph3D_SimpleEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.fast_glyph_3D.out_obj};
            };
         };
      };

      float+Port x[] = {1.,2.,1.,2.};
      float+Port y[] = {0.,0.,1.,1.};
      float+Port z[] = {0.,1.,2.,3.};

      FLD_MAP.interleave_3_arrays interleave_3_arrays {
         in1 => <-.x;
         in2 => <-.y;
         in3 => <-.z;
      };
      GEOMS.Diamond3D Diamond3D;

      XP_COMM_PROJ.FastGlyph.FastGlyphMacs.fast_glyph_3D fast_glyph_3D {
         scale = 0.5;
         glyph_fld => <-.Diamond3D.out_fld;
         coords => <-.interleave_3_arrays.out;
      };
   };



   APPS.MultiWindowApp FastGlyph3D_SizeEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.fast_glyph_3D.out_obj};
               Xform {
                  ocenter = {31.5,31.5,31.5};
                  dcenter = {3.94984,3.94984,3.94984};
                  mat = {
                     0.0706942,-0.00221173,-0.0437982,0.,
                     -0.0423578,0.0180737,-0.069282,0.,
                     0.0113573,0.0811745,0.0142325,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-3.94984,-3.94984,-3.94984};
               };
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };


      GEOMS.Cross3D Cross3D;


      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/hydrogen.fld";
         };
      };

      MODS.external_edges external_edges {
         in_field => <-.Read_Field.field;
      };
      MODS.orthoslice orthoslice {
         in_field => <-.Read_Field.field;
      };
      MODS.isosurface isosurface {
         in_field => <-.Read_Field.field;
         IsoParam {
            iso_level => 200.;
         };
      };

      FLD_MAP.extract_coordinate_array extract_coordinate_array_edges {
         in => <-.external_edges.out_fld;
         comp = {0,1,2};
      };
      FLD_MAP.extract_coordinate_array extract_coordinate_array_slices {
         in => <-.orthoslice.out_fld;
         comp = {0,1,2};
      };
      FLD_MAP.extract_coordinate_array extract_coordinate_array_surf {
         in => <-.isosurface.out_fld;
         comp = {0,1,2};
      };
      FLD_MAP.concat_3_arrays concat_3_arrays {
         in1 => <-.extract_coordinate_array_edges.coord;
         in2 => <-.extract_coordinate_array_slices.coord;
         in3 => <-.extract_coordinate_array_surf.coord;
      };


      XP_COMM_PROJ.FastGlyph.FastGlyphMacs.fast_glyph_3D fast_glyph_3D {
         scale = 0.8;
         glyph_fld => <-.Cross3D.out_fld;
         coords => <-.concat_3_arrays.out;
      };
   };


};

