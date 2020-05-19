
flibrary ReadDXFApps<compile_subs=0> {

   APPS.SingleWindowApp ReadDXFEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.readDXF.out_geom
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

      XP_COMM_PROJ.ReadDXF.ReadDXFMacs.readDXF readDXF {
         params {
            file_name = "$XP_PATH<1>/xp_comm_proj/rd_dxf/data/passjet.dxf";
         };
      };
   };


   ReadDXFEg ReadDXFExtractEg {
      Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.external_edges_ARR.out_obj,
                  <-.<-.<-.external_faces.out_obj
               };
            };
         };
      };

      XP_COMM_PROJ.ExtractObjects.ExtractObjectsMacs.extract_single_object extract_single_object {
         in_objs => <-.readDXF.out_flds;
      };
      MODS.external_faces external_faces {
         in_field => <-.extract_single_object.out_obj;
      };
      MODS_ARR.external_edges_ARR external_edges_ARR {
         in_field => <-.readDXF.out_flds;
      };
   };

};

