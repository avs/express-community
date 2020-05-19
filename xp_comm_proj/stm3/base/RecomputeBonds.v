macro RecomputeBonds<NEhelpContextID=1,NEhelpFile="STM3/RecomputeBonds.html"> {
	link molecule_in<NEportLevels={2,1},NEx=165.,NEy=275.>;
	macro UI<NEx=495.,NEy=66.> {
	  UImod_panel UImod_panel<NEx=22.,NEy=22.> {
		  title => name_of(<-.<-.<-,1);
	  };
     UIlabel title<NEx=385.,NEy=396.> {
        parent => <-.UImod_panel;
        label = "Select bond parameters";
        y = 0;
        width => parent.clientWidth;
        color {
           foregroundColor = "white";
           backgroundColor = "blue";
        };
     };
     UItoggle H_bonds<NEx=671.,NEy=209.> {
        parent => <-.UImod_panel;
        set<NEportLevels={0,3}> = 1;
        y = 30;
        x = 5;
        width = 100;
     };
	  UItoggle Disable_Bond_Comp<NEx=671.,NEy=297.> {
   	  parent => <-.UImod_panel;
     	  set<NEportLevels={0,3}>;
   	  y = 30;
   	  x = 125;
   	  width = 125;
		  label = "Freeze bonds";
	  };
     UIslider H_Bond_Max_Distance<NEx=385.,NEy=440.,NEportLevels={0,1}> {
        parent => <-.UImod_panel;
        value<NEportLevels={2,3}> = 2.3;
        y = 180;
        max = 10.;
        min = 0.;
        decimalPoints = 2;
        active => <-.H_bonds.set;
        width => (parent.clientWidth - 25);
		  immediateMode = 1;
     };
     VUIslider_typein H_Bond_Max_DistanceVUI<NEx=638.,NEy=473.> {
        slider => H_Bond_Max_Distance;
        width => 22;
        active => <-.H_bonds.set;
     };
     UIslider H_Bond_Max_Angle<NEportLevels={0,1},NEx=385.,NEy=484.> {
        parent => <-.UImod_panel;
        value<NEportLevels={2,3}> = 30.;
        y = 240;
        max = 180.;
        min = 0.;
        decimalPoints = 2;
        active => <-.H_bonds.set;
        width => (parent.clientWidth - 25);
		  immediateMode = 1;
     };
     VUIslider_typein H_Bond_Max_AngleVUI<NEx=638.,NEy=517.,NEportLevels={0,0}> {
        slider => H_Bond_Max_Angle;
        width => 22;
        active => <-.H_bonds.set;
     };
     UIslider Bonding_Range_Adjustment<NEx=385.,NEy=528.,NEportLevels={0,1}> {
        parent => <-.UImod_panel;
        y = 60;
        min = -20.;
        max = 20.;
        value<NEportLevels={2,3}> = 0.;
        decimalPoints = 1;
        width => (parent.clientWidth - 25);
		  title = "Bonding Range Adjustment (%)";
		  immediateMode = 1;
     };
     VUIslider_typein Bonding_Range_AdjustmentVUI<NEx=638.,NEy=561.> {
        slider => Bonding_Range_Adjustment;
        width => 22;
     };
     UIslider H_Bonding_Range_Adjustment<NEx=385.,NEy=572.,NEportLevels={0,1}> {
        parent => <-.UImod_panel;
        y = 120;
        min = -20.;
        max = 20.;
        value<NEportLevels={2,3}> = 0.;
        decimalPoints = 1;
        width => (parent.clientWidth - 25);
		  title = "H Bonding Range Adjustment (%)";
		  immediateMode = 1;
     };
     VUIslider_typein H_Bonding_Range_AdjustmentVUI<NEx=638.,NEy=605.> {
        slider => H_Bonding_Range_Adjustment;
        width => 22;
     };
  };
  XP_COMM_PROJ.STM3.BASE.ComputeBonds ComputeBonds<NEx=495.,NEy=374.> {
     in => <-.molecule_in;
     compute_h_bonds => <-.UI.H_bonds.set;
     max_h_bond_dist => <-.UI.H_Bond_Max_Distance.value;
     max_h_bond_angle => <-.UI.H_Bond_Max_Angle.value;
     adj_bonding_len => <-.UI.Bonding_Range_Adjustment.value;
     adj_H_bonding_len => <-.UI.H_Bonding_Range_Adjustment.value;
	  force_recomputation = 1;
	  enable => switch((<-.UI.Disable_Bond_Comp.set + 1),1);	  
  };
  link molecule<NEportLevels={1,2},NEx=704.,NEy=451.> => .ComputeBonds.out;
};
