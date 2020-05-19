
flibrary ReadPapyrusApps<compile_subs=0> {

   APPS.SingleWindowApp TilePapyrusEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.tile_volume.DataObject.obj
               };
            };
         };
      };
   
      XP_COMM_PROJ.ReadPapyrus.ReadPapyrusMacs.read_papyrus read_papyrus {
         filename = "$XP_PATH<1>/xp_comm_proj/rd_papy/mipettra.pap";
      };

      MODS.tile_volume tile_volume {
         FieldIn => <-.read_papyrus.out_fld;
         Tiled_Volume {
            width = 3;
            height = 3;
            border_width = 2;
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



   APPS.SingleWindowApp IsosurfacePapyrusEg {
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

      XP_COMM_PROJ.ReadPapyrus.ReadPapyrusMacs.read_papyrus read_papyrus {
         filename = "$XP_PATH<1>/xp_comm_proj/rd_papy/mipettra.pap";
      };

      MODS.downsize downsize {
         in_field => <-.read_papyrus.out_fld;
         DownsizeParam {
            factor0 = 2.;
            factor1 = 2.;
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

