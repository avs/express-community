macro ReadAnimMolecule<NEhelpContextID=1,NEhelpFile="STM3/ReadAnimMolecule.html"> {
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
         selectedText = "Set Kino";
         selectedItem<NEportLevels={0,3}> = 0;
         parent => <-.UImod_panel;
         height = 60;
         width => (parent.clientWidth - 1);
         y = 27;
         strings = {
				"Set Kino",
            "Set Multi PDB",
				"Set DCD",
				"Set MultiFile PDB"
			};
      };
      UIlabel units_lbl<NEx=121.,NEy=209.> {
         parent => <-.UImod_panel;
         label = "Units:";
         y = 100;
         x = 5;
         width = 60;
         alignment = "left";
      };
      UIlist Units_List<NEx=275.,NEy=209.> {
         selectedText = "Angstroms";
         strings = {
				"Bohrs",
            "Angstroms"
			};
         selectedItem<NEportLevels={0,3}> = 1;
         parent => <-.UImod_panel;
         width => ((parent.clientWidth - .x) - 1);
         x = 80;
         height = 45;
         y = 90;
      };
      UIlabel file_lbl<NEx=121.,NEy=253.> {
         parent => <-.UImod_panel;
         label = "File name:";
         y = 145;
         x = 5;
         width = 70;
         alignment = "left";
      };
      UItext filename<NEx=275.,NEy=253.> {
         parent => <-.UImod_panel;
         text<NEportLevels={2,3}> => <-.UIfileDialog.filename;
         y = 143;
         x = 80;
      };
      UIbutton browse_btn<NEx=418.,NEy=253.> {
         parent => <-.UImod_panel;
         height = 30;
         label = "Browse...";
         x = 180;
         y = 143;
         width = 70;
      };
      UIfileDialog UIfileDialog<NEx=561.,NEy=253.> {
         visible => <-.browse_btn.do;
         title = "Select molecule file";
      };
      UIlabel title2<NEx=726.,NEy=110.> {
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
         label => switch(((<-.<-.ReadAnimMol.out.num_bonds == 0) || <-.Force_Bond_Recomp.set)+1,"(Bonds from file)","Select bond parameters");
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
         active => <-.H_bonds.set && (<-.<-.ReadAnimMol.out.num_bonds == 0) || <-.Force_Bond_Recomp.set;
         width => (parent.clientWidth - 25);
         immediateMode = 1;
      };
      VUIslider_typein H_Bond_Max_DistanceVUI<NEx=638.,NEy=473.> {
         slider => <-.H_Bond_Max_Distance;
         width = 22;
         active => <-.H_bonds.set && (<-.<-.ReadAnimMol.out.num_bonds == 0) || <-.Force_Bond_Recomp.set;
      };
      UIslider H_Bond_Max_Angle<NEportLevels={0,1},NEx=385.,NEy=484.> {
         parent => <-.UImod_panel;
         value<NEportLevels={2,3}> = 30.;
         y = 525;
         max = 180.;
         min = 0.;
         decimalPoints = 2;
         active => <-.H_bonds.set && (<-.<-.ReadAnimMol.out.num_bonds == 0) || <-.Force_Bond_Recomp.set;
         width => (parent.clientWidth - 25);
         immediateMode = 1;
      };
      VUIslider_typein H_Bond_Max_AngleVUI<NEx=638.,NEy=517.,NEportLevels={0,0}> {
         slider => <-.H_Bond_Max_Angle;
         width = 22;
         active => <-.H_bonds.set && (<-.<-.ReadAnimMol.out.num_bonds == 0) || <-.Force_Bond_Recomp.set;
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
         active => <-.<-.ReadAnimMol.out.num_bonds == 0 || <-.Force_Bond_Recomp.set;
      };
      VUIslider_typein Bonding_Range_AdjustmentVUI<NEx=638.,NEy=561.> {
         slider => <-.Bonding_Range_Adjustment;
         width = 22;
         active => <-.<-.ReadAnimMol.out.num_bonds == 0 || <-.Force_Bond_Recomp.set;
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
         active => <-.Hydrogens.set && (<-.<-.ReadAnimMol.out.num_bonds == 0 || <-.Force_Bond_Recomp.set);
      };
      VUIslider_typein H_Bonding_Range_AdjustmentVUI<NEx=638.,NEy=605.> {
         slider => <-.H_Bonding_Range_Adjustment;
         width = 22;
         active => <-.H_Bonding_Range_Adjustment.active;
      };
      UItoggle Hydrogens<NEx=726.,NEy=165.> {
         parent => <-.UImod_panel;
         set<NEportLevels={0,3}> = 1;
         y = 260;
         x = 5;
         width = 120;
      };
      UItoggle H_bonds<NEx=726.,NEy=209.> {
         parent => <-.UImod_panel;
         set<NEportLevels={0,3}> = 1;
         y = 285;
         x = 5;
         width = 120;
         active => <-.<-.ReadAnimMol.out.num_bonds == 0 || <-.Force_Bond_Recomp.set;
      };
      GMOD.parse_v No_HB_if_no_H<NEx=880.,NEy=209.> {
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
         active => <-.<-.ReadAnimMol.out.num_bonds > 0;
	   };
      GMOD.parse_v No_disable_if_force<NEx=825.,NEy=297.> {
         v_commands = "lock.set = 0;";
         trigger => (<-.Force_Bond_Recomp.set == 1);
         on_inst = 0;
         relative => <-;
      };
      UItoggle lock<NEx=726.,NEy=253.> {
         parent => <-.UImod_panel;
         label => "Freeze bonds";
         set<NEportLevels={0,3}>;
         y = 285;
         x = 125;
   	   width = 125;
         active => <-.<-.ReadAnimMol.out.num_bonds == 0 || <-.Force_Bond_Recomp.set;
      };
      UIlabel file_lbl#1<NEx=121.,NEy=308.> {
         parent => <-.UImod_panel;
         label = "DCD file:";
         y = 182;
         x = 5;
         width = 70;
         alignment = "left";
         visible => (<-.File_Type.selectedItem == 2);
      };
      UItext filename_dcd<NEx=275.,NEy=308.> {
         parent => <-.UImod_panel;
         text<NEportLevels={2,3}> => <-.UIfileDialog#1.filename;
         y = 180;
         x = 80;
         visible => (<-.File_Type.selectedItem == 2);
      };
      UIbutton browse_btn#1<NEx=418.,NEy=308.> {
         parent => <-.UImod_panel;
         height = 30;
         label = "Browse...";
         x = 180;
         y = 180;
         width = 70;
         visible => (<-.File_Type.selectedItem == 2);
      };
      UIfileDialog UIfileDialog#1<NEx=561.,NEy=308.> {
         visible => <-.browse_btn#1.do;
         title = "Select DCD file";
         searchPattern = "*.dcd";
      };
      XP_COMM_PROJ.STM3.BASE.SyncDir SyncDir<NEx=561.,NEy=198.> {
         filename => <-.UIfileDialog#1.filename;
         ext => switch((<-.File_Type.selectedItem + 1),"xyz","pdb","pdb","pdb");
         srch => <-.UIfileDialog.searchPattern;
      };
      XP_COMM_PROJ.STM3.BASE.SyncDir SyncDir_dcd<NEx=561.,NEy=363.> {
         filename => <-.UIfileDialog.filename;
         ext = "dcd";
         srch => <-.UIfileDialog#1.searchPattern;
      };
   };
   XP_COMM_PROJ.STM3.BASE.ReadAnimMol ReadAnimMol<NEx=264.,NEy=209.> {
      filename => <-.UI.filename.text;
      format => <-.UI.File_Type.selectedItem;
      units => <-.UI.Units_List.selectedItem;
      read_hydrogens => <-.UI.Hydrogens.set;
      step => <-.Loop.count;
      num_steps => <-.Loop.end;
      filename_dcd => <-.UI.filename_dcd.text;
   };
   XP_COMM_PROJ.STM3.BASE.ComputeBonds ComputeBonds<NEx=495.,NEy=374.> {
      in => <-.ReadAnimMol.out;
      enable => switch((<-.UI.lock.set + 1),1);
	   force_recomputation => <-.UI.Force_Bond_Recomp.set;
      compute_h_bonds => <-.UI.H_bonds.set;
      max_h_bond_dist => <-.UI.H_Bond_Max_Distance.value;
      max_h_bond_angle => <-.UI.H_Bond_Max_Angle.value;
      adj_bonding_len => <-.UI.Bonding_Range_Adjustment.value;
      adj_H_bonding_len => <-.UI.H_Bonding_Range_Adjustment.value;
   };
   MODS.Loop Loop<NEx=649.,NEy=121.> {
      start = 1.;
      end = 100.;
      incr = 1.;
      LoopUI {
         cycle_toggle {
            x = 0;
         };
         start_typein {
            x = 0;
            field {
               mode = "integer";
               decimalPoints = 0;
               updateMode = 7;
            };
         };
         end_typein {
            x = 0;
            field {
               mode = "integer";
               decimalPoints = 0;
               updateMode = 7;
            };
         };
         incr_typein {
            x = 0;
            field {
               mode = "integer";
               decimalPoints = 0;
               updateMode = 7;
            };
         };
         count_typein {
            x = 0;
            field {
               mode = "integer";
               decimalPoints = 0;
               updateMode = 7;
            };
         };
      };
   };
   link molecule<NEportLevels={1,2},NEx=704.,NEy=451.> => .ComputeBonds.out;
   int step<NEportLevels={1,2},NEx=869.,NEy=495.> => .Loop.count;
};
