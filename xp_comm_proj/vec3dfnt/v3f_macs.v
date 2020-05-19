flibrary Vector3dFontsMacs <compile_subs=0> {

	// Vector3dFontsFunc - Functional Macro

	macro Vector3dFontsFunc {
		XP_COMM_PROJ.Vector3dFonts.Vector3dFontsMods.Vector3dFontsCore Vector3dFontsCore {
    		Label => <-.params.label;
      		Kerning => <-.params.kerning;
      		Stroke_Width => <-.params.stroke_width;
      		Font => <-.params.font;
      		LineStyle => <-.params.linestyle;
   		};
   		XP_COMM_PROJ.Vector3dFonts.Vector3dFontsMods.Vector3dFontsParams+IPort2 &params;
   		olink out => .Vector3dFontsCore.out;
	};


	// Vector3dFontsUI - User Interface

	macro Vector3dFontsUI {
   		UImod_panel UImod_panel {
      		title => name_of(<-.<-.<-,1);
   		};
   		UIlabel UIlabel {
      		parent => <-.UImod_panel;
      		label => "Label String";
      		y = 0;
      		alignment = "left";
			width => parent.clientWidth;
   		};
   		UItext UItext {
      		parent => <-.UImod_panel;
      		y => <-.UIlabel.y + <-.UIlabel.height + 5;
      		text => <-.params.label;
			width => parent.clientWidth;
   		};
   		UIslider UIslider {
    	  	parent => <-.UImod_panel;
      		title => "Kerning";
      		y => <-.UItext.y + <-.UItext.height + 10;
      		min = 0.;
      		max = 3.;
      		value => <-.params.kerning;
			width => parent.clientWidth;
   		};
   		UIslider UIslider#1 {
      		parent => <-.UImod_panel;
      		title => "Stroke Width";
      		y => <-.UIslider.y + <-.UIslider.height + 10;
      		min = 0.1;
      		max = 1.;
      		value => <-.params.stroke_width;
			width => parent.clientWidth;
   		};
		UIlabel UIlabel#1 {
      		parent => <-.UImod_panel;
      		label => "Font Name";
      		y => <-.UIslider#1.y + <-.UIslider#1.height + 10;
      		alignment = "left";
			width => parent.clientWidth;
   		};
   		UIradioBox UIradioBox {
      		parent => <-.UImod_panel;
      		cmdList => {
         		<-.UIoption, <-.UIoption#1, <-.UIoption#2, <-.UIoption#3, <-.UIoption#4, <-.UIoption#5, <-.UIoption#6, <-.UIoption#7, <-.UIoption#8, <-.UIoption#9, <-.UIoption#10, <-.UIoption#11,<-.UIoption#12
      		};
	  		width => parent.clientWidth;
      		selectedItem => <-.params.font;
      		y => <-.UIlabel#1.y + <-.UIlabel#1.height + 5;
   		};
   		UIoption UIoption {
      		label => "Roman Plain";
   		};
   		UIoption UIoption#1 {
      		label => "Roman Simplex";
   		};
   		UIoption UIoption#2 {
      		label => "Roman Duplex";
   		};
   		UIoption UIoption#3 {
      		label => "Roman Triplex";
   		};
   		UIoption UIoption#4 {
      		label => "Roman Complex";
   		};
   		UIoption UIoption#5 {
      		label => "Script Simplex";
   		};
   		UIoption UIoption#6 {
      		label => "Script Complex";
   		};
   		UIoption UIoption#7 {
      		label => "Italics Complex";
   		};
   		UIoption UIoption#8 {
      		label => "Italics Triplex";
   		};
   		UIoption UIoption#9 {
      		label => "Gothic English";
   		};
   		UIoption UIoption#10 {
      		label => "Greek Plain";
   		};
   		UIoption UIoption#11 {
      		label => "Greek Simple";
   		};
   		UIoption UIoption#12 {
      		label => "Greek Complex";
   		};
   		UIlabel UIlabel#2 {
      		parent => <-.UImod_panel;
      		label => "Character Style";
      		y => <-.UIradioBox.y + <-.UIradioBox.height + 10;
      		alignment = "left";
			width => parent.clientWidth;
   		};
   		UIradioBox UIradioBox#1 {
      		parent => <-.UImod_panel;
      		cmdList => { <-.LineUIoption#2, <-.LineUIoption#3, <-.LineUIoption#4};
	  		width => parent.clientWidth;
      		selectedItem => <-.params.linestyle;
      		y => <-.UIlabel#2.y + <-.UIlabel#2.height + 5;
  		};   
   		UIoption LineUIoption#2 {
      		label => "Lines";
   		};
   		UIoption LineUIoption#3 {
      		label => "Stroke Lines";
   		};
   		UIoption LineUIoption#4 {
      		label => "3D Extrusions";
   		};
   		XP_COMM_PROJ.Vector3dFonts.Vector3dFontsMods.Vector3dFontsParams+IPort2 &params;
	};

	
	// Vector3dFonts - User Macro

	macro Vector3dFonts {
   		XP_COMM_PROJ.Vector3dFonts.Vector3dFontsMacs.Vector3dFontsFunc Vector3dFontsFunc {
      		params => <-.params;
   		};
   		XP_COMM_PROJ.Vector3dFonts.Vector3dFontsMacs.Vector3dFontsUI Vector3dFontsUI {
      		params => <-.params;
   		};
   		XP_COMM_PROJ.Vector3dFonts.Vector3dFontsMods.Vector3dFontsParams+OPort params {
      		label+IPort3 = "";
      		kerning = 1.;
      		stroke_width = 0.25;
      		font = 0;
      		linestyle = 0;
   		};
   		GDM.DataObject DataObject {
      		in => <-.Vector3dFontsFunc.out;
   		};
   		olink out=> .Vector3dFontsFunc.out;
   		olink obj=> .DataObject.obj;
	};
	
};
