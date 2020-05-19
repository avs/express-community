
flibrary StartProcessMods {

   macro SingleParamProcess {
      ilink parameter;
      ilink trigger;

      XP_COMM_PROJ.StartProcess.StartProcessMods.StartProcessParams params {
         trigger => <-.trigger;
         wait_on_exit = 0;
         command => str_format("myprocess %s", <-.parameter);
      };
      XP_COMM_PROJ.StartProcess.StartProcessMods.StartProcessFunc StartProcessFunc {
         params => <-.params;
      };
   };


   SingleParamProcess EditFile {
      params {
#ifdef MSDOS
         command => str_format("notepad %s", <-.parameter);
#else
         command => str_format("emacs %s", <-.parameter);
#endif
      };
   };

   SingleParamProcess ViewFile {
      params {
#ifdef MSDOS
         command => str_format("type %s", <-.parameter);
#else
         command => str_format("more %s", <-.parameter);
#endif
      };
   };


   macro EditFile_ViewFile {
      string+OPort filename;

      XP_COMM_PROJ.Common.UIWidgets.IAC_StandardUI IAC_StandardUI {
         ilink filename => <-.filename;

         mainTitleLabel {
            label = "Choose File to Edit/View";
            alignment = "center";
         };

         SmartTypeinParams;
         XP_COMM_PROJ.Common.UIWidgets.SmartTextTypein FilenameSmartTextTypein {
            slabel => "Filename: ";
            stext => <-.filename;
            panel {
               x => <-.<-.mainTitleLabel.x;
               y => <-.<-.mainTitleLabel.y + <-.<-.mainTitleLabel.height + 10;
            };
         };

         UIfileDialog chooseFileDialog {
            visible => <-.OpenFileButton.do;
            parent => <-.panel;
            title => "Open File...";
            filename+Port2 => <-.filename;
            searchPattern = "*.*";
         };

         UIbutton OpenFileButton {
            x => parent.width / 2;
            y => <-.FilenameSmartTextTypein.panel.y + <-.FilenameSmartTextTypein.panel.height + 5;
            parent => <-.panel;
            label => "Browse...";
         };

         UIbutton EditFileButton {
            x = 0;
            y => <-.OpenFileButton.y + <-.OpenFileButton.height + 15;
            parent => <-.panel;
            label => "Edit File...";
         };
         UIbutton ViewFileButton {
            x => parent.width / 2;
            y => <-.EditFileButton.y;
            parent => <-.panel;
            label => "View File...";
         };

         olink trig_edit => .EditFileButton.do;
         olink trig_view => .ViewFileButton.do;
      };

      EditFile EditFile {
         parameter => <-.filename;
         trigger => <-.IAC_StandardUI.trig_edit;
      };
      ViewFile ViewFile {
         parameter => <-.filename;
         trigger => <-.IAC_StandardUI.trig_view;
      };
   };



   macro RunProcess {
      string+OPort filename;

      XP_COMM_PROJ.Common.UIWidgets.IAC_StandardUI RunProcessUI {
         ilink result => <-.result;

         mainTitleLabel {
            label = "Run Process";
            alignment = "center";
         };

         UIbutton RunProcessButton {
            x => (parent.width/2) - (width/2);
            y => <-.mainTitleLabel.y + <-.mainTitleLabel.height + 15;
            parent => <-.panel;
            label => "Run Program";
         };

         SmartTypeinParams;
         XP_COMM_PROJ.Common.UIWidgets.SmartFieldTypein ReturnValueSmartFieldTypein {
            panel {
               x => <-.<-.mainTitleLabel.x;
               y => <-.<-.RunProcessButton.y + <-.<-.RunProcessButton.height + 15;
            };
            flabel => "Return Value: ";
            fval => <-.result;
         };

         olink trig_process => .RunProcessButton.do;
      };


      XP_COMM_PROJ.StartProcess.StartProcessMods.StartProcessParams params {
         trigger => <-.RunProcessUI.trig_process;
         wait_on_exit = 1;
         command => <-.filename;
      };
      XP_COMM_PROJ.StartProcess.StartProcessMods.StartProcessFunc StartProcessFunc {
         params => <-.params;
      };

      int+Port result => StartProcessFunc.result;

      olink out_result => result;
   };



   APPS.ModuleStack StartProcessEg {
      EditFile_ViewFile EditFile_ViewFile {
         filename = "$XP_PATH<1>/xp_comm_proj/xp_comm_proj.v";
      };

      RunProcess RunProcess {
         filename = "$XP_PATH<1>/xp_comm_proj/st_proc/test/rand_ret";
      };
   };

};

