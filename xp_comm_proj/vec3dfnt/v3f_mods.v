flibrary Vector3dFontsMods<build_dir="xp_comm_proj/vec3dfnt", out_hdr_file="gen.h">
{

	// Parameter block - Vector3dFontsParams

	group+OPort Vector3dFontsParams {
   		string+Port2 label;
   		float+Port2 kerning;
   		float+Port2 stroke_width;
   		int+Port2 font;
   		int+Port2 linestyle;
	};


	// Module - Vector3dFontsCore

	module Vector3dFontsCore<c_src_files="vec_lab.c vecfonts.c hfont.c"> {

    	string+IPort2 Label;
		float+IPort2 Kerning;
		float+IPort2 Stroke_Width;
		int+IPort2 Font;
		int+IPort2 LineStyle;

		omethod+notify_inst+req update(
			.Label+req+read+notify,
			.Kerning+read+notify,
			.Stroke_Width+read+notify,
			.Font+read+notify,
			.LineStyle+read+notify,
			.out+write
		) = "vector_label";
		
		Mesh+Node_Data+OPort2 out;
	}; 
};
