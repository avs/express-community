
flibrary StreamTimeMods <build_dir="xp_comm_proj/streamt",
                         out_src_file="gen.cxx",
                         out_hdr_file="gen.hxx">
{

   // custom mesh with optional cell and node data
   Cell_Set VIScell_set {
      int+opt+notify ncell_data;
      Data_Array+opt+notify cell_data[.ncell_data];
   };

   Grid+Xform VISmesh {
      int+req+notify ncell_sets;
      VIScell_set cell_set[.ncell_sets];
      int+opt+notify nnode_data;
      Data_Array+opt+notify node_data[.nnode_data] {
         nvals => <-.nnodes;
      };
   };


   // StreamTimeParams group
   group+OPort StreamTimeParams {
      int+Port2   axis<export=2>;
      int+Port2   do_transform<export=2>;
      float+Port2 z_scale<export=2>;
      int+Port2   resolution<export=2>;
      int+Port2   normalise<export=2>;
      float+Port2 radius_scale<export=2>;
      int+Port2   node_cell<export=2>;
      int+Port2   node_data_comp<export=2>;
      int+Port2   cell_data_comp<export=2>;
      int+Port2   col_data_comp<export=2>;
   };


   // tube low-level module
   module StreamTimeCore <src_file="streamt.cxx",
                          libdeps="FLD",
                          cxx_hdr_files="fld/Xfld.h">
   {

      VISmesh+IPort2 &in<export=2>;
      int+IPort2     axis<export=2>;
      int+IPort2     xform<export=2>;
      float+IPort2   zscale<export=2>;
      int+IPort2     resolution<export=2>;
      int+IPort2     normalise<export=2>;
      float+IPort2   scale<export=2>;
      int+IPort2     node_cell<export=2>;
      int+IPort2     node_data_comp<export=2>;
      int+IPort2     cell_data_comp<export=2>;
      int+IPort2     col_data_comp<export=2>;

      cxxmethod+req update(
         .in+notify+read+req,
         .axis+notify+read+req,
         .xform+notify+read+req,
         .zscale+notify+read+req,
         .resolution+notify+read+req,
         .normalise+read+notify+req,
         .scale+read+notify+req,
         .node_cell+read+notify+req,
         .node_data_comp+read+notify+req,
         .cell_data_comp+read+notify+req,
         .col_data_comp+notify+read+req,
         .out+write,
         .err+write,
         .err_str+write
      );

      Field+OPort2   out<export=2>;
      int+OPort2     err<export=2>;
      string+OPort2  err_str<export=2>;
   };

};

