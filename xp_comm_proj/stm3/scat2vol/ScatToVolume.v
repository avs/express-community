macro Scat_to_Volume<NEhelpContextID=1,NEhelpFile="STM3/Scat_to_Volume.html"> {
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEportLevels={2,0},NEx=44.,NEy=33.>;
   UImod_panel UImod_panel<NEx=363.,NEy=22.> {
      title => name_of(<-.<-,1);
   };
   UIlabel title<NEx=660.,NEy=88.> {
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
      parent => <-.UImod_panel;
      label => "Atoms to volume conversion";
   };
   UIlabel UIlabel#1<NEx=605.,NEy=132.> {
      x = 5;
      y = 33;
      parent => <-.UImod_panel;
      label => "Sphere radius:";
      alignment = "left";
   };
   UIfield radius<NEx=759.,NEy=132.> {
      x = 106;
      y = 30;
      parent => <-.UImod_panel;
      min = 0.;
      value = 0.3;
   };
   UIlabel UIlabel#2<NEx=605.,NEy=176.> {
      x = 5;
      y = 63;
      parent => <-.UImod_panel;
      label => "Box resolution:";
      alignment = "left";
   };
   UIfield resolution<NEx=759.,NEy=176.> {
      x = 106;
      y = 60;
      parent => <-.UImod_panel;
      min = 0.01;
      value = 0.05;
   };
   UIlabel title_limits<NEx=682.,NEy=275.> {
      y = 97;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
      parent => <-.UImod_panel;
      label => "Volume limits";
   };
   UItoggle use_manual_limits<NEx=671.,NEy=319.> {
      y = 125;
      width = 200;
      parent => <-.UImod_panel;
      set => <-.scat_to_vol.use_manual_limits;
   };
   UIlabel min<NEx=693.,NEy=385.> {
      x = 50;
      y = 145;
      parent => <-.UImod_panel;
   };
   UIlabel max<NEx=847.,NEy=385.> {
      x = 150;
      y = 145;
      parent => <-.UImod_panel;
   };
   UIlabel UIlabel#4<NEx=528.,NEy=429.> {
      x = 5;
      y = 170;
      width = 40;
      parent => <-.UImod_panel;
      label => "X";
   };
   UIlabel UIlabel#5<NEx=528.,NEy=462.> {
      x = 5;
      y = 197;
      width = 40;
      parent => <-.UImod_panel;
      label => "Y";
   };
   UIlabel UIlabel#6<NEx=528.,NEy=495.> {
      x = 5;
      y = 224;
      width = 40;
      parent => <-.UImod_panel;
      label => "Z";
   };
   UIfield UIfield<NEx=693.,NEy=429.> {
      x = 50;
      y = 168;
      width = 90;
      parent => <-.UImod_panel;
      value => <-.scat_to_vol.manual_limits[0][0];
   };
   UIfield UIfield#1<NEx=847.,NEy=429.> {
      x = 150;
      y = 195;
      width = 90;
      parent => <-.UImod_panel;
      value => <-.scat_to_vol.manual_limits[1][0];
   };
   UIfield UIfield#2<NEx=693.,NEy=462.> {
      x = 50;
      y = 222;
      width = 90;
      parent => <-.UImod_panel;
      value => <-.scat_to_vol.manual_limits[0][1];
   };
   UIfield UIfield#3<NEx=847.,NEy=462.> {
      x = 150;
      y = 168;
      width = 90;
      parent => <-.UImod_panel;
      value => <-.scat_to_vol.manual_limits[1][1];
   };
   UIfield UIfield#4<NEx=693.,NEy=495.> {
      x = 50;
      y = 195;
      width = 90;
      parent => <-.UImod_panel;
      value => <-.scat_to_vol.manual_limits[0][2];
   };
   UIfield UIfield#5<NEx=847.,NEy=495.> {
      x = 150;
      y = 222;
      width = 90;
      parent => <-.UImod_panel;
      value => <-.scat_to_vol.manual_limits[1][2];
   };
   FLD_MAP.point_mesh point_mesh<NEx=55.,NEy=110.> {
      coord => <-.in.xyz_lst;
   };
   FLD_MAP.node_scalar node_scalar<NEx=198.,NEy=110.> {
      in_data => <-.in.charge_lst;
   };
   FLD_MAP.combine_mesh_data combine_mesh_data<NEx=143.,NEy=176.> {
      in_mesh => <-.point_mesh.out;
      in_nd => <-.node_scalar.out;
   };
   XP_COMM_PROJ.STM3.SCAT2VOL.scat_to_vol scat_to_vol<NEx=143.,NEy=253.> {
      in => <-.combine_mesh_data.out;
      radius => <-.radius.value;
      resolution => <-.resolution.value;
   };
   MODS.volume_render volume_render<NEx=143.,NEy=330.> {
      in_mesh => <-.scat_to_vol.out;
      DataObject {
         Props {
            fat_ray = 0;
            inherit = 0;
				line_width = 2;
         };
         Modes {
            mode = {0,0,0,3,2};
         };
         VolRenderDatamap {
            DataRange = {
               {
                  DataMaxValue=127.5,,,selectColorRange=1,selectAlphaRange=0,,,,,,,,,,,,,,,,,
               },
               {
                  selectColorRange=1,,,,,,,,,,,,,DataMinValue=><-.DataRange[0].DataMaxValue,,,,,,,size=2,,
               }};
            DatamapValue = {
               {
                  v4=0.5,v3=0.5,v2=0.,value=0.,v1=0.002
               },
               {
                  value=127.5,v4=0.5,v3=0.5,v2=0.,v1=0.09
               },
               {
                  value=>127.5,v4=0.,v3=1.,v2=1.,
               },
               {
                  value=255.,v4=0.,,v2=1.,
               }};
            currentColorModel = 1;
         };
         Obj {
            use_altobj = 0;
            name => name_of(<-.<-.<-.<-,1);
         };
      };
      GDmodes_edit {
         volume = "Ray Tracer";
      };
      GDprops_edit {
         voxel_interp = "Trilinear";
         ray_algo = "Direct Composite";
         fat_ray = 0;
         inherit = 0;
      };
      UImod_panel {
         title => name_of(<-.<-,1);
      };
      modes_ui {
         inherit {
            set = 1;
         };
         surf_rb {
            x = 0;
         };
         ray_tracer {
            set = 1;
         };
         vol_rb {
            x = 0;
         };
      };
      props_ui {
         interp_tri {
            set = 1;
         };
         interp_rb {
            x = 0;
         };
         ray_direct {
            set = 1;
         };
         ray_rb {
            x = 0;
         };
         dist_glob {
            set = 1;
         };
         dist_rb {
            x = 0;
         };
      };
      datamap_ui {
         Dmap2ImageLegend {
            Scene {
               Top {
                  Xform {
                     ocenter = {117.5,14.5,0.};
                     dcenter = {-557.5,-68.798,0.};
                  };
               };
               View {
                  View {
                     trigger = 2;
                  };
               };
            };
         };
         alpha_linear {
            set = 1;
         };
         alpha_rb {
            x = 0;
         };
      };
   };
   link out<NEportLevels={1,2},NEx=143.,NEy=561.> => .volume_render.out_obj;
};
