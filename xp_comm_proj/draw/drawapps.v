
flibrary DrawApps<compile_subs=0> {

   APPS.SingleWindowApp SuperDrawEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Image.image,
                  <-.<-.<-.SuperDraw.out_obj
               };
            };
         };
      };

      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<0>/data/image/marble.x";
               dirMaskCache = "$XP_PATH<0>/data/image/*";
            };
         };
      };

      XP_COMM_PROJ.Draw.DrawMacs.SuperDraw SuperDraw {
         obj_in => <-.Read_Image.image;
         view_in => <-.Uviewer2D.Scene_Selector.curr_view;
      };
   };



   APPS.SingleWindowApp LineResamplerEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Image.image,
                  <-.<-.<-.SuperDraw.out_obj,
                  <-.<-.<-.geo_glyph.out_obj
               };
            };
         };
      };

      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<0>/data/image/marble.x";
               dirMaskCache = "$XP_PATH<0>/data/image/*";
            };
         };
      };

      XP_COMM_PROJ.Draw.DrawMacs.SuperDraw SuperDraw {
         obj_in => <-.Read_Image.image;
         view_in => <-.Uviewer2D.Scene_Selector.curr_view;
      };
      XP_COMM_PROJ.Draw.DrawMacs.LineResampler LineResampler {
         in_points => <-.SuperDraw.out_points;
         in_dims => <-.Read_Image.field.dims;
      };

      GEOMS.Cross2D Cross2D;
      MODS.geo_glyph geo_glyph {
         in_field => <-.LineResampler.out_mesh;
         in_glyph => <-.Cross2D.out_fld;
         GlyphParam {
            scale = 10.;
            color_r = 1.;
            color_b = 0.;
         };
      };
   };



   APPS.MultiWindowApp RegionResamplerEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Image.image,
                  <-.<-.<-.SuperDraw.out_obj
               };
            };
         };
         Scene_Selector {
            input_views => {<-.Scene.View.View, <-.ResampledScene.View.View};
         };
         GDM.Uscene2D ResampledScene {
            Top {
               child_objs<NEportLevels={4,1}> => {
                  <-.<-.<-.RegionResampler.out_obj
               };
            };
         };
      };


      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<0>/data/image/marble.x";
               dirMaskCache = "$XP_PATH<0>/data/image/*";
            };
         };
      };

      XP_COMM_PROJ.Draw.DrawMacs.SuperDraw SuperDraw {
         obj_in => <-.Read_Image.image;
         view_in => <-.Uviewer2D.Scene_Selector.curr_view;
      };

      XP_COMM_PROJ.Draw.DrawMacs.RegionResampler RegionResampler {
         in_points => <-.SuperDraw.out_points;
         in_dims   => <-.Read_Image.field.dims;
      };
   };

};

