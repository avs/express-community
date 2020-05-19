APPS.SingleWindowApp AnimDisplayApp<NEdisplayMode="maximized",NEhelpContextID=1,NEhelpFile="STM3/AnimDisplayApp.html"> {
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
   XP_COMM_PROJ.STM3.BASE.ReadAnimMolecule Read_Anim_Molecule<NEx=176.,NEy=66.> {
      UI {
         UImod_panel {
            option {
               set = 1;
            };
         };
      };
   };
   XP_COMM_PROJ.STM3.BASE.DisplayMolecule Display_Molecule<NEx=374.,NEy=187.> {
      molecule_in => <-.Read_Anim_Molecule.molecule;
   };
   XP_COMM_PROJ.STM3.BASE.MolViewer MolViewer<NEx=539.,NEy=297.> {
      Scene {
         Top {
            child_objs => {<-.<-.<-.Display_Molecule.obj};
         };
         View {
            View {
               trigger = 1;
            };
         };
         Camera {
            Camera {
               auto_norm = "None";
            };
         };
      };
   };
	XP_COMM_PROJ.STM3.BASE.make_movie make_movie<NEx=693.,NEy=418.> {
      imcapCompute {
         ImageCap {
            LGDView => <-.<-.<-.MolViewer.Scene_Selector.curr_view;
         };
      };
	};
};
