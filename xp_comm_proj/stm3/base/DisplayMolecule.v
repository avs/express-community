macro DisplayMolecule<NEhelpContextID=1,NEhelpFile="STM3/DisplayMolecule.html"> {
  XP_COMM_PROJ.STM3.BASE.MolecularDataType &molecule_in<NEx=55.,NEy=11.,NEportLevels={2,1}>;
  UImod_panel UImod_panel<NEx=11.,NEy=55.> {
     title => name_of(<-.<-,1);
  };
  UIlabel title<NEx=154.,NEy=110.> {
     parent => <-.UImod_panel;
     label => "Visibility and quality";
     y = 0;
     width => parent.clientWidth;
     color {
        foregroundColor = "white";
        backgroundColor = "blue";
     };
  };
  UIoptionBox visibility<NEx=154.,NEy=341.> {
     parent => <-.UImod_panel;
     cmdList => {<-.atoms,<-.bonds,<-.atom_names,<-.residue_names};
     selectedItems = {0,1};
     y = 35;
     x = 13;
     width = 150;
  };
  UIoption atoms<NEx=154.,NEy=154.> {
     set = 1;
  };
  UIoption bonds<NEx=154.,NEy=198.> {
     set = 1;
  };
  UIoption atom_names<NEx=154.,NEy=242.>;
  UIoption residue_names<NEx=154.,NEy=286.>;
  UItoggle FastRot<NEx=154.,NEy=385.> {
     parent => <-.UImod_panel;
     y = 35;
     x = 164;
     width = 86;
     label => "Fast Rot";
  };
  UIoption High_Res<NEx=154.,NEy=429.>;
  UIoption Mid_Res<NEx=154.,NEy=462.>;
  UIoption Low_Res<NEx=154.,NEy=495.>;
  UIradioBox resolution_rb<NEx=154.,NEy=539.> {
      parent => <-.UImod_panel;
      cmdList => {<-.High_Res,<-.Mid_Res,<-.Low_Res};
      selectedItem = 0;
      y = 60;
      x = 164;
  };
  int resolution<NEportLevels={0,1},NEx=154.,NEy=572.> => switch(resolution_rb.selectedItem+1,18,10,3);
  UItoggle single_res_name<NEx=154.,NEy=616.> {
      parent => <-.UImod_panel;
      y = 131;
      x = 11;
      width = 140;
		set = 0;
		active => <-.residue_names.set;
  };
  UIlabel title2<NEx=363.,NEy=110.> {
     parent => <-.UImod_panel;
     label => "Visualization format";
     y = 160;
     width => parent.clientWidth;
     color {
        foregroundColor = "white";
        backgroundColor = "blue";
     };
  };
  UIlist representation<NEx=363.,NEy=154.> {
     selectedText = "Ball and Stick";
     strings => {
        "Ball and Stick","CPK","Colored Stick","Colored Residue","Stick",
        "Licorice", "Licorice Stick",
		  "Ball and Stick (charge)", "CPK (charge)", "Licorice (charge)"
     };
     width => parent.clientWidth;
     parent => <-.UImod_panel;
     height => 130;
     selectedItem<NEportLevels={0,2}> = 0;
     y = 187;
  };
  UIlabel title3<NEx=550.,NEy=110.> {
     parent => <-.UImod_panel;
     label => "Bonds format";
     y = 320;
     width => parent.clientWidth;
     color {
        foregroundColor = "white";
        backgroundColor = "blue";
     };
  };
  UIradioBox bonds_representation<NEx=550.,NEy=308.> {
     parent => <-.UImod_panel;
     cmdList => {<-.option_l,<-.option_t,<-.option_h,<-.option_hb};
     selectedItem = 3;
     y = 355;
     x = 10;
     width = 150;
  };
  UIoption option_l<NEx=550.,NEy=154.> {
     label => "line";
  };
  UIoption option_t<NEx=550.,NEy=187.> {
     label => "tube";
  };
  UIoption option_h<NEx=550.,NEy=220.> {
     label => "all H as lines";
  };
  UIoption option_hb<NEx=550.,NEy=253.> {
     label => "H-bonds as lines";
  };
  UItoggle split_toggle<NEx=550.,NEy=352.> {
		parent => <-.UImod_panel;
		label => "Split color";
		x = 160;
		y = 355;
		set => <-.DrawBonds.split;
  };
  XP_COMM_PROJ.STM3.BASE.SummaryResidueNames SummResNames<NEx=715.,NEy=363.> {
     in => <-.molecule_in;
	  summarize => switch(<-.single_res_name.set,1);
  };
  Mesh point_mesh_res<NEportLevels={0,1},NEx=715.,NEy=418.> {
     nnodes<NEportLevels={2,0}> => switch(<-.single_res_name.set+1,<-.molecule_in.num_atoms,array_size(<-.SummResNames.coords)/3);
     nspace = 3;
     coordinates {
        values<NEportLevels={3,0}> => switch(<-.<-.single_res_name.set+1,<-.<-.molecule_in.xyz_lst,<-.<-.SummResNames.coords);
     };
     ncell_sets = 1;
     Point cell_set {
        ncells => <-.nnodes;
        node_connect_list => init_array(ncells,0,(ncells - 1));
     };
  };
  macro Options<NEx=770.,NEy=110.> {
		UIbutton open_options<NEx=55.,NEy=44.> {
		  parent<NEportLevels={3,0}> => <-.<-.UImod_panel;
		  do = 0;
		  y = 460;
		  label = "Open display parameter settings";
		  width => parent.clientWidth;
		};
		GMOD.instancer instancer<NEx=209.,NEy=88.> {
		  Value => <-.open_options.do;
		  Group => <-.OptionWindow;
		  active = 2;
		};
		UIshell OptionWindow<NEx=55.,NEy=88.,instanced=0> {
		  title = "Set molecule display options";
		  visible => <-.open_options.do;
		  height = 540;
		  width = 792;
		  showStatusBar = 0;
		  x = 100;
		  y = 100;
		};
		UIscrolledWindow ScrolledOptionWindow<NEx=55.,NEy=220.> {
   		parent => <-.OptionWindow;
   		x = 0;
   		y = 30;
			width => parent.clientWidth;
			height => parent.clientHeight - 70;
			virtualWidth = 755;
			virtualHeight = 950;
		};
		UIbutton close_options<NEx=55.,NEy=132.> {
		  parent => <-.OptionWindow;
		  y => <-.OptionWindow.clientHeight-37;
		  x => (parent.clientWidth - .width)/2;
		  height = 35;
		  label = "Close";
		};
		GMOD.parse_v do_close_options<NEx=55.,NEy=176.> {
		  v_commands = "open_options.do=0;";
		  trigger => <-.close_options.do;
		  relative => <-;
		};
		UIlabel title_a<NEx=253.,NEy=231.> {
		  parent => <-.OptionWindow;
		  label => "Atom name format";
		  x = 3;
		  y = 6;
		  width = 250;
		  color {
   		  foregroundColor = "white";
   		  backgroundColor = "blue";
		  };
		};
		UIlabel title_r<NEx=418.,NEy=231.> {
		  parent => <-.OptionWindow;
		  label => "Residue name format";
		  x = 255;
		  y = 6;
		  width = 250;
		  color {
   		  foregroundColor = "white";
   		  backgroundColor = "blue";
		  };
		};
 		UIlabel title_color<NEx=583.,NEy=231.> {
		  parent => <-.OptionWindow;
		  label => "Default color and dimensions";
		  x = 507;
		  y = 6;
		  width = 247;
		  color {
   		  foregroundColor = "white";
   		  backgroundColor = "blue";
		  };
		};
		macro ColorEditor<NEx=583.,NEy=297.> {
   		UIpanel panel<NEx=55.,NEy=66.>
			{
				parent<NEportLevels={3,1}> => <-.<-.ScrolledOptionWindow;
				x = 505;
				y = 0;
				width = 250;
				height = 205;
			};
   		GMOD.hsv_to_rgb HSVToRGB<NEx=209,NEy=297> {
      		h => <-.Dials.HDial.value;
      		s => <-.Dials.SDial.value;
      		v => <-.Dials.VDial.value;
   		};
   		VUIColorPanel ColorEcho {
      		parent => <-.panel;
      		x      =  4;
      		y      => 2;
      		width  => parent.clientWidth - 8;
      		height => 30;
      		color  => { <-.HSVToRGB.r, <-.HSVToRGB.g, <-.HSVToRGB.b };
   		};
   		group Dials {
      		UIframe DialFrame<NEx=341,NEy=231> {
         		parent => <-.<-.panel;
         		x      =  4;
         		y      => <-.<-.ColorEcho.y + <-.<-.ColorEcho.height;
         		width  => parent.clientWidth - 8;
         		height = 170;
      		};

      		int tw => <-.panel.clientWidth - 8;
      		int w3 => (tw-16) / 3;

      		VUIDial HDial<NEx=77,NEy=231> {
         		parent  => <-.DialFrame;
         		x       =  0;
         		y       =  0;
         		width   => <-.w3;
         		height  => <-.DialFrame.clientHeight;
         		title   = "Hue";
         		min     = 0.0;
         		max     = 1.0;
         		value<NEportLevels={2,2}> = 0.;
         		immediate<NEportLevels={2,1}> = 1;
      		};
      		HDial SDial<NEx=231,NEy=231,NEvisible=1> {
         		x     => <-.HDial.x + <-.HDial.width + 4;
         		title = "Saturation";
         		value<NEportLevels={2,2}> = 0.;
      		};
      		HDial VDial<NEx=385,NEy=231,NEvisible=1> {
         		x     => <-.SDial.x + <-.SDial.width + 4;
         		title = "Value";
         		value<NEportLevels={2,2}> = 0.4;
      		};
   		};
			float color<NEportLevels={0,3},NEx=176.,NEy=462.>[] => {HSVToRGB.r,HSVToRGB.g,HSVToRGB.b};
		};
		UIlabel bond_radius_lbl<NEx=737.,NEy=231.> {
   		parent => <-.ScrolledOptionWindow;
   		label = "Bond radius:";
   		y = 220;
   		x = 510;
   		alignment = "left";
		};
		UIfield bond_radius<NEx=880.,NEy=231.> {
   		parent => <-.ScrolledOptionWindow;
   		value<NEportLevels={2,3}> = 0.1;
   		x = 615;
   		y = 218;
   		min = 0.001;
   		decimalPoints = 2;
   		updateMode = 7;
		};
		UIlabel atom_scale_lbl<NEx=737.,NEy=275.> {
   		parent => <-.ScrolledOptionWindow;
   		label => "Atom scale:";
   		y = 250;
   		x = 510;
   		alignment = "left";
		};
		UIfield atom_scale<NEx=880.,NEy=275.> {
   		parent => <-.ScrolledOptionWindow;
   		value<NEportLevels={2,3}> = 0.2;
   		x = 615;
   		y = 248;
   		min = 0.001;
   		decimalPoints = 2;
   		updateMode = 7;
		};
  };
  macro charge_limits<NEx=913.,NEy=110.> {
   	UIpanel UIpanel<NEx=55.,NEy=44.> {
      	visible<NEportLevels={3,0}> => (<-.<-.representation.selectedItem > 6);
      	parent<NEportLevels={3,0}> => <-.<-.UImod_panel;
      	y = 490;
      	width => parent.clientWidth;
   	};
   	UIlabel UIlabel<NEx=275.,NEy=132.> {
      	parent => <-.UIpanel;
      	label => "Charge value limits";
      	y = 0;
      	width => parent.clientWidth;
      	color {
         	foregroundColor = "white";
         	backgroundColor = "blue";
      	};
   	};
   	UItoggle use<NEx=275.,NEy=198.> {
      	parent => <-.UIpanel;
      	label => "use manual limits";
      	set<NEportLevels={0,3}> => <-.<-.DrawAtoms.use_manual_limits;
      	y = 24;
      	width = 200;
   	};
   	UIlabel min<NEx=429.,NEy=286.> {
      	parent => <-.UIpanel;
      	y = 50;
      	width => (parent.clientWidth / 2);
   	};
   	UIlabel max<NEx=583.,NEy=286.> {
      	parent => <-.UIpanel;
      	y = 50;
      	width => (parent.clientWidth / 2);
      	x => (parent.clientWidth / 2);
   	};
   	UIfield cmin<NEx=429.,NEy=330.> {
      	parent => <-.UIpanel;
      	value<NEportLevels={2,3}> => <-.<-.DrawAtoms.charge_manual_limits[0];
      	y = 74;
      	width => (parent.clientWidth / 2);
			decimalPoints = 4;
			updateMode = 7;
   	};
   	UIfield cmax<NEx=583.,NEy=330.> {
      	parent => <-.UIpanel;
      	y = 74;
      	width => (parent.clientWidth / 2);
      	value<NEportLevels={2,3}> => <-.<-.DrawAtoms.charge_manual_limits[1];
      	x => (parent.clientWidth / 2);
			decimalPoints = 4;
			updateMode = 7;
   	};
  };
  MODS.text_glyph residue_name_text<NEx=704.,NEy=484.> {
     in_text => switch(<-.single_res_name.set+1,<-.molecule_in.residue_name,<-.SummResNames.names);
     in_mesh => <-.point_mesh_res;
     UIpanel UImod_panel {
        parent<NEportLevels={3,0}> => <-.<-.Options.ScrolledOptionWindow;
        height = 950;
        y = 0;
		  x = 255;
		  width = 250;
     };
     TextUI {
        Font {
           text = "-adobe-times-*-r-*-*-14-*-*-*-*-*-*-*";
        };
        AlignHorizMenu {
           selectedItem = 1;
           x = 0;
        };
        UIoption {
           set = 0;
        };
        UIoption#1 {
           set = 1;
        };
        AlignVertMenu {
           selectedItem = 2;
           x = 0;
        };
        UIoption#3 {
           set = 0;
        };
        UIoption#5 {
           set = 1;
        };
        Offset {
           set = 1;
        };
        OffsetXValue {
           field {
              value = 0.06;
           };
        };
        OffsetYValue {
           field {
              value = -0.01;
           };
        };
     };
     obj {
        Props {
           col = {0.,1.,1.};
        };
        Obj {
           visible => <-.<-.<-.residue_names.set;
           name => "residue names";
           use_altobj => <-.<-.<-.FastRot.set;
        };
        AltObject {
           Obj {
              visible = 0;
           };
        };
     };
     out_obj<NEportLevels={1,2}>;
  };
  MODS.text_glyph atom_name_text<NEx=550.,NEy=484.> {
     in_text => <-.molecule_in.atom_name;
     in_mesh => <-.point_mesh;
     UIpanel UImod_panel {
        parent<NEportLevels={3,0}> => <-.<-.Options.ScrolledOptionWindow;
        height = 950;
		  x = 3;
        y = 0;
		  width = 250;
     };
     TextUI {
        Font {
           text = "-adobe-times-bold-r-*-*-14-*-*-*-*-*-*-*";
        };
        AlignHorizMenu {
           selectedItem = 1;
           x = 0;
        };
        AlignVertMenu {
           selectedItem = 2;
           x = 0;
        };
        UIoption#3 {
           set = 0;
        };
        UIoption#5 {
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
              value = 0.;
           };
        };
        OffsetZValue {
           field {
              value = 0.15;
           };
        };
        UIoption#1 {
           set = 1;
        };
     };
     obj {
        Obj {
           visible => <-.<-.<-.atom_names.set;
           name => "atom names";
           use_altobj => <-.<-.<-.FastRot.set;
        };
        AltObject {
           Obj {
              visible = 0;
           };
        };
        Props {
           inherit = 0;
        };
        Modes {
           mode = {1,0,0,0,0};
        };
     };
     out_obj<NEportLevels={1,2}>;
  };
