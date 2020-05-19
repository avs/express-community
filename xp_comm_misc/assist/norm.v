//standard commands for reset/normalize buttons
assist0.assist1
   {
   norm
     {  
     parse_v
       {
       v_commands => "normalize=1; center=1;";
       relative => 
       Applications.ModuleStack.Viewer3D.ObjectEditors.ObjectEditor.obj_edit;
       };
     parse_v_reset
       {
       v_commands => "reset=1; normalize=1; center=1;";
       relative => 
       Applications.ModuleStack.Viewer3D.ObjectEditors.ObjectEditor.obj_edit;
       };
     };
   };
ModuleStack.Viewer3D
      {
      ViewUI.Shell
         {
         x=520;
         y=0;
         width=750;
         height=550;
         };
      ViewEditors.CameraEditor.GDcamera_edit
	 {
	 perspec=1;
         auto_norm=3;
         };
      ObjectEditors
         {
         PropsEditor.GDprops_editor.line_aa=1;
         ObjectEditor.obj_edit
             {reset=1; normalize=1; center=1;};
         };
      };

