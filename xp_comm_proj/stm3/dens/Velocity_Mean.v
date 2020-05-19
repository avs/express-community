macro Velocity_Mean<NEhelpContextID=1,NEhelpFile="STM3/Velocity_Mean.html"> {
	XP_COMM_PROJ.STM3.BASE.MolecularDataType &molecule<NEportLevels={2,1},NEx=50.,NEy=14.>;
   UImod_panel UImod_panel<NEx=638.,NEy=33.> {
      title => name_of(<-.<-,1);
   };
   XP_COMM_PROJ.STM3.UTILS.Select_Atoms Select_Atoms<NEx=253.,NEy=132.> {
      in => <-.molecule;
      UIpanel UImod_panel;
  	  UImod_panel {
	 	 parent => <-.<-.UImod_panel;
	     width => parent.clientWidth;	
         y = 0;
         height = 343;
      };
      SelectAtoms {
         atom_names[];
      };
      UIradioBox {
         selectedItem = 2;
      };
   };
   UIlabel title<NEx=748.,NEy=198.> {
      parent => <-.UImod_panel;
      label => "Select operation";
      y = 345;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UItoggle use_cubic_cells<NEx=605.,NEy=264.> {
      parent => <-.UImod_panel;
      y = 370;
      width = 200;
      set => <-.VelocityMean.use_cubic_slots;
   };
   UIlabel size_label<NEx=605.,NEy=308.> {
      parent => <-.UImod_panel;
      label => "Cells per side:";
      x = 5;
      y = 403;
      width = 120;
      alignment = "left";
   };
   UIfield size<NEx=759.,NEy=308.> {
      parent => <-.UImod_panel;
      y = 400;
      min = 1.;
      x = 130;
      mode = "integer";
      decimalPoints = 0;
      immediateMode = 7;
      value => <-.VelocityMean.side_slots;
   };
	UIlabel step_label<NEx=605.,NEy=341.> {
   	parent => <-.UImod_panel;
   	label => "Step time lenght:";
   	x = 5;
   	y = 433;
   	width = 120;
   	alignment = "left";
	};
	UIfield step<NEx=759.,NEy=341.> {
   	parent => <-.UImod_panel;
   	y = 430;
   	min = 0.;
   	x = 130;
   	mode = "integer";
   	decimalPoints = 0;
   	immediateMode = 7;
   	value => <-.VelocityMean.timestep;
	};
	UIbutton reset<NEx=759.,NEy=374.> {
   	parent => <-.UImod_panel;
   	height = 30;
   	y = 480;
   	width => parent.clientWidth;
   	do => <-.VelocityMean.clear;
		label = "Reset";
	};
   UIlabel title_2<NEx=594.,NEy=374.> {
      parent => <-.UImod_panel;
      label => "Volume limits";
      y = 520;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UItoggle use_manual_limits<NEx=594.,NEy=429.> {
      parent => <-.UImod_panel;
      y = 550;
      width = 200;
      set => <-.VelocityMean.use_manual_limits;
   };
   UIlabel min<NEx=616.,NEy=495.> {
      parent => <-.UImod_panel;
      y = 570;
      x = 50;
   };
   UIlabel max<NEx=770.,NEy=495.> {
      parent => <-.UImod_panel;
      y = 570;
      x = 150;
   };
   UIlabel label_x<NEx=451.,NEy=539.> {
      parent => <-.UImod_panel;
      label => "X";
      y = 600;
      x = 5;
      width = 40;
   };
   UIlabel label_y<NEx=451.,NEy=572.> {
      parent => <-.UImod_panel;
      label => "Y";
      y = 635;
      x = 5;
      width = 40;
   };
   UIlabel label_Z<NEx=451.,NEy=605.> {
      parent => <-.UImod_panel;
      label => "Z";
      y = 670;
      x = 5;
      width = 40;
   };
   UIfield x_min<NEx=616.,NEy=539.> {
      parent => <-.UImod_panel;
      y = 595;
      x = 50;
      width = 90;
      value => <-.VelocityMean.manual_limits[0][0];
   };
   UIfield x_max<NEx=770.,NEy=539.> {
      parent => <-.UImod_panel;
      y = 595;
      x = 150;
      width = 90;
      value => <-.VelocityMean.manual_limits[1][0];
   };
   UIfield y_min<NEx=616.,NEy=572.> {
      parent => <-.UImod_panel;
      y = 630;
      x = 50;
      width = 90;
      value => <-.VelocityMean.manual_limits[0][1];
   };
   UIfield y_max<NEx=770.,NEy=572.> {
      parent => <-.UImod_panel;
      y = 630;
      x = 150;
      width = 90;
      value => <-.VelocityMean.manual_limits[1][1];
   };
   UIfield z_min<NEx=616.,NEy=605.> {
      parent => <-.UImod_panel;
      y = 665;
      x = 50;
      width = 90;
      value => <-.VelocityMean.manual_limits[0][2];
   };
   UIfield z_max<NEx=770.,NEy=605.> {
      parent => <-.UImod_panel;
      y = 665;
      x = 150;
      width = 90;
      value => <-.VelocityMean.manual_limits[1][2];
   };
   XP_COMM_PROJ.STM3.DENS.VelocityMean VelocityMean<NEx=264.,NEy=220.> {
      in => <-.Select_Atoms.out;
      side_slots = 16;
   };
   GDM.DataObject DataObject<NEx=66.,NEy=396.> {
      in => <-.VelocityMean.out_velocity;
      Obj {
         name => "particle mean velocity";
      };
   };
   link selected_atoms<NEportLevels={1,2},NEx=242.,NEy=341.> => .Select_Atoms.out;
   link fld_velocity<NEportLevels={1,2},NEx=242.,NEy=396.> => .VelocityMean.out_velocity;
   link obj_velocity<NEportLevels={1,2},NEx=242.,NEy=451.> => .DataObject.obj;
};
