
flibrary ReadSLPSTLMacs {

   // ReadSLPSTLUI
   //   UI Macro
   //   UImod_panel containing filename chooser and error message components

   macro ReadSLPSTLUI {
      XP_COMM_PROJ.ReadSLPSTL.ReadSLPSTLMods.ReadSLPSTLParams
         &ReadSLPSTLParams<NEx=231.,NEy=33.,export=1,NEportLevels={2,1}>;

      UImod_panel panel<NEx=33.,NEy=396.> {
         message = "Select Read SLP/STL control panel.";
         title => name_of(<-.<-.<-);
         parent<NEportLevels={4,0}>;
      };

      UIlabel UCD_Filename<NEx=704.,NEy=385.> {
         parent => <-.panel;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Read SLP/STL File Name";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

      UItext file_name<NEx=528.,NEy=341.> {
         parent => panel;
         y => UCD_Filename.y + UCD_Filename.height + 5;
         text<NEportLevels={2,2}> => ReadSLPSTLParams.filename;
         width = 170;
         showLastPosition = 1;
      };

      UIbutton visible<NEx=770.,NEy=220.> {
         parent => panel;
         x => file_name.x + file_name.width + 5;
         y => file_name.y;
         width = 75;
         height => <-.file_name.height;
         label => "Browse...";
      };

      UIfileSB file_browser<NEx=770.,NEy=154.> {
         GMOD.copy_on_change copy_on_change {
            trigger => <-.<-.visible.do;
            input => <-.<-.visible.do;
            output => <-.visible;
         };
         visible => <-.visible.do;
         title => "Read SLP/STL Filename";
         searchPattern = "$XP_PATH<1>/data/*";
         filename<NEportLevels={2,2}> => <-.ReadSLPSTLParams.filename;
      };

      UIlabel err_label<NEx=198.,NEy=319.> {
         parent => <-.panel;
         visible => <-.ReadSLPSTLParams.err;
         y => file_name.y + file_name.height + 5;
         width => parent.width;
         alignment = "left";
         label => "Read Error:";
         color {
            foregroundColor = "red";
         };
      };

      UIlabel err_label_msg<NEx=363.,NEy=319.> {
         parent => <-.panel;
         visible => <-.ReadSLPSTLParams.err;
         y => err_label.y + err_label.height + 5;
         width => parent.width;
         alignment = "left";
         label => <-.ReadSLPSTLParams.err_str;
      };

   };


   // readSLPSTL
   //   User Macro (ReadSLPSTLFile, ReadSLPSTLParams, Data_Object conversion and ReadSLPSTLUI)

   macro readSLPSTL {
      XP_COMM_PROJ.ReadSLPSTL.ReadSLPSTLMods.ReadSLPSTLParams read_slp_params<NEx=132.,NEy=22.>;

      XP_COMM_PROJ.ReadSLPSTL.ReadSLPSTLMods.ReadSLPSTLFile read_slp {
         ReadSLPSTLParams => <-.read_slp_params;
      };

      XP_COMM_PROJ.ReadSLPSTL.ReadSLPSTLMacs.ReadSLPSTLUI ReadSLPSTLUI<NEx=330.,NEy=99.> {
         ReadSLPSTLParams => <-.read_slp_params;
      };

      GDM.DataObject ObjLite<NEx=429.,NEy=231.> {
         in => <-.read_slp.out;
      };

      olink out<NEx=88.,NEy=231.> => read_slp.out;
      olink obj<NEx=440.,NEy=308.> => ObjLite.obj;
   };


   // ReadSLPSTLEg
   //   Example Application (SingleWindowApp)

   APPS.SingleWindowApp ReadSLPSTLEg {
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

      GDM.Uviewer3D Uviewer3D<NEx=198.,NEy=231.> {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.readSLPSTL.obj
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

      readSLPSTL readSLPSTL<NEx=198.,NEy=77.>;
   };
};
