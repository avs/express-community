// This code is part of the CAMARA project
// funded by the Sir Jules Thorne Trust 
// by James S Perrin  Time-stamp: <Wednesday 16/05/01 13:21:54 zzcgujp>

flibrary IsoObjsReduceMods <build_dir="xp_comm_proj/isoored",out_src_file="gen.cxx",out_hdr_file="gen.hxx", cxx_hdr_files="fld/Xfld.h"> 
{
	group _iso_obj<NEvisible=0> 
	{
		int nNodes;
		int nTris;
	};
	
	group IsoObjsReduceParams{
		int nObjs;
		int totObjs;
	};
	
	module iso_objs_reduce<src_file="ior.cxx", cxx_members="void FindNodesAndTris( int node, int obj, int &nNodes, int &nTris);"> 
	{
		Mesh &in_mesh<NEportLevels={2,0}>;
		int get_objs<NEportLevels={2,0}>;
		Mesh out_mesh<NEportLevels={0,2}>;
		int tot_objs<NEportLevels={0,2}>;
		int _vObjs<NEvisible=0>[];
		int _tObjs<NEvisible=0>[];
		int _sortedObjs<NEvisible=0>[tot_objs];
		_iso_obj _objs<NEvisible=0>[tot_objs];
		cxxmethod+req threshold (
			in_mesh+read+req+notify,
			get_objs+read+req+notify,
			out_mesh+write,
			tot_objs+read+write,
			_vObjs+read+write,
			_tObjs+read+write,
			_sortedObjs+read+write,
			_objs+read+write
		);
	};
	
	group IsoObjsReduce2Params{
		int totObjs;
		int show_objs[];
	};
	
	module iso_objs_reduce2<src_file="ior2.cxx", cxx_members="void FindNodesAndTris( int node, int obj, int &nNodes, int &nTris);"> 
	{
		Mesh &in_mesh<NEportLevels={2,0}>;
		int show_objs<NEportLevels={2,0}>[]; // list of objs indices to display
		Mesh out_mesh<NEportLevels={0,2}>;
		int tot_objs<NEportLevels={0,2}>;
		int _vObjs<NEvisible=0>[];
		int _tObjs<NEvisible=0>[];
		int _sortedObjs<NEvisible=0>[tot_objs];
		_iso_obj _objs<NEvisible=0>[tot_objs];
		cxxmethod+req threshold (
			in_mesh+read+req+notify,
			show_objs+read+req+notify,
			out_mesh+write,
			tot_objs+read+write,
			_vObjs+read+write,
			_tObjs+read+write,
			_sortedObjs+read+write,
			_objs+read+write
		);
	};
};
