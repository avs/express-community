flibrary ReadAnalyzeMacs <compile_subs=0> {


       //ReadAnalyzeUI
       //readAnalyze
       //ReadAnalyzeEg

//************************************************************************

   //ReadAnalyzeUI

    macro ReadAnalyzeUI {
      Root.UImod_panel UImod_panel {
	title => "readAnalyze";
	parent<NEportLevels={3,0}>;
      };
      UIlabel Filename {
	parent => <-.UImod_panel;
	y = 0;
	width = 200;
	alignment = 0;
      };
      UItext file_name {
	parent => <-.UImod_panel;
	y => ((Filename.y + Filename.height) + 5);
	text => <-.filename_out;
	width = 170;
	showLastPosition = 1;
      };
      UIbutton visible {
	parent => <-.UImod_panel;
	x => ((file_name.x + file_name.width) + 5);
	y => file_name.y;
	width = 75;
	height => <-.file_name.height;
	label => "Browse...";
      };
      UIfileSB file_browser {
	GMOD.copy_on_change copy_on_change {
	  trigger => <-.<-.visible.do;
	  input => <-.<-.visible.do;
	  output => <-.visible;
	};
	title => "Filename";
	filename => <-.filename_out;
	searchPattern = "./*.hdr";
      };
      string filename_out<NEportLevels={0,2}>;
    };
 




//************************************************************************

   //readAnalyze

   macro readAnalyze {
      XP_COMM_PROJ.ReadAnalyze.ReadAnalyzeMacs.ReadAnalyzeUI ReadAnalyzeUI;
      XP_COMM_PROJ.ReadAnalyze.ReadAnalyzeMods.ReadAnalyzeFile ReadAnalyzeFile {
         filename_in => <-.ReadAnalyzeUI.filename_out;
      };

      GDM.DataObject DataObject {
         in => <-.ReadAnalyzeFile.out;
      };
      olink out_fld => .ReadAnalyzeFile.out;
      olink out_obj => .DataObject.obj;
   };

//************************************************************************

   //ReadAnalyzeEg

   APPS.SingleWindowApp ReadAnalyzeEg {

      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.orthoslice.out_obj,<-.<-.<-.orthoslice#1.out_obj,
                  <-.<-.<-.orthoslice#2.out_obj,<-.<-.<-.bounds.out_obj};
            };
         };
      };

      XP_COMM_PROJ.ReadAnalyze.ReadAnalyzeMacs.readAnalyze readAnalyze{
    	 ReadAnalyzeUI {
        	file_browser {
               dirMaskCache = "xp_comm_proj/rd_anlyz/*.hdr";
        	};
        	filename_out = "xp_comm_proj/rd_anlyz/head.hdr";
    	 };
	  };

      MODS.bounds bounds {
         in_field => <-.readAnalyze.out_fld;
      };
      MODS.orthoslice orthoslice {
         in_field => <-.readAnalyze.out_fld;
      };
      MODS.orthoslice orthoslice#1 {
         in_field => <-.readAnalyze.out_fld;
         OrthoSliceParam {
            axis = 1;
         };
      };
      MODS.orthoslice orthoslice#2 {
         in_field => <-.readAnalyze.out_fld;
         OrthoSliceParam {
            axis = 2;
            plane => 1;
         };
      };
   };


};
