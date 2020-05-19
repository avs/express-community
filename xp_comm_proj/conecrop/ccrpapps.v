flibrary ConeCropApps <compile_subs=0> {
  APPS.MultiWindowApp ConeCropEg {
     GDM.Uviewer3D Uviewer3D {
        Scene {
           Top {
              child_objs => {
                 <-.<-.<-.bounds.out_obj,<-.<-.<-.ConeCrop.out_obj,
                 <-.<-.<-.Axis_Glyph3D.out_obj};
           };
        };
     };
     MODS.Read_Field Read_Field {
        DVread_field {
           Mesh_Unif+Node_Data Output_Field;
        };
     };
     MODS.bounds bounds {
        in_field => <-.Read_Field.field;
     };
     XP_COMM_PROJ.ConeCrop.ConeCropMacs.ConeCrop ConeCrop {
        in_fld => <-.Read_Field.field;
        ConeCropUI {
           panel {
              option {
                 set = 1;
              };
           };
        };
     };
     GEOMS.Axis_Glyph3D Axis_Glyph3D {
        axis {
           xform {
              mat = {
                 24.,0.,0.,0.,0.,24.,0.,0.,0.,0.,24.,0.,0.,0.,0.,1.
              };
           };
        };
        probe_edit {
           GDxform_editor {
              scale = 24.;
              abs_scale = 24.;
           };
        };
     };
  };

};
