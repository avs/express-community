macro AddBonds<NEhelpContextID=1,NEhelpFile="STM3/AddBonds.html"> {
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &molecule_in<NEx=88.,NEy=22.,NEportLevels={2,1}>;
   UImod_panel UImod_panel<NEx=176.,NEy=88.> {
      title => name_of(<-.<-,1);
   };
   UIlabel UIlabel<NEx=242.,NEy=198.> {
      parent => <-.UImod_panel;
      label => "N. bonds to add:";
      y = 2;
      x = 5;
      alignment = "left";
   };
   UIfield nb<NEx=418.,NEy=198.> {
      parent => <-.UImod_panel;
      value = 1.;
      y = 0;
      x = 110;
      min = 0.;
      mode = "integer";
      decimalPoints = 0;
      updateMode = 7;
   };
   UIlabel lbl1<NEx=396.,NEy=264.> {
      parent => <-.UImod_panel;
      label => "Atom from";
      y = 40;
   };
   UIlabel lbl2<NEx=660.,NEy=264.> {
      parent => <-.UImod_panel;
      label => "Atom to";
      y = 40;
      x = 110;
   };
   UIfield a1<NEx=396.,NEy=308.>[nb.value] {
      parent => <-.UImod_panel;
      value = 1.;
      y => (65 + (30 * index_of(a1)));
      min = 1.;
      mode = "integer";
      decimalPoints = 0;
      updateMode = 7;
   };
   UIfield a2<NEx=660.,NEy=308.>[nb.value] {
      parent => <-.UImod_panel;
      value = 2.;
      y => (65 + (30 * index_of(a2)));
      x = 110;
      min = 1.;
      mode = "integer";
      decimalPoints = 0;
      updateMode = 7;
   };
   FLD_MAP.interleave_2_arrays interleave_2_arrays<NEx=528.,NEy=374.> {
      in1 => (<-.a1.value - 1);
      in2 => (<-.a2.value - 1);
   };
   int bt<NEx=704.,NEy=374.>[] => init_array(nb.value, 4, 4);
   XP_COMM_PROJ.STM3.BASE.MolecularDataType molecule_out<NEx=176.,NEy=484.,NEportLevels={0,2}> {
   	  num_atoms => <-.molecule_in.num_atoms;
      xyz_lst => <-.molecule_in.xyz_lst;
      atom_z => <-.molecule_in.atom_z;
      residue_id => <-.molecule_in.residue_id;
      charge_lst => <-.molecule_in.charge_lst;
      atom_name => <-.molecule_in.atom_name;
      residue_name => <-.molecule_in.residue_name;
      num_bonds => <-.molecule_in.num_bonds + <-.nb.value;
      connect_lst => switch((<-.nb.value > 0)+1, <-.molecule_in.connect_lst, concat_array(<-.molecule_in.connect_lst, <-.interleave_2_arrays.out));
      bond_type_lst => switch((<-.nb.value > 0)+1, <-.molecule_in.bond_type_lst, concat_array(<-.molecule_in.bond_type_lst, <-.bt));
      offset_lst => <-.molecule_in.offset_lst;
		&unit_cell => <-.molecule_in.unit_cell;
		&symmetry => <-.molecule_in.symmetry;
   };
};
