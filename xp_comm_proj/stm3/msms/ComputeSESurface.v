macro ComputeSESurface<NEhelpContextID=1,NEhelpFile="STM3/ComputeSESurface.html"> {
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &molecule_in<NEx=319.,NEy=22.,NEportLevels={2,0}>;

   XP_COMM_PROJ.STM3.MSMS.ComputeSES ComputeSES<NEx=396.,NEy=374.> {
      xyz_lst => <-.molecule_in.xyz_lst;
      atom_z => <-.molecule_in.atom_z;
      charge => <-.molecule_in.charge_lst;
      probe_radius => <-.radius.value;
      density => <-.density.value;
      color => <-.coloring.UIradioBox.selectedItem;
   };
   GDM.DataObject DataObject<NEx=363.,NEy=495.> {
	   in => <-.ComputeSES.fld;
	   Props {
	      trans => <-.<-.opacity_slider.value;
		   material = {0.2,0.4,0.7,50.};
			line_width => <-.<-.line_width;
			line_aa = 1;
      };
      Modes {
         mode => <-.<-.mode;
      };
	   Obj {
         cache_size => cache(switch(is_valid(getenv("CACHE_SIZE"))+1, 64, getenv("CACHE_SIZE")));
	      name = "solvent excluded surface";
	   };
   };
   link fld<NEportLevels={1,2},NEx=561.,NEy=473.> => .ComputeSES.fld;
   link obj<NEportLevels={1,2},NEx=539.,NEy=528.> => .DataObject.obj;
   UImod_panel UImod_panel<NEx=682.,NEy=22.> {
      title => name_of(<-.<-,1);
   };
   UIlabel title<NEx=737.,NEy=99.> {
      parent => <-.UImod_panel;
      label => "MSMS parameters";
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UIlabel UIlabel#1<NEx=660.,NEy=187.> {
      parent => <-.UImod_panel;
      label => "Probe radius:";
      y = 40;
      x = 5;
      width = 150;
      alignment = "left";
   };
   UIfield radius<NEx=825.,NEy=187.> {
      parent => <-.UImod_panel;
      value = 1.4;
      y = 38;
      x = 157;
      width = 80;
      min = 0.5;
      updateMode = 7;
   };
   UIlabel UIlabel#2<NEx=660.,NEy=242.> {
      parent => <-.UImod_panel;
      label => "Triangulation density:";
      y = 80;
      x = 5;
      width = 150;
      alignment = "left";
   };
   UIfield density<NEx=825.,NEy=242.> {
      parent => <-.UImod_panel;
      value = 4.;
      y = 78;
      x = 157;
      width = 80;
      min = 0.6;
      updateMode = 7;
   };
   string color_types<NEportLevels={0,1},NEx=506.,NEy=264.>[] => {
      "no color",
      "color by nearest atom type",
	   "color by charge",
	   "color by atomic number"
   };
   UIradioBoxLabel coloring<NEx=660.,NEy=297.> {
      parent => <-.UImod_panel;
      labels => <-.color_types;
      selectedItem = 1;
      title => "Surface coloring";
      UIradioBox {
         selectedItem<NEportLevels={0,3}>;
      };
      label_cmd {
         cmd[];
      };
      width => parent.clientWidth;
      y = 120;
   };
   UIlabel title1<NEx=759.,NEy=385.> {
	  parent => <-.UImod_panel;
	  label => "Surface parameters";
     y => <-.coloring.y + <-.coloring.height + 10;
	  width => parent.clientWidth;
	  color {
    	 foregroundColor = "white";
    	 backgroundColor = "blue";
	  };
   };
   UIslider opacity_slider<NEx=759.,NEy=440.> {
	  parent => <-.UImod_panel;
	  value = 1.0;
	  title => "Surface opacity";
     y => (title1.y + 40);
	  max = 1.;
	  min = 0.;
	  width => parent.clientWidth;
   };
   UIoption surface<NEx=44.,NEy=253.>;
   UIoption wireframe<NEx=44.,NEy=286.>;
   UIoption points<NEx=44.,NEy=319.>;
   UIradioBox mode_radio_box<NEx=44.,NEy=352.> {
      parent => <-.UImod_panel;
      cmdList => {<-.surface,<-.wireframe,<-.points};
      selectedItem = 0;
      y => (title1.y + 110);
   };
   int surf_arr<NEportLevels={0,1},NEx=44.,NEy=385.>[] => {
      0,0,0,0,0
   };
   int wire_arr<NEportLevels={0,1},NEx=44.,NEy=418.>[] => {
      0,2,1,0,1
   };
   int point_arr<NEportLevels={0,1},NEx=44.,NEy=451.>[] => {
      2,0,1,0,1
   };
   link mode<NEportLevels=1,NEx=44.,NEy=484.> => switch((.mode_radio_box.selectedItem + 1),.surf_arr,.wire_arr,.point_arr);
   int line_width<NEportLevels=1,NEx=44.,NEy=517.> => switch((.mode_radio_box.selectedItem + 1),0,1,2);
   UIlabel status<NEx=759.,NEy=627.> {
      parent => <-.UImod_panel;
      label => <-.ComputeSES.status;
      y => (title1.y + 190);
      x = 0;
      height = 50;
      width => parent.clientWidth;
      alignment = "center";
	   color {
    	  foregroundColor = "red";
	   };
   };
};
