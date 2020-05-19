
flibrary IsoObjectLabelApps <compile_subs=0> {

   APPS.SingleWindowApp IsoObjectLabelEg1 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
           Top {
              child_objs => {
                 <-.<-.<-.bounds.out_obj,
                 <-.<-.<-.isoline.out_obj,
                 <-.<-.<-.IsoObjectLabel.out_obj
              };
           };
         };
      };
      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>\\data\\field\\water.fld";
         };
      };
      MODS.bounds bounds {
         in_field => <-.Read_Field.field;
      };
      MODS.orthoslice orthoslice {
         in_field => <-.Read_Field.field;
         OrthoSliceParam {
            axis = 2;
            plane = 13;
         };
      };
      MODS.isoline isoline {
         in_field => <-.orthoslice.out_fld;
         IsoParam {
            level_min => 0.;
            level_max => 0.44;
         };
      };
      XP_COMM_PROJ.IsoObjectLabel.IsoObjectLabelMacs.IsoObjectLabel IsoObjectLabel {
         in_fld => <-.isoline.out_fld;
         params {
            ContourFrequency = 1;
            NodeFrequency = 8;
         };
         text_glyph {
            TextUI {
               Leadline.set = 1;
               Radial.set = 1;
               Offset.set = 1;
               StrokeHeight.value = 0.5;
               Font.text = "-adobe-times-*-r-*-*-14-*-*-*-*-*-*-*";
            };
         };
      };
   };

   APPS.SingleWindowApp IsoObjectLabelEg2 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.isoline.out_obj,
                  <-.<-.<-.IsoObjectLabel.out_obj
               };
            };
         };
      };
      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/hydrogen.fld";
         };
      };
      MODS.bounds bounds {
         in_field => Read_Field.field;
      };
      MODS.orthoslice orthoslice {
         in_field => Read_Field.field;
         OrthoSliceParam {
            axis = 2;
         };
      };
      MODS.isoline isoline {
         in_field => <-.orthoslice.out_fld;
         IsoParam {
            ncontours = 5;
            level_min => 16.41;
            level_max => 160.32;
         };
      };
      MODS.data_math data_math {
         in_field1 => <-.isoline.out_fld;
      };
      XP_COMM_PROJ.IsoObjectLabel.IsoObjectLabelMacs.IsoObjectLabel IsoObjectLabel {
         in_fld => <-.data_math.out_fld;
         params {
            Decimals = 0;
            ContourFrequency = 2;
            NodeFrequency = 30;
         };
         text_glyph {
            TextUI {
               Leadline {
                  set = 1;
               };
               Radial {
                  set = 1;
               };
               Offset {
                  set = 1;
               };
            };
         };
      };
   };

   APPS.SingleWindowApp IsoObjectLabelEg3 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.isoline.out_obj,
                  <-.<-.<-.IsoObjectLabel.out_obj
               };
            };
         };
      };
      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/helens1.fld";
         };
      };
      MODS.bounds bounds {
         in_field => <-.Read_Field.field;
      };
      MODS.surf_plot surf_plot {
         in_field => <-.Read_Field.field;
         SurfPlotParam {
            scale = 0.100;
         };
      };
      MODS.isoline isoline {
         in_field => <-.surf_plot.out_fld;
         IsoParam {
            ncontours = 12;
            level_min => 2.50;
            level_max => 3000.;
         };
      };
      MODS.data_math data_math {
         in_field1 => <-.isoline.out_fld;
      };
      XP_COMM_PROJ.IsoObjectLabel.IsoObjectLabelMacs.IsoObjectLabel IsoObjectLabel {
         in_fld => <-.data_math.out_fld;
         params {
            Decimals = 0;
            ContourFrequency = 5;
            NodeFrequency = 100;
         };
         text_glyph {
            TextUI {
               Background {
                  set = 1;
               };
               Bounds {
                  set = 1;
               };
               Font {
                  text = "-adobe-times-bold-r-*-*-12-*-*-*-*-*-*-*";
               };
            };
         };
      };
   };

}; // end of flibrary

