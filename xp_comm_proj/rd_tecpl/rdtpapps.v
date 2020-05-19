// 
// file: rdtpmods.v
// author: I.Hallstein, DLR, WT-DV
//
// modifications: Tobias Schiebeck, IAC
//
// purpose: Example Application read Tecplot-Data-File module
//

flibrary ReadTecplotApps<compile_subs=0>
{
APPS.SingleWindowApp ReadTecplotEg {
   GDM.Uviewer3D Uviewer3D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.bounds_ARR.out_obj,<-.<-.<-.isosurface_ARR.out_obj,
               <-.<-.<-.isoline_ARR.out_obj};
         };
      };
      Scene_Editor {
         View_Editor {
            InfoPanel {
               curobj_sel {
                  width = 235;
                  height = 254;
                  x = 0;
                  y = 0;
               };
            };
         };
      };
   };
   XP_COMM_PROJ.ReadTecplot.ReadTecplotMacs.ReadTecplot ReadTecplot{
      params {
	 filename = "./xp_comm_proj/rd_tecpl/data/vel.020.plt";
      };
	ReadTecplotUI{
		Panel {
			parent => <-.<-.<-.UI.Modules.IUI.mod_panel;
		};
	};
   };
   MODS_ARR.bounds_ARR bounds_ARR {
      in_field => <-.ReadTecplot.out_fields;
   };
   MODS_ARR.isoline_ARR isoline_ARR {
      in_field => <-.ReadTecplot.out_fields;
   };
   MODS_ARR.isosurface_ARR isosurface_ARR {
      in_field => <-.ReadTecplot.out_fields;
   };
};

};
