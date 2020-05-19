

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


flibrary CompressMacs {

      macro Compress_UI<NEx=396.,NEy=110.> {
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
            label = "(un)compress no";
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
            x      =  5;
            y      =  5;
            width  => (.parent.clientWidth - 10);
            parent => <-.UImod_panel;
            height => .parent.clientHeight;
         };
         UImod_panel UImod_panel<NEx=154.,NEy=121.> {
            x     =  0;
            y     =  0;
            width => .parent.clientWidth;
            title = "compress";
            option {
               set = 1;
            };
         };
         UIframe UIframe<NEx=154.,NEy=209.> {
            parent => <-.UIpanel;
            x = 0;
            y = 0;
            width => .parent.clientWidth;
            height => .parent.clientHeight;
         };
         XP_COMM_PROJ.Compress.CompressMods.Compress_Param &Compress_Param<NEx=156.,NEy=363.,NEportLevels={2,0}>;
      };

      macro Compress {
         XP_COMM_PROJ.Compress.CompressMods.Compress_Param &Compress_Param
         <NEx=143.,NEy=110.,NEportLevels={2,0}>;
         XP_COMM_PROJ.Compress.CompressMods.Compress_Prim Compress_Prim
         <NEx=143.,NEy=187.>
         {
            compress_all   => <-.Compress_Param.compress_all;
            uncompress_all => <-.Compress_Param.uncompress_all;
            process_nth    => <-.Compress_Param.process_nth;
            nth            => <-.Compress_Param.nth;
            data <NEportLevels={3,0}>;
         };
      };

      macro compress
      <NExOffset=538.,NEyOffset=188.>
      {
         XP_COMM_PROJ.Compress.CompressMacs.Compress Compress
         <NEx=-319.,NEy=88.>
         {
            Compress_Param => <-.Compress_Param;
            Compress_Prim {
               data<NEportLevels={4,0}>;
            };
         };
         XP_COMM_PROJ.Compress.CompressMacs.Compress_UI Compress_UI
         <NEx=-165.,NEy=88.>
         {
            Compress_Param => <-.Compress_Param;
            UImod_panel {
               title => name_of(<-.<-.<-);
            };
         };
         XP_COMM_PROJ.Compress.CompressMods.Compress_Param Compress_Param
         <NEx=0.,NEy=11.,NEportLevels={0,1}>;
      };

      APPS.SingleWindowApp Compress_Eg<NEdisplayMode="maximized"> {
	 UI {
	    shell {
	       x = 156;
	       y = 332;
	    };
	    Modules {
	       IUI {
		  optionList {
		     selectedItem = 1;
		  };
	       };
	    };
	 };
	 MODS.Read_Field Read_Field<NEx=33.,NEy=33.> {
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
                  node_data<NEdisplayMode="open"> = {
		     {
			null_flag=0,,min=>cache(min_array(magnitude(values),null_flag,null_value)),max=>
			cache(max_array(magnitude(values),null_flag,null_value)),min_vec=>
			cache(min_array(values,null_flag,null_value)),max_vec=>
			cache(max_array(values,null_flag,null_value)),,,
		     },
		     ,,,
		  };
	       };
	    };
	 };
	 group &group_ref<NEportLevels={1,0},NEx=198.,NEy=33.,NEdisplayMode="open",NEwidth=220.,NEheight=462.> => .Read_Field.field;
         XP_COMM_PROJ.Compress.CompressMacs.compress compress<NEx=33.,NEy=121.> {
	    Compress {
	       Compress_Prim {
		  data => <-.<-.<-.Read_Field.field;
	       };
	    };
	    Compress_Param {
	       nth = 2;
	    };
	 };
      };

};
