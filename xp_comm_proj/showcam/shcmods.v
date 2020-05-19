
flibrary ShowcamMods <build_dir="xp_comm_proj/showcam",
                      out_hdr_file="gen.h",
                      out_src_file="gen.cxx">
{

   // Low-level module that takes a camera object, extracts
   // the camera xform and alters it so that it can be used to
   // position the glyph.
   module prep_cam_xform <src_file="cam_xfm.cxx",
                          cxx_hdr_files = "fld/Xfld.h">
   {
      cxxmethod+notify_inst update (
         xform_in+notify+read+req,
         xform_out+write
      );

      GDcamera+IPort2 &camera_in;
      GDxform         &xform_in => camera_in.xform;

      DefaultXform+OPort2 xform_out {
         center => xform_in.center;
         ocenter => xform_in.ocenter;
         dcenter => xform_in.dcenter;
         rspace => xform_in.rspace;
         xlate => camera_in.from;
      };

      Xform+OPort2 mesh_out {
         &xform => <-.xform_out;
      };
   };


   // Geometric glyph object that is used to represent the camera
   macro CameraGlyph <compile_subs=0> {
      Mesh+Node_Data+OPort camera_glyph {
         nspace = 3;
         nnodes = 54;
         coordinates {
            values = {
               -0.75,0.5,0.2,
               -0.75,-0.5,0.2,
               0.75,-0.5,0.2,
               0.75,0.5,0.2,
               0.75,-0.5,-0.15,
               0.75,0.5,-0.15,
               -0.75,0.5,-0.15,
               -0.75,-0.5,-0.15,
               0.,-0.4,-0.15,
               0.,-0.4,-0.4,
               -0.175,-0.353109,-0.15,
               -0.175,-0.353109,-0.4,
               -0.303109,-0.225,-0.15,
               -0.303109,-0.225,-0.4,
               -0.35,-0.05,-0.15,
               -0.35,-0.05,-0.4,
               -0.303109,0.125,-0.15,
               -0.303109,0.125,-0.4,
               -0.175,0.253109,-0.15,
               -0.175,0.253109,-0.4,
               0.,0.3,-0.15,
               0.,0.3,-0.4,
               0.175,0.253109,-0.15,
               0.175,0.253109,-0.4,
               0.303109,0.125,-0.15,
               0.303109,0.125,-0.4,
               0.35,-0.05,-0.15,
               0.35,-0.05,-0.4,
               0.303109,-0.225,-0.15,
               0.303109,-0.225,-0.4,
               0.175,-0.353109,-0.15,
               0.175,-0.353109,-0.4,
               0.,-0.4,-0.15,
               0.,-0.4,-0.4,
               -0.7,0.45,-0.16,
               -0.7,0.3,-0.16,
               -0.45,0.3,-0.16,
               -0.45,0.45,-0.16,
               -0.7,0.45,0.21,
               -0.7,0.3,0.21,
               -0.45,0.3,0.21,
               -0.45,0.45,0.21,
               0.,-0.4,-0.35,
               -0.175,-0.353109,-0.35,
               -0.303109,-0.225,-0.35,
               -0.35,-0.05,-0.35,
               -0.303109,0.125,-0.35,
               -0.175,0.253109,-0.35,
               0.,0.3,-0.35,
               0.175,0.253109,-0.35,
               0.303109,0.125,-0.35,
               0.35,-0.05,-0.35,
               0.303109,-0.225,-0.35,
               0.175,-0.353109,-0.35
            };
         };
         Quad quad_set {
            ncells = 12;
            node_connect_list = {
               2,3,5,4,4,5,6,7,7,6,0,1,0,3,2,1,3,0,6,5,1,2,4,7,34,37,
               36,35,35,39,38,34,39,40,41,38,40,36,37,41,37,34,38,41,36,40,39,35
            };
         };
         Polytri ptri_set {
            npolys = 1;
            poly_connect_list = {8,33};
         };
         Polyhedron pdron_set {
            npolys = 1;
            poly_nnodes = {12};
            poly_connect_list = {
               42,43,44,45,46,47,48,49,50,51,52,53
            };
         };
         ncell_sets = 3;
         cell_set => {quad_set,ptri_set,pdron_set};

         float cam_body_vals[8] => 0.1;
         float lens_cyl_vals[26] => 1.;
         float vf_vals[8] => 0.5;
         float lens_glass_vals[12] => 0.0;

         nnode_data = 1;
         node_data = {
            {
               veclen=1,values=>
               cache(concat_array(cam_body_vals,lens_cyl_vals,vf_vals,lens_glass_vals)),,,,,,,
            }
         };
      };

      GDM.DataObject DataObject {
         in => <-.camera_glyph;
         Modes {
            normals = "None";
         };
      };

      olink out_fld => .camera_glyph;
      olink out_obj => .DataObject.obj;
   };

};

