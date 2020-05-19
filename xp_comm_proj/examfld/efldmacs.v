

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


flibrary Examine_FieldMacs {

      macro Examine_Field_UI<NExOffset=181.,NEyOffset=74.> {
         UItext+notify_inst examine_field_text<NEx=154.,NEy=132.> {
            parent => <-.UIframe;
            text<NEportLevels={2,2}> => <-.text_string;
            y = 0;
            width => .parent.clientWidth;
            rows<NEportLevels={2,0}> => <-.text_rows;
            outputOnly = 1;
            resizeToText = 0;
         };
         int text_rows<NEportLevels={1,1},NEx=264.,NEy=176.> => .Examine_Field_Param.text_rows;
         string text_string<NEportLevels={1,1},NEx=264.,NEy=231.> => .Examine_Field_Param.text_string;
         group &Examine_Field_Param<NEx=-44.,NEy=330.,NEportLevels={2,0}> {
            int text_rows<NEportLevels={2,2}>;
            string text_string<NEportLevels={2,2}>;
         };
         UImod_panel UImod_panel<NEx=-33.,NEy=0.> {
            x = 0;
            y = 0;
            title => "examine_field";
            width => .parent.clientWidth;
         };
         UIpanel UIpanel<NEx=-33.,NEy=44.> {
            parent => <-.UImod_panel;
            width => (.parent.clientWidth - 10);
            height => (<-.examine_field_text.height + 5);
            y = 5;
            x = 5;
         };
         UIframe UIframe<NEx=-33.,NEy=88.> {
            parent => <-.UIpanel;
            x = 0;
            y = 0;
            width => .parent.clientWidth;
            height => .parent.clientHeight;
         };
      };

      macro Examine_Field {
         XP_COMM_PROJ.Examine_Field.Examine_FieldMods.Examine_Field_Prim Examine_Field_Prim<NEx=363.,NEy=231.> {
            text_rows   => <-.Examine_Field_Param.text_rows;
            text_string => <-.Examine_Field_Param.text_string;
            data<NEportLevels={3,0}>;
         };
         XP_COMM_PROJ.Examine_Field.Examine_FieldMods.Examine_Field_Param &Examine_Field_Param<NEx=363.,NEy=55.,NEportLevels={2,0}>;
      };

      macro examine_field {
         XP_COMM_PROJ.Examine_Field.Examine_FieldMods.Examine_Field_Param Examine_Field_Param<NEx=561.,NEportLevels={0,1}> {
            text_rows   = 2;
            text_string = "";
         };
         XP_COMM_PROJ.Examine_Field.Examine_FieldMacs.Examine_Field Examine_Field<NEy=209.,NEx=264.> {
            Examine_Field_Prim {
               data<NEportLevels={4,0}>;
            };
            Examine_Field_Param => <-.Examine_Field_Param;
         };
         XP_COMM_PROJ.Examine_Field.Examine_FieldMacs.Examine_Field_UI Examine_Field_UI<NEx=429.,NEy=209.> {
            Examine_Field_Param => <-.Examine_Field_Param;
            UImod_panel {
               title => name_of(<-.<-.<-);
            };
         };
      };

      APPS.SingleWindowApp Examine_Field_Eg<NEdisplayMode="maximized"> {
	 UI {
	    shell {
	       x = 148;
	       y = 300;
	    };
	    Modules {
	       IUI {
		  optionList {
		     cmdList => {
			<-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
			<-.<-.<-.<-.examine_field.Examine_Field_UI.UImod_panel.option};
		     selectedItem = 1;
		  };
	       };
	    };
	 };
	 GDM.Uviewer3D Uviewer3D<NEx=603,NEy=453> {
	    Scene {
	       Top {
		  child_objs => {
		     <-.<-.<-.Read_Field.out_obj};
		  Xform {
		     ocenter = {3.27323,4.16378,
      2.86213};
		     dcenter = {1.06827,1.51046,
      0.8852};
		  };
	       };
	       View {
		  View {
		     trigger = 2;
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
	 group &group_ref<NEportLevels={1,0},NEx=198.,NEy=33.,NEdisplayMode="open",NEwidth=220.,NEheight=462.> => .Read_Field.field;
	 XP_COMM_PROJ.Examine_Field.Examine_FieldMacs.examine_field examine_field<NEx=33.,NEy=165.> {
	    Examine_Field {
	       Examine_Field_Prim {
		  data => <-.<-.<-.Read_Field.field;
	       };
	    };
	    Examine_Field_UI {
	       examine_field_text {
		  height = 318;
	       };
	       UImod_panel {
		  option {
		     set = 1;
		  };
	       };
	    };
	 };
      };
};
