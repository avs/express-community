APPS.SingleWindowApp SingleWindowApp {
   UI {
      shell {
         x = 15;
         y = 46;
      };
      Modules {
         IUI {
            optionList {
               selectedItem = 3;
               cmdList => {
                  <-.<-.<-.<-.cell_to_node.CellToNodeUI.UIpanel.option,
                  <-.<-.<-.<-.external_edges.ExtEdgeUI.UIpanel.option,
                  <-.<-.<-.<-.isosurface.UIpanel.option,
                  <-.<-.<-.<-.ReadSTARMacro.readSTARdialog.UImod_panel.option};
            };
         };
      };
      Windows {
         IUI {
            panel {
               height = 676;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=600,NEy=450> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.external_edges.out_obj,<-.<-.<-.isosurface.out_obj};
            Xform {
               mat = {
                  -11.974,-69.8835,-27.5371,0.,67.8118,1.93683,-34.3983,0.,
32.3054,-29.9659,61.9981,0.,0.,0.,0.,1.
               };
               xlate = {-1.00442,-14.9956,
9.0133};
            };
         };
         View {
            View {
               trigger = 1;
            };
         };
      };
   };
   MODS.cell_to_node cell_to_node<NEx=121.,NEy=198.> {
      in_field => <-.ReadSTARMacro.readSTAR.out_fld;
      CellToNodeUI {
         UIoptionBoxLabel {
            label_cmd {
               cmd[4] = {
                  {
                     active=1,
                  },
                  {
                     set=1,,,,,,,,,,,,,,,,,,,active=1,
                  },
                  {
                     set=1,,,,,,,,,,,,,,,,,,,active=1,
                  },
                  {
                     set=1,,,,,,,,,,,,,,,,,,,active=1,
                  }};
            };
         };
         DVcell_data_labels {
            labels[];
         };
      };
      CellToNodeParam {
         comps = {0,1,2,3};
      };
   };
   MODS.external_edges external_edges<NEx=517.,NEy=132.> {
      in_field => <-.ReadSTARMacro.readSTAR.out_fld;
   };
   MODS.isosurface isosurface<NEx=385.,NEy=352.> {
      in_field => <-.cell_to_node.out_fld;
      IsoParam {
         iso_component = 2;
         iso_level => 2353.43042;
      };
      IsoUI {
         UIoptionBoxLabel {
            label_cmd {
               cmd[4];
            };
         };
         UIoptionBoxLabel_cell {
            label_cmd {
               cmd[4] = {
                  {
                     active=1,
                  },
                  {
                     active=1,
                  },
                  {
                     active=1,
                  },
                  {
                     active=1,
                  }};
            };
         };
         UIradioBoxLabel {
            label_cmd {
               cmd[];
            };
         };
      };
      DVcell_data_labels {
         labels[];
      };
      DVnode_data_labels {
         labels[];
      };
   };
   WORKSPACE_2.ReadSTARMacro ReadSTARMacro<NEx=110.,NEy=44.> {
      readSTARparams {
         case = "";
         load_type = 1;
         cdconv = 1;
         cdsel = {0,1,3,4};
         wdsel = {0};
      };
      readSTARdialog {
         noResultOpt {
            set = 0;
         };
         stResultOpt {
            set = 1;
         };
         cdNoneOpt {
            set = 0;
         };
         cdScalOpt {
            set = 1;
         };
         cdScalMacro {
            cdScalMenu {
               label_cmd {
                  cmd[] = {
                     ,
                     {
                        set=1,,,
                     },,
                     {
                        set=1,,,
                     },
                     {
                        set=1,,,
                     },,,,
                     ,,,,,
                  };
               };
            };
            pconvMenu {
               label_cmd {
                  cmd[];
               };
            };
            tconvMenu {
               label_cmd {
                  cmd[];
               };
            };
         };
         cdFluxMacro<instanced=0> {
            cdFluxMenu {
               label_cmd {
                  cmd[];
               };
            };
         };
         WdataMenu {
            label_cmd {
               cmd[];
            };
         };
      };
   };
};
