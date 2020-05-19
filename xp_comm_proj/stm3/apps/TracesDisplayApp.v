APPS.SingleWindowApp TracesDisplayApp<NEdisplayMode="maximized",NEhelpContextID=1,NEhelpFile="STM3/TracesDisplayApp.html"> {
   UI {
      toolbar {
         width = 507;
      };
      Modules {
         IUI {
            optionList {
               selectedItem = 0;
            };
         };
      };
      shell {
         x = 20;
         y = 20;
      };
      Windows {
         IUI {
            panel {
               width = 518;
               height = 518;
            };
         };
      };
   };
   XP_COMM_PROJ.STM3.BASE.ReadAnimMolecule Read_Anim_Molecule<NEx=330.,NEy=88.> {
      UI {
         UImod_panel {
            option {
               set = 1;
            };
         };
      };
      Loop {
         end<NEportLevels={2,3}>;
         start<NEportLevels={2,3}>;
         incr<NEportLevels={2,3}>;
      };
   };
   XP_COMM_PROJ.STM3.BASE.DisplayMolecule Display_Molecule<NEx=495.,NEy=231.> {
      molecule_in => <-.Read_Anim_Molecule.molecule;
   };
   XP_COMM_PROJ.STM3.BASE.MolViewer MolViewer<NEx=693.,NEy=473.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Display_Molecule.obj,<-.<-.<-.bounds.out_obj,
               <-.<-.<-.volume_render.out_obj};
            Xform {
               ocenter = {31.5,31.5,31.5};
               dcenter = {4.,4.,4.};
               mat = {
                  0.0991135,-0.00413906,0.023798,0.,-0.00162614,0.0991345,0.0240142,
0.,-0.0241005,-0.0237106,0.0962492,0.,0.,0.,0.,1.
               };
               xlate = {-4.,-4.,-4.};
            };
         };
         Camera {
            Camera {
               auto_norm = "None";
            };
         };
         View {
            View {
               trigger = 2;
            };
         };
      };
   };
   XP_COMM_PROJ.STM3.TRACES.Accumulate_Traces Accumulate_Traces<NEx=231.,NEy=308.> {
      molecule => <-.Read_Anim_Molecule.molecule;
      AccumulateTraces {
         start => <-.<-.Read_Anim_Molecule.Loop.start;
         end => <-.<-.Read_Anim_Molecule.Loop.end;
         increment => <-.<-.Read_Anim_Molecule.Loop.incr;
         step => <-.<-.Read_Anim_Molecule.step;
      };
   };
   MODS.bounds bounds<NEx=407.,NEy=407.> {
      in_field => <-.Accumulate_Traces.out_occupancy;
   };
   MODS.volume_render volume_render<NEx=165.,NEy=418.> {
      in_mesh => <-.Accumulate_Traces.out_occupancy;
      DataObject {
         Obj {
            use_altobj = 0;
         };
         Props {
            fat_ray = 0;
            inherit = 0;
         };
         Modes {
            mode = {0,0,0,2,0};
         };
      };
      GDmodes_edit {
         volume = "BTF Texture";
      };
      GDprops_edit {
         fat_ray = 0;
         inherit = 0;
      };
      UImod_panel {
         title => name_of(<-.<-,1);
      };
      modes_ui {
         inherit {
            set = 1;
         };
         surf_rb {
            x = 0;
         };
         btf {
            set = 1;
         };
         vol_rb {
            x = 0;
         };
      };
      props_ui {
         interp_tri {
            set = 1;
         };
         interp_rb {
            x = 0;
         };
         ray_direct {
            set = 1;
         };
         ray_rb {
            x = 0;
         };
         dist_glob {
            set = 1;
         };
         dist_rb {
            x = 0;
         };
      };
      datamap_ui {
         Dmap2ImageLegend {
            Scene {
               Top {
                  Xform {
                     ocenter = {117.5,14.5,0.};
                     dcenter = {5.,0.617021,0.};
                  };
               };
               View {
                  View {
                     trigger = 2;
                  };
               };
            };
         };
         alpha_step {
            set = 1;
         };
         alpha_rb {
            x = 0;
         };
      };
   };
	XP_COMM_PROJ.STM3.BASE.make_movie make_movie<NEx=693.,NEy=583.> {
      imcapCompute {
         ImageCap {
            LGDView => <-.<-.<-.MolViewer.Scene_Selector.curr_view;
         };
      };
	};
};
