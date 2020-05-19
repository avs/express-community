
flibrary CombineDataMods <build_dir="xp_comm_proj/combdata",
//                        process="user",
                          out_hdr_file="gen.hxx",
                          out_src_file="gen.cxx",
                          cxx_hdr_files="fld/Xfld.h">
{

   group+OPort CombineDataParams {
      int+Port2   comp0 = 0;
      int+Port2   comp1 = 0;
      int+Port2   comp2 = 0;
      int+Port2   constant0on = 0;
      int+Port2   constant1on = 0;
      int+Port2   constant2on = 0;
      float+Port2 constant0val = 1.;
      float+Port2 constant1val = 1.;
      float+Port2 constant2val = 1.;
      int+Port2   invert0on = 0;
      int+Port2   invert1on = 0;
      int+Port2   invert2on = 0;
   };


   group CMBD_extract_data_array {
      Node_Data+IPort2 &in;
      int+IPort2       comp;
      prim+OPort2      data[] => in.node_data[.comp].values;
   };


   module DVcombine_data_to_HSV <src_file="comb_hsv.cxx"> {

      float+IPort2 data0[];
      float+IPort2 data1[];
      float+IPort2 data2[];

      CombineDataParams+IPort2 &params;

      int   constant0on => params.constant0on;
      int   constant1on => params.constant1on;
      int   constant2on => params.constant2on;
      float constant0val => params.constant0val;
      float constant1val => params.constant1val;
      float constant2val => params.constant2val;
      int   invert0on => params.invert0on;
      int   invert1on => params.invert1on;
      int   invert2on => params.invert2on;

      int Nin => array_size(data0);
      float min0 => min_array(data0);
      float min1 => min_array(data1);
      float min2 => min_array(data2);
      float max0 => max_array(data0);
      float max1 => max_array(data1);
      float max2 => max_array(data2);


      cxxmethod+req update(
         data0+read+notify+req,
         data1+read+notify+req,
         data2+read+notify+req,
         Nin+read,
         max0+read, max1+read, max2+read,
         min0+read, min1+read, min2+read,
         constant0on+read+notify+req, constant0val+read+notify+req,
         constant1on+read+notify+req, constant1val+read+notify+req,
         constant2on+read+notify+req, constant2val+read+notify+req,
         invert0on+read+notify+req,
         invert1on+read+notify+req,
         invert2on+read+notify+req,
         r+write,
         g+write,
         b+write
      );


      float+OPort2 r[Nin];
      float+OPort2 g[Nin];
      float+OPort2 b[Nin];
   };



   module DVcombine_data_to_RGB<src_file="comb_rgb.cxx"> {

      float+IPort2 data0[];
      float+IPort2 data1[];
      float+IPort2 data2[];

      CombineDataParams+IPort2 &params;

      int   constant0on => params.constant0on;
      int   constant1on => params.constant1on;
      int   constant2on => params.constant2on;
      float constant0val => params.constant0val;
      float constant1val => params.constant1val;
      float constant2val => params.constant2val;
      int   invert0on => params.invert0on;
      int   invert1on => params.invert1on;
      int   invert2on => params.invert2on;

      int Nin => array_size(data0);
      float min0 => min_array(data0);
      float min1 => min_array(data1);
      float min2 => min_array(data2);
      float max0 => max_array(data0);
      float max1 => max_array(data1);
      float max2 => max_array(data2);


      cxxmethod+req update(
         data0+read+notify+req,
         data1+read+notify+req,
         data2+read+notify+req,
         Nin+read,
         max0+read, max1+read, max2+read,
         min0+read, min1+read, min2+read,
         constant0on+read+notify+req, constant0val+read+notify+req,
         constant1on+read+notify+req, constant1val+read+notify+req,
         constant2on+read+notify+req, constant2val+read+notify+req,
         invert0on+read+notify+req,
         invert1on+read+notify+req,
         invert2on+read+notify+req,
         r+write,
         g+write,
         b+write
      );


      float+OPort2 r[Nin];
      float+OPort2 g[Nin];
      float+OPort2 b[Nin];
   };


   module DVadd_node_data<src_file="add_nd_d.cxx"> {
      Mesh+Node_Data+IPort2 &in_fld;
      Node_Data+nres+IPort2 &in_nd;

      Node_Data empty_nd {
         nnodes => in_fld.nnodes;
         nnode_data = 0;
      };
      Mesh &in_mesh => merge(.empty_nd,.in_fld);

      int+nres in_fld_nnodedata => in_fld.nnode_data;
      int in_nd_nnodedata => in_nd.nnode_data;

      cxxmethod+req update(
         in_fld+read+req,
         in_nd+read+notify+req,
         in_fld_nnodedata+read+req,
         in_nd_nnodedata+read+req,
         new_nd+write
      );

      int+nres nnew_nd => (in_fld_nnodedata + in_nd_nnodedata);
      Node_Data new_nd {
         nnodes => in_fld.nnodes;
         nnode_data => nnew_nd;
         &node_data[];
      };

      Mesh+Node_Data+OPort2 &out_fld => merge(.new_nd,.in_mesh);
      GDM.DataObject DataObject {
         in => <-.out_fld;
      };
      olink out_obj => .DataObject.obj;
   };

};

