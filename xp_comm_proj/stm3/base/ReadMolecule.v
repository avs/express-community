macro ReadMolecule<NEhelpContextID=1,NEhelpFile="STM3/ReadMolecule.html"> {
  macro UI<NEx=198.,NEy=33.> {
     UImod_panel UImod_panel<NEx=22.,NEy=22.> {
        title => name_of(<-.<-.<-,1);
     };
     UIlabel title1<NEx=121.,NEy=110.> {
        parent => <-.UImod_panel;
        label = "Select file and format";
        y = 0;
        width => parent.clientWidth;
        color {
           foregroundColor = "white";
           backgroundColor = "blue";
        };
     };
     UIlist File_Type<NEx=121.,NEy=165.> {
        selectedText = "Set PDB";
        selectedItem<NEportLevels={0,3}> = 3;
        parent => <-.UImod_panel;
        height = 110;
        width => (parent.clientWidth - 1);
        y = 27;
        strings = {
           "Set PlainCoord","Set Coordinate","Set Gaussian Cube","Set PDB",
           "Set PGamess","Set Gamess Cube","Set Dcar","Set DMOL_XYZ",
           "Set Mopac ESP","Set Mopac XYZ","Set mol2","Set mol","Set Chem3D",
           "Set mm3", "Set SHELX"
        };
     };
     UIlabel units_lbl<NEx=121.,NEy=209.> {
        parent => <-.UImod_panel;
        label = "Units:";
        y = 150;
        x = 5;
        width = 60;
        alignment = "left";
     };
     UIlist Units_List<NEx=275.,NEy=209.> {
        selectedText = "Angstroms";
        strings => {
		     "Bohrs",
           "Angstroms"
		  };
        selectedItem<NEportLevels={0,3}> = 1;
        parent => <-.UImod_panel;
        width => ((parent.clientWidth - .x) - 1);
        x = 80;
        height = 45;
        y = 140;
     };
     UIlabel file_lbl<NEx=121.,NEy=253.> {
        parent => <-.UImod_panel;
        label => "File name:";
        y = 195;
        x = 5;
        width = 70;
        alignment = "left";
     };
     UItext filename<NEx=275.,NEy=253.> {
        parent => <-.UImod_panel;
        text<NEportLevels={2,3}> => <-.UIfileDialog.filename;
        y = 193;
        x = 80;
     };
     UIbutton browse_btn<NEx=418.,NEy=253.> {
        parent => <-.UImod_panel;
        height = 30;
        label => "Browse...";
        x = 180;
        y = 193;
        width = 70;
     };
     UIfileDialog UIfileDialog<NEx=418.,NEy=297.> {
        visible => <-.browse_btn.do;
        title => "Select molecule file";
        searchPattern => switch((<-.File_Type.selectedItem +
		  1),"*","*","*.cube","*.pdb","*","*","*","*","*.mop","*.mop","*.mol2","*","*.c3d","*","*.res");
     };
     UIlabel title2<NEx=671.,NEy=110.> {
        parent => <-.UImod_panel;
        label = "Select bonds computation type";
        y = 230;
        width => parent.clientWidth;
        color {
           foregroundColor = "white";
           backgroundColor = "blue";
        };
     };
     UIlabel title3<NEx=385.,NEy=396.> {
        parent => <-.UImod_panel;
        label => switch(((<-.<-.ReadMol.out.num_bonds == 0) || <-.Force_Bond_Recomp.set)+1,"(Bonds from file)","Select bond parameters");
        y = 315;
        width => parent.clientWidth;
        color {
           foregroundColor = "white";
           backgroundColor = "blue";
        };
     };
     UIslider H_Bond_Max_Distance<NEx=385.,NEy=440.,NEportLevels={0,1}> {
        parent => <-.UImod_panel;
        value<NEportLevels={2,3}> = 2.3;
        y = 465;
        max = 10.;
        min = 0.;
        decimalPoints = 2;
        active => <-.H_bonds.set && (<-.<-.ReadMol.out.num_bonds == 0) || <-.Force_Bond_Recomp.set;
        width => (parent.clientWidth - 25);
		  immediateMode = 1;
     };
     VUIslider_typein H_Bond_Max_DistanceVUI<NEx=638.,NEy=473.> {
        slider => H_Bond_Max_Distance;
        width => 22;
        active => <-.H_bonds.set && (<-.<-.ReadMol.out.num_bonds == 0) || <-.Force_Bond_Recomp.set;
     };
     UIslider H_Bond_Max_Angle<NEportLevels={0,1},NEx=385.,NEy=484.> {
        parent => <-.UImod_panel;
        value<NEportLevels={2,3}> = 30.;
        y = 525;
        max = 180.;
        min = 0.;
        decimalPoints = 2;
        active => <-.H_bonds.set && (<-.<-.ReadMol.out.num_bonds == 0) || <-.Force_Bond_Recomp.set;
        width => (parent.clientWidth - 25);
		  immediateMode = 1;
     };
     VUIslider_typein H_Bond_Max_AngleVUI<NEx=638.,NEy=517.,NEportLevels={0,0}> {
        slider => H_Bond_Max_Angle;
        width => 22;
        active => <-.H_bonds.set && (<-.<-.ReadMol.out.num_bonds == 0) || <-.Force_Bond_Recomp.set;
     };
     UIslider Bonding_Range_Adjustment<NEx=385.,NEy=528.,NEportLevels={0,1}> {
        parent => <-.UImod_panel;
        y = 345;
        min = -20.;
        max = 20.;
        value<NEportLevels={2,3}> = 0.;
        decimalPoints = 1;
        width => (parent.clientWidth - 25);
		  title = "Bonding Range Adjustment (%)";
		  immediateMode = 1;
        active => <-.<-.ReadMol.out.num_bonds == 0 || <-.Force_Bond_Recomp.set;
     };
     VUIslider_typein Bonding_Range_AdjustmentVUI<NEx=638.,NEy=561.> {
        slider => Bonding_Range_Adjustment;
        width => 22;
        active => <-.<-.ReadMol.out.num_bonds == 0 || <-.Force_Bond_Recomp.set;
     };
     UIslider H_Bonding_Range_Adjustment<NEx=385.,NEy=572.,NEportLevels={0,1}> {
        parent => <-.UImod_panel;
        y = 405;
        min = -20.;
        max = 20.;
        value<NEportLevels={2,3}> = 0.;
        decimalPoints = 1;
        width => (parent.clientWidth - 25);
		  title = "H Bonding Range Adjustment (%)";
		  immediateMode = 1;
        active => <-.Hydrogens.set && (<-.<-.ReadMol.out.num_bonds == 0 || <-.Force_Bond_Recomp.set);
     };
     VUIslider_typein H_Bonding_Range_AdjustmentVUI<NEx=638.,NEy=605.> {
        slider => H_Bonding_Range_Adjustment;
        width => 22;
        active => <-.H_Bonding_Range_Adjustment.active;
     };
     UItoggle Hydrogens<NEx=671.,NEy=165.> {
        parent => <-.UImod_panel;
        set<NEportLevels={0,3}> = 1;
        y = 260;
        x = 5;
        width = 100;
     };
     UItoggle H_bonds<NEx=671.,NEy=209.> {
        parent => <-.UImod_panel;
        set<NEportLevels={0,3}> = 1;
        y = 285;
        x = 5;
        width = 100;
        active => <-.<-.ReadMol.out.num_bonds == 0 || <-.Force_Bond_Recomp.set;
     };
     GMOD.parse_v No_HB_if_no_H<NEx=825.,NEy=209.> {
        v_commands = "H_bonds.set = 0;";
        trigger => (<-.Hydrogens.set == 0);
        on_inst = 0;
        relative => <-;
     };
	  UItoggle Force_Bond_Recomp<NEx=671.,NEy=253.> {
   	  parent => <-.UImod_panel;
   	  set<NEportLevels={0,3}>;
   	  y = 260;
   	  x = 125;
   	  width = 125;
        active => <-.<-.ReadMol.out.num_bonds > 0;
	  };
	  UItoggle Disable_Bond_Comp<NEx=671.,NEy=297.> {
   	  parent => <-.UImod_panel;
     	  set<NEportLevels={0,3}>;
   	  y = 285;
   	  x = 125;
   	  width = 125;
		  active => <-.Force_Bond_Recomp.set == 0;
		  label = "Freeze bonds";
	  };
     GMOD.parse_v No_disable_if_force<NEx=825.,NEy=297.> {
        v_commands = "Disable_Bond_Comp.set = 0;";
        trigger => (<-.Force_Bond_Recomp.set == 1);
        on_inst = 0;
        relative => <-;
     };
  };
  XP_COMM_PROJ.STM3.BASE.ReadMol ReadMol<NEx=264.,NEy=209.> {
     filename => <-.UI.filename.text;
     format => <-.UI.File_Type.selectedItem;
     units => <-.UI.Units_List.selectedItem;
     read_hydrogens => <-.UI.Hydrogens.set;
  };
  XP_COMM_PROJ.STM3.BASE.ComputeBonds ComputeBonds<NEx=495.,NEy=374.> {
     in => <-.ReadMol.out;
     compute_h_bonds => <-.UI.H_bonds.set;
     max_h_bond_dist => <-.UI.H_Bond_Max_Distance.value;
     max_h_bond_angle => <-.UI.H_Bond_Max_Angle.value;
     adj_bonding_len => <-.UI.Bonding_Range_Adjustment.value;
     adj_H_bonding_len => <-.UI.H_Bonding_Range_Adjustment.value;
	  force_recomputation => <-.UI.Force_Bond_Recomp.set;
	  enable => switch((<-.UI.Disable_Bond_Comp.set + 1),1);	  
  };
  link molecule<NEportLevels={1,2},NEx=704.,NEy=451.> => .ComputeBonds.out;
};
