
flibrary ProjectNodeDataMacs 
{
	macro ProjectNodeDataFunc {
		ilink in;
		XP_COMM_PROJ.ProjectNodeData.ProjectNodeDataMods.ProjectNodeDataParams+IPort2 &params;
		XP_COMM_PROJ.ProjectNodeData.ProjectNodeDataMods.ProjectNodeDataCore ProjectNodeDataCore {
			in => <-.in;
			axis => <-.params.axis;
			method => <-.params.method;
		};
		GDM.DataObject DataObject {
			in => <-.ProjectNodeDataCore.out;
		};
		olink out => ProjectNodeDataCore.out;
		olink obj => DataObject.obj;
	};
	
	macro ProjectNodeDataUI {
		XP_COMM_PROJ.ProjectNodeData.ProjectNodeDataMods.ProjectNodeDataParams+IPort2 &params;
		UImod_panel UImod_panel {
			title => name_of(<-.<-.<-,1);
		};
		UIslider axis {
			parent => <-.UImod_panel;
			value => <-.params.axis;
			y = 0;
			max = 2.;
			min = 0.;
			width => parent.clientWidth;
			mode = "integer";
		};
		UIradioBoxLabel Mapping {
			parent => <-.UImod_panel;
			labels => <-.params.method.choices;
			selectedItem => <-.params.method;
			width => parent.clientWidth;
			y => ((<-.axis.y + <-.axis.height) + 10);
		};
	};
	
	macro ProjectNodeData {
		ilink in;
		XP_COMM_PROJ.ProjectNodeData.ProjectNodeDataMods.ProjectNodeDataParams params
		{
			axis = 0;
			method = "Maximum";
		};	
		XP_COMM_PROJ.ProjectNodeData.ProjectNodeDataMacs.ProjectNodeDataUI ProjectNodeDataUI {
			params => <-.params;
		};
		XP_COMM_PROJ.ProjectNodeData.ProjectNodeDataMacs.ProjectNodeDataFunc ProjectNodeDataFunc {
			in => <-.in;
			params => <-.params;
		};
		olink out => ProjectNodeDataFunc.out;
		olink obj => ProjectNodeDataFunc.obj;
	};

};