#ifdef VIZ_EXPRESS
  GDM.DataObjectLite bonds_obj<NEx=869.,NEy=484.> {
     in => <-.DrawBonds.out;
     Obj {
        name => "bonds";
        visible => <-.<-.bonds.set;
        cache_size => cache(switch(is_valid(getenv("CACHE_SIZE"))+1, 64, getenv("CACHE_SIZE")));
        pickable = 0;
        use_altobj => <-.<-.FastRot.set;
     };
     Props {
        line_style => switch(((<-.<-.DrawBonds.lineflag == 0) + 1),"Dashed","Solid");
        inherit = 0;
        line_aa = 1;
        line_width = 2;
        material = {0.3,0.4,0.4,50.};
		  cull => switch(<-.<-.representation.selectedItem + 1, "Back", "Back", "Normal", "Normal", "Normal", "Back");
     };
     Modes {
        mode = {0,2,0,0,1};
        outline = "Off";
     };
     AltObject {
        AltModes {
           mode = {1,1,1,1,1};
           normals = "None";
        };
     };
  };
#else
	macro bonds_obj<NEx=869.,NEy=484.,locked=0> {
   	group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
      	GDxform_templ &xform;
         method render;
   	} => <-.DrawBonds.out;
   	DefaultProps Props<NEy=198,NEx=297,export_all=1> {
      	material = {0.3,0.4,0.4,50.};
      	line_width = 2;
      	line_style => switch(((<-.<-.DrawBonds.lineflag == 0) + 1),"Dashed","Solid");
      	line_aa = 1;
      	inherit = 0;
		   cull => switch(<-.<-.representation.selectedItem + 1, "Back", "Back", "Normal", "Normal", "Normal", "Back");
   	};
   	DefaultModes Modes<NEy=165,NEx=341,export_all=1> {
      	mode = {0,2,0,0,1};
      	outline = "Off";
   	};
   	DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
   	AltObject AltObject<instanced=0,export_all=1> {
      	alt_in => in;
      	AltModes {
         	mode = {1,1,1,1,1};
         	normals = "None";
      	};
   	};
   	DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
      	input => in;
      	xform => in.xform;
      	props => Props;
      	modes => Modes;
      	use_altobj => <-.<-.FastRot.set;
      	altobj => AltObject.obj;
      	visible => <-.<-.bonds.set;
      	cache_size => cache(switch(is_valid(getenv("CACHE_SIZE"))+1, 64, getenv("CACHE_SIZE")));
      	name = "bonds";
      	pick_info => PickInfo;
      	pickable = 0;
   	};
   	GMOD.instancer instancer {
      	Value => Obj.use_altobj;
      	Group => AltObject;
   	};
   	olink obj<NEy=385,NEx=341> => Obj;
	};
