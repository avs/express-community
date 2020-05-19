/* This module includes color specifications and designs 
   developed by Cynthia Brewer (http://colorbrewer.org/).*/

flibrary CB_ColorMapMods <build_dir="xp_comm_proj/cb_cmap", 
                          out_src_file="gen.cxx",
                          out_hdr_file="gen.h"
			  >
{
	group color_map {
		int ncolors;
		string category;
		string name;
		int colors[ncolors][3] ;	
	};
	
	enum cmap_type_sel{
			choices = {"Name","No of Colors"};
	};
	
	group CB_ColorMapParams {
		int ncats;
		string categories[ncats];
		cmap_type_sel map_type;
		int catidx = 0;
		int ncatmaps;
		string catmaps[ncatmaps];
		int mapidx = 0;
		int ncmap;
		color_map color_map[ncmap];
		int colidx = 0;
		int stepped = 0;
	};
	
	module CB_ColorMapCore <src_file="cb_cmap.c"> {
		string+IPort2 cbFile = "xp_comm_proj/cb_cmap/ColorBrewer_all_schemes_RGBonly3.csv";
		int ncats;
		string categories[ncats];
		int catidx;
		int mapidx;
		cmap_type_sel selector;
		omethod+notify_inst+req inst (
			cbFile+read+notify+req,
			ncats+write,
			categories+write+nosave,
			cmapptr+write+nosave
		)="CB_ColorMap_Inst";
		ptr cmapptr;
		omethod+notify_inst+req update (
			cmapptr+read+req,
			selector+read+notify,
			catidx+read+notify,
			mapidx+read+notify,
			ncatmaps+write,
			catmaps+write,
			ncmap+write,
			colormaps+write
		)="CB_ColorMapCore";
		omethod+notify_deinst+req destroy (
			cmapptr+read+req
		)="CB_ColorMap_Destroy";
		int ncatmaps;
		string catmaps[ncatmaps];
		int ncmap;
		color_map colormaps[ncmap];
	};

};
