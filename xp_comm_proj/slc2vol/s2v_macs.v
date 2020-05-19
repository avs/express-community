flibrary Slice_to_Volume_Macs<compile_subs=0>
{
	macro Slice_to_Volume_UI {
		XP_COMM_PROJ.Slice_to_Volume.Slice_to_Volume_Mods.Slice_to_Volume_Params+IPort2 & params;
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
			width => (parent.clientWidth - 10);
		};
		UIslider axis {
			parent => <-.UImod_panel;
			value => <-.params.axis;
			y = 0;
			width => parent.clientWidth;
			min = 0.;
			max => array_size(<-.params.dims)-1;
			mode = "integer";
		};
		UIslider Slice {
			parent => <-.UImod_panel;
			value => <-.params.Slice;
			y => ((<-.axis.y + <-.axis.height) + 4);
			max => <-.params.dims[<-.params.axis]-1;
			min = 0.;
			width => parent.clientWidth;
			mode = "integer";
		};
		UItoggle use_value {
			parent => <-.UImod_panel;
			set => <-.params.use_value;
			y => ((<-.Slice.y + <-.Slice.height) + 10);
			width => parent.clientWidth;
		};
		UIlabel Value<NEx=110.,NEy=275.> {
			parent => <-.UImod_panel;
			y => ((<-.use_value.y + <-.use_value.height) + 10);
			x = 10;
			width => ((parent.clientWidth / 3) - 10);
			active => <-.params.use_value;
			alignment = "left";
		};
		UIfield UIfield<NEx=407.,NEy=352.> {
			parent => <-.UImod_panel;
			value => <-.params.value;
			y => <-.Value.y;
			x => (<-.Value.x + <-.Value.width);
			width => (2 * (parent.clientWidth / 3));
			active => <-.Value.active;
			nullString = "0";
		};
	};

	macro Slice_to_Volume_FUNC 
	{
		ilink In_Slice;
		ilink Ref_Volume;
		ilink In_Volume;
		XP_COMM_PROJ.Slice_to_Volume.Slice_to_Volume_Mods.Slice_to_Volume_Params+IPort2 & params;
		XP_COMM_PROJ.Slice_to_Volume.Slice_to_Volume_Mods.Slice_to_Volume_Core Slice_to_Volume_Core
		{
			axis => <-.params.axis;
			Slice => <-.params.Slice;
			value => <-.params.value;
			use_value => <-.params.use_value;
			In_Slice => <-.In_Slice;
			Ref_Volume => <-.Ref_Volume;
			Out_Volume => <-.In_Volume;
		};
		olink Out_Volume => Slice_to_Volume_Core.Out_Volume;
		GDM.DataObject DataObject{
			in => <-.Slice_to_Volume_Core.Out_Volume;
		};
		olink Out_Object => DataObject.obj;
	};
	
	macro Slice_to_Volume {
		ilink In_Slice;
		Mesh_Unif+Node_Data+IPort2 & Ref_Volume;
		Mesh_Unif+Node_Data+OPort Volume{
			dims => Ref_Volume.dims;
			ndim => Ref_Volume.ndim;
			nspace => Ref_Volume.nspace;
			points => Ref_Volume.points;
			nnode_data = 1;
			node_data = {
				{
					veclen=1,,,,,,,
				}
			};
		};
		XP_COMM_PROJ.Slice_to_Volume.Slice_to_Volume_Mods.Slice_to_Volume_Params params{
			axis = 0;
			Slice => dims[axis]/2;
			use_value = 0;
			value = 1;
			dims => <-.Ref_Volume.dims;			
		};
		Slice_to_Volume_UI Slice_to_Volume_UI {
			params => <-.params;
		};

		Slice_to_Volume_FUNC Slice_to_Volume_FUNC {
			params => <-.params;
			In_Volume => <-.Volume;
			Ref_Volume => <-.Ref_Volume;
			In_Slice => <-.In_Slice;
		};
		olink out =>  Slice_to_Volume_FUNC.Out_Volume;
		olink obj =>  Slice_to_Volume_FUNC.Out_Object;
	};
	
};
