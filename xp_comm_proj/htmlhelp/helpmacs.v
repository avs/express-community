

flibrary HtmlHelpMacs <compile_subs=0> {

//
// help launching button
// ---------------------
   group HtmlHelpButton {
      string help_filename<NEportLevels={2,1}>;

      UIbutton ShowHelpButton {
         visible<NEportLevels={3,0}>;
         parent<NEportLevels={3,0}>;
         label<NEportLevels={3,0}> => "Help...";
      };

      XP_COMM_PROJ.HtmlHelp.HtmlHelpMods.HtmlHelpCore HtmlHelpCore {
         help_filename => <-.help_filename;
         show_help => <-.ShowHelpButton.do;
      };
   };




//
// help launching interface panel
// ------------------------------
   macro HtmlHelp_ModPanel {
      UImod_panel panel {
         title => "HTML Help";
      };

      XP_COMM_PROJ.HtmlHelp.HtmlHelpMacs.HtmlHelpButton HtmlHelpButton {
         help_filename<NEportLevels={3,1}>;
         ShowHelpButton {
            parent => <-.<-.panel;
         };
      };
   };



//
// help launching example
// ----------------------
   APPS.ModuleStack HtmlHelpEg {

      string+Port filename = "xp_comm_proj/htmlhelp/htmlhelp.html";

      XP_COMM_PROJ.HtmlHelp.HtmlHelpMacs.HtmlHelp_ModPanel HtmlHelp_ModPanel {
         panel {
            option {
               set = 1;
            };
         };

         HtmlHelpButton {
            help_filename => <-.<-.filename;
         };
      };
   };


};

