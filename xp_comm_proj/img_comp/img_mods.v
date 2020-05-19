flibrary ImageCompareMods<build_dir="xp_comm_proj/img_comp", out_hdr_file="gen.h"> {

	group+OPort ImageCompareParams 
	{
		int+Port2 Mode;
		boolean+Port2 Switch;
		float+Port2 Value;
	};

	module ImageCompareCore <src_file="img_comp.c"> {

		Mesh_Unif+Node_Data+Dim2+IPort2 &In_Data_1;
		DatamapTempl+IPort2 &Datamap_1;
		Mesh_Unif+Node_Data+Dim2+IPort2 &In_Data_2;
		DatamapTempl+IPort2 &Datamap_2;

		ImageCompareParams+IPort2 &params;
		int Switch=>params.Switch;
		int Mode=>params.Mode;
		float Value=>params.Value;

		omethod+notify_inst+req update(
			.In_Data_1+read+notify+req,
			.Datamap_1+read+notify+req,
			.In_Data_2+read+notify+req,
			.Datamap_2+read+notify+req,
			.Switch+read+notify+req,
			.Mode+read+notify+req,
			.Value+read+notify,
			.Out_Data+write
		) = "Image_Compare";

		Mesh_Unif+Node_Data+OPort2 Out_Data {
			ndim => In_Data_1.ndim;
			dims => In_Data_1.dims;
			coordinates {
				id => In_Data_1.coordinates.id;
			};
			points => In_Data_1.points;
			nspace => In_Data_1.nspace;
		};
	};
};
