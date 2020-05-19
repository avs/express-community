
flibrary MandelbrotMacs {

   // MandelbrotUI
   //   UI Macro
   //   UImod_panel containing 6 UIfieldTypein macros and associated strings

   macro MandelbrotUI {
      XP_COMM_PROJ.Mandelbrot.MandelbrotMods.MandelbrotParams
         &MandelbrotParams<NEx=407.,NEy=55.,NEportLevels={2,1}>;

      UImod_panel UImod_panel<NEx=308.,NEy=363.> {
         title => name_of(<-.<-.<-);
      };

      UIfieldTypein LeftxUIfieldTypein<NEx=198.,NEy=132.> {
         UIparent => <-.UImod_panel;
         flabel => <-.leftxString;
         fval => <-.MandelbrotParams.leftx;
      };

      UIfieldTypein RightxUIfieldTypein<NEx=209.,NEy=176.> {
         UIparent => <-.UImod_panel;
         flabel => <-.rightxString;
         fval => <-.MandelbrotParams.rightx;
      };

      UIfieldTypein TopyUIfieldTypein<NEx=231.,NEy=220.> {
         UIparent => <-.UImod_panel;
         flabel => <-.topyString;
         fval => <-.MandelbrotParams.topy;
      };

      UIfieldTypein BottomUIfieldTypein<NEx=242.,NEy=264.> {
         UIparent => <-.UImod_panel;
         flabel => <-.bottomyString;
         fval => <-.MandelbrotParams.bottomy;
      };

      // Label strings, linked to above macros
      string leftxString<NEportLevels={1,1},NEx=11.,NEy=132.> = "Left Edge";
      string rightxString<NEportLevels=1,NEx=22.,NEy=176.> = "Right Edge";
      string topyString<NEportLevels=1,NEx=33.,NEy=220.> = "Top Edge";
      string bottomyString<NEportLevels=1,NEx=44.,NEy=264.> = "Bottom Edge";

      UIfieldTypein MaxValUIfieldTypein<NEx=583.,NEy=132.> {
         UIparent => <-.UImod_panel;
         flabel => <-.maxValString;
         fval => <-.MandelbrotParams.max_value;
      };

      UIfieldTypein MaxIterUIfieldTypein<NEx=594.,NEy=176.> {
         UIparent => <-.UImod_panel;
         flabel => <-.maxIterString;
         fval => <-.MandelbrotParams.max_iter;
      };

      UIfieldTypein XResUIfieldTypein<NEx=605.,NEy=220.> {
         UIparent => <-.UImod_panel;
         flabel => <-.xresString;
         fval => <-.MandelbrotParams.resx;
      };

      UIfieldTypein YResUIfieldTypein<NEx=616.,NEy=264.> {
         flabel => <-.yresString;
         fval => <-.MandelbrotParams.resy;
         UIparent => <-.UImod_panel;
      };

      // Label strings, linked to above macros
      string maxValString<NEportLevels={1,1},NEx=814.,NEy=132.> = "Maximum Value";
      string maxIterString<NEportLevels={1,1},NEx=825.,NEy=176.> = "Max. No. Iterations";
      string xresString<NEportLevels={1,1},NEx=836.,NEy=220.> = "X Resolution";
      string yresString<NEportLevels={1,1},NEx=847.,NEy=264.> = "Y Resolution";

   };


   // mandelbrot
   //   User Macro (MandelbrotGen, MandelbrotParams, Data_Object conversion and MandelbrotUI)

   macro mandelbrot<NEx=429.,NEy=66.,NEwidth=275.,NEheight=220.> {

      XP_COMM_PROJ.Mandelbrot.MandelbrotMods.MandelbrotParams MandelbrotParams<NEx=374.,NEy=55.> {
         leftx = -1.5;
         rightx = 0.5;
         topy = 1.0;
         bottomy = -1.0;
         max_value = 4.;
         max_iter = 50.;
         resx = 100;
         resy = 100;
      };

      DataObjectNoTexture DataObjectNoTexture<NEx=154.,NEy=242.> {
         in => <-.MandelbrotGen.out;
      };

      XP_COMM_PROJ.Mandelbrot.MandelbrotMods.MandelbrotGen MandelbrotGen<NEx=154.,NEy=154.> {
         MandelbrotParams => <-.MandelbrotParams;
      };

      XP_COMM_PROJ.Mandelbrot.MandelbrotMacs.MandelbrotUI MandelbrotUI<NEx=418.,NEy=154.,NEwidth=242.,NEheight=187.> {
         MandelbrotParams => <-.MandelbrotParams;
      };
      
      link out_fld<NEportLevels={1,2},NEx=44.,NEy=341.> => .MandelbrotGen.out;
      link out_obj<NEportLevels={1,2},NEx=297.,NEy=341.> => .DataObjectNoTexture.obj;
   };


   // MandelbrotEg
   //   Example Application (SingleWindowApp)

   APPS.SingleWindowApp MandelbrotEg {
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
                  <-.<-.<-.mandelbrot.out_obj};
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

      mandelbrot mandelbrot<NEx=198.,NEy=77.> {
         MandelbrotUI {
            LeftxUIfieldTypein {
               y = 0;
            };
            RightxUIfieldTypein {
               y = 40;
            };
            TopyUIfieldTypein {
               y = 80;
            };
            BottomUIfieldTypein {
               y = 120;
            };
            MaxValUIfieldTypein {
               y = 160;
            };
            MaxIterUIfieldTypein {
               y = 200;
            };
            XResUIfieldTypein {
               y = 240;
            };
            YResUIfieldTypein {
               y = 280;
            };
         };
      };
   };
};
