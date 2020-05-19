
flibrary Isolabel2Macs <compile_subs=0> {


   macro Isolabel2UI {
      UImod_panel UImod_panel {
         title => "Isolabel 2";
         parent<NEportLevels={4,0}>;
      };

      XP_COMM_PROJ.Isolabel2.Isolabel2Mods.Isolabel2Params &params<NEportLevels={2,1}>;

      UIslider contourFrequencySlider {
         parent => <-.UImod_panel;
         value => <-.params.ContourFrequency;
         title => "Contour Frequency of Labels";

         min = 1.;
         max = 10.;
         mode = "integer";
         decimalPoints = 0;
         horizontal = 1;
         increment = 1;
      };

      UIslider NodeFrequencySlider {
         parent => <-.UImod_panel;
         value => <-.params.NodeFrequency;
         title => "Node Frequency of Labels";

         min = 1.;
         max = 250.;
         mode = "integer";
         decimalPoints = 0;
         horizontal = 1;
         increment = 1;
      };

      UIslider decimalsSlider {
         parent => <-.UImod_panel;
         value => <-.params.Decimals;
         title => "Number of Decimal Points";

         min = 0.;
         max = 10.;
         mode = "integer";
         decimalPoints = 0;
         horizontal = 1;
         increment = 1;
      };
   };



   macro Isolabel2Func {
      ilink in_fld;

      XP_COMM_PROJ.Isolabel2.Isolabel2Mods.Isolabel2Params &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.Isolabel2.Isolabel2Mods.Isolabel2Core Isolabel2Core {
         in_fld => <-.in_fld;
         params => <-.params;
      };

      link+Port   out_vals => Isolabel2Core.out.node_data[0].values;
      string+Port string_vals[] => str_format(params.Format, out_vals);

      olink out_fld  => Isolabel2Core.out;
      olink out_text => string_vals;
   };




   macro isolabel2 {
      ilink in_fld;

      XP_COMM_PROJ.Isolabel2.Isolabel2Mods.Isolabel2Params params {
         Active = 1;
         ContourFrequency = 1;
         NodeFrequency = 25;
         Format => "%." + Decimals + "f";
         Decimals = 2;
      };

      XP_COMM_PROJ.Isolabel2.Isolabel2Macs.Isolabel2Func Isolabel2Func {
         in_fld => <-.in_fld;
         params => <-.params;
      };

      XP_COMM_PROJ.Isolabel2.Isolabel2Macs.Isolabel2UI Isolabel2UI {
         params => <-.params;
      };

      MODS.text_glyph text_glyph {
         in_mesh => <-.Isolabel2Func.out_fld;
         in_text => <-.Isolabel2Func.out_text;
         TextUI {
            Font {
               text = "-adobe-times-bold-r-*-*-16-*-*-*-*-*-*-*";
            };
         };
      };

      olink out_fld  => Isolabel2Func.out_fld;
      olink out_text => Isolabel2Func.out_text;
      olink out_obj  => text_glyph.out_obj;
   };



   APPS.SingleWindowApp Isolabel2Eg1 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
           Top {
              child_objs => {
                 <-.<-.<-.bounds.out_obj,
                 <-.<-.<-.isoline.out_obj,
                 <-.<-.<-.isolabel2.out_obj
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

      XP_COMM_PROJ.Isolabel2.Isolabel2Macs.isolabel2 isolabel2 {
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



   APPS.SingleWindowApp Isolabel2Eg2 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.isoline.out_obj,
                  <-.<-.<-.isolabel2.out_obj
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
      XP_COMM_PROJ.Isolabel2.Isolabel2Macs.isolabel2 isolabel2 {
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


   APPS.SingleWindowApp Isolabel2Eg3 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.isoline.out_obj,
                  <-.<-.<-.isolabel2.out_obj
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
      XP_COMM_PROJ.Isolabel2.Isolabel2Macs.isolabel2 isolabel2 {
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

