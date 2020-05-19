macro Accumulate_Traces<NEhelpContextID=1,NEhelpFile="STM3/Accumulate_Traces.html"> {
   link molecule<NEportLevels={2,1},NEx=50.,NEy=47.>;

	macro Select_Atoms<NEx=242.,NEy=143.> {
	   link in<NEportLevels={2,1},NEx=231.,NEy=55.> => <-.molecule;
	   XP_COMM_PROJ.STM3.UTILS.SelectAtoms SelectAtoms<NEx=220.,NEy=242.> {
    	  in => <-.in;
    	  selected_atoms => <-.MUltilistWithReset.selected;
    	  operation => <-.UIradioBox.selectedItem;
	   };
	   link out<NEportLevels={1,2},NEx=352.,NEy=572.> => .SelectAtoms.out;
	   UIpanel panel<NEx=506.,NEy=132.> {
    	  parent<NEportLevels={3,0}> => <-.<-.UImod_panel;
		  x = 0;
		  y = 0;
		  width => parent.clientWidth;
		  height = 340;
	   };
	   UIlabel title<NEx=726.,NEy=231.> {
    	  parent => <-.panel;
    	  label => "Select atoms to trace";
    	  y = 0;
    	  width => parent.clientWidth;
    	  color {
        	 foregroundColor = "white";
        	 backgroundColor = "blue";
    	  };
	   };
	   macro MUltilistWithReset<NEx=715.,NEy=341.> {
    	  UIpanel panel<NEx=308.,NEy=165.> {
        	 parent<NEportLevels={3,0}> => <-.<-.panel;
        	 y = 30;
        	 width => parent.clientWidth;
    	  };
    	  string Labels<NEportLevels={2,1},NEx=110.,NEy=99.>[] => <-.SelectAtoms.atom_names;
    	  int Labels_size<NEx=110.,NEy=143.> => array_size(Labels);
    	  UImultiList UImultiList<NEx=451.,NEy=330.> {
        	 parent => <-.panel;
        	 height => (parent.clientHeight - 30);
        	 y = 0;
        	 x = 5;
        	 width => (parent.clientWidth - 10);
        	 selectedItems<NEportLevels={0,2}>[<-.Labels_size];
        	 fontAttributes {
            	family = "courier";
            	height = 14;
        	 };
    	  };
    	  UIbutton reset<NEx=605.,NEy=330.> {
        	 parent => <-.panel;
        	 label => "Deselect All";
        	 x = 5;
        	 y => <-.UImultiList.height;
        	 width => (<-.UImultiList.width / 2);
    	  };
    	  GMOD.parse_v parse_v_reset<NEx=605.,NEy=385.> {
        	 v_commands = "UImultiList.selectedItems=>;UImultiList.strings=>Labels;Labels_size=>array_size(Labels);";
        	 trigger => <-.reset.do;
        	 relative => <-;
        	 mode = 1;
    	  };
    	  UIbutton set<NEx=759.,NEy=330.> {
        	 parent => <-.panel;
        	 y => <-.UImultiList.height;
        	 label => "Select All";
        	 x => ((<-.UImultiList.width / 2) + 5);
        	 width => (<-.UImultiList.width / 2);
    	  };
    	  GMOD.parse_v parse_v_set<NEx=759.,NEy=385.> {
        	 v_commands = "Labels_size=>array_size(Labels);UImultiList.selectedItems=>;UImultiList.selectedItems[Labels_size];Labels_size=>array_size(Labels);UImultiList.selectedItems=>init_array(Labels_size,0,Labels_size-1);UImultiList.strings=>Labels;";
        	 trigger => <-.set.do;
        	 on_inst = 0;
        	 relative => <-;
        	 mode = 1;
    	  };
    	  link selected<NEportLevels={1,2},NEx=176.,NEy=616.> => .UImultiList.selectedItems;
	   };
	   UIradioBox UIradioBox<NEx=715.,NEy=605.> {
    	  parent => <-.panel;
    	  cmdList => {<-.UIoption#0,<-.UIoption#1,<-.UIoption#2,<-.UIoption#3};
    	  selectedItem = 2;
    	  y = 240;
    	  x = 5;
    	  width => parent.clientWidth;
    	  itemWidth = 250;
	   };
	   UIoption UIoption#0<NEx=715.,NEy=440.> {
    	  label => "pass all selected";
	   };
	   UIoption UIoption#1<NEx=715.,NEy=473.> {
    	  label => "remove all selected";
	   };
	   UIoption UIoption#2<NEx=715.,NEy=506.> {
    	  label => "pass all similar";
	   };
	   UIoption UIoption#3<NEx=715.,NEy=539.> {
    	  label => "remove all similar";
	   };
	};



  XP_COMM_PROJ.STM3.TRACES.AccumulateTraces AccumulateTraces<NEx=363.,NEy=286.> {
     in => <-.Select_Atoms.out;
     clear => <-.Clear.do;
     use_manual_limits => <-.use_manual_limits.set;
     start<NEportLevels={3,0}>;
     end<NEportLevels={3,0}>;
     increment<NEportLevels={3,0}>;
     step<NEportLevels={3,0}>;
     operation => <-.UIradioBox.selectedItem;
     accumulator_size => <-.size.value;
  };
  GDM.DataObject DataObject<NEx=385.,NEy=385.> {
    in => <-.AccumulateTraces.out_trace;
	 Obj {
	    name = "atom trace";
	 };
	 Props {
	   line_width = 2;
		line_aa = 1;
	 };
  };
  link out_occupancy<NEportLevels={1,2}> => .AccumulateTraces.out_occupancy;
  link out_trace<NEportLevels={1,2}> => .AccumulateTraces.out_trace;
  link obj_trace<NEportLevels={1,2}> => .DataObject.obj;
  UImod_panel UImod_panel<NEx=638.,NEy=33.> {
     title => name_of(<-.<-,1);
  };
  UIlabel title<NEx=726.,NEy=231.> {
      parent => <-.UImod_panel;
      label => "Select operation";
      y = 345;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
  };
  UIbutton Clear<NEx=638.,NEy=99.> {
     parent => <-.UImod_panel;
	 x = 130;
     y = 440;
  };
  UIoption inactive<NEx=825.,NEy=308.>;
  UIoption immediate<NEx=825.,NEy=341.>;
  UIoption accumulate<NEx=825.,NEy=341.>;
  UIoption playback<NEx=825.,NEy=374.>;
  UIradioBox UIradioBox<NEx=836.,NEy=495.> {
     parent => <-.UImod_panel;
     cmdList => {<-.inactive,
        <-.immediate,<-.accumulate,<-.playback};
     selectedItem = 0;
     y = 375;
     x = 1;
  };
  UIlabel size_label<NEx=682.,NEy=572.> {
     parent => <-.UImod_panel;
     label => "Accumulator side:";
     x = 5;
     y = 480;
     width = 120;
     alignment = "left";
  };
  UIfield size<NEx=836.,NEy=572.> {
     parent => <-.UImod_panel;
     y = 478;
     value = 64.;
     min = 1.;
     x = 130;
     mode = "integer";
     decimalPoints = 0;
     immediateMode = 7;
  };
  UIlabel title_2<NEx=198.,NEy=418.> {
     parent => <-.UImod_panel;
     label => "Volume limits";
     y = 515;
     width => parent.clientWidth;
     color {
        foregroundColor = "white";
        backgroundColor = "blue";
     };
  };
  UItoggle use_manual_limits<NEx=198.,NEy=473.> {
     parent => <-.UImod_panel;
     y = 540;
     width = 200;
  };
  UIlabel min<NEx=220.,NEy=539.> {
     parent => <-.UImod_panel;
     y = 560;
     x = 50;
  };
  UIlabel max<NEx=374.,NEy=539.> {
     parent => <-.UImod_panel;
     y = 560;
     x = 150;
  };
  UIlabel UIlabel#1<NEx=55.,NEy=583.> {
     parent => <-.UImod_panel;
     label => "X";
     y = 585;
     x = 5;
     width = 40;
  };
  UIlabel UIlabel#2<NEx=55.,NEy=616.> {
     parent => <-.UImod_panel;
     label => "Y";
     y = 610;
     x = 5;
     width = 40;
  };
  UIlabel UIlabel#3<NEx=55.,NEy=649.> {
     parent => <-.UImod_panel;
     label => "Z";
     y = 635;
     x = 5;
     width = 40;
  };
  UIfield UIfield<NEx=220.,NEy=583.> {
     parent => <-.UImod_panel;
     y = 583;
     x = 50;
     width = 90;
	 value => <-.AccumulateTraces.manual_limits[0][0];
  };
  UIfield UIfield#1<NEx=374.,NEy=583.> {
     parent => <-.UImod_panel;
     y = 583;
     x = 150;
     width = 90;
	 value => <-.AccumulateTraces.manual_limits[1][0];
  };
  UIfield UIfield#2<NEx=220.,NEy=616.> {
     parent => <-.UImod_panel;
     y = 607;
     x = 50;
     width = 90;
	 value => <-.AccumulateTraces.manual_limits[0][1];
  };
  UIfield UIfield#3<NEx=374.,NEy=616.> {
     parent => <-.UImod_panel;
     y = 607;
     x = 150;
     width = 90;
	 value => <-.AccumulateTraces.manual_limits[1][1];
  };
  UIfield UIfield#4<NEx=220.,NEy=649.> {
     parent => <-.UImod_panel;
     y = 632;
     x = 50;
     width = 90;
	 value => <-.AccumulateTraces.manual_limits[0][2];
  };
  UIfield UIfield#5<NEx=374.,NEy=649.> {
     parent => <-.UImod_panel;
     y = 632;
     x = 150;
     width = 90;
	 value => <-.AccumulateTraces.manual_limits[1][2];
  };
};

