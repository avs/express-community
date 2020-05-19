
flibrary GetFilesApps<compile_subs=0> {

   APPS.SingleWindowApp FiniteWingEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.streamlines.out_obj,
                  <-.<-.<-.advector.out_obj,
                  <-.<-.<-.finite_wing.out_probe_obj,
                  <-.<-.<-.finite_wing.out_wing_obj
               };
               Props {
                  line_width = 2;
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,
               };
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };



      XP_COMM_PROJ.FiniteWing.FiniteWingMacs.finite_wing finite_wing;

      MODS.streamlines streamlines {
         in_field => <-.finite_wing.out_fld;
         in_probe => <-.finite_wing.out_probe;
         StreamParam {
            order = 3;
         };
         obj {
            Datamap {
               dataMin => <-.in.node_data[0].min;
               dataMax => <-.in.node_data[0].max;
            };
         };
      };


      MODS.magnitude magnitude {
         in_field => <-.finite_wing.out_fld;
      };

      MODS.bounds bounds {
         in_field => <-.magnitude.out_fld;
      };

      GEOMS.Arrow1 Arrow1;

      MODS.advector advector {
         in_field => <-.finite_wing.out_fld;
         in_probe => <-.finite_wing.out_probe;
         in_glyph => <-.Arrow1.out_fld;
         AdvectParam {
            min_vel = 0.;
            end = 23.;
            incr = 0.2;
            release_interval = 5.;
         };
         obj {
            Datamap {
               dataMin => <-.in.node_data[0].min;
               dataMax => <-.in.node_data[0].max;
            };
         };
      };

   };


};

