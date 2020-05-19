
// Defines library used to hold application objects
// Also specifies that this library should not be considered during compilation
flibrary ZipIOApps <compile_subs=0> {

   APPS.SingleWindowApp WriteCompressedImageEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Geom.geom
               };
            };
         };
      };


      MODS.Read_Geom Read_Geom {
         read_geom_ui {
            filename = "$XP_PATH<0>/data/geom/teapot.geo";
         };
      };

      HLM.OutputImage OutputImage {
         view_in => <-.Uviewer3D.Scene_Selector.curr_view;
         output_field {
            write_image_ui {
               flip_toggle {
                  set = 0;
               };
               file_browser {
                  filename = "$XP_PATH<1>/teapot.x";
               };
               filename<NEportLevels={1,4}>;
               output<NEportLevels={1,4}>;
            };
         };
      };

      GMOD.copy_on_change copy_on_change {
         trigger => <-.OutputImage.output_field.write_image_ui.output;
         input => <-.OutputImage.output_field.write_image_ui.filename;
      };

      XP_COMM_PROJ.ZipIO.ZipIOMacs.compress_file compress_file {
         in_filename => <-.copy_on_change.output;
         
         params {
            output_dir = "$XP_PATH<1>";
            remove_orig = 1;
         };
      };
   };



   APPS.SingleWindowApp ReadCompressedImageEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Image.image
               };
            };
         };
      };


      XP_COMM_PROJ.ZipIO.ZipIOMacs.decompress_file decompress_file {
         params {
            filename = "$XP_PATH<1>/teapot.x.gz";
            output_dir = "$XP_PATH<1>";
         };
      };

      MODS.Read_Image Read_Image {
         read_image_ui {
            file_browser {
               filename<NEportLevels={4,2}> => <-.<-.<-.decompress_file.out_filename;
            };
         };
      };
   };


};

