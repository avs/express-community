flibrary LEAMacs {

	group+OPort LEAUIParams {
		int+Port2 yoffset;
		int+Port2 foffset;
		int+Port2 fw;
		int+Port2 framew;
		int+Port2 shadowType;
		int+Port2 shadowThickness;
		int+Port2 maxw;
	};

	macro AdvectionUI {
		XP_COMM_PROJ.LEA.LEAMods.LEAParams+IPort2 & params;
		XP_COMM_PROJ.LEA.LEAMacs.LEAUIParams+IPort2 & pUI;
		ilink parent;
		UIframe UIframe {
			width => <-.pUI.framew;
			height = 180;
			parent => <-.parent;
			shadowType => <-.pUI.shadowType;
			shadowThickness => <-.pUI.shadowThickness;
			displayFrame = 1;
			y = 0;
		};
		UIlabel Advection_Parameters {
			y = 0;
			width => <-.pUI.maxw;
			color {
				foregroundColor = "white";
				backgroundColor = "blue";
			};
			parent => <-.UIframe;
		};
		UIlabel tr {
			label => "Texture Resolution:";
			y => <-.pUI.yoffset;
			width = 120;
			parent => <-.UIframe;
			color {
				foregroundColor = "black";
			};
		};
		UIfield xresolution {
			value => <-.params.nx;
			x => tr.width;
			y => <-.pUI.yoffset;
			width => <-.pUI.fw;
			parent => <-.UIframe;
			min = 1.;
			max = 10000.;
			mode = "integer";
			message = "x resolution";
		};
		UIfield yresolution {
			value => <-.params.ny;
			x => (xresolution.x + xresolution.width);
			width => <-.pUI.fw;
			y => xresolution.y;
			parent => <-.UIframe;
			min = 1.;
			max = 10000.;
			mode = "integer";
			message = "y resolution";
		};
		group AspectRatio {
			float+nres dimx => Vector_Field.dims[0];
			float+nres dimy => Vector_Field.dims[1];
			float ar => (dimx / dimy);
			float value => switch((is_valid(ar) + 1),1,ar);
		};
		UIslider Integration_Step_Size {
			value => <-.params.integrationStep;
			y => (<-.pUI.yoffset * 2);
			width => <-.pUI.maxw;
			min = 1.;
			max => params.integrationStepMax;
			parent => <-.UIframe;
			mode = "integer";
		};
		UIslider Temporal_Blending {
			value => <-.params.blendCoef;
			y => (<-.pUI.yoffset * 4);
			width => <-.pUI.maxw;
			min = 0.05;
			max = 1.;
			decimalPoints = 2;
			parent => <-.UIframe;
		};
		olink height => UIframe.height;
	};


	macro LICUI {
		XP_COMM_PROJ.LEA.LEAMods.LEAParams+IPort2 & params;
		XP_COMM_PROJ.LEA.LEAMacs.LEAUIParams+IPort2 & pUI;
		ilink parent;
		ilink y;

		UIframe UIframe {
			y => <-.y;
			width => <-.pUI.framew;
			height => (<-.pUI.yoffset * 5);
			parent => <-.parent;
			shadowType => <-.pUI.shadowType;
			shadowThickness => <-.pUI.shadowThickness;
			displayFrame = 1;
		};
		UIlabel LIC_Filter {
			y = 0;
			width => <-.pUI.maxw;
			color {
				foregroundColor = "white";
				backgroundColor = "blue";
			};
			parent => <-.UIframe;
		};
		UItoggle Enable_LIC {
			set => <-.params.licOn;
			y => <-.pUI.yoffset;
			parent => <-.UIframe;
			width = 90;
		};
		UItoggle Normalization {
			set => <-.params.normalizationOn;
			x => Enable_LIC.width;
			y => <-.pUI.yoffset;
			width => (<-.pUI.maxw - x);
			parent => <-.UIframe;
		};
		UItoggle Intensity_Correction {
			set => <-.params.intensityCorrectionOn;
			x => Enable_LIC.width;
			y => (<-.pUI.yoffset * 2);
			width => (<-.pUI.maxw - x);
			parent => <-.UIframe;
		};
		UIslider Kernel_Length {
			value => <-.params.kernelLength;
			y => (<-.pUI.yoffset * 3);
			width => <-.pUI.maxw;
			min = 2.;
			max => (<-.params.integrationStepMax * 2);
			parent => <-.UIframe;
			mode = "integer";
		};
		olink height => UIframe.height;
	};

	macro VMaskUI {
		XP_COMM_PROJ.LEA.LEAMods.LEAParams+IPort2 & params;
		XP_COMM_PROJ.LEA.LEAMacs.LEAUIParams+IPort2 & pUI;
		ilink parent;
		ilink y;
		UIframe UIframe {
			y => <-.y;
			width => <-.pUI.framew;
			height => (<-.pUI.yoffset * 4);
			parent => <-.parent;
			shadowType => <-.pUI.shadowType;
			shadowThickness => <-.pUI.shadowThickness;
			displayFrame = 1;
		};
		UIlabel Velocity_Mask {
			y = 0;
			width => <-.pUI.maxw;
			color {
				foregroundColor = "white";
				backgroundColor = "blue";
			};
			parent => <-.UIframe;
		};
		UItoggle Enable_Mask {
			set => <-.params.maskOn;
			y => <-.pUI.yoffset;
			parent => <-.UIframe;
			width => (<-.pUI.maxw / 2);
		};
		UItoggle Transparency {
			set => <-.params.alphaOn;
			x => Enable_Mask.width;
			y => <-.pUI.yoffset;
			width => (<-.pUI.maxw - x);
			parent => <-.UIframe;
		};
		UIslider Contrast {
			value => <-.params.velocityMaskVersion;
			y => (<-.pUI.yoffset * 2);
			width => <-.pUI.maxw;
			min = 1.;
			max = 3.;
			parent => <-.UIframe;
			mode = "integer";
		};
		olink height => UIframe.height;
	};

	macro ComputationUI {
		XP_COMM_PROJ.LEA.LEAMods.LEAParams+IPort2 & params;
		XP_COMM_PROJ.LEA.LEAMacs.LEAUIParams+IPort2 & pUI;
		ilink parent;
		ilink y;
		UIframe UIframe {
			y => <-.y;
			width => <-.pUI.framew;
			height => (<-.pUI.yoffset * 4);
			parent => <-.parent;
			shadowType => <-.pUI.shadowType;
			shadowThickness => <-.pUI.shadowThickness;
			displayFrame = 1;
		};
		UIlabel Computation {
			y = 0;
			width => <-.pUI.maxw;
			color {
				foregroundColor = "white";
				backgroundColor = "blue";
			};
			parent => <-.UIframe;
		};
		UIlabel nislab {
			label => "Nb Initial Steps:";
			y => <-.pUI.yoffset;
			width = 150;
			parent => <-.UIframe;
			color {
				foregroundColor = "black";
			};
			alignment = "right";
		};
		UIfield nis {
			x => nislab.width;
			y => <-.pUI.yoffset;
			width => <-.pUI.fw;
			parent => <-.UIframe;
			min = 1.;
			max = 30.;
			value => <-.params.nInitialSteps;
			mode = "integer";
			message = "At first init and when resolution changes";
		};
		UIlabel nsolab {
			label => "Nb Steps at Once:";
			y => (<-.pUI.yoffset * 2);
			width => nislab.width;
			parent => <-.UIframe;
			color {
				foregroundColor = "black";
			};
			alignment = "right";
		};
		UIfield nso {
			x => nsolab.width;
			y => (<-.pUI.yoffset * 2);
			width => <-.pUI.fw;
			parent => <-.UIframe;
			min = 1.;
			max = 10.;
			value => <-.params.nStepsAtOnce;
			mode = "integer";
			message = "Nb integration steps at each computation";
		};
		UItoggle Automatic_Update {
			set = 1;
			y => (<-.pUI.yoffset * 3);
			parent => <-.UIframe;
			width => nislab.width;
		};
		UItoggle run {
			x => Automatic_Update.width;
			y => (<-.pUI.yoffset * 3);
			parent => <-.UIframe;
			width => (<-.pUI.maxw - Automatic_Update.width);
		};
		GMOD.loop looprun {
			run => <-.run.set;
			cycle = 1;
			start_val = 0.;
			end_val = 1.;
			incr = 1.;
		};
		GMOD.copy_on_change copy_on_change2 {
			trigger => looprun.count;
			input => <-.run.set;
		};
		GMOD.copy_on_change trigger {
			input => <-.Automatic_Update.set + <-.copy_on_change2.output;
			output => <-.params.trigger;
		};
	};


	macro LEAUI{
		XP_COMM_PROJ.LEA.LEAMods.LEAParams+IPort2 & params;
		UImod_panel LEA_UI {
			title => name_of(<-.<-.<-,1);
		};
		
		XP_COMM_PROJ.LEA.LEAMacs.LEAUIParams pUI {
			int yoffset = 30;
			int foffset = 10;
			int fw = 55;
			int framew => LEA_UI.width;
			int shadowType = 2;
			int shadowThickness = 2;
			int maxw => (framew - (2 * shadowThickness));
		};
		XP_COMM_PROJ.LEA.LEAMacs.AdvectionUI AdvectionUI {
			params => <-.params;
			pUI => <-.pUI;
			parent => <-.LEA_UI;
		};
		XP_COMM_PROJ.LEA.LEAMacs.LICUI LICUI {
			params => <-.params;
			pUI => <-.pUI;
			parent => <-.LEA_UI;
			y => <-.AdvectionUI.height + <-.pUI.foffset;
		};
		XP_COMM_PROJ.LEA.LEAMacs.VMaskUI VMaskUI {
			params => <-.params;
			pUI => <-.pUI;
			parent => <-.LEA_UI;
			y => <-.LICUI.y + <-.LICUI.height + <-.pUI.foffset;
		};
		XP_COMM_PROJ.LEA.LEAMacs.ComputationUI ComputationUI {
			params => <-.params;
			pUI => <-.pUI;
			parent => <-.LEA_UI;
			y => <-.VMaskUI.y + <-.VMaskUI.height + <-.pUI.foffset;
		};
	
	};

	macro LEAFunc{
		Mesh_Struct+Node_Data+IPort2 &Vector_Field;
		
		XP_COMM_PROJ.LEA.LEAMods.LEAParams+IPort2 & params;
		
		XP_COMM_PROJ.LEA.LEAMods.LEACore LEACore {
			trigger => <-.params.trigger;
			nx => <-.params.nx;
			ny => <-.params.ny;
			integrationStepMax => <-.params.integrationStepMax;
			integrationStep => <-.params.integrationStep;
			swapPercentage => <-.params.swapPercentage;
			blendCoef => <-.params.blendCoef;
			nInitialSteps => <-.params.nInitialSteps;
			nStepsAtOnce => <-.params.nStepsAtOnce;
			licOn => <-.params.licOn;
			kernelLength => <-.params.kernelLength;
			kernelShape => <-.params.kernelShape;
			intensityCorrectionOn => <-.params.intensityCorrectionOn;
			normalizationOn => <-.params.normalizationOn;
			maskOn => <-.params.maskOn;
			velocityMaskVersion => <-.params.velocityMaskVersion;
			alphaOn => <-.params.alphaOn;
			vf => <-.Vector_Field;
		};

		MODS.set_xform set_xform_img {
			in_field1 => <-.LEACore.lea_img;
			in_field2 => <-.Vector_Field;
		};
		IPfld+OPort2 &lea_img<NEcolor2=16776960> {
			node_data {
				byte values[nvals][veclen];
				byte+opt null_value;
				byte min;
				byte max;
				byte min_vec[veclen];
				byte max_vec[veclen];
			};
		} => .set_xform_img.out_fld;
		
		GDM.DataObject lea_obj {
			in => <-.set_xform_img.out_fld;
		};
		olink obj => lea_obj.obj;

	#ifdef NOTDEF
		Field_Unif+OPort texture_base {
			dims = {2,2};
			ndim = 2;
			points => <-.Vector_Field.points;
			nspace = 2;
			node_data = {
			{
				values={0,0,1,0,0,1,1,1},veclen=2,,,,,,,,id=670,,
			}};
			nnode_data = 1;
		};
		MODS.set_xform set_xform_tex {
			in_field1 => <-.texture_base;
			in_field2 => <-.Vector_Field;
		};

		GDM.DataObject textured_fld {
			in => <-.set_xform_tex.out_fld;
			texture_in => <-.LEACore.lea_img;
			Texture {
				alpha => switch(<-.<-.params.alphaOn+1, "Replace", "Modulate");
			};
		};
	#else
		XP_COMM_PROJ.LEA.LEAMods.texture_base texture_base {
			dims => <-.Vector_Field.dims;
			uv[][];
		};
		FLD_MAP.node_uv node_uv {
			in_data => <-.texture_base.uv;
		};
		FLD_MAP.combine_mesh_data combine_mesh_data {
			in_mesh => <-.Vector_Field;
			in_nd => <-.node_uv.out;
		};
		GDM.DataObject textured_fld {
			in => <-.combine_mesh_data.out;
			texture_in => <-.LEACore.lea_img;
			Texture {
				filter = "Bilinear";
				alpha => switch(<-.<-.params.alphaOn+1, "Replace", "Modulate");
			};
		};
	#endif	
		olink textured_obj => .textured_fld.obj;
	};

	macro LEA_Advector {
		ilink Vector_Field;
		XP_COMM_PROJ.LEA.LEAMods.LEAParams params {
			int trigger = 0;
			int nx = 256;
			int ny = 256;
			int integrationStepMax = 20;
			float integrationStep = 5.;
			float swapPercentage = 3.;
			float blendCoef = 0.1;
			int nInitialSteps = 10;
			int nStepsAtOnce = 1;
			int licOn = 0;
			int kernelLength = 5;
			int kernelShape = 1;
			int intensityCorrectionOn = 1;
			int normalizationOn = 0;
			int maskOn = 0;
			int velocityMaskVersion = 2;
			int alphaOn = 0;
		};
		XP_COMM_PROJ.LEA.LEAMacs.LEAUI LEAUI {
			params => <-.params;
		};
		XP_COMM_PROJ.LEA.LEAMacs.LEAFunc LEAFunc {
			Vector_Field => <-.Vector_Field;
			params => <-.params;
		};
		olink lea_img => LEAFunc.lea_img;
		olink obj => LEAFunc.obj;
		olink textured_obj => LEAFunc.textured_obj;
	};	

};
