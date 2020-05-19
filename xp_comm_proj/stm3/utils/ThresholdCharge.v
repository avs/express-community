macro ThresholdCharge<NEhelpContextID=1,NEhelpFile="STM3/ThresholdCharge.html"> {
   link molecule_in<NEportLevels={2,1},NEx=55.,NEy=44.>;
   XP_COMM_PROJ.STM3.UTILS.ThresholdChargeCore ThresholdChargeCore<NEx=286.,NEy=429.> {
      molecule_in => <-.molecule_in;
      min_val = 0;
      max_val = 1;
      operation => <-.UIradioBox.selectedItem;
   };
   UImod_panel UImod_panel<NEx=440.,NEy=55.> {
      title => name_of(<-.<-,1);
   };
   UIlabel title1<NEx=682.,NEy=99.> {
      parent => <-.UImod_panel;
      label => "Threshold limits";
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UIslider min<NEx=572.,NEy=154.,NEportLevels={0,1}> {
      parent => <-.UImod_panel;
      y = 30;
      width => (parent.clientWidth - 25);
      min = -10.0;
      max =  10.0;
		value => <-.ThresholdChargeCore.min_val;
   };
   VUIslider_typein minVUI<NEx=748.,NEy=187.> {
      slider => <-.min;
      width => 22;
   };
   UIslider max<NEx=572.,NEy=220.,NEportLevels={0,1}> {
      parent => <-.UImod_panel;
      y = 90;
      width => (parent.clientWidth - 25);
      min = -10.0;
      max =  10.0;
		value => <-.ThresholdChargeCore.max_val;
   };
   VUIslider_typein maxVUI<NEx=748.,NEy=264.> {
      slider => <-.max;
      width => 22;
   };
   UIlabel title2<NEx=682.,NEy=319.> {
      parent => <-.UImod_panel;
      label => "Threshold operation";
      y = 160;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UIoption below_min<NEx=561.,NEy=407.>;
   UIoption below_or_equal_min<NEx=561.,NEy=440.>;
   UIoption between_min_and_max<NEx=561.,NEy=473.>;
   UIoption proper_between_min_and_max<NEx=561.,NEy=506.>;
   UIoption above_max<NEx=561.,NEy=539.>;
   UIoption above_or_equal_max<NEx=561.,NEy=572.>;
   UIoption exactly_equal_max<NEx=561.,NEy=605.>;
   UIradioBox UIradioBox<NEx=737.,NEy=671.> {
      parent => <-.UImod_panel;
      cmdList => {
         <-.below_min,<-.below_or_equal_min,<-.between_min_and_max,
         <-.proper_between_min_and_max,<-.above_max,<-.above_or_equal_max,
         <-.exactly_equal_max
      };
      selectedItem = 0;
      y = 190;
      width = 200;
      itemWidth = 200;
   };
   link molecule_out<NEportLevels={1,2},NEx=286.,NEy=583.> => .ThresholdChargeCore.molecule_out;
};
