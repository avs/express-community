//
// file: rdflmacs.v
// authors: Dr. Jean M. Favre, CSCS, Swiss Center for Scientific Computing
//   		Tobias Schiebeck, IAC
// 		    
// purpose: High level macro library for read fluent module
//


flibrary ReadFluentMacs<compile_subs=0> {

   macro ReadFluentUI {
	  XP_COMM_PROJ.ReadFluent.ReadFluentMods.ReadFluentParams &params;
	  	
      UImod_panel UImod_panel {
         parent<NEportLevels={4,0}>;
         title => name_of(<-.<-.<-,1);
      };

      UIfileDialog Select_File {
         parent => <-.UImod_panel;
         visible<NEportLevels={2,1}> => <-.BrowseButton.do;
         searchPattern = "*";
		 filename => <-.params.filename;
      };


      UIlabel FilenameLabel {
         parent => <-.UImod_panel;
         label => "Read Fluent Filename";
         alignment = 0;
         x = 0;
         y = 0;
         width => parent.clientWidth;
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

      UIbutton BrowseButton {
         parent => <-.UImod_panel;
         label = "Browse...";
         x = 0;
         y => <-.FilenameLabel.y + <-.FilenameLabel.height + 10;
      };
      UIbutton TriggerButton {
         parent => <-.UImod_panel;
         label = "Open File";
         x => <-.BrowseButton.x + <-.BrowseButton.width + 15;
         y => <-.BrowseButton.y;
		 do => <-.params.trigger;
      };


      UItext FilenameText {
         parent => <-.UImod_panel;
         text => <-.Select_File.filename;
         x = 0;
         y => <-.BrowseButton.y + <-.BrowseButton.height + 10;
         width => parent.clientWidth;
      };

   };

	macro ReadFluentFunc{
		XP_COMM_PROJ.ReadFluent.ReadFluentMods.ReadFluentParams &params;
		XP_COMM_PROJ.ReadFluent.ReadFluentMods.read_fluent_core ReadFluentCore{
			filename => <-.params.filename;
			trigger => <-.params.trigger;
		};
		olink out => ReadFluentCore.out;	
	};	

   macro ReadFluent {
	  XP_COMM_PROJ.ReadFluent.ReadFluentMods.ReadFluentParams params;
      ReadFluentUI ReadFluentUI{
	  		params =><-.params;
	  };
	  ReadFluentFunc ReadFluentFunc{
	  		params => <-.params;
	  };
	  GDM.DataObject DataObj{
	  	in => <-.ReadFluentFunc.out;
	  };	  
	  olink out_mesh => ReadFluentFunc.out;
	  olink out_obj => DataObj.obj;	
   };

};

