macro ApplySymmetries<NEhelpContextID=1,NEhelpFile="STM3/ApplySymmetries.html"> {
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &molecule_in<NEx=88.,NEy=44.,NEportLevels={2,1}>;
   XP_COMM_PROJ.STM3.CRYSTAL.Symmetry Symmetry<NEx=264.,NEy=209.> {
      in => <-.molecule_in;
   };
   XP_COMM_PROJ.STM3.CRYSTAL.RemoveDuplicates RemoveDuplicates<NEx=264.,NEy=341.> {
      in => <-.Symmetry.out;
   };
   link molecule_out<NEportLevels={1,2},NEx=132.,NEy=550.> => .RemoveDuplicates.out;
};
