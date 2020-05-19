macro Read_Gaussian_Cube<NEhelpContextID=1,NEhelpFile="STM3/Read_Gaussian_Cube.html"> {
   XP_COMM_PROJ.STM3.BASE.ReadGaussianCube ReadGaussianCube<NEx=473.,NEy=231.> {
      units => <-.Units_List.selectedItem;
      filename => <-.UIfileBrowser.file_name;
   };
   UIfileBrowser UIfileBrowser<NEx=308.,NEy=154.> {
      UIfileDialog {
         title = "Select the Gaussian Cube File";
         searchPattern = "./*.cube";
      };
      File_Browser_Panel {
         UIpanel {
            y = 35;
         };
         parent => <-.<-.UImod_panel;
         width => parent.clientWidth;
      };
	  copy_on_change {
	     input => trigger;
	  };
   };
   UImod_panel UImod_panel<NEx=99.,NEy=55.> {
      title => name_of(<-.<-,1);
   };
   UIlabel UIlabel<NEx=396.,NEy=88.> {
      parent => <-.UImod_panel;
      label => "Select Gaussian Cube file";
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
      UIlist Units_List {
         parent => <-.UImod_panel;
         height => 45;
		 width => parent.clientWidth-x;
         strings => {"Bohrs", "Angstroms"};
		 x = 4;
         y = 105;
         selectedItem = 0;
      };
      UIlabel Select_Units {
         parent => <-.UImod_panel;
	     x = 5;
         y = 80;
         height => 20;
         label => "Select units...";
		 alignment = "left";
         width = 200;
      };
   link out<NEportLevels={1,2},NEx=517.,NEy=352.> => .ReadGaussianCube.out;
};
