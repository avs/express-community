"$XP_PATH<0>\\v\\templ.v" Templates
{
	library ISmods
	{
		module ISWrapperMod
		<
			out_hdr_file = "ISWrapperMod_gen.hxx",
			out_src_file = "ISWrapperMod_gen.cxx",
			src_file = "ISWrapperMod.cxx",
			build_dir = "src"
		>
		{
			int+IPort2 OpenDeviceTrigger;

			int+IPort2 CloseDeviceTrigger;

			int+IPort2 ShowStatsTrigger;

			int+OPort2 UpdateXformTrigger;

			int DeviceState;

			float+OPort2 Xrot = 0.0;
			float+OPort2 Yrot = 0.0;
			float+OPort2 Zrot = 0.0;
			float+OPort2 Xtrans = 0.0;
			float+OPort2 Ytrans = 0.0;
			float+OPort2 Ztrans = 0.0;

			cxxmethod+req OpenDevice(
				OpenDeviceTrigger+notify,
				DeviceState+write
			);

			cxxmethod+req CloseDevice(
				CloseDeviceTrigger+notify,
				DeviceState+write
			);

			cxxmethod+req ShowStats(
				ShowStatsTrigger+notify,
				DeviceState+write
			);

			cxxmethod+req UpdateXform(
				UpdateXformTrigger+notify,
				Xrot+write,
				Yrot+write,
				Zrot+write,
				Xtrans+write,
				Ytrans+write,
				Ztrans+write
			);
		};

		macro ISWrapper
		{
			ISWrapperMod ISWrapperMod;
		};
	};