#endif
  XP_COMM_PROJ.STM3.BASE.DrawAtoms DrawAtoms<NEx=363.,NEy=418.> {
     in => <-.molecule_in;
     representation => <-.representation.selectedItem;
	  default_colors => <-.Options.ColorEditor.color;
	  licorice_scale => <-.Options.bond_radius.value;
	  normal_scale => <-.Options.atom_scale.value;
  };
#ifdef VIZ_EXPRESS
  GDM.DataObjectLite atoms_obj<NEx=363.,NEy=484.> {
     in => <-.DrawAtoms.fld;
     Props {
        material = {0.3,0.4,0.4,50.};
        inherit = 0;
        subdiv<NEportLevels={3,0}> => <-.<-.resolution;
		  col => <-.<-.bonds_obj.Props.col;
     };
	  Modes {
	     normals = "None";
	  };
     Obj {
        cache_size => cache(switch(is_valid(getenv("CACHE_SIZE"))+1, 64, getenv("CACHE_SIZE")));
        name => "atoms";
        use_altobj => FastRot.set;
        visible => <-.<-.atoms.set;
     };
     AltObject {
        AltModes {
           mode = {2,1,1,1,1};
	        normals = "None";
        };
     };
  };
#else
	macro atoms_obj<NEx=363.,NEy=484.,locked=0> {
   	group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
      	GDxform_templ &xform;
         method render;
   	} => <-.DrawAtoms.fld;
   	DefaultProps Props<NEy=66.,NEx=330.,export_all=1> {
      	material = {0.3,0.4,0.4,50.};
      	subdiv<NEportLevels={3,0}> => <-.<-.resolution;
      	inherit = 0;
		   col => <-.<-.bonds_obj.Props.col;
   	};
   	DefaultModes Modes<NEy=99.,NEx=319.,export_all=1> {
	      normals = "None";
		};
   	DefaultPickInfo PickInfo<NEy=110.,NEx=704.,export_all=1>;
   	AltObject AltObject<instanced=0,export_all=1,NEx=495.,NEy=242.> {
      	alt_in => in;
      	AltModes {
         	mode = {2,1,1,1,1};
	         normals = "None";
      	};
   	};
   	DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
      	input => in;
      	xform => in.xform;
      	props => Props;
      	modes => Modes;
      	use_altobj => FastRot.set;
      	altobj => AltObject.obj;
      	visible => <-.<-.atoms.set;
         cache_size => cache(switch(is_valid(getenv("CACHE_SIZE"))+1, 64, getenv("CACHE_SIZE")));
      	name = "atoms";
      	pick_info => PickInfo;
   	};
   	GMOD.instancer instancer<NEx=561.,NEy=187.> {
      	Value => Obj.use_altobj;
      	Group => AltObject;
   	};
   	olink obj<NEy=385.,NEx=396.> => Obj;
	};
