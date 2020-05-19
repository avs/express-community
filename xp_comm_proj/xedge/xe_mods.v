
flibrary XedgeMods <build_dir="xp_comm_proj/xedge",
                    out_src_file="gen.c",
                    out_hdr_file="gen.h">
{

   /* parameters used by xedge module */
   group+OPort XedgeParams {
      float+Port2 angle;  /* maximum face angle */
      int+Port2   xtest;  /* flags to control axis crossing test */
      int+Port2   ytest;
      int+Port2   ztest;
   };


#define XE_PI 3.1415926526

   /* extract external edges from a field */
   module XedgeCore <src_file="xedge.c",
                     libdeps="FLD">
   {
      /* input field */
      Mesh+Node_Data_Opt+read+IPort2 &in {
      	xform+nonotify;
      };

      XedgeParams+IPort2 &params;

      float+read+req angle => (params.angle * XE_PI)/180.0;  /* maximum face angle */

      int+read+req   xtest => params.xtest;  /* flags to control axis crossing test */
      int+read+req   ytest => params.ytest;
      int+read+req   ztest => params.ztest;


      Mesh+Node_Data+nonotify out_field {
      	&xform+nowrite => in.xform;
      };

      // This is set up this way so that the 'write' flag is not set
      // on objects that will be connected to the input field.
      group+Oparam &out {
      	int ncell_sets;
      } => out_field;

      omethod+notify_val+notify_inst+req xedge_update<status=1> = "xedge_update";

   };


};

