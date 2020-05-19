
flibrary JuliaMacs {

   // JuliaUI
   //   UI Macro
   //   UImod_panel containing 6 UIfieldTypein macros and associated strings

   macro JuliaUI {
      XP_COMM_PROJ.Julia.JuliaMods.JuliaParams
         &JuliaParams<NEx=407.,NEy=55.,NEportLevels={2,1}>;

      UImod_panel UImod_panel<NEx=308.,NEy=363.> {
         title => name_of(<-.<-.<-);
      };

      UIfieldTypein XMandelUIfieldTypein<NEx=616.,NEy=308.> {
         flabel => <-.xmandelString;
         fval => <-.JuliaParams.mandelx;
         UIparent => <-.UImod_panel;
      };

      UIfieldTypein YMandelUIfieldTypein<NEx=616.,NEy=352.> {
         flabel => <-.ymandelString;
         fval => <-.JuliaParams.mandely;
         UIparent => <-.UImod_panel;
      };

      string xmandelString<NEportLevels={1,1},NEx=836.,NEy=308.> = "X Mandelbrot coord.";
      string ymandelString<NEportLevels={1,1},NEx=847.,NEy=352.> = "Y Mandelbrot coord.";

      UIfieldTypein LeftxUIfieldTypein<NEx=198.,NEy=132.> {
         UIparent => <-.UImod_panel;
         flabel => <-.leftxString;
         fval => <-.JuliaParams.leftx;
      };

      UIfieldTypein RightxUIfieldTypein<NEx=209.,NEy=176.> {
         UIparent => <-.UImod_panel;
         flabel => <-.rightxString;
         fval => <-.JuliaParams.rightx;
      };

      UIfieldTypein TopyUIfieldTypein<NEx=231.,NEy=220.> {
         UIparent => <-.UImod_panel;
         flabel => <-.topyString;
         fval => <-.JuliaParams.topy;
      };

      UIfieldTypein BottomUIfieldTypein<NEx=242.,NEy=264.> {
         UIparent => <-.UImod_panel;
         flabel => <-.bottomyString;
         fval => <-.JuliaParams.bottomy;
      };

      // Label strings, linked to above macros
      string leftxString<NEportLevels={1,1},NEx=11.,NEy=132.> = "Left Edge";
      string rightxString<NEportLevels=1,NEx=22.,NEy=176.> = "Right Edge";
      string topyString<NEportLevels=1,NEx=33.,NEy=220.> = "Top Edge";
      string bottomyString<NEportLevels=1,NEx=44.,NEy=264.> = "Bottom Edge";

      UIfieldTypein MaxValUIfieldTypein<NEx=583.,NEy=132.> {
         UIparent => <-.UImod_panel;
         flabel => <-.maxValString;
         fval => <-.JuliaParams.max_value;
      };

      UIfieldTypein MaxIterUIfieldTypein<NEx=594.,NEy=176.> {
         UIparent => <-.UImod_panel;
         flabel => <-.maxIterString;
         fval => <-.JuliaParams.max_iter;
      };

      UIfieldTypein XResUIfieldTypein<NEx=605.,NEy=220.> {
         UIparent => <-.UImod_panel;
         flabel => <-.xresString;
         fval => <-.JuliaParams.resx;
      };

      UIfieldTypein YResUIfieldTypein<NEx=616.,NEy=264.> {
         flabel => <-.yresString;
         fval => <-.JuliaParams.resy;
         UIparent => <-.UImod_panel;
      };

      // Label strings, linked to above macros
      string maxValString<NEportLevels={1,1},NEx=814.,NEy=132.> = "Maximum Value";
      string maxIterString<NEportLevels={1,1},NEx=825.,NEy=176.> = "Max. No. Iterations";
      string xresString<NEportLevels={1,1},NEx=836.,NEy=220.> = "X Resolution";
      string yresString<NEportLevels={1,1},NEx=847.,NEy=264.> = "Y Resolution";

   };


   // Julia
   //   User Macro (JuliaGen, JuliaParams, Data_Object conversion and JuliaUI)

   macro julia<NEx=429.,NEy=66.,NEwidth=275.,NEheight=220.> {

      XP_COMM_PROJ.Julia.JuliaMods.JuliaParams JuliaParams<NEx=374.,NEy=55.> {
         leftx = -2;
         rightx = 2;
         topy = 2;
         bottomy = -2;
         max_value = 4.;
         max_iter = 50.;
         resx = 100;
         resy = 100;
         mandelx = 0.5;
         mandely = 0.5;
      };

      DataObjectNoTexture DataObjectNoTexture<NEx=154.,NEy=242.> {
         in => <-.JuliaGen.out;
      };

      XP_COMM_PROJ.Julia.JuliaMods.JuliaGen JuliaGen<NEx=154.,NEy=154.> {
         JuliaParams => <-.JuliaParams;
      };

      XP_COMM_PROJ.Julia.JuliaMacs.JuliaUI JuliaUI<NEx=418.,NEy=154.,NEwidth=242.,NEheight=187.> {
         JuliaParams => <-.JuliaParams;
      };
      
      link out_fld<NEportLevels={1,2},NEx=44.,NEy=341.> => .JuliaGen.out;
      link out_obj<NEportLevels={1,2},NEx=297.,NEy=341.> => .DataObjectNoTexture.obj;
   };


   // JuliaEg
   //   Example Application (SingleWindowApp)

   APPS.SingleWindowApp JuliaEg {
      UI {
         shell {
            x = 95;
            y = 81;
         };
         Modules {
            IUI {
               optionList {
                  selectedItem = 0;
               };
            };
         };
      };

      GDM.Uviewer2D Uviewer2D<NEx=198.,NEy=231.> {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.julia.out_obj};
               Xform {
                  ocenter = {-0.5,0.,0.};
                  dcenter = {-3.5,0.,0.};
               };
            };
         };

         Scene_Editor {
            View_Editor {
               InfoPanel {
                  curobj_sel {
                     width = 238;
                     height = 245;
                     x = 0;
                     y = 0;
                  };
               };
            };
         };
      };

      julia julia<NEx=198.,NEy=77.> {
         JuliaUI {
            XMandelUIfieldTypein {
               y = 0;
            };
            YMandelUIfieldTypein {
               y = 40;
            };
            LeftxUIfieldTypein {
               y = 80;
            };
            RightxUIfieldTypein {
               y = 120;
            };
            TopyUIfieldTypein {
               y = 160;
            };
            BottomUIfieldTypein {
               y = 200;
            };
            MaxValUIfieldTypein {
               y = 240;
            };
            MaxIterUIfieldTypein {
               y = 280;
            };
            XResUIfieldTypein {
               y = 320;
            };
            YResUIfieldTypein {
               y = 360;
            };
         };
      };
   };
};
