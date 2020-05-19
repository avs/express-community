macro PickAtoms<NEhelpContextID=1,NEhelpFile="STM3/PickAtoms.html"> {
   link molecule_in<NEportLevels={2,1},NEx=77.,NEy=88.>;
   link picked_obj<NEportLevels={2,1},NEx=77.,NEy=44.>;
   XP_COMM_PROJ.STM3.UTILS.Picker Picker<NEx=231.,NEy=319.> {
      in => <-.molecule_in;
      mode => <-.operation.selectedItem;
      picked_obj => <-.picked_obj;
      line_conn[];
   };
   MODS.text_glyph text_glyph<NEx=198.,NEy=462.> {
      in_mesh => <-.point_mesh.out;
      in_text => {<-.Picker.label};
      UIpanel UImod_panel {
         parent<NEportLevels={3,0}> => <-.<-.UImod_panel;
         width => parent.clientWidth;
         height = 950;
         y = 320;
      };
      TextUI {
         Font {
            text = "-adobe-times-*-r-*-*-20-*-*-*-*-*-*-*";
         };
         AlignHorizMenu {
            selectedItem = 1;
            x = 0;
         };
         AlignVertMenu {
            selectedItem = 1;
            x = 0;
         };
         Background {
            set = 1;
         };
         UIoption#3 {
            set = 0;
         };
         Leadline {
            set = 1;
         };
         Offset {
            set = 1;
         };
         OffsetXValue {
            field {
               value = 0.;
            };
         };
         OffsetYValue {
            field {
               value = 0.06;
            };
         };
         OffsetZValue {
            field {
               value = 0.20;
            };
         };
         UIoption#4 {
            set = 1;
         };
         Bounds {
            set = 1;
         };
         UIframe {
            y => ((<-.UIlabel.y + <-.UIlabel.height) + 14);
         };
      };
      obj {
         Props {
            col = {0.5,0.,0.3};
         };
         Obj {
            name => "pick label";
            pickable = 0;
         };
         AltObject {
            Obj {
               visible = 0;
            };
         };
      };
      out_obj<NEportLevels={1,2}>;
   };
   FLD_MAP.point_mesh point_mesh<NEx=204.,NEy=394.> {
      coord => <-.Picker.label_pt;
   };
   FLD_MAP.polyline_mesh polyline_mesh<NEx=341.,NEy=462.> {
      coord => <-.Picker.line_pt;
      connect => <-.Picker.line_conn;
      DataObject {
         Obj {
            pickable = 0;
            name => "measurement guides";
         };
         Props {
            line_aa = 1;
            inherit = 0;
            line_width = 4;
         };
      };
   };
   GDM.GroupObject GroupObject<NEx=341.,NEy=550.> {
      child_objs => {<-.polyline_mesh.obj,<-.text_glyph.out_obj};
      Top {
         name => "measurement tool";
      };
   };
   link label_obj<NEportLevels={1,2},NEx=341.,NEy=605.> => .GroupObject.obj;
   UImod_panel UImod_panel<NEx=550.,NEy=22.> {
      title => name_of(<-.<-,1);
   };
   UIlabel title1<NEx=682.,NEy=77.> {
      parent => <-.UImod_panel;
      label => "Select pick operation";
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UIoption disabled<NEx=594.,NEy=143.> {
      set = 1;
   };
   UIoption atom_info<NEx=594.,NEy=176.>;
   UIoption distance<NEx=594.,NEy=209.>;
   UIoption angle<NEx=594.,NEy=242.>;
   UIoption torsion<NEx=594.,NEy=275.>;
   UIradioBox operation<NEx=759.,NEy=308.> {
      parent => <-.UImod_panel;
      cmdList => {<-.disabled,<-.atom_info,<-.distance,<-.angle,<-.torsion};
      selectedItem = 0;
      y = 30;
      x = 10;
   };
   UIlabel title2<NEx=682.,NEy=374.> {
      parent => <-.UImod_panel;
      label => "Label format";
      y = 290;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
	UIlabel title3<NEx=682.,NEy=440.> {
   	parent => <-.UImod_panel;
   	label => "Pick result";
   	y = 160;
   	width => parent.clientWidth;
   	color {
      	foregroundColor = "white";
      	backgroundColor = "blue";
   	};
	};
	UItext measure<NEx=682.,NEy=495.> {
   	parent => <-.UImod_panel;
   	text<NEportLevels={2,2}> => <-.Picker.extended_label;
   	y = 190;
   	width => parent.clientWidth;
   	height = 90;
   	rows = 5;
   	multiLine = 1;
   	outputOnly = 1;
	};
};
