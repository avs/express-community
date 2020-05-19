
flibrary ReadFidapMacs <compile_subs=0> {

   macro rdTemplDialog {
      ilink parent;
      UIpanel ReaderPanel {
         parent => <-.parent;
         y = 0;
         width => parent.clientWidth;
         height => parent.clientHeight;
      };
      UIframe SetupFrame {
         parent => <-.ReaderPanel;
         x = 5;
         y = 5;
         width => (parent.clientWidth - (2 * x));
         height = 50;
      };
      UIpanel MainPanel {
         parent => <-.ReaderPanel;
         x = 5.;
         y => <-.SetupFrame.y + <-.SetupFrame.height + 5;
         width => parent.clientWidth - 2 * .x;
         height => parent.clientHeight - .y - 5;
      };
   };


   rdTemplDialog rdFidapDialog {
      XP_COMM_PROJ.ReadFidap.ReadFidapMods.IVrdFidapInputParams &params<NEportLevels={2,0}>;
      XP_COMM_PROJ.ReadFidap.ReadFidapMods.IVrdFidapOutputParams &out_params<NEportLevels={2,0}>;

      //
      //  file Reading Frame
      //
      SetupFrame {
         height => (<-.fnLabel.height + <-.fnText.height) + 12;
      };
      UIlabel fnLabel {
         parent => <-.SetupFrame;
         x = 3;
         y = 3;
         width => parent.clientWidth - 6;
         alignment = 0;
         label = "FIDAP Neutral File Name (ascii)";
         color {
               backgroundColor = "blue";
               foregroundColor = "white";
         };
      };
      UItext fnText {
         parent => <-.SetupFrame;
         x = 3;
         y => (<-.fnLabel.y + <-.fnLabel.height + 3);
         width => ((parent.clientWidth - <-.fnButton.width) - 6);
         updateMode = 3;
         text => <-.params.filename;
      };
      UIbutton fnButton {
         parent => <-.SetupFrame;
         x => ((parent.clientWidth - .width) - 3);
         y => (<-.fnLabel.y + <-.fnLabel.height + 3);
         width = 80;
         height => <-.fnText.height;
         label = "Browse...";
      };
      UIfileDialog fnDialog {
         visible => <-.fnButton.do;
         title = "Read FIDAP Neutral File";
         filename => <-.params.filename;
         searchPattern = "*";
      };

      //
      // Data Selection and Mesh Reading Menu
      //
      UIframe DataFrame {
         parent => <-.MainPanel;
         x = 0;
         y = 0;
         width => ((parent.clientWidth / 2) - 3);
         height => parent.clientHeight;
      };
      UIoptionBoxLabel selData {
         parent => <-.DataFrame;
         labels => <-.out_params.result_labels;
         title = "Select Data Items";
         x = 3;
         y = 3;
         selectedItems => <-.params.load_items;
         width => parent.clientWidth;
         UIlabel {
             color {
                   backgroundColor = "blue";
                   foregroundColor = "white";
             };
         };
      };
      UIlabel tsLabel {
         parent => <-.DataFrame;
         x = 3;
         y => <-.selData.y + <-.selData.height + 10;
         width => parent.clientWidth - 2 * x;
         label = "Select Time Steps";
         color {
               backgroundColor = "blue";
               foregroundColor = "white";
         };
      };
      UIlabel tsMinLabel {
         parent => <-.DataFrame;
         x = 3;
         y => (<-.tsMinField.y + ((<-.tsMinField.height - .height) / 2));
         width => parent.clientWidth/2;
         label = "Time Min";
      };
      UIfield tsMinField {
         parent => <-.DataFrame;
         x => <-.tsMinLabel.x + <-.tsMinLabel.width + 3;
         y => <-.tsLabel.y + <-.tsLabel.height + 5;
         width => parent.clientWidth - .x - 3;
         mode = 1;
         min = 0;
         //max => <-.out_params.out_ndat_dim - 1;
         value => <-.params.time_control[0];
         updateMode = 3;
      };
      UIlabel tsMaxLabel {
         parent => <-.DataFrame;
         x = 3;
         y => (<-.tsMaxField.y + ((<-.tsMaxField.height - .height) / 2));
         width => parent.clientWidth/2;
         label = "Time Max";
      };
      UIfield tsMaxField {
         parent => <-.DataFrame;
         x => <-.tsMaxLabel.x + <-.tsMaxLabel.width + 3;
         y => <-.tsMinField.y + <-.tsMinField.height + 5;
         width => parent.clientWidth - .x - 3;
         mode = 1;
         min = 0;
         //max => <-.out_params.out_ndat_dim - 1;
         value => <-.params.time_control[1];
         updateMode = 3;
      };
      UIlabel tsIncLabel {
         parent => <-.DataFrame;
         x = 3;
         y => (<-.tsIncField.y + ((<-.tsIncField.height - .height) / 2));
         width => parent.clientWidth/2;
         label = "Time Inc";
      };
      UIfield tsIncField {
         parent => <-.DataFrame;
         x => <-.tsIncLabel.x + <-.tsIncLabel.width + 3;
         y => <-.tsMaxField.y + <-.tsMaxField.height + 5;
         width => parent.clientWidth - .x - 3;
         mode = 1;
         min = 0;
         value => <-.params.time_control[2];
         updateMode = 3;
      };
      UIbutton Read_MESH_Button {
         parent => <-.DataFrame;
         label = "Read Mesh";
         x = 10;
         y => <-.tsIncField.y +<-.tsIncField.height + 10;
         width => parent.clientWidth - 2 * x;
         do => <-.params.read_mesh;
      };
      UIbutton Read_NDAT_Button {
         parent => <-.DataFrame;
         label = "Read Results";
         x = 10;
         y => <-.Read_MESH_Button.y +<-.Read_MESH_Button.height + 10;
         width => parent.clientWidth - 2 * x;
         do => <-.params.read_ndat;
      };


      //
      // Node Data Animation Controls
      //
      UIframe LoopFrame {
         parent => <-.MainPanel;
         x => ((parent.clientWidth / 2) + 3);
         y = 0;
         width => ((parent.clientWidth / 2) - 3);
         height => parent.clientHeight;
      };
      UIlabel loopTitle {
         parent => <-.LoopFrame;
         label = "Animate Results";
         x = 3;
         y = 3;
         width => parent.clientWidth - 2 * x;
         alignment = 1;
         color {
               backgroundColor = "blue";
               foregroundColor = "white";
         };
      };
      UIlabel loopLabel {
         parent => <-.LoopFrame;
         label = "Time Loop";
         x = 3;
         y => (<-.loopTitle.y + <-.loopTitle.height );
         width => parent.clientWidth - 2 * x;
         alignment = 1;
      };
      UIlabel loopMinLabel {
         parent => <-.LoopFrame;
         x = 3;
         y => (<-.loopMinField.y + ((<-.loopMinField.height - .height) / 2));
         width => parent.clientWidth/2;
         label = "Start Value";
      };
      UIfield loopMinField {
         parent => <-.LoopFrame;
         x => <-.loopMinLabel.x + <-.loopMinLabel.width + 3;
         y => <-.loopLabel.y + <-.loopLabel.height + 5;
         width => parent.clientWidth - .x - 3;
         mode = 1;
         min = 0;
         //max => <-.out_params.out_ndat_dim - 1;
         value => <-.loop.start_val;
         updateMode = 3;
      };
      UIlabel loopMaxLabel {
         parent => <-.LoopFrame;
         x = 3;
         y => (<-.loopMaxField.y + ((<-.loopMaxField.height - .height) / 2));
         width => parent.clientWidth/2;
         label = "End Value";
      };
      UIfield loopMaxField {
         parent => <-.LoopFrame;
         x => <-.loopMaxLabel.x + <-.loopMaxLabel.width + 3;
         y => <-.loopMinField.y + <-.loopMinField.height + 5;
         width => parent.clientWidth - .x - 3;
         mode = 1;
         min = 0;
         //max => <-.out_params.out_ndat_dim - 1;
         value => <-.loop.end_val;
         updateMode = 3;
      };
      UIlabel loopIncLabel {
         parent => <-.LoopFrame;
         x = 3;
         y => (<-.loopIncField.y + ((<-.loopIncField.height - .height) / 2));
         width => parent.clientWidth/2;
         label = "Increment";
      };
      UIfield loopIncField {
         parent => <-.LoopFrame;
         x => <-.loopIncLabel.x + <-.loopIncLabel.width + 3;
         y => <-.loopMaxField.y + <-.loopMaxField.height + 5;
         width => parent.clientWidth - .x - 3;
         mode = 1;
         min = 1;
         value => <-.loop.incr;
         updateMode = 3;
      };
      UIlabel loopCntLabel {
         parent => <-.LoopFrame;
         x = 3;
         y => (<-.loopCntField.y + ((<-.loopCntField.height - .height) / 2));
         width => parent.clientWidth/2;
         label = "Index";
      };
      UIfield loopCntField {
         parent => <-.LoopFrame;
         x => <-.loopCntLabel.x + <-.loopCntLabel.width + 3;
         y => <-.loopIncField.y + <-.loopIncField.height + 5;
         width => parent.clientWidth - .x - 3;
         mode = 1;
         value => <-.loop.count;
         updateMode = 3;
      };
      UItoggle runToggle {
         parent => <-.LoopFrame;
         x = 5;
         y => <-.loopCntField.y + <-.loopCntField.height + 10;
         label = "Run";
         set => <-.loop.run;
      };
      UItoggle resetToggle {
         parent => <-.LoopFrame;
         x = 5;
         y => <-.runToggle.y + <-.runToggle.height + 5;
         label = "Reset";
         set => <-.loop.reset;
      };
      UItoggle cycleToggle {
         parent => <-.LoopFrame;
         x = 5;
         y => <-.resetToggle.y + <-.resetToggle.height + 5;
         label = "Cycle";
         set => <-.loop.cycle;
      };
      GMOD.loop loop {
         count => <-.out_params.time_index;
      };
      GMOD.parse_v set_loop {
         v_commands = "start_val=0;incr=1;reset=1;";
         trigger => <-.out_params.out_ndat_dim;
         on_inst = 0;
         relative = <-.loop;
      };
      GMOD.copy_on_change set_loop_max {
         trigger => <-.out_params.out_ndat_dim;
         input =>      <-.out_params.out_ndat_dim-1;
         output =>     <-.loop.end_val;
         on_inst = 0;
      };


      UIslider count_slider {
         parent => <-.LoopFrame;
         x = 5;
         y => <-.cycleToggle.y + <-.cycleToggle.height + 5;
         width => parent.clientWidth - 2*x;
         title = "Index";
         mode = 1;
         value => <-.loop.count;
         min => <-.loop.start_val;
         max => <-.loop.end_val;
         immediateMode = 0;
      };
      UItoggle immediate_toggle {
         parent => <-.LoopFrame;
         x = 5;
         y => <-.count_slider.y + <-.count_slider.height + 5;
         label = "Immediate";
         set => <-.count_slider.immediateMode;
      };

   };


  macro rdFidapFunc {

   XP_COMM_PROJ.ReadFidap.ReadFidapMods.IVrdFidapInputParams &params<NEportLevels={2,1}>;

   XP_COMM_PROJ.ReadFidap.ReadFidapMods.IVrdFidapResult IVrdFidapResult {
      filename => <-.params.filename;
      read_mesh => <-.params.read_mesh;
      read_ndat => <-.params.read_ndat;
      time_control => <-.params.time_control;
      load_items => <-.params.load_items;
   };

   XP_COMM_PROJ.ReadFidap.ReadFidapMods.IVrdFidapOutputParams &out_params<NEportLevels={1,2}> {
      nresults => <-.IVrdFidapResult.nresults;
      result_labels => <-.IVrdFidapResult.result_labels;
      ntime_steps => <-.IVrdFidapResult.ntime_steps;
      out_ndat_dim => <-.IVrdFidapResult.out_ndat_dim;
      time_index => <-.IVrdFidapResult.time_index;
   };

   GDM.DataObject DataObject {
      in => <-.IVrdFidapResult.out;
   };

   olink out_time => IVrdFidapResult.time;
   olink out_fld => IVrdFidapResult.out;
   olink out_obj => DataObject.obj;
  };


  macro rdFidap_base {

   XP_COMM_PROJ.ReadFidap.ReadFidapMods.IVrdFidapInputParams IVrdFidapInputParams {
      read_mesh = 1;
   };

   rdFidapFunc rdFidapFunc {
      params => <-.IVrdFidapInputParams;
   };

   rdFidapDialog rdFidapDialog {
      params => <-.IVrdFidapInputParams;
      out_params => <-.rdFidapFunc.out_params;
   };

   olink out_time => rdFidapFunc.out_time;
   olink out_fld => rdFidapFunc.out_fld;
   olink out_obj => rdFidapFunc.out_obj;
  };


  rdFidap_base Read_Fidap_Dialog {
      int visible<NEportLevels={2,1}>;
      UItemplateDialog ReaderDialog {
         visible<NEportLevels={2,0}> => <-.visible;
         width = 350;
         height = 600;
         okLabelString = "Close";
         title = "Read FIDAP";
      };
      rdFidapDialog {
         parent => <-.ReaderDialog;
      };
  };


  rdFidap_base Read_Fidap_Module {
      UImod_panel Read_Fidap_Neutral_File {
         width = 275;
         height = 600;
      };
      rdFidapDialog {
         parent => <-.Read_Fidap_Neutral_File;
      };
  };


  APPS.SingleWindowApp ReadFidapEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.Read_Fidap_Module.out_obj};
            };
         };
      };

      Read_Fidap_Module Read_Fidap_Module;
  };

};

