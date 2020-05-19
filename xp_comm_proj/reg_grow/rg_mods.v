
flibrary RegionGrowMods <build_dir="xp_comm_proj/reg_grow",
                         out_hdr_file="gen.h",
                         out_src_file="gen.c"> {

    group+OPort RegionGrowParams {
       int+Port2   go;
       float+Port2 mean_min;
       float+Port2 mean_max;
       float+Port2 stddev;
       int+Port2   max;
       int+Port2   reg_threshold;
    };

    module RegionGrowCore  {
       Mesh_Unif+Node_Data+IPort2  &in;
       Mesh+Node_Data+IPort2       &probe_in;

       float+IPort2                coherance[];
       int+IPort2                  reg_threshold;
       int+IPort2                  go;

       omethod+notify_inst+req update<src_file="reg_grow.c">(
          in+read+req,
          probe_in+read+req,
          coherance+read+req,
          reg_threshold+read+req,
          go+read+notify+req,
          out+write
       ) = "region_grow_update";

       Mesh_Unif+Node_Data+OPort2 out;
    };


//========================================================================================


    module GenerateGuassianProbs {
       float+IPort2 mean_min;
       float+IPort2 mean_max;
       float+IPort2 stddev;
       int+IPort2   max;

       omethod+notify_inst+req update<src_file="genguas.c", status=1>(
             mean_min+read+notify+req,
             mean_max+read+notify,
             stddev+read+notify+req,
             max+read+notify+req,
             probs+write
       ) = "generate_gausian";

       float+OPort2 probs[];
    };


//========================================================================================


    group+OPort AccumulateParams {
       int+Port2 go;
       int+Port2 reset;
       int+Port2 dims[];
       int+Port2 dims_choice;
    };


    module AccumulateCore {
       Mesh_Unif+Node_Data+IPort2 &in_fld;

       AccumulateParams+IPort2    &params;

       int go          => params.go;
       int reset       => params.reset;
       int dims[]      => params.dims[];
       int dims_choice => params.dims_choice;

       int dtype;


       omethod+notify_inst+req update<src_file="rg_accum.c">(
          in_fld+read+notify+req,
          in_fld.nnodes+req,
          dims_choice+read+notify+req,
          go+read+notify+req,
          reset+read+notify+req,
          dtype+read+write,
          out_fld+read+write
       ) = "rg_accum_update";

       omethod+notify_inst+req update_dims<src_file="rg_acc_d.c">(
          in_fld+read+notify+req,
          in_fld.nnodes+req,
          dims+read+notify+req,
          dims_choice+read+notify+req,
          go+read+notify+req,
          reset+read+notify+req,
          dtype+read+write,
          out_fld+read+write
       ) = "rg_accum_update_dims";


       Mesh_Unif+Node_Data+OPort2 out_fld;
    };   

};

