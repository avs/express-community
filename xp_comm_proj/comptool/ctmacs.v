

// Copyright / Disclaimer :

// This software/documentation was produced as part of the INDEX project
// (Intelligent Data Extraction) which is funded under contract ESPRIT EP22745 of 
// the European Community. For further details see
// http://www.man.ac.uk/MVC/research/INDEX/Public/. 

// Copyright (c) June 1998, Manchester Visualisation Centre, UK. 
// All Rights Reserved.

// Permission to use, copy, modify and distribute this software and its 
// documentation is hereby granted without fee, provided that the above copyright
// notice and this permission notice appear in all copies of this software / 
// documentation.

// This software/documentation is provided with no warranty, express or implied, 
// including, without limitation, warrant of merchantability or fitness for a 
// particular purpose.


flibrary Compress_ToolMacs {

      macro compress_tool<NEx=308.,NEy=99.,NExOffset=461.,NEyOffset=240.> {
         XP_COMM_PROJ.Compress.CompressMacs.compress compress<NEx=-143.,NEy=22.,NEwidth=396.,NEheight=396.,NExOffset=576.,NEyOffset=268.> {
            Compress {
               Compress_Prim {
                  data => <-.<-.<-.data;
               };
            };
            macro Compress_UI<NEx=-165.,NEy=88.> {
               UIbutton compress_all<NEx=464.,NEy=253.> {
                  x => ((parent.clientWidth - .width) / 2);
                  parent => <-.UIframe;
                  width = 120;
                  y = 10;
                  do<NEportLevels={2,2}> => <-.Compress_Param.compress_all;
               };
               UIbutton uncompress_all<NEx=464.,NEy=297.> {
                  x => ((parent.clientWidth - .width) / 2);
                  parent => <-.UIframe;
                  width = 120;
                  y = 50;
                  do<NEportLevels={2,2}> => <-.Compress_Param.uncompress_all;
               };
               UIbutton process_nth<NEx=464.,NEy=341.> {
                  x => ((parent.clientWidth - .width) / 2);
                  parent => <-.UIframe;
                  width = 120;
                  y = 90;
                  label => "(un)compress no";
                  do<NEportLevels={2,2}> => <-.Compress_Param.process_nth;
               };
               UIfield nth<NEx=462.,NEy=385.> {
                  x => ((parent.clientWidth - .width) / 2);
                  parent => <-.UIframe;
                  width = 120;
                  y = 114;
                  value<NEportLevels={2,2}> => <-.Compress_Param.nth;
                  mode = "integer";
                  updateMode = 2;
                  nullString = "";
               };
               UIpanel UIpanel<NEx=154.,NEy=165.> {
                  x = 5;
                  y = 5;
                  width => (.parent.clientWidth - 10);
                  parent<NEportLevels={4,0}> => <-.<-.<-.UImod_panel;
                  height = 160;
               };
               UIframe UIframe<NEx=154.,NEy=209.> {
                  y = 0;
                  width => .parent.clientWidth;
                  parent => <-.UIpanel;
                  height => .parent.clientHeight;
               };
               XP_COMM_PROJ.Compress.CompressMods.Compress_Param &Compress_Param<NEx=156.,NEy=363.,NEportLevels={2,0}> => <-.Compress_Param;
            };
            Compress_Param {
               nth = 2;
            };
         };
         UImod_panel UImod_panel<NEx=-429.,NEy=-77.> {
            option {
               set = 1;
            };
            title => name_of(<-.<-);
         };
         XP_COMM_PROJ.Examine_Field.Examine_FieldMacs.examine_field examine_field<NEx=33.,NEy=22.> {
            Examine_Field {
               Examine_Field_Prim {
                  data => <-.<-.<-.data;
               };
            };
            macro Examine_Field_UI<NEx=429.,NEy=209.,NExOffset=181.,NEyOffset=74.> {
               UItext examine_field_text<NEx=154.,NEy=132.> {
                  y = 0;
                  width => .parent.clientWidth;
                  parent => <-.UIframe;
                  height = 318;
                  text<NEportLevels={2,2}> => <-.text_string;
                  rows<NEportLevels={2,0}> => <-.text_rows;
                  resizeToText = 0;
                  outputOnly = 1;
               };
               int text_rows<NEportLevels={1,1},NEx=264.,NEy=176.> => .Examine_Field_Param.text_rows;
               string text_string<NEportLevels={1,1},NEx=264.,NEy=231.> => .Examine_Field_Param.text_string;
               group &Examine_Field_Param<NEx=-44.,NEy=330.,NEportLevels={2,0}> {
                  int text_rows<NEportLevels={2,2}>;
                  string text_string<NEportLevels={2,2}>;
               } => <-.Examine_Field_Param;
               UIpanel UIpanel<NEx=-33.,NEy=44.,NEdisplayMode="open"> {
                  x = 5;
                  y => ((<-.<-.<-.compress.Compress_UI.UIpanel.height + <-.<-.<-.compress.Compress_UI.UIpanel.y) + 5);
                  width => (.parent.clientWidth - 10);
                  parent<NEportLevels={4,0}> => <-.<-.<-.UImod_panel;
                  height => (<-.examine_field_text.height + 5);
               };
               UIframe UIframe<NEx=-33.,NEy=88.> {
                  y = 0;
                  width => .parent.clientWidth;
                  parent => <-.UIpanel;
                  height => .parent.clientHeight;
               };
            };
         };
         link data<NEportLevels={2,1},NEx=-429.,NEy=-143.>;
      };

      APPS.SingleWindowApp Compress_Tool_Eg<NEdisplayMode="maximized"> {
	 UI {
	    shell {
	       x = 86;
	       y = 165;
	    };
	    Windows {
	       IUI {
		  panel {
		     height = 508;
		  };
	       };
	    };
	    Modules {
	       IUI {
		  optionList {
		     selectedItem = 1;
		  };
	       };
	    };
	 };
	 MODS.Read_Field Read_Field<NEx=33.,NEy=55.> {
	    read_field_ui {
	       file_browser {
		  x = 490;
		  y = 267;
		  width = 300;
		  height = 390;
		  ok = 1;
		  dirMaskCache = "$XP_ROOT/data/field/*.fld";
	       };
	       filename = "$XP_ROOT/data/field/bluntfin.fld";
	    };
	    DVread_field {
	       Mesh_Struct+Node_Data Output_Field {
		  node_data = {

		     {
			null_flag=0,,min=>
			cache(min_array(magnitude(.values),.null_flag,.null_value)),max=>
			cache(max_array(magnitude(.values),.null_flag,.null_value)),min_vec=>
			cache(min_array(.values,.null_flag,.null_value)),max_vec=>
			cache(max_array(.values,.null_flag,.null_value)),,,
		     },
		     ,,,
		  };
	       };
	    };
	 };
	 XP_COMM_PROJ.Compress_Tool.Compress_ToolMacs.compress_tool compress_tool<NEx=33.,NEy=121.> {
	    data => <-.Read_Field.field;
	 };
      };

};
