macro WritePDB<NEhelpContextID=1,NEhelpFile="STM3/WritePDB.html"> {
   link molecule<NEportLevels={2,1},NEx=77.,NEy=88.>;
   int step<NEportLevels={2,1},NEx=77.,NEy=33.>;
   XP_COMM_PROJ.STM3.UTILS.WritePdbCore WritePDB<NEx=231.,NEy=451.> {
      molecule => <-.molecule;
      filename => <-.filename.text;
      animation_support => <-.animation.selectedItem;
      step => <-.step;
   };
   UImod_panel UImod_panel<NEx=504.,NEy=44.> {
      title => name_of(<-.<-,1);
   };
   UIlabel title1<NEx=605.,NEy=88.> {
      parent => <-.UImod_panel;
      label => "PDB output file";
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UItext filename<NEx=649.,NEy=143.> {
      parent => <-.UImod_panel;
      text<NEportLevels={2,2}> => <-.UIfileDialog.filename;
      y = 40;
      width = 180;
   };
   UIbutton browse_btn<NEx=792.,NEy=143.> {
      parent => <-.UImod_panel;
      height = 30;
      label => "Browse...";
      x = 180;
      y = 40;
      width = 70;
   };
   UIfileDialog UIfileDialog<NEx=792.,NEy=187.> {
      visible => <-.browse_btn.do;
      title => "Select output file";
      searchPattern = "*.pdb";
   };
   UIoption No_animation<NEx=550.,NEy=220.> {
      set = 1;
   };
   UIoption Multi_PDB_animation<NEx=550.,NEy=253.>;
   UIoption MultiFile_PDB_animation<NEx=550.,NEy=286.>;
   UIradioBox animation<NEx=693.,NEy=374.> {
      parent => <-.UImod_panel;
      cmdList => {<-.No_animation, <-.Multi_PDB_animation, <-.MultiFile_PDB_animation};
      selectedItem = 0;
      y = 80;
      x = 10;
      width = 200;
      itemWidth = 200;
   };
};
