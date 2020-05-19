

// Defines library used to hold low-level components
// Also specifies details of how modules should be compiled
flibrary EllipseMods <build_dir="xp_comm_proj/ellipse",
                      out_src_file="gen.cxx",
                      out_hdr_file="gen.h"> {

   // Common Parameter Block for the Ellipse Project
   // Enables easy connection of separate components
   group+OPort EllipseParams {
      int+Port2 num_ellipses;    // Number of ellipses to be generated
      int+Port2 num_points;      // Number of points per ellipse
      float+Port2 aboutxc;       // X-coordinate of centre point
      float+Port2 aboutyc;       // Y-coordinate of centre point
   };


   // Low-level module.
   // Acts as a wrapper for the C++ code that generates coordinates and connectivity
   module EllipseCore <src_file="ellipse.cxx"> {

      // Defines name of C++ method and specifies how it
      // interacts with the module parameters
      cxxmethod+req+notify_inst update(
         .EllipseParams+req,
         .num_ellipses+read+notify+req,
         .num_points+read+notify+req,
         .aboutxc+read+notify+req,           
         .aboutyc+read+notify+req,           
         .x_dims+read+notify+req,
         .y_dims+read+notify+req,
         .ncoords+write,
         .nconn+write,
         .nquads+write,
         .line_conn+write,
         .quad_conn+write,
         .line_coords+write
      );

      // Reference to external parameter block
      EllipseParams+IPort2 &EllipseParams;

      // Links into external parameter block
      int num_ellipses => .EllipseParams.num_ellipses;
      int num_points => .EllipseParams.num_points;
      float aboutxc => .EllipseParams.aboutxc;
      float aboutyc => .EllipseParams.aboutyc;

      // Arrays containing dimensions of each ellipse
      float+IPort2 x_dims[num_ellipses];
      float+IPort2 y_dims[num_ellipses];

      int+OPort2 nconn;    // Number of polyline connections generated
      int+OPort2 ncoords;  // Number of coordinates generated
      int+OPort2 nquads;   // Number of quads generated

      int+OPort2 line_conn[nconn];           // Polyline connectivity data
      float+OPort2 line_coords[ncoords][3];  // Generated coordinates
      int+OPort2 quad_conn[nquads*4];        // Quad connectivity data
   };

};

