XP_COMM_PROJ.STM3.APPS.MolDisplayApp ParticleDensityApp<NEdisplayMode="maximized",NEhelpContextID=1,NEhelpFile="STM3/ParticleDensityApp.html"> {
   UI {
      Modules {
         IUI {
            optionList {
               selectedItem = 0;
               cmdList => {
                  <-.<-.<-.<-.Read_Molecule.UI.UImod_panel.option,
                  <-.<-.<-.<-.Display_Molecule.UImod_panel.option,
                  <-.<-.<-.<-.cell_to_node.CellToNodeUI.UIpanel.option,
                  <-.<-.<-.<-.isovolume.UIpanel.option,
                  <-.<-.<-.<-.Particle_Density.UImod_panel.option
               };
            };
         };
      };
      shell {
         x = 0;
         y = 0;
      };
      Windows {
         IUI {
            panel {
               width = 544;
               height = 740;
            };
         };
      };
      Editors {
         IUI {
            optionList {
               selectedItem = 0;
            };
         };
      };
   };
   Read_Molecule<NEx=154.,NEy=22.> {
      UI {
         UImod_panel {
            option {
               set = 1;
            };
         };
         UIfileDialog {
            x = 654;
            y = 377;
            width = 300;
            height = 386;
         };
         Hydrogens {
            set = 0;
         };
         H_bonds {
            set = 0;
         };
      };
   };
   Display_Molecule<NEx=11.,NEy=297.> {
      molecule_in => <-.Particle_Density.selected_atoms;
      resolution_rb {
         selectedItem = 2;
      };
      Low_Res {
         set = 1;
      };
      option_hb {
         set = 1;
      };
   };
   MolViewer<NEx=319.,NEy=473.> {
      Scene {
         Top {
            Xform {
               dcenter = {392.119,373.543,22.471};
               mat = {
                  0.0243512,0.259982,0.0326253,0.,0.0200202,0.0308242,-0.260571,
0.,-0.261255,0.0265949,-0.0169267,0.,0.,0.,0.,1.
               };
               xlate = {-390.898,-377.114,-20.0258};
            };
            child_objs => {
               <-.<-.<-.Display_Molecule.obj,<-.<-.<-.isovolume.out_obj,
               <-.<-.<-.edges.out_obj};
            Props {
               material = {0.3,0.7,0.4,12.};
            };
         };
      };
      Scene_Editor {
         View_Editor {
            InfoPanel {
               curobj_sel {
                  width = 235;
                  height = 249;
                  x = 0;
                  y = 0;
               };
            };
         };
         Object_Editor {
            GDprops_edit {
               inherit = 0;
               ambient = 0.3000000119;
               diffuse = 0.6999999881;
               trans = 1.;
            };
            IUI {
               optionList {
                  selectedItem = 3;
               };
               Properties {
                  IUI {
                     panel {
                        visible = 1;
                     };
                     EditAltProps {
                        y = 0;
                     };
                     Type {
                        IUI {
                           optionList {
                              selectedItem = 2;
                           };
                           Surface {
                              IUI {
                                 panel {
                                    visible = 1;
                                 };
                              };
                           };
                        };
                     };
                  };
               };
            };
         };
      };
   };
   MODS.edges edges<NEx=517.,NEy=308.> {
      in_field => <-.cell_to_node.out_fld;
   };
   MODS.cell_to_node cell_to_node<NEx=363.,NEy=231.> {
      CellToNodeParam {
         order = 1;
      };
      in_field => <-.Particle_Density.fld_density;
   };
   MODS.isovolume isovolume<NEx=374.,NEy=308.> {
      in_field => <-.cell_to_node.out_fld;
      IsoVolParam {
         iso_level => 0.024;
      };
      IsoVolUI {
         cut_level {
            decimalPoints = 4;
         };
      };
      obj {
         Props {
            material = {0.3,0.7,0.4,12.};
            inherit = 0;
            trans = 0.4;
         };
      };
   };
   XP_COMM_PROJ.STM3.DENS.Particle_Density Particle_Density<NEx=209.,NEy=121.> {
      molecule => <-.Read_Molecule.molecule;
      Select_Atoms {
         SelectAtoms {
            atom_names[];
         };
         UIoption#0 {
            set = 0;
         };
         UIoption#2 {
            set = 1;
         };
      };
   };
};
