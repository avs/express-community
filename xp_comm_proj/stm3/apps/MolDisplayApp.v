APPS.SingleWindowApp MolDisplayApp<NEdisplayMode="maximized",NEhelpContextID=1,NEhelpFile="STM3/MolDisplayApp.html"> {
   UI {
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
   XP_COMM_PROJ.STM3.BASE.ReadMolecule Read_Molecule<NEx=253.,NEy=77.> {
      UI {
         UImod_panel {
            option {
               set = 1;
            };
         };
      };
   };
   XP_COMM_PROJ.STM3.BASE.DisplayMolecule Display_Molecule<NEx=341.,NEy=253.> {
      molecule_in => <-.Read_Molecule.molecule;
   };
   XP_COMM_PROJ.STM3.BASE.MolViewer MolViewer<NEx=528.,NEy=418.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Display_Molecule.obj};
         };
         View {
            View {
               back_col = {0.55,0.6,0.6};
            };
         };
      };
      Scene_Editor {
         View_Editor {
            IUI {
               General {
                  IUI {
                     Color {
                        rgb_or_hsv {
                           h = 0.49;
                           s = 0.08;
                        };
                     };
                  };
               };
            };
         };
      };
   };
};
