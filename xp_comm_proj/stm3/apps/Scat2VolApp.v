APPS.SingleWindowApp Scat2VolumeDisplayApp<NEdisplayMode="maximized",NEhelpContextID=1,NEhelpFile="STM3/Scat2VolumeDisplayApp.html"> {
   UI {
      shell {
         x = 20;
         y = 20;
      };
      toolbar {
         width = 507;
      };
      Windows {
         IUI {
            panel {
               width = 518;
               height = 518;
            };
         };
      };
      Modules {
         IUI {
            optionList {
               cmdList => {
                  <-.<-.<-.<-.Read_Anim_Molecule.UI.UImod_panel.option,
                  <-.<-.<-.<-.Read_Anim_Molecule.Loop.UIpanel.option,
                  <-.<-.<-.<-.make_movie.UImod_panel.option,
                  <-.<-.<-.<-.Scat_to_Volume.UImod_panel.option,
                  <-.<-.<-.<-.Scat_to_Volume.volume_render.UImod_panel.option
               };
               selectedItem = 0;
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
      Loop {
         count = 1.;
      };
   };
   XP_COMM_PROJ.STM3.BASE.MolViewer MolViewer<NEx=539.,NEy=297.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.Scat_to_Volume.out};
            Xform {
               ocenter = {3.58465,3.50191,
3.59795};
               dcenter = {3.48197,3.41154,
3.4797};
               mat = {
                  0.748032,0.0643391,-0.114036,0.,-0.039585,0.741574,0.158733,
0.,0.124807,-0.150412,0.733822,0.,0.,0.,0.,1.
               };
               xlate = {-3.48197,-3.41154,
-3.4797};
               center = {3.58465,3.50191,
3.59795};
            };
         };
         View {
            View {
               renderer => "Software";
               trigger = 2;
            };
         };
      };
      Scene_Editor {
         View_Editor {
            IUI {
               optionList {
                  selectedItem = 1;
               };
               General {
                  IUI {
                     panel {
                        visible = 0;
                     };
                     Renderer {
                        OPcmdList = {
                           
                           {
                              set=1,,
                           },
                           {
                              set=0,,,
                           },
                           ,,
                        };
                     };
                  };
               };
               Options {
                  IUI {
                     panel {
                        visible = 1;
                     };
                     DoubleBuffer {
                        OPcmdList = {,,
                           {
                              set=1,,,
                           }};
                     };
                     Aspect {
                        OPcmdList = {
                           {
                              set=1,,
                           },
                           };
                     };
                     BlendMode {
                        OPcmdList = {
                           {
                              set=1,,,
                           },
                           };
                     };
                  };
               };
            };
         };
      };
   };
   XP_COMM_PROJ.STM3.SCAT2VOL.Scat_to_Volume Scat_to_Volume<NEx=319.,NEy=187.> {
      in => <-.Read_Anim_Molecule.molecule;
      volume_render {
         DataObject {
            VolRenderDatamap {
               DatamapValue = {,
                  {v1=0.09},,};
            };
         };
         datamap_ui {
            Dmap2ImageLegend {
               Scene {
                  Top {
                     Xform {
                        dcenter = {-670.,-82.681,0.};
                     };
                  };
                  View {
                     View {
                        trigger = 1;
                     };
                  };
               };
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