#endif
  XP_COMM_PROJ.STM3.BASE.DrawBonds DrawBonds<NEx=869.,NEy=418.> {
     in => <-.molecule_in;
     lineflag => <-.bonds_representation.selectedItem;
     representation => <-.representation.selectedItem;
     facets => <-.resolution;
	  default_colors => <-.Options.ColorEditor.color;
	  bondradius => <-.Options.bond_radius.value;
	  charge_manual_limits => <-.DrawAtoms.charge_manual_limits;
  };
  Mesh point_mesh<NEportLevels={0,1},NEx=550.,NEy=418.> {
     nnodes<NEportLevels={2,0}> => <-.molecule_in.num_atoms;
     nspace = 3;
     coordinates {
        values<NEportLevels={3,0}> => <-.<-.molecule_in.xyz_lst;
     };
     ncell_sets = 1;
     Point cell_set {
        ncells => <-.nnodes;
        node_connect_list => init_array(ncells,0,(ncells - 1));
     };
  };
#ifdef VIZ_EXPRESS
  GDM.GroupObject Group<NEx=726.,NEy=561.> {
     child_objs => {
	   				<-.atoms_obj.obj,
						<-.bonds_obj.obj,
						<-.atom_name_text.out_obj,
						<-.residue_name_text.out_obj
	  };
     Top {
		  name = "molecule";
	 };
  };
