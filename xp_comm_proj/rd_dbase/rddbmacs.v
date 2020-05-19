
flibrary ReadDbaseMacs <compile_subs=0> {

   module comb_mesh_and_data {
      Mesh+IPort2 &in_mesh ;
      Node_Data+nres+IPort2 &in_nd;
      Mesh+Node_Data+OPort2 &out => merge(in_nd, in_mesh);
   };


   APPS.SingleWindowApp ReadDbaseEg {

      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.glyph.out_obj,<-.<-.<-.text_glyph.out_obj
               };
            };
         };
      };


      string+OPort filename = "xp_comm_proj/rd_dbase/cities.dbf";

      XP_COMM_PROJ.ReadDbase.ReadDbaseMods.ReadDBFString ReadDBFString {
         DBFFileName => <-.filename;
         ColumnName = "CITY_NAME";
      };
      XP_COMM_PROJ.ReadDbase.ReadDbaseMods.ReadDBFFloat ReadDBFFloat {
         DBFFileName => <-.filename;
         ColumnName = "POPULATION";
      };
      XP_COMM_PROJ.ReadDbase.ReadDbaseMods.ReadDBFFloat x_ReadDBFFloat {
         DBFFileName => <-.filename;
         ColumnName = "XCOORD";
      };
      XP_COMM_PROJ.ReadDbase.ReadDbaseMods.ReadDBFFloat y_ReadDBFFloat {
         DBFFileName => <-.filename;
         ColumnName = "YCOORD";
      };
      XP_COMM_PROJ.ReadDbase.ReadDbaseMods.ReadDBFDate ReadDBFDate {
         DBFFileName => <-.filename;
         ColumnName = "SAMPLE_DAT";
      };


      FLD_MAP.interleave_2_arrays interleave_2_arrays {
         in1 => <-.x_ReadDBFFloat.ColumnValues;
         in2 => <-.y_ReadDBFFloat.ColumnValues;
      };
      FLD_MAP.point_mesh point_mesh {
         coord => <-.interleave_2_arrays.out;
      };

      FLD_MAP.node_scalar node_scalar {
         in_data => <-.ReadDBFFloat.ColumnValues;
      };
      comb_mesh_and_data comb_mesh_and_data {
         in_mesh => <-.point_mesh.out;
         in_nd => <-.node_scalar.out;
      };


      MODS.text_glyph text_glyph {
         in_mesh => <-.point_mesh.out;
         in_text => <-.ReadDBFString.ColumnValues;
         TextUI {
            Leadline {
               set = 1;
            };
            Offset {
               set = 1;
            };
            OffsetXValue {
               field {
                  value = 0.03;
               };
            };
            OffsetYValue {
               field {
                  value = 0.03;
               };
            };
            Font {
               text = "-adobe-times-bold-r-*-*-18-*-*-*-*-*-*-*";
            };
         };
      };


      GEOMS.Diamond2D Diamond2D;

      MODS.glyph glyph {
         in_field => <-.comb_mesh_and_data.out;
         in_glyph => <-.Diamond2D.out_fld;
         GlyphParam {
            vector = 0;
            scale = 3e-007;
         };
         GlyphUI {
            scale_slider {
               decimalPoints = 8;
            };
         };
      };

   };

};

