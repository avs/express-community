
flibrary Read6DOFMacs <compile_subs=0> {

   macro Read_6DOF_UI {
      string &filename<NEportLevels={2,1}>;

      UImod_panel UImod_panel {
         title => name_of(<-.<-.<-);
         parent+IPort3;
      };
      UIlabel filenameLabel {
         parent => <-.UImod_panel;
         x = 0;
         y = 0;
         width = parent.width;
         alignment = "left";
         label = "Filename...";
      };

      UItext filenameText {
         parent => <-.UImod_panel;
         x => <-.filenameLabel.x;
         y => <-.filenameLabel.y + <-.filenameLabel.height + 5;
         width => parent.width * (2.0/3.0);
         showLastPosition = 1;
         text => <-.filename;
      };
      UIbutton filenameVisible {
         parent => <-.UImod_panel;
         x => <-.filenameText.x + <-.filenameText.width + 5;
         y => <-.filenameText.y;
         width => parent.width - x;
         height => <-.file_name.height;
         label => "Browse...";
      };

      UIfileSB file_browser {
         GMOD.copy_on_change copy_on_change {
            trigger => <-.<-.filenameVisible.do;
            input => <-.<-.filenameVisible.do;
            output => <-.visible;
         };
         title => "6DOF Filename";
         filename => <-.filename;
         searchPattern = "./*.*";
      };
   };


   macro read_6DOF<data_import_module=1> {
      string filename<NEportLevels={2,1}>;

      file FILE {
         name => <-.filename;
      };

      Read_6DOF_UI Read_6DOF_UI {
         filename => <-.filename;
      };

      float+OPort2 x_comp[] => cache(file_obj(.FILE,0,4,{
               1,0,0,0,0,0
            },0,1));
      float+OPort2 y_comp[] => cache(file_obj(.FILE,0,4,{
               0,1,0,0,0,0
            },0,1));
      float+OPort2 z_comp[] => cache(file_obj(.FILE,0,4,{
               0,0,1,0,0,0
            },0,1));
      float+OPort2 pitch[] => cache(file_obj(.FILE,0,4,{
               0,0,0,1,0,0
            },0,1));
      float+OPort2 roll[] => cache(file_obj(.FILE,0,4,{
               0,0,0,0,1,0
            },0,1));
      float+OPort2 yaw[] => cache(file_obj(.FILE,0,4,{
               0,0,0,0,0,1
            },0,1));
   };


   macro sequence_6DOF {
      ilink in_xform;
      string filename<NEportLevels={2,1}>;

      float+Port start = 0.;
      float+Port end => (array_size(.read_6DOF.x_comp) - 1);
      float+Port incr = 1.;
      float+Port count => .Loop.count;
      float+Port scale = 20.;

      GDM.GDxform_edit GDxform_edit {
         xform => <-.in_xform;
         x_rot => read_6DOF.roll[<-.count];
         y_rot => read_6DOF.pitch[<-.count];
         z_rot => read_6DOF.yaw[<-.count];
         x_trans => read_6DOF.x_comp[<-.count];
         y_trans => read_6DOF.y_comp[<-.count];
         z_trans => read_6DOF.z_comp[<-.count];
         scale+IPort2 => <-.scale;
         absolute = 1;
         shell_vis = 0;
      };

      MODS.Loop Loop {
         start => <-.start;
         end => <-.end;
         incr => <-.incr;
         count = start;    // NB: Assign value, don't connect value
         UIpanel {
            title => name_of(<-.<-.<-);
         };
      };

      read_6DOF read_6DOF {
         filename => <-.filename;
      };
   };



   APPS.SingleWindowApp Read_6DOF_Eg {
      MODS.Read_Geom Read_Geom {
         read_geom_ui {
            filename = "$XP_PATH<0>/data/geom/teapot.geo";
         };
         ACread_geom {
            out_field {
               xform<NEportLevels={0,4}>;
            };
         };
      };

      XP_COMM_PROJ.Read6DOF.Read6DOFMacs.sequence_6DOF sequence_6DOF {
         in_xform => <-.Read_Geom.ACread_geom.out_field.xform;
         filename = "$XP_PATH<1>/xp_comm_proj/rd_6dof/6dof.dat";
      };


      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Geom.geom
               };
               Xform {
                  ocenter = {-46.4823,18.2781,2393.15};
                  dcenter = {-1.81867,0.715151,93.6343};
                  mat = {
                     0.028255,-0.00807174,-0.0258331,0.,
                     -0.0220771,0.0147288,-0.0287491,0.,
                     0.0156557,0.0353377,0.00608194,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-7.21734,4.28241,-93.6343};
                  center = {-46.4823,18.2781,2393.15};
               };
            };
            Camera {
               Camera {
                  auto_norm = "None";
                  depth_scale = 0.1;
                  front = 1.;
                  perspec = 1;
                  fov = 90.;
                  wsize = 7.;
               };
            };
         };
      };

   };


};