#else
	macro Group<NEx=726.,NEy=561.,locked=0> {
   	imlink child_objs<NEy=99,NEx=209> => {
		   <-.atoms_obj.obj,
      	<-.bonds_obj.obj,
			<-.atom_name_text.out_obj,
			<-.residue_name_text.out_obj
		};
   	DefaultObject Top<NEportLevels={0,1},NEy=352,NEx=77> {
      	objects => child_objs;
      	name = "molecule";
   	};
   	olink obj<NEy=352.,NEx=363.> => Top;
	};
#endif
   link obj<NEportLevels={1,2},NEx=572.,NEy=627.> => .Group.obj;
   XP_COMM_PROJ.STM3.BASE.DisplayParams display_params<NEx=814.,NEy=627.,NEportLevels={0,2}> {
		num_atoms => <-.molecule_in.num_atoms;
		atom_z => <-.molecule_in.atom_z;
		residue_name => <-.molecule_in.residue_name;
		charge_min => <-.DrawAtoms.charge_manual_limits[0];
		charge_max => <-.DrawAtoms.charge_manual_limits[1];
		atoms_representation => <-.representation.selectedItem;
		bonds_representation => <-.bonds_representation.selectedItem;
		bonds_split_color => <-.DrawBonds.split;
		default_colors => <-.Options.ColorEditor.color;
		licorice_scale => <-.Options.bond_radius.value;
		normal_scale => <-.Options.atom_scale.value;
		cpk_scale => <-.DrawAtoms.cpk_scale;
	};
};
