flibrary LEAMods <cxx_name="",
	out_hdr_file="gen.h", 
	out_src_file="gen.cxx"> 
{

	group+OPort LEAParams {
		int+Port2 trigger;
		int+Port2 nx;
		int+Port2 ny;
		int+Port2 integrationStepMax;
		float+Port2 integrationStep;
		float+Port2 swapPercentage;
		float+Port2 blendCoef;
		int+Port2 nInitialSteps;
		int+Port2 nStepsAtOnce;
		int+Port2 licOn;
		int+Port2 kernelLength;
		int+Port2 kernelShape;
		int+Port2 intensityCorrectionOn;
		int+Port2 normalizationOn;
		int+Port2 maskOn;
		int+Port2 velocityMaskVersion;
		int+Port2 alphaOn;
	};

	module LEACore<src_file="lea.cxx",
		cxx_hdr_files="fld/Xfld.h",
		libdeps="FLD"> 
	{
		cxxmethod+notify_inst   OnInstance(
			.local_ptr+write,
			.vx_ptr+write,
			.vy_ptr+write,
			.vmag_ptr+write,
			.static_vars+write
		);

		cxxmethod+notify_deinst OnDeInstance(
			.local_ptr+read,
			.vx_ptr+read,
			.vy_ptr+read,
			.vmag_ptr+read,
			.static_vars+read
		);

		cxxmethod+req Compute(
			.trigger+read+notify+req,
			.nx+read+write+notify+req,
			.ny+read+write+notify+req,
			.integrationStepMax+read+notify+req,
			.integrationStep+read+notify+req,
			.swapPercentage+read+notify+req,
			.blendCoef+read+notify+req,
			.nInitialSteps+read+req,
			.nStepsAtOnce+read+req,
			.licOn+read+notify+req,
			.kernelLength+read+notify+req,
			.kernelShape+read+notify+req,
			.intensityCorrectionOn+read+notify+req,
			.normalizationOn+read+notify+req,
			.maskOn+read+notify+req,
			.velocityMaskVersion+read+notify+req,
			.alphaOn+read+notify+req,
			.vmax+read+notify,
			.vf+read+notify+req,
			.static_vars+read,
			.lea_img+write
		);

		int   trigger;
		int   nx;
		int   ny;
		int   integrationStepMax;
		float integrationStep;
		float swapPercentage;
		float blendCoef;
		int   nInitialSteps;
		int   nStepsAtOnce;
		int   licOn;
		int   kernelLength;
		int   kernelShape;
		int   intensityCorrectionOn;
		int   normalizationOn;
		int   maskOn;
		int   velocityMaskVersion;
		int   alphaOn;
		float vmax;        // global max of velocity magnitude

		Mesh_Struct+Node_Data+Float+IPort2 &vf;
		IPfld+Byte+OPort2 lea_img;
		
		ptr+nosave local_ptr<NEvisible=0>;
		ptr+nosave vx_ptr<NEvisible=0>;
		ptr+nosave vy_ptr<NEvisible=0>;
		ptr+nosave vmag_ptr<NEvisible=0>;
		ptr+nosave static_vars<NEvisible=0>;
	};

	module texture_base<src_file="texture_base.cxx">
	{
		cxxmethod+req ComputeTextureBase(
			.dims+read+notify+req,
			.uv+write
		);
		int+IPort2    dims[];
		float+OPort2  uv[][];
	};

};
