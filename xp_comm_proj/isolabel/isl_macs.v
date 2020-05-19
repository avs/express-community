
flibrary IsolabelMacs <compile_subs=0> {

   macro Isolabel {
      ilink in;

      XP_COMM_PROJ.Isolabel.IsolabelMods.Isolabel_Params &Isolabel_Params<NEportLevels={2,1}>;

      XP_COMM_PROJ.Isolabel.IsolabelMods.Isolabel_core Isolabel_core {
         in => <-.in;
         num_labels => <-.Isolabel_Params.num_labels;
         format => <-.Isolabel_Params.format;
      };

      olink out_mesh => Isolabel_core.out;
      olink out_text => Isolabel_core.string_vals;
   };



   macro IsolabelUI {
      UImod_panel UImod_panel {
         title => "Isolabel";
         parent<NEportLevels={4,0}>;
      };

      XP_COMM_PROJ.Isolabel.IsolabelMods.Isolabel_Params &Isolabel_Params<NEportLevels={2,1}>;

      UIslider num_labels_slider {
         parent => <-.UImod_panel;
         value => <-.Isolabel_Params.num_labels;
         title => "Number of Labels";

         min = 0.;
         max = 50.;
         mode = "integer";
         decimalPoints = 0;
         horizontal = 1;
         increment = 1;
      };

      UIslider dec_pnts_slider {
         parent => <-.UImod_panel;
         value => <-.Isolabel_Params.dec_pnts;
         title => "Number of Decimal Points";

         min = 0.;
         max = 10.;
         mode = "integer";
         decimalPoints = 0;
         horizontal = 1;
         increment = 1;
      };
   };


   macro isolabel {
      ilink in;

      XP_COMM_PROJ.Isolabel.IsolabelMods.Isolabel_Params Isolabel_Params {
         num_labels = 25;
         format => "%0." + dec_pnts + "f";
         dec_pnts = 2;
      };

      XP_COMM_PROJ.Isolabel.IsolabelMacs.Isolabel Isolabel {
         in => <-.in;
         Isolabel_Params => <-.Isolabel_Params;
      };

      XP_COMM_PROJ.Isolabel.IsolabelMacs.IsolabelUI IsolabelUI {
         Isolabel_Params => <-.Isolabel_Params;
      };

      MODS.text_glyph text_glyph {
         in_mesh => <-.Isolabel.out_mesh;
         in_text => <-.Isolabel.out_text;
         TextUI {
            Font {
               text = "-adobe-times-bold-r-*-*-16-*-*-*-*-*-*-*";
            };
         };
      };

      olink out_mesh => Isolabel.out_mesh;
      olink out_text => Isolabel.out_text;
      olink out_obj => text_glyph.out_obj;
   };



   APPS.SingleWindowApp IsolabelEg1 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {bounds.out_obj,<-.<-.<-.isoline.out_obj,<-.<-.<-.isolabel.out_obj};
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
      XP_COMM_PROJ.Isolabel.IsolabelMacs.isolabel isolabel {
         in => <-.isoline.out_fld;
      };

/*
      MODS.text_glyph text_glyph {
         in_mesh => <-.isolabel.out_mesh;
         in_text => <-.isolabel.out_text;
         TextUI {
            Font {
               text = "-adobe-times-bold-r-*-*-16-*-*-*-*-*-*-*";
            };
         };
      };
*/
   };


   APPS.SingleWindowApp IsolabelEg2 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {bounds.out_obj,<-.<-.<-.isoline.out_obj,<-.<-.<-.isolabel.out_obj};
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
      XP_COMM_PROJ.Isolabel.IsolabelMacs.isolabel isolabel {
         in => <-.isoline.out_fld;
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

/*
      MODS.text_glyph text_glyph {
         in_mesh => <-.isolabel.out_mesh;
         in_text => <-.isolabel.out_text;
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
*/

   };


}; // end of flibrary

