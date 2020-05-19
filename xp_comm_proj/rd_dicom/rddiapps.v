
flibrary ReadDICOMApps<compile_subs=0> {

   APPS.SingleWindowApp ImageDicomEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.ReadDICOM.obj
               };
            };
         };
      };
   
      XP_COMM_PROJ.ReadDICOM.ReadDICOMMacs.ReadDICOM_Series ReadDICOM;
   };


   APPS.SingleWindowApp TileDicomEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.tile_volume.DataObject.obj
               };
            };
         };
      };
   
      XP_COMM_PROJ.ReadDICOM.ReadDICOMMacs.ReadDICOM_Series ReadDICOM;

      MODS.tile_volume tile_volume {
         FieldIn => <-.ReadDICOM.out;
         Tiled_Volume {
            width = 3;
            height = 2;
            border_width = 10;
            mode = 0;
            orientation = 1;
         };
         DataObject {
            Datamap {
               DatamapValue = {
                  {
                     v2=0.,v3=0.,v4=0.,,
                  },
                  {
                     v3=0.,,
                  }
               };
            };
         };
      };

   };



   APPS.SingleWindowApp IsosurfaceDicomEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.isosurface.out_obj
               };
            };
            Lights {
               Lights = { { type="BiDirectional" },,,};
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };

      XP_COMM_PROJ.ReadDICOM.ReadDICOMMacs.ReadDICOM_Series ReadDICOM;

      MODS.downsize downsize {
         in_field => <-.ReadDICOM.out;
         DownsizeParam {
            factor0 = 8.;
            factor1 = 8.;
            factor2 = 1.;
         };
      };

      MODS.bounds bounds {
         in_field => <-.downsize.out_fld;
         BoundsParam {
            data = 1;
         };
      };

      MODS.isosurface isosurface {
         in_field => <-.downsize.out_fld;
      };
   };

};

