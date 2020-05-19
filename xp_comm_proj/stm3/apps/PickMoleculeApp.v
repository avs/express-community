APPS.SingleWindowApp PickMoleculeApp<NEdisplayMode="maximized",NEhelpContextID=1,NEhelpFile="STM3/PickMoleculeApp.html"> {
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
   XP_COMM_PROJ.STM3.BASE.ReadMolecule Read_Molecule<NEx=231.,NEy=66.> {
      UI {
         UImod_panel {
            option {
               set = 1;
            };
         };
      };
   };
   XP_COMM_PROJ.STM3.BASE.DisplayMolecule Display_Molecule<NEx=440.,NEy=242.> {
      molecule_in => <-.Read_Molecule.molecule;
   };
   XP_COMM_PROJ.STM3.BASE.MolViewer MolViewer<NEx=605.,NEy=396.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Display_Molecule.obj,<-.<-.<-.Pick_Atom.label_obj};
         };
      };
   };
   XP_COMM_PROJ.STM3.UTILS.PickAtoms Pick_Atom<NEx=319.,NEy=484.> {
      molecule_in => <-.Read_Molecule.molecule;
      picked_obj => <-.MolViewer.Scene.View.View.picked_obj;
   };
};
