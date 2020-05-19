
flibrary Isoline2PolylineApps <compile_subs=0> {

   APPS.SingleWindowApp Isoline2PolylineEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  bounds.out_obj,
                  bounds#1.out_obj,
                  isoline2polyline.out_obj
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
            plane => 13;
         };
      };

      MODS.bounds bounds#1 {
         in_field => <-.orthoslice.out_fld;
         BoundsParam {
            data = 1;
         };
      };

      MODS.isoline isoline {
         in_field => <-.orthoslice.out_fld;
         IsoParam {
            level_min => 0.0;
            level_max => 0.5;
         };
      };

      XP_COMM_PROJ.Isoline2Polyline.Isoline2PolylineMacs.isoline2polyline isoline2polyline {
         in_fld => <-.isoline.out_fld;
      };
   };


   Isoline2PolylineEg BlendyIsolineEg {
      Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  bounds.out_obj,
                  bounds#1.out_obj,
                  blendy_tubes.out_obj
               };
            };
         };
      };

      orthoslice {
         OrthoSliceParam {
            axis = 1;
         };
      };

      XP_COMM_PROJ.Isoline2Polyline.Isoline2PolylineMacs.expand_polyline_coords expand_polyline_coords {
         in_fld => <-.isoline2polyline.out_fld;
      };

      XP_COMM_PROJ.BlendyTubes.BlendyTubesMacs.blendy_tubes blendy_tubes {
         lines => <-.expand_polyline_coords.out_fld;
         data => <-.Read_Field.field;
      };

   };


};

