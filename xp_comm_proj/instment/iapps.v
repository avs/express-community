

flibrary InstrumentationApps<compile_subs=0> {

   APPS.SingleWindowApp InstrumentationEg1 {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.isosurface.out_obj
               };
            };
         };
      };


      MODS.Read_Field Read_Field {
         read_field_ui {
            file_browser {
               dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
            };
            filename = "$XP_PATH<0>/data/field/lobster.fld";
         };
         Read_Field_Param {
            current_step = 1;
            one_time = 0;
         };
      };

      MODS.bounds bounds {
         in_field => <-.Read_Field.field;
      };

      MODS.isosurface isosurface {
         in_field => <-.Read_Field.field;
         IsoParam {
            iso_level<NEportLevels={2,3}>;
         };
      };

      XP_COMM_PROJ.Instrumentation.InstrumentationMacs.gated_verbose gated_verbose {
         Trigger => <-.isosurface.IsoParam.iso_level;
      };
      XP_COMM_PROJ.Instrumentation.InstrumentationMacs.gated_trace gated_trace {
         Trigger => <-.isosurface.IsoParam.iso_level;
      };
      XP_COMM_PROJ.Instrumentation.InstrumentationMacs.object_stats object_stats;
   };




   InstrumentationEg1 InstrumentationEg2 {
      -gated_verbose;
      -gated_trace;
      -object_stats;

      XP_COMM_PROJ.Instrumentation.InstrumentationMacs.time_activity time_activity {
         Trigger => <-.isosurface.IsoParam.iso_level;
      };
      XP_COMM_PROJ.Instrumentation.InstrumentationMacs.time_on_off time_on_off {
         Trigger_On => <-.isosurface.IsoParam.iso_level;
         Trigger_Off => <-.isosurface.out_fld.nnodes;
      };
   };


};

