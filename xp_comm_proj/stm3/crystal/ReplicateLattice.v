macro ReplicateLattice<NEhelpContextID=1,NEhelpFile="STM3/ReplicateLattice.html"> {
   link molecule_in<NEportLevels={2,1},NEx=110.,NEy=33.>;
   XP_COMM_PROJ.STM3.CRYSTAL.LatticeReplication Lattice_Replication<NEx=308.,NEy=242.> {
      in => <-.molecule_in;
   };
   XP_COMM_PROJ.STM3.CRYSTAL.RemoveDuplicates RemoveDuplicates<NEx=308.,NEy=330.> {
      in => <-.Lattice_Replication.out;
   };
   UImod_panel UImod_panel<NEx=539.,NEy=44.> {
      title => name_of(<-.<-,1);
   };
   UIlabel title<NEx=704.,NEy=132.> {
      parent => <-.UImod_panel;
      label => "Replications";
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UIslider repl_a<NEx=704.,NEy=187.> {
      parent => <-.UImod_panel;
      value => <-.Lattice_Replication.replications_a;
      title = "base vector a";
      y = 30;
      max = 10.;
      min = 1.;
      width = 165;
      mode = "integer";
      decimalPoints = 0;
   };
   UIslider repl_b<NEx=704.,NEy=242.> {
      parent => <-.UImod_panel;
      value => <-.Lattice_Replication.replications_b;
      title = "base vector b";
      y = 90;
      max = 10.;
      min = 1.;
      width = 165;
      mode = "integer";
      decimalPoints = 0;
   };
   UIslider repl_c<NEx=704.,NEy=297.> {
      parent => <-.UImod_panel;
      value => <-.Lattice_Replication.replications_c;
      title = "base vector c";
      y = 150;
      max = 10.;
      min = 1.;
      width = 165;
      mode = "integer";
      decimalPoints = 0;
   };
   UItoggle reverse_a<NEx=869.,NEy=187.> {
      parent => <-.UImod_panel;
      label => "reverse";
      x = 170;
      y = 47;
      width = 85;
      set => <-.Lattice_Replication.reverse_a;
   };
   UItoggle reverse_b<NEx=869.,NEy=242.> {
      parent => <-.UImod_panel;
      label => "reverse";
      x = 170;
      y = 107;
      width = 85;
      set => <-.Lattice_Replication.reverse_b;
   };
   UItoggle reverse_c<NEx=869.,NEy=297.> {
      parent => <-.UImod_panel;
      label => "reverse";
      x = 170;
      y = 167;
      width = 85;
      set => <-.Lattice_Replication.reverse_c;
   };
   link out<NEportLevels={1,2},NEx=539.,NEy=418.> => .RemoveDuplicates.out;
};
