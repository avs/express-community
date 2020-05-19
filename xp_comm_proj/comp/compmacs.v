
flibrary CompositeMacs <compile_subs=0> {

   macro composite {

      Image_ARGB &foreground<NEportLevels={2,1}>;
      Image_ARGB &background<NEportLevels={2,1}>;

      XP_COMM_PROJ.Composite.CompositeMods.CompositeCore CompositeCore {
         input_foreground => <-.foreground.node_data[0].values;
         input_background => <-.background.node_data[0].values;
      };

      FLD_MAP.image_field_argb image_field_argb {
         mesh {
            in_dims => <-.<-.foreground.dims;
         };
         data {
            in_data => <-.<-.CompositeCore.output;
         };
      };

      olink out_image => .image_field_argb.out;
      olink out_obj   => .image_field_argb.obj;
   };



   APPS.MultiWindowApp CompositeEg {

      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<1>/xp_comm_proj/comp/test.x";
            };
         };
      };

      MODS.crop crop#1 {
         in_field => <-.Read_Image.field;
         CropParam {
            min = {50,50};
            max = {350,250};
         };
      };

      MODS.crop crop#2 {
         in_field => <-.Read_Image.field;
         CropParam {
            min = {100,200};
            max = {400,400};
         };
      };

      XP_COMM_PROJ.Composite.CompositeMacs.composite composite {
         foreground => <-.crop#1.out_fld;
         background => <-.crop#2.out_fld;
      };

      MODS.extract_scalar extract_scalar {
         in_field => <-.crop#1.out_fld;
      };

      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.composite.out_obj};
            };
         };
         Scene_Selector {
            input_views => {Scene.View.View,
                            <-.Foreground_Scene.View.View,
                            <-.Background_Scene.View.View,
                            <-.Alpha_Scene.View.View};
         };
         GDM.Uscene2D Foreground_Scene {
            Top {
               child_objs<NEportLevels={4,1}> => {<-.<-.<-.crop#1.out_obj};
            };
         };
         GDM.Uscene2D Background_Scene {
            Top {
               child_objs<NEportLevels={4,1}> => {<-.<-.<-.crop#2.out_obj};
            };
         };
         GDM.Uscene2D Alpha_Scene {
            Top {
               child_objs<NEportLevels={4,1}> => {<-.<-.<-.extract_scalar.out_obj};
            };
         };
      };

   };


};

