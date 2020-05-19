
flibrary XformMacs <compile_subs=0> {

   macro cart2sph {
      Mesh+Space3+IPort2 &cart {
         coordinates {
            values+OPort3;
         };
      };

      XP_COMM_PROJ.Xform.XformMods.cartesian_to_spherical cartesian_to_spherical {
         cart => <-.cart.coordinates.values;
      };

      olink sph => .cartesian_to_spherical.sph;
   };
   macro sph2cart {
      ilink sph;

      XP_COMM_PROJ.Xform.XformMods.spherical_to_cartesian spherical_to_cartesian {
         sph => <-.sph;
      };
      FLD_MAP.point_mesh cartesian_point_mesh {
         coord => <-.spherical_to_cartesian.cart;
      };

      olink cart => .cartesian_point_mesh.out;
      olink obj => .cartesian_point_mesh.obj;
   };
   macro cart2cyl {
      Mesh+Space3+IPort2 &cart {
         coordinates {
            values+OPort3;
         };
      };

      XP_COMM_PROJ.Xform.XformMods.cartesian_to_cylindrical cartesian_to_cylindrical {
         cart => <-.cart.coordinates.values;
      };

      olink cyl => .cartesian_to_cylindrical.cyl;
   };
   macro cyl2cart {
      ilink cyl;

      XP_COMM_PROJ.Xform.XformMods.cylindrical_to_cartesian cylindrical_to_cartesian {
         cyl => <-.cyl;
      };
      FLD_MAP.point_mesh cartesian_point_mesh {
         coord => <-.cylindrical_to_cartesian.cart;
      };

      olink cart => .cartesian_point_mesh.out;
      olink obj => .cartesian_point_mesh.obj;
   };



   APPS.MultiWindowApp CylindricalCoordsEg {

      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.cyl2cart.obj
               };
            };
         };
         Scene_Selector {
            input_views => {<-.Scene.View.View, <-.PolarGraphScene.View.View};
         };
         GDM.Uscene2D PolarGraphScene {
            Top {
               child_objs<NEportLevels={4,1}> => {
                  <-.<-.<-.CylindricalCoordsGraph.AGGraphViewportObj.dataObject.obj
               };
            };
         };
      };

      int+Port n = 721;
      float+Port r[.n] => init_array(.n, 0.5, 2);
      float+Port t[.n] => init_array(.n, 0, (.n - 1));
      float+Port z[.n] => init_array(.n, -2, 2);

      FLD_MAP.interleave_3_arrays interleave_3_arrays {
         in1 => <-.r;
         in2 => <-.t;
         in3 => <-.z;
      };

      XP_COMM_PROJ.Xform.XformMacs.cyl2cart cyl2cart {
         cyl => <-.interleave_3_arrays.out;
      };
      XP_COMM_PROJ.Xform.XformMacs.cart2cyl cart2cyl {
         cart => <-.cyl2cart.cart;
      };

      float+Port r_array[] => .cart2cyl.cyl[][0];
      float+Port t_array[] => .cart2cyl.cyl[][1];

      macro CylindricalCoordsGraph {
         AGPolarGraph AGPolarGraph {
            valuesR+IPort3 => <-.<-.r_array;
            valuesT+IPort3 => <-.<-.t_array;
         };
         AGPolarRTicklines AGPolarRTicklines;
         AGPolarTTicklines AGPolarTTicklines;
         AGPolarGraphWorld AGPolarGraphWorld {
            children => {<-.AGPolarGraph,<-.AGPolarRTicklines,<-.AGPolarTTicklines};
            majorTickWidth = 0.3;
            minorTickCount = 1;
            minorTickWidth = 0.1;
            majorStepR = 0.5;
            majorStepT = 45.;
            limitsR = {0.,2.};
            limitsT = {0.,3600.};
         };
         AGGraphViewportObj AGGraphViewportObj {
            graphViewport {
               children => {<-.<-.AGPolarGraphWorld};
               frameWidth = 0.;
            };
            dataObject {
               obj<NEportLevels={1,4}>;
            };
         };
      };

   };



   APPS.SingleWindowApp SphericalCoordsEg {

      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.ScaledSphere.out_obj,<-.<-.<-.glyph.out_obj
               };
               Props {
                  subdiv = 12;
               };
            };
         };
      };


      int+Port n = 361;
      float+Port radius = 1.0;
      float+Port r[.n] => init_array(.n, .radius, .radius);
      float+Port t[.n] => init_array(.n, 0, (.n - 1));
      float+Port p[.n] => init_array(.n, 0, 180);
      float+Port data[.n] => init_array(.n, 0, (.n - 1));

      macro ScaledSphere {
         float sphereDiameter<NEportLevels={2,1}> => <-.radius * 2;
         GEOMS.Sphere Sphere {
            subdiv = 16;
         };
         DVM.DVscale DVscale {
            in => <-.Sphere.bounds.out;
            scale_x => <-.sphereDiameter;
            scale_y => <-.sphereDiameter;
            scale_z => <-.sphereDiameter;
         };
         GDM.DataObject DataObject {
            in => <-.DVscale.out;
         };
         olink out_obj => .DataObject.obj;
      };

      macro DemoUI {
         ilink radiusValue => <-.radius;

         UImod_panel panel {
            title = "Sphere Radius";
         };

         UIslider radiusSlider {
            parent => <-.panel;
            value => <-.radiusValue;
            title => "Sphere radius";
            message = "Set sphere radius";
            min = 0.;
            max = 10.;
            mode = "real";
            decimalPoints = 2;
         };
      };

      macro ConstructSpiral {
         FLD_MAP.interleave_3_arrays interleave_3_arrays {
            in1+IPort3 => <-.<-.r;
            in2+IPort3 => <-.<-.t;
            in3+IPort3 => <-.<-.p;
         };

         XP_COMM_PROJ.Xform.XformMacs.sph2cart sph2cart {
            sph => <-.interleave_3_arrays.out;
         };

         FLD_MAP.node_scalar node_scalar {
            in_data+IPort3 => <-.<-.data;
         };

         FLD_MAP.combine_mesh_data combine_mesh_data {
            in_mesh => <-.sph2cart.cart;
            in_nd => <-.node_scalar.out;
            out+OPort3;
         };
      };

      GEOMS.Diamond3D Diamond3D;

      MODS.glyph glyph {
         in_field => <-.ConstructSpiral.combine_mesh_data.out;
         in_glyph => <-.Diamond3D.out_fld;
         GlyphParam {
            vector = 0;
            scale = 0.1;
            normalize = 1;
         };

         obj {
            Modes {
               mode = {0,0,0,0,0};
            };
         };
      };

   };

};

