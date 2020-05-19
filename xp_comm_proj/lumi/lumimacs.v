
flibrary LuminanceMacs <compile_subs=0> {

   macro luminance {
      Image_ARGB &in<NEportLevels={2,1}>;

      XP_COMM_PROJ.Luminance.LuminanceMods.LuminanceCore LuminanceCore {
         input => <-.in.node_data[0].values;
      };

      FLD_MAP.uniform_scalar_field uniform_scalar_field {
         mesh {
            in_dims => <-.<-.in.dims;
         };
         data {
            in_data => <-.<-.LuminanceCore.output;
         };
      };

      olink out_fld => .uniform_scalar_field.out;
      olink out_obj => .uniform_scalar_field.obj;
   };



   APPS.SingleWindowApp LuminanceEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.Read_Image.image, <-.<-.<-.luminance.out_obj};
            };
         };
      };


      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename = "$XP_PATH<0>/data/image/mandrill.x";
            };
         };
         DVread_image {
            out {
               xform {
                  xlate = {0.,300.,0.};
               };
            };
         };
      };


      XP_COMM_PROJ.Luminance.LuminanceMacs.luminance luminance {
         &in => <-.Read_Image.field;

         uniform_scalar_field {
            DataObject {
               Datamap {
                  DatamapValue = {
                     {
                        v4=0.,v3=0.,v2=0.,
                     },
                     {
                        v3=0.,,
                     }
                  };
               };
            };
            mesh {
               out {
                  xform {
                     xlate = {0.,-300.,0.};
                  };
               };
            };
         };
      };
   };

};

