
flibrary CollapseMods <build_dir="xp_comm_proj/collapse",
//                     process="user",
                       out_hdr_file="gen.hxx",
                       out_src_file="gen.cxx">
{

   // define the parameter block group
   group+OPort CollapseParams {
      enum+Port2 operation {
         choices = {"collapse", "slice", "collapse_except", "collapse_only"};
      } = "collapse";
      int+Port2 axis = 0;
      int+Port2 planes[];
   };


   // define the low-level module

   module CollapseFieldCore <src_file="collapse.cxx">
   {
      double+IPort2 values[][];
      int+IPort2    dims[];
      int           veclen[] => array_dims(values);

      CollapseParams+IPort2 &CollapseParams;

      int operation => CollapseParams.operation;
      int axis      => CollapseParams.axis;
      int planes[]  => CollapseParams.planes;

      cxxmethod+req+notify_inst update (
         operation+read+notify+req,
         axis+read+notify+req,
         planes+read+notify,
         values+read+notify+req,
         dims+read+notify+req,
         veclen+read+notify+req,
         out_dims+write,
         out_values+write
      );

      int+OPort2    out_dims[switch(array_size(dims),1,1,2)];
      double+OPort2 out_values[prod(out_dims)][veclen[0]];
  };

};

