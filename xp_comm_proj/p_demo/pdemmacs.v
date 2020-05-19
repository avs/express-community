
flibrary PresentDemoMacs <compile_subs=0> {

   macro PresentDemoMainUI {
      XP_COMM_PROJ.PresentDemo.PresentDemoMods.PresentDemoParams &params<NEportLevels={2,1}>;

      UIshell+OPort UIshell {
         menu => <-.FileCmdList;
         parent<NEportLevels={4,0}>;
         title => <-.params.title;
         width = 350;
         height = 400;
      };

      UIcmdList FileCmdList {
         cmdList => {<-.File};
      };
      UIcmdList File {
         cmdList => {<-.Exit};
      };
      UIcmd Exit;

      GMOD.exit_process exit_process {
         do_exit => <-.Exit.do;
      };

      olink parent => .UIshell;
   };


   macro PresentDemoControlsUI {
      ilink parent;

      int    &inc_objects<NEportLevels={2,1}>[];
      string &objectnames<NEportLevels={2,1}>[];

      XP_COMM_PROJ.PresentDemo.PresentDemoMods.PresentDemoParams &params<NEportLevels={2,1}>;

      UIpanel UIpanel {
         parent => <-.parent;
         x = 5;
         y = 0;
         width => parent.clientWidth - 10;
         height => <-.ObjectsOptionBox.y + <-.ObjectsOptionBox.height + 5;
      };

      UIbutton RotateButton {
         parent => <-.UIpanel;
         label => "R";
         x = 5;
         y = 5;
         width = 40;
         height = 40;
         message = "Mb1 = Rotate Objects";
         do => <-.params.rotate;
      };
      UIbutton ScaleButton {
         parent => <-.UIpanel;
         label => "S";
         x => <-.RotateButton.x + <-.RotateButton.width + 2;
         y => <-.RotateButton.y;
         width => <-.RotateButton.width;
         height => <-.RotateButton.height;
         message = "Mb1 = Scale Objects";
         do => <-.params.scale;
      };
      UIbutton TranslateButton {
         parent => <-.UIpanel;
         label => "T";
         x => <-.ScaleButton.x + <-.ScaleButton.width + 2;
         y => <-.ScaleButton.y;
         width => <-.RotateButton.width;
         height => <-.RotateButton.height;
         message = "Mb1 = Translate Objects";
         do => <-.params.translate;
      };
      UIbutton RNC_Button {
         parent => <-.UIpanel;
         label => "RNC";
         x => <-.TranslateButton.x + <-.TranslateButton.width + 2;
         y => <-.TranslateButton.y;
         width => <-.RotateButton.width;
         height => <-.RotateButton.height;
         message = "Reset, Normalize, Centre";
         do => <-.params.rnc;
      };

      UItoggle ShowMenuToggle {
         parent => <-.UIpanel;
         label = "Show Menu";
         x => <-.RNC_Button.x + <-.RNC_Button.width + 20;
         y => <-.RNC_Button.y;
         width = 150;
         set => <-.params.show_menu;
      };
#ifdef MPU_RENDERER
      UItoggle UseMPUToggle {
         parent => <-.UIpanel;
         label = "Use MPU Renderer";
         x => <-.ShowMenuToggle.x;
         y => <-.ShowMenuToggle.y + <-.ShowMenuToggle.height + 5;
         width = 150;
         set => <-.params.use_mpu;
      };
#endif

      UIoptionBoxLabel ObjectsOptionBox {
         parent => <-.UIpanel;
         labels+IPort2 => <-.objectnames;
         &selectedItems+IPort2 => <-.inc_objects;
         title = "Select Visualizations";
         x = 0;
         y => <-.RotateButton.y + <-.RotateButton.height + 20;
         width => parent.width;
      };

      olink panel => .UIpanel;
   };


   macro PresentDemoModulesUI {
      ilink parent;
      ilink top_panel;

      UIpanel UIpanel {
         parent => <-.parent;
         x = 5;
         y => <-.top_panel.height + 10;
         width => parent.clientWidth - 10;
         height => parent.clientHeight - <-.top_panel.height - 10;
      };

      olink panel => .UIpanel;
   };


   macro present_demo {
      imlink in_objs;

      int+OPort    inc_objects[];
      group gen_objectnames[array_size(.in_objs)] {
         string+OPort2 text => "Object[" + index_of(<-.gen_objectnames) + "]";
      };
      string+Port &objectnames[] => gen_objectnames.text;

      XP_COMM_PROJ.PresentDemo.PresentDemoMods.PresentDemoParams params {
         title = "Present Demo Control Box";
         
         rotate+nres => UI.toolbar.buttons.button3.do;
         scale+nres => UI.toolbar.buttons.button4.do;
         translate+nres => UI.toolbar.buttons.button5.do;
         rnc+nres => UI.toolbar.buttons.button10.do;

         show_menu+nres => UI.shell.visible;

#ifdef MPU_RENDERER
/*
         use_mpu+nres => !<-.<-.Uviewer.Scene_Editor.View_Editor.HW_SW.set;
*/
#endif
      };

      PresentDemoMainUI PresentDemoMainUI {
         params => <-.params;
      };

      PresentDemoControlsUI PresentDemoControlsUI {
         parent => <-.PresentDemoMainUI.parent;
         inc_objects => <-.inc_objects;
         objectnames => <-.objectnames;
         params => <-.params;
      };

      PresentDemoModulesUI PresentDemoModulesUI {
         parent => <-.PresentDemoMainUI.parent;
         top_panel => <-.PresentDemoControlsUI.panel;
      };

#ifdef MPU_RENDERER
/*
      GMOD.copy_on_change mpu_cponch {
         trigger => params.use_mpu;
         input => !params.use_mpu;
         output => <-.HW_SW;
         on_inst = 0;
      };

      int+Port+nres HW_SW => <-.Uviewer.Scene_Editor.View_Editor.HW_SW.set;
*/
#endif

      XP_COMM_PROJ.ExtractObjects.ExtractObjectsMods.ExtractObjectsCore ExtractObjectsCore {
         in_objs  => <-.in_objs;
         inc_objs => <-.inc_objects;
      };

      GDM.GroupObject GroupObject {
         child_objs => ExtractObjectsCore.out_objs;
         Top.name => name_of(<-.<-.<-);
      };

      olink out_objs => GroupObject.obj;
      olink module_panel => PresentDemoModulesUI.panel;
   };

};

