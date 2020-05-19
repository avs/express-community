
flibrary Ret2NullMacs <compile_subs=0> {

   APPS.DefaultApplication V_Ret2NullEg {
      UIshell UIshell {
         title => name_of(<-.<-,1);
      };

      UIfield MyField {
         parent => <-.UIshell;
         value+Port2 = 6.;
         x = 10;
         y = 10;
      };

      UIbutton MyButton {
         parent => <-.UIshell;
         x => <-.MyField.x;
         y => <-.MyField.y + <-.MyField.height + 10;
         label => "Reset to Null";
      };

      GMOD.parse_v parse_v {
         v_commands = "MyField.value = ;";
         trigger => <-.MyButton.do;
         relative => <-.MyField;
         on_inst = 0;
      };
   };


   APPS.DefaultApplication C_Ret2NullEg {
      UIshell UIshell {
         title => name_of(<-.<-,1);
      };

      UIfield+OPort MyField {
         parent => <-.UIshell;
         value+Port2 = 6.;
         x = 10;
         y = 10;
      };

      UIbutton MyButton {
         parent => <-.UIshell;
         x => <-.MyField.x;
         y => <-.MyField.y + <-.MyField.height + 10;
         label => "Reset to Null";
      };

      XP_COMM_PROJ.Ret2Null.Ret2NullMods.ReturnToNull ReturnToNull {
         ui_field => <-.MyField;
         trigger => <-.MyButton.do;
      };
   };

};

