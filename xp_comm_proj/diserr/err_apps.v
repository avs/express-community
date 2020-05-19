
flibrary DisplayErrorApps<compile_subs=0>
{
   // working error dialog example application structure
   //
   // This small application shows the naming conventions and
   // usage structure for the display error utility module, to be used by 
   // application developers. A C api call is made from within 
   // the DisplayErrorExampleMod module. The utility module then pushes
   // the current OM state, and invoked the Error UI macro.
   // This can be used used when a module wants to pop up an
   // error dialog box in the middle of computation.

   APPS.DefaultApplication DisplayErrorDialogEg {

      // application window parent for error dialog
      UIapp ErrorExample;

      UIbutton DisplayErrorDialogButton {
         parent => <-.ErrorExample;
         label => "Display Error Dialog...";
         y => (parent.clientHeight / 2) - (height / 2);
         x => (parent.clientWidth / 4);
         width => (parent.clientWidth / 2);
      };

      XP_COMM_PROJ.DisplayErrorDialog.DisplayErrorMods.DisplayErrorExampleMod DisplayErrorExampleMod {
         trigger => <-.DisplayErrorDialogButton.do;
         title = "Test Error";
         message = "This is just a test error dialog box";
      };

      XP_COMM_PROJ.DisplayErrorDialog.DisplayErrorMods.Error Error {
         parent => <-.ErrorExample;
      };
   };

};

