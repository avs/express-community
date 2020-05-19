macro Select_Residues<NEhelpContextID=1,NEhelpFile="STM3/Select_Residues.html"> {
   link in<NEportLevels={2,1},NEx=231.,NEy=55.>;
   XP_COMM_PROJ.STM3.UTILS.SelectResidues SelectResidues<NEx=220.,NEy=242.> {
      in => <-.in;
      selected_residues => <-.MUltilistWithReset.selected;
      operation => <-.UIradioBox.selectedItem;
      residue_names[];
   };
   link out<NEportLevels={1,2},NEx=352.,NEy=572.> => .SelectResidues.out;
   UImod_panel UImod_panel {
		title => name_of(<-.<-,1);
   };
   UIlabel title<NEx=726.,NEy=231.> {
      parent => <-.UImod_panel;
      label => "Select residues";
      y = 0;
      width => parent.clientWidth;
      color<NEdisplayMode="open"> {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   macro MUltilistWithReset<NEx=715.,NEy=341.> {
      UIpanel panel<NEx=308.,NEy=165.> {
         parent<NEportLevels={3,0}> => <-.<-.UImod_panel;
         y = 30;
         width => parent.clientWidth;
      };
      string Labels<NEportLevels={2,1},NEx=110.,NEy=99.>[] => <-.SelectResidues.residue_names;
      int Labels_size<NEx=110.,NEy=143.> => array_size(Labels);
      UImultiList+nosave UImultiList<NEx=451.,NEy=330.> {
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
      parent => <-.UImod_panel;
      cmdList => {<-.UIoption#0,
         <-.UIoption#1,<-.UIoption#2,<-.UIoption#3};
      selectedItem = 0;
      y = 240;
      x = 5;
      width => parent.clientWidth;
      itemWidth = 250;
   };
   UIoption UIoption#0<NEx=715.,NEy=440.> {
      label => "pass all selected";
      set = 1;
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

