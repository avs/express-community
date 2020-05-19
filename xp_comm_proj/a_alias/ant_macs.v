
flibrary AntiAliasMacs <compile_subs=0> {

   macro anti_alias {

      Image_ARGB &in<NEportLevels={2,1}>;

      XP_COMM_PROJ.AntiAlias.AntiAliasMods.AntiAliasCore AntiAliasCore {
         input => <-.in.node_data[0].values;
         input_dims => <-.in.dims;
      };

      FLD_MAP.image_field_argb image_field_argb {
         mesh {
            in_dims => <-.<-.AntiAliasCore.output_dims;
         };
         data {
            in_data => <-.<-.AntiAliasCore.output;
         };
      };

      olink out_image => image_field_argb.out;
      olink out_obj   => image_field_argb.obj;
   };


   APPS.SingleWindowApp AntiAliasEg1 {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Image.image,
                  <-.<-.<-.anti_alias.out_obj
               };
            };
         };
      };

      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<0>/data/image/mandrill.x";
            };
         };
      };

      XP_COMM_PROJ.AntiAlias.AntiAliasMacs.anti_alias anti_alias {
         in => <-.Read_Image.field;
      };
   };


   APPS.MultiWindowApp AntiAliasEg2 {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.Read_Image.image};
            };
         };
         Scene_Selector {
            input_views => {Scene.View.View, <-.AntiAlias_Scene.View.View};
         };
         GDM.Uscene3D AntiAlias_Scene {
            Top {
               child_objs<NEportLevels={4,1}> => {<-.<-.<-.anti_alias.out_obj};
            };
         };
      };


      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<0>/data/image/mandrill.x";
            };
         };
      };


      XP_COMM_PROJ.AntiAlias.AntiAliasMacs.anti_alias anti_alias {
         in => <-.Read_Image.field;
      };

   };


};

