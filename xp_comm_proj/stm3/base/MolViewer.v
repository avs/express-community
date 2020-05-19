GDM.Uviewer3D MolViewer<NEhelpContextID=1,NEhelpFile="STM3/MolViewer.html"> {
   Scene {
      View {
         View {
            trigger = 1;
            back_col = {0.6,0.6,0.6};
         };
		 VirtPal {
		    gamma = 1.1;
		 };
      };
   };
   Scene_Editor {
      View_Editor {
         IUI {
            optionList {
               selectedItem = 0;
            };
            General {
               IUI {
                  panel {
                     visible = 1;
                  };
                  Renderer {
                     OPcmdList = {
                        ,
                        {
                           set=1,,,
                        },
                        ,,
                     };
                  };
                  Color {
                     ColorEcho {
                        ColorView {
                           trigger = 2;
                        };
                     };
                     rgb_or_hsv {
                        h = 0.5;
                        v = 0.5;
                     };
                  };
                  TimerFrame {
                     height => 40;
                  };
                  Timer {
                     y = 0;
                  };
               };
            };
         };
			GDobj_sel {
				cur_obj<NEportLevels={0,4}>;
			};
      };
      Track_Editor {
         cur_obj => <-.<-.Scene.Top.Top;
      };
   };
};
