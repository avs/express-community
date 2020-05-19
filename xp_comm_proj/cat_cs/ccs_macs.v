
flibrary CatCSMacs
{
	macro cat_cell_sets
	{
		ilink in;
		XP_COMM_PROJ.CatCellSets.CatCSMods.CatCellSets CatCellSets
		{
			mesh_in => <-.in;
		};
		XP_COMM_PROJ.CatCellSets.CatCSMods.Add_Data_to_Mesh Add_Data_to_Mesh
		{
			mesh_in => <-.CatCellSets.mesh_out;
			data_in => <-.in;
		};
		GDM.DataObject DataObject {
			in => <-.Add_Data_to_Mesh.out;
		};	
		olink out => .Add_Data_to_Mesh.out;
		olink obj => DataObject.obj;
	};
};
