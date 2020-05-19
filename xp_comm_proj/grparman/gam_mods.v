// gam_mods.v
// by James S. Perrin Time-stamp: <Monday 10/09/01 16:00:50 zzcgujp>
library GroupArrManMods<build_dir="xp_comm_proj/grparman",out_src_file="gam_gen.cxx",out_hdr_file="gam_gen.h", cxx_hdr_files="fld/Xfld.h"> {
	group+OPort grouparrmanParams {
		int+Port2 selected;
		int+Port2 other;
		int+Port2 add;
		int+Port2 replace;
		int+Port2 swap;
		int+Port2 delete;
		int+Port2 nGroups;
	};
	
	module group_array_manager<src_file="grparman.cxx"> {
		group+IPort2 &in;
		int+IPort2 selected;
		int+IPort2 other;
		int+IPort2 add;
		int+IPort2 replace;
		int+IPort2 swap;
		int+IPort2 delete;
		int+OPort2 nGroups;
		group+IPort2 &farm; // fields/farm getit?
		group+OPort2 &out[nGroups];
		cxxmethod+req add_group (
			add+req+notify,
			in+read+req,
			farm+write,
			nGroups+read+write,
			out+write
		);
		cxxmethod+req replace_group (
			replace+req+notify,
			in+read+req,
			selected+read+req,
			farm+read+write,
			nGroups+read+write,
			out+write
		);
		cxxmethod+req swap_group (
			swap+req+notify,
			selected+read+req,
			other+read+req,
			farm+read+write,
			nGroups+read+write,
			out+read+write
		);
		cxxmethod+req delete_group (
			delete+req+notify,
			selected+read+req,
			farm+read+write,
			nGroups+read+write,
			out+read+write
		);
	};
};
