flibrary CutPolylinesMacs {
	macro CutPolylines {
		MODS.cut_plane+IPort2 &in_cut;
		link in_level => in_cut.Cut.DVcut.level;
		link in_plane => in_cut.out_plane;
		ilink in_polyline;
		Field Field<NEportLevels={0,1}> {
			coordinates {
				values = {0.};
			};
			nspace = 1;
			nnodes = 1;
		};
		XP_COMM_PROJ.CutPolylines.CutPolylinesMods.CutPolylinesTest CutPolylinesTest {
			value+nres => in_polyline.nspace;
			in_poly => <-.in_polyline;
			in_field => <-.Field;
		};		  
		XP_COMM_PROJ.CutPolylines.CutPolylinesMods.CutPolylinesCore CutPolylinesCore {
			level => <-.in_level;
			in_dists => <-.cut.Cut.DVplane_dist.out;
			in_poly => <-.in_polyline;
		}; 
		MODS.cut cut {
			in_field => <-.CutPolylinesTest.out;
			in_plane => <-.in_plane;
			Cut {
				DVcut {
					in => ;
					cut_comp => ;
					above => ;
				};
				DVplane_dist {
					out<NEportLevels={0,4}>;
				};
			};
			UIframe UIpanel;
		};

		GDM.DataObject cut_polyline_obj {
			in => <-.CutPolylinesCore.out;
		};
		olink out => CutPolylinesCore.out;
		olink obj => cut_polyline_obj.obj;
	};

};


