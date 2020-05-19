
flibrary XformMods <build_dir="xp_comm_proj/xform",
                    out_hdr_file="gen.hxx",
                    out_src_file="gen.cxx"> {

   module cartesian_to_spherical <src_file="cart2sph.cxx"> {
      float+IPort2 cart[][3];

      cxxmethod+req+notify_inst update(
         .cart+read+notify+req,
         .n+write,
         .sph+write
      );

      int n;
      float+OPort2 sph[n/3][3];
   };

   module spherical_to_cartesian <src_file="sph2cart.cxx"> {
      float+IPort2 sph[][3];

      cxxmethod+req+notify_inst update(
         .sph+read+notify+req,
         .n+write,
         .cart+write
      );

      int n;
      float+OPort2 cart[n/3][3];
   };

   module cartesian_to_cylindrical <src_file="cart2cyl.cxx"> {
      float+IPort2 cart[][3];

      cxxmethod+req+notify_inst update(
         .cart+read+notify+req,
         .n+write,
         .cyl+write
      );

      int n;
      float+OPort2 cyl[n/3][3];
   };

   module cylindrical_to_cartesian <src_file="cyl2cart.cxx"> {
      float+IPort2 cyl[][3];

      cxxmethod+req+notify_inst update(
         .cyl+read+notify+req,
         .n+write,
         .cart+write
      );

      int n;
      float+OPort2 cart[n/3][3];
   };

};

