
flibrary CFDValsMods <build_dir="xp_comm_proj/cfdvals",
	              out_hdr_file="gen.h",
		      out_src_file="gen.cxx"> {

  // define the parameter block group

  group CFDValsParams <NEportLevels={0,1}> {
    double+Port2 gamma;
    double+Port2 gas_constant;
    int+Port2 do_energy;
    int+Port2 do_pres;
    int+Port2 do_enthalpy;
    int+Port2 do_mach;
    int+Port2 do_temp;
    int+Port2 do_totpres;
    int+Port2 do_tottemp;
  };

  // define the low-level modules

  module CFDVals_comb_mesh_and_data {
    Mesh+IPort2 &in_mesh ;
    Node_Data+nres+IPort2 &in_nd;
    Mesh+Node_Data+OPort2 &out => merge(in_nd, in_mesh);
  };

  module CFDValsNodeData <src_file="cfdvals.c"> {
    Node_Data+read+notify+req &in <NEportLevels={2,0}> [];
    CFDValsParams &CFDValsParams <NEportLevels={2,0}>;

    double+read+notify+req gamma => CFDValsParams.gamma;
    double+read+notify+req gas_constant => CFDValsParams.gas_constant;
    int+read+notify+req do_energy => CFDValsParams.do_energy;
    int+read+notify+req do_pres => CFDValsParams.do_pres;
    int+read+notify+req do_enthalpy => CFDValsParams.do_enthalpy;
    int+read+notify+req do_mach => CFDValsParams.do_mach;
    int+read+notify+req do_temp => CFDValsParams.do_temp;
    int+read+notify+req do_totpres => CFDValsParams.do_totpres;
    int+read+notify+req do_tottemp => CFDValsParams.do_tottemp;

    int nblocks => array_size(in);
    Node_Data+write+nonotify out[nblocks];

    CFDVals_comb_mesh_and_data CFDVals_comb_mesh_and_data[nblocks] {
      int index => index_of(CFDVals_comb_mesh_and_data);
      in_mesh => <-.in[index];
      in_nd => <-.out[index];
    };

    mlink+OPort2 fields => CFDVals_comb_mesh_and_data.out;

    omethod+notify_val+notify_inst update<status=1> = "cfd_values_update";
  };
};
