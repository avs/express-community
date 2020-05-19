
flibrary VolumeToContourMods<build_dir="xp_comm_proj/vcontour",
  		          out_hdr_file="gen.h"> {

  group+OPort VolumeToContourParams 
  {
    int+Port2 axis;
    int+Port2  color;
    int+Port2  first_slice;
    int+Port2 last_slice;
    int+Port2 slice_spacing;
	int+Port2 nslices;	
	int+Port2  ncontours;
	float+Port2	data_min;
	float+Port2  level_min;
	float+Port2	data_max;
	float+Port2  level_max;
  };

  module VolumeToContour<src_file="vcontour.c"> {
    omethod+req UpdateField(.axis+read+notify,
                            .color+read+notify,
                            .first_slice+read+notify,
                            .last_slice+read+notify,
                            .slice_spacing+read+notify,
                            .level+read+req+notify,
                            .Input_Field+read+req+notify,
                            .Temp_Field+read+write,
                            .Temp_Contour+read+write,
                            .Contours+write
                            ) = "GenContours";

    VolumeToContourParams+IPort2 &Params;

    int axis =>Params.axis;
    int color =>Params.color;
    int first_slice =>Params.first_slice;
    int last_slice =>Params.last_slice;
    int slice_spacing =>Params.slice_spacing;
    float+IPort2 level[] => init_array(Params.ncontours,Params.level_min,Params.level_max);
    Mesh_Unif+Node_Data Temp_Field;
    Mesh+Node_Data Temp_Contour;
    Mesh_Unif+Node_Data+IPort2 &Input_Field;
    Mesh+Node_Data+OPort2 Contours;
  };
};
