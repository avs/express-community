
flibrary Build3DApps {

   // Build3dEg
   //    Example application. Uses isosurface and bounds modules to visualise the supplied
   //    BMP slices.

   APPS.SingleWindowApp Build3DEg {

      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.bounds.out_obj, <-.<-.<-.isosurface.out_obj};
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };

      XP_COMM_PROJ.Build3D.Build3DMacs.build3D build3D {
         FileParams {
            dir = "$XP_PATH<1>/xp_comm_proj/build3d/cube/";
            filename_stub = "cube";
            extension = "bmp";
            index_width = 0;
         };
         UIImageParams {
            start_layer = 0;
            end_layer = 3;
         };
      };

      MODS.downsize downsize {
         in_field => <-.build3D.out;
         DownsizeParam {
            factor0 = 1.;
            factor1 = 1.;
            factor2 = 1.;
         };
      };

      MODS.bounds bounds {
         in_field => <-.downsize.out_fld;
      };

      MODS.isosurface isosurface {
         in_field => <-.downsize.out_fld;
         IsoParam {
            iso_level = 250.;
         };
      };
   };



   APPS.SingleWindowApp CreateImageSlicesEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.orthoslice.out_obj
               };
               Xform {
                  mat = {
                     0.15873,0.,0.,0.,
                     0.,0.15873,0.,0.,
                     0.,0.,0.15873,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-5.,-5.,-5.07937};
                  center = {31.5,31.5,32.};
               };
            };
            Camera {
               Camera {
                  norm_scale = 1.;
               };
            };
         };
      };


      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/water.fld";
         };
      };

      MODS.orthoslice orthoslice {
         in_field => <-.Read_Field.field;
         OrthoSliceParam {
            axis = 2;
            plane<NEportLevels={3,2}> => <-.<-.animFilename.VideoLoopParams.count;
         };
         obj {
            Obj {
               interp_type = "Bilinear";
            };
            Datamap {
               DatamapValue = {
                  {
                     v4=0.,v3=0.,v2=0.,,
                  },
                  {
                     v3=0.,,
                  }
               };
            };
         };
      };


      GDM.OutputField OutputField {
         view_in => <-.Uviewer2D.Scene_Selector.curr_view;
         output_field {
            View {
               mode = "Automatic";
               update<NEportLevels={4,0}>;
            };
         };
      };


      XP_COMM_PROJ.Common.IOWidgets.animFilename animFilename {
         VideoLoopParams {
            start_val = 10;
            end_val = 20;
            incr = 1;
            count = 10;
         };

         IndexedFileParams {
            dir = "$XP_PATH<1>/images/";
            filename_stub = "image";
            extension = "tiff";
            index_width = 0;
         };

         AnimFilename {
            CreateIndexedFilename {
               index => <-.VideoLoop.count - <-.VideoLoop.start_val;
            };
         };
      };


      GMOD.copy_on_change copy_on_change {
         trigger => <-.OutputField.output_field.View.update;
         input => <-.animFilename.out;
         on_inst = 0;
      };


      MODS.Write_Image Write_Image {
         in => <-.OutputField.output_field.Output.output;
         DVwrite_image {
            filename<NEportLevels={3,1}>  => <-.<-.copy_on_change.output;
         };
         write_image_ui {
            overwrite_toggle{
               set = 1;
            };
            format_tiff {
               set = 1;
            };
            format_rb {
               selectedItem = 7;
            };
            bpp_0 {
               set = 1;
            };
            color_rgb {
               set = 1;
            };
            compression_rle {
               set = 1;
            };
            reduction_na {
               set = 1;
            };
            filetype_binary {
               set = 1;
            };
         };
      };

   };


};

