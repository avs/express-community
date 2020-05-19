macro CropUnitCell<NEhelpContextID=1,NEhelpFile="STM3/CropUnitCell.html"> {
   link molecule_in<NEportLevels={2,1},NEx=77.,NEy=123.>;
   XP_COMM_PROJ.STM3.CRYSTAL.SubCell SubCell<NEx=77.,NEy=352.> {
      in => <-.molecule_in;
      miller_planes => {
         0,1,0,1,0,1
      };
		epsilon = 0.00001;
   };
   link out<NEportLevels={1,2},NEx=77.,NEy=581.> => SubCell.out;
   UImod_panel UImod_panel<NEx=286.,NEy=33.> {
      title => name_of(<-.<-,1);
   };
   UIlabel title1<NEx=506.,NEy=99.> {
      parent => <-.UImod_panel;
      label => "Miller indices for unit cell sides";
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UIslider a_min<NEx=440.,NEy=176.> {
      parent => <-.UImod_panel;
      value => <-.SubCell.miller_planes[0];
      y = 30;
      max = 10.;
      min = 0.;
      width => (parent.clientWidth / 2)-5;
      mode = "integer";
      decimalPoints = 0;
   };
   UIslider a_max<NEx=594.,NEy=176.> {
      parent => <-.UImod_panel;
      value => <-.SubCell.miller_planes[1];
      y = 30;
      max = 10.;
      min = 0.;
      x => (parent.clientWidth / 2)+5;
      width => (parent.clientWidth / 2)-5;
      mode = "integer";
      decimalPoints = 0;
   };
   UIslider b_min<NEx=440.,NEy=209.> {
      parent => <-.UImod_panel;
      value => <-.SubCell.miller_planes[2];
      y = 90;
      max = 10.;
      min = 0.;
      width => (parent.clientWidth / 2)-5;
      mode = "integer";
      decimalPoints = 0;
   };
   UIslider b_max<NEx=594.,NEy=209.> {
      parent => <-.UImod_panel;
      value => <-.SubCell.miller_planes[3];
      y = 90;
      max = 10.;
      min = 0.;
      x => (parent.clientWidth / 2)+5;
      width => (parent.clientWidth / 2)-5;
      mode = "integer";
      decimalPoints = 0;
   };
   UIslider c_min<NEx=440.,NEy=242.> {
      parent => <-.UImod_panel;
      value => <-.SubCell.miller_planes[4];
      y = 150;
      max = 10.;
      min = 0.;
      width => (parent.clientWidth / 2)-5;
      mode = "integer";
      decimalPoints = 0;
   };
   UIslider c_max<NEx=594.,NEy=242.> {
      parent => <-.UImod_panel;
      value => <-.SubCell.miller_planes[5];
      y = 150;
      max = 10.;
      min = 0.;
      x => (parent.clientWidth / 2)+5;
      width => (parent.clientWidth / 2)-5;
      mode = "integer";
      decimalPoints = 0;
   };
	UIlabel title2<NEx=506.,NEy=308.> {
   	parent => <-.UImod_panel;
   	label => "Tolerance";
   	y = 220;
   	width => parent.clientWidth;
   	color {
      	foregroundColor = "white";
      	backgroundColor = "blue";
   	};
	};
	UIfield epsilon<NEx=583.,NEy=363.> {
   	parent => <-.UImod_panel;
   	value => <-.SubCell.epsilon;
   	y = 250;
   	x = 76;
   	min = 0.;
   	decimalPoints = 5;
   	updateMode = 7;
	};
	UIlabel eps_lbl<NEx=440.,NEy=363.> {
   	parent => <-.UImod_panel;
   	label => "Epsilon:";
   	y = 252;
   	x = 5;
   	width = 70;
   	alignment = "left";
	};
};
