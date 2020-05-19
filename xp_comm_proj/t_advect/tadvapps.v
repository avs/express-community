
flibrary TailedAdvectorApps <compile_subs=0> {

   APPS.SingleWindowApp SingleTailedAdvectorEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.external_edges.out_obj,
                  <-.<-.<-.tailed_advector.out_obj,
                  <-.<-.<-.magnitude.out_obj
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,
               };
            };
         };
      };


      MODS.Read_UCD Read_UCD {
         read_ucd_ui {
            filename = "$XP_PATH<0>/data/ucd/bluntfin.inp";
         };
      };
      MODS.combine_vect combine_vect {
         in_field => Read_UCD.field;
         CombineVectParam {
            components = {1,2,3};
         };
      };
      MODS.external_edges external_edges {
         in_field => Read_UCD.field;
      };

      GEOMS.FPlane FPlane {
         in => <-.Read_UCD.field;
         dim1 = 18;
         dim2 = 8;
         plane {
            xform {
               xlate = {-1.53021,0.387175,1.61171};
               ocenter = {1.23047,2.30992,0.};
               dcenter = {1.13833,2.13694,0.};
               mat = {
                  4.65e-010,0.,-0.93,0.,
                  0.,0.93,0.,0.,
                  0.93,0.,4.65e-010,0.,
                  0.,0.,0.,1.
               };
               center = {1.23047,2.30992,0.};
            };
         };
      };


      GEOMS.Arrow1 Arrow1;

      XP_COMM_PROJ.TailedAdvector.TailedAdvectorMacs.tailed_advector tailed_advector {
         in_field => <-.combine_vect.out_fld;
         in_probe => <-.FPlane.out_fld;
         in_glyph => <-.Arrow1.out_fld;
         params {
            min_vel = 0.5;
            reset = 0;
            count = 0.;
            end = 2.;
            scale = 0.2;
            tracer_len = 4;
            tracer_style = 1;
         };
      };

      MODS.magnitude magnitude {
         in_field => <-.tailed_advector.out_tracer;
      };
   };



   APPS.SingleWindowApp DualTailedAdvectorEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.external_edges.out_obj,
                  <-.<-.<-.tailed_advector#1.out_obj,
                  <-.<-.<-.magnitude#1.out_obj,
                  <-.<-.<-.tailed_advector#2.out_obj,
                  <-.<-.<-.magnitude#2.out_obj
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,
               };
            };
         };
      };


      MODS.Read_UCD Read_UCD {
         read_ucd_ui {
            filename = "$XP_PATH<0>/data/ucd/bluntfin.inp";
         };
      };
      MODS.combine_vect combine_vect {
         in_field => Read_UCD.field;
         CombineVectParam {
            components = {1,2,3};
         };
      };
      MODS.external_edges external_edges {
         in_field => Read_UCD.field;
      };

      GEOMS.FPlane FPlane#1 {
         in => Read_UCD.field;
         dim1 = 8;
         dim2 = 8;
      };
      GEOMS.FPlane FPlane#2 {
         in => <-.Read_UCD.field;
         dim1 = 18;
         dim2 = 8;
         plane {
            xform {
               xlate = {-1.53021,0.387175,1.61171};
               ocenter = {1.23047,2.30992,0.};
               dcenter = {1.13833,2.13694,0.};
               mat = {
                  4.65e-010,0.,-0.93,0.,
                  0.,0.93,0.,0.,
                  0.93,0.,4.65e-010,0.,
                  0.,0.,0.,1.
               };
               center = {1.23047,2.30992,0.};
            };
         };
      };


      XP_COMM_PROJ.TailedAdvector.TailedAdvectorMods.Synchronize Synchronize;
      GEOMS.Arrow1 Arrow1;

      XP_COMM_PROJ.TailedAdvector.TailedAdvectorMacs.tailed_advector tailed_advector#1 {
         in_field => <-.combine_vect.out_fld;
         in_probe => <-.FPlane#1.out_fld;
         in_glyph => <-.Arrow1.out_fld;
         params {
            run<NEportLevels={3,0}> => <-.<-.Synchronize.run1;
            reset<NEportLevels={3,2}> => <-.<-.Synchronize.reset1;
            min_vel = 0.5;
            count = 0.;
            end = 2.;
            scale = 0.2;
            tracer_len = 4;
            tracer_style = 1;
         };
      };
      MODS.magnitude magnitude#1 {
         in_field => <-.tailed_advector#1.out_tracer;
      };

      XP_COMM_PROJ.TailedAdvector.TailedAdvectorMacs.tailed_advector tailed_advector#2 {
         in_field => <-.combine_vect.out_fld;
         in_probe => <-.FPlane#2.out_fld;
         in_glyph => <-.Arrow1.out_fld;
         params {
            run<NEportLevels={3,0}> => <-.<-.Synchronize.run2;
            reset<NEportLevels={3,2}> => <-.<-.Synchronize.reset2;
            min_vel = 0.5;
            count = 0.;
            end = 2.;
            scale = 0.2;
            tracer_len = 4;
            tracer_style = 1;
         };
      };
      MODS.magnitude magnitude#2 {
         in_field => <-.tailed_advector#2.out_tracer;
      };

   };



   APPS.SingleWindowApp TimeDependentAdvectorEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.TimeDependentAdvector.out_obj,
                  <-.<-.<-.TimeDependentAdvector.tracer_obj,
                  <-.<-.<-.external_edges.out_obj
               };
            };
         };
      };

      GEOMS.Sphere Sphere;

      XP_COMM_PROJ.TailedAdvector.TailedAdvectorMacs.TimeDependentAdvector TimeDependentAdvector {
         in_glyph => <-.Sphere.bounds.out;
         filename = "$XP_PATH<0>/data/ucd/yoke.inp";

         extract_component {
            ExtrCompParam {
               component = 1;
            };
            out_fld+OPort3;
         };

         tailed_advector {
            in_field => <-.extract_component.out_fld;
            params {
               min_vel = 0.5;
               reset = 0;
               end = 1.48;
               incr = 0.05;
               time_step_interval = 0.3;
               scale = 2.;
               normalize = 1.;
               tracer_len = 4;
               tracer_style = 1;
            };
            obj {
               Datamap {
                  DatamapValue = {
                     {
                        v2=0.,
                     },
                  };
               };
            };
         };

         FPlane {
            plane {
               xform {
                  xlate = {-0.256889,0.668691,2.9983};
               };
            };
            dim1 = 16;
         };
      };


      MODS.magnitude magnitude {
         in_field => <-.TimeDependentAdvector.extract_component.out_fld;
      };
      MODS.external_edges external_edges {
         in_field => <-.magnitude.out_fld;
      };
   };


};

