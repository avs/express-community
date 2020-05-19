
flibrary FastAdvectorApps <compile_subs=0> {

   APPS.SingleWindowApp FastAdvectorEg1 {

      // turn module flashing off for faster update:
      //NetworkEditor.optionsMenu.flashingItem.option.set = 0;

      // turn off status-checking overhead
      Scheduler.status_check = 0;


      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.external_edges.out_obj,
                  <-.<-.<-.fast_advector.out_obj
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


      MODS.Read_UCD Read_UCD {
         read_ucd_ui {
            filename = "$XP_PATH<0>/data/ucd/bluntfin.inp";
         };
      };

      MODS.external_edges external_edges {
         in_field => <-.Read_UCD.field;
      };

      MODS.combine_vect combine_vect {
         in_field => <-.Read_UCD.field;
         CombineVectParam {
            components = {1,2,3};
         };
      };
      GEOMS.FPlane FPlane {
         in => <-.Read_UCD.field;
         plane {
            xform {
               mat = {
                  0.75,0.,0.,0.,
                  0.,0.75,0.,0.,
                  0.,0.,0.75,0.,
                  0.,0.,0.,1.
               };
            };
         };
         dim1 = 30;
         dim2 = 40;
      };

      XP_COMM_PROJ.FastAdvector.FastAdvectorMacs.fast_advector fast_advector {
         in_field => <-.combine_vect.out_fld;
         in_probe => <-.FPlane.out_fld;
         AdvectParam {
            cycle = 1;
            incr = 0.01;
            count = 0.;
         };
      };

   };



   APPS.SingleWindowApp FastAdvectorEg2 {

      // turn module flashing off for faster update:
      //NetworkEditor.optionsMenu.flashingItem.option.set = 0;

      // turn off status-checking overhead
      Scheduler.status_check = 0;

      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.fast_advector.out_obj,<-.<-.<-.Read_Geom.geom
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


      MODS.Read_Geom Read_Geom {
         read_geom_ui {
            filename = "$XP_PATH<0>/MicroAVS/demo_data/room/room.geo";
         };
         DataObject {
            Props {
               col = {0.82,0.6396,0.6396};
               inherit = 0;
            };
         };
      };


      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/MicroAVS/demo_data/room/room.fld";
         };
      };
      MODS.combine_vect combine_vect {
         in_field => <-.Read_Field.field;
      };
      GEOMS.FPlane FPlane {
         in => <-.Read_Field.field;
         plane {
            xform {
               mat = {
                  0.3,0.,0.,0.,
                  0.,0.3,0.,0.,
                  0.,0.,0.3,0.,
                  0.,0.,0.,1.
               };
               xlate = {2.18096,1.96814, 0.648724};
            };
         };
         dim1 = 15;
         dim2 = 20;
      };

      XP_COMM_PROJ.FastAdvector.FastAdvectorMacs.fast_advector fast_advector {
         in_field => <-.combine_vect.out_fld;
         in_probe => <-.FPlane.out_fld;
         AdvectParam {
            cycle = 1;
            incr = 0.07;
            count = 0.;
            nseg = 1;
            max_seg = 100;
            end = 20.;
            release_interval = 1.;
         };
         DataObjectLite {
            Props {
               col = {0.98,1.,0.};
               inherit = 0;
            };
         };
      };

   };


};

