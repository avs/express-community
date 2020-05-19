
flibrary PlaneProjectMacs <compile_subs=0> {

   macro PlaneProjectUI {
      ilink in_fld;
      DV_Param_slice+IPort2     &param;
      DVnode_data_labels+IPort2 &DVnode_data_labels;
      DVcell_data_labels+IPort2 &DVcell_data_labels;

      UImod_panel UIpanel {
         parent<NEportLevels={4,0}>;
         message = "Select plane project control panel.";
         title => name_of(<-.<-.<-);
      };

      UItoggle xform_toggle {
         y = 0;
         width => <-.UIpanel.width;
         parent => <-.UIpanel;
         label => "Plane Transform Editor";
      };
      UIslider plane_dist {
         y => ((<-.xform_toggle.y + <-.xform_toggle.height) + 4);
         width => <-.UIpanel.width;
         parent => <-.UIpanel;
         min+nres+IPort2 => cache((-0.5 * magnitude((in_fld.coordinates.max_vec - in_fld.coordinates.min_vec))));
         max+nres+IPort2 => cache( (0.5 * magnitude((in_fld.coordinates.max_vec - in_fld.coordinates.min_vec))));
         value+IPort2 => param.dist;
         title => "plane distance";
      };
      UIoptionBoxLabel UIoptionBoxLabel {
         parent => <-.UIpanel;
         labels+IPort2 => DVnode_data_labels.labels;
         title => "map components";
         visible => <-.UIpanel.visible;
         &selectedItems+IPort2 => param.component;
         max => DVnode_data_labels.ncomp;
         y => ((<-.plane_dist.y + <-.plane_dist.height) + 4);
         width => <-.UIpanel.width;
      };
      UIoptionBoxLabel UIoptionBoxLabel_cell {
         parent => <-.UIpanel;
         labels+IPort2 => DVcell_data_labels.labels;
         title => "map cell components";
         visible => (<-.UIpanel.visible & (DVcell_data_labels.ncomp > 0));
         &selectedItems+IPort2 => param.cell_data;
         max => DVcell_data_labels.ncomp;
         y => ((<-.UIoptionBoxLabel.y + <-.UIoptionBoxLabel.height) + 4);
         width => <-.UIpanel.width;
      };
      XformEditor plane_xform {
         // obj_in => <-.<-.plane;
         vis => xform_toggle.set;
      };
   };


   macro plane_project {
      ilink in_field<export_all=1>;

      DV_Param_slice+Port SliceParam<export_all=2> {
         component = {0};
         dist = 0.;
         cell_data = {0};
      };

      int has_cell_data => (DVcell_data_labels.ncomp > 0);
      float+nres xlate[3] => {0., 0., ((in_field.coordinates.min_vec[2] + in_field.coordinates.max_vec[2]) / 2)};

      Mesh_Unif+OPort plane<NEcolor1=65280> {
         nspace = 2;
         ndim = 2;
         dims => {2,2};
         points+nres => {
            in_field.coordinates.min_vec[0],in_field.coordinates.min_vec[1],
            in_field.coordinates.max_vec[0],in_field.coordinates.max_vec[1]
         };
         xform {
            xlate+nres => cache(<-.<-.xlate);
         };
      };

      DVnode_data_labels DVnode_data_labels {
         in => in_field;
         int+nres ncomp => in.nnode_data;
      };
      DVcell_data_labels DVcell_data_labels {
         in => in_field;
      };

      PlaneProjectUI PlaneProjectUI {
         in_fld => in_field;
         &param => SliceParam;
         &DVnode_data_labels => <-.DVnode_data_labels;
         &DVcell_data_labels => <-.DVcell_data_labels;
         plane_xform.obj_in => <-.<-.plane;
      };

      XP_COMM_PROJ.PlaneProject.PlaneProjectMods.plane_proj2 plane_proj2 {
         in => <-.in_field;
         plane => <-.plane;
         dist => <-.SliceParam.dist;
         map_comp => <-.SliceParam.component;
         cell_data => <-.SliceParam.cell_data;
      };

      DataObject plane_obj {
         in => <-.plane;
      };
      DataObject slice_obj {
         in => <-.plane_proj2.out;
         Obj {
            alt_xform+nres => <-.<-.plane.xform;
            xform_mode = "Alternate";
         };
      };
      DataObject slice_obj_2d {
         in => <-.plane_proj2.out_2d;
      };

      olink out_fld => .plane_proj2.out;
      olink out_plane => .plane;
      olink out_obj_slice => .slice_obj.obj;
      olink out_obj_plane => .plane_obj.obj;
      olink out_obj_2d => .slice_obj_2d.obj;
   };


   plane_project plane_project_ARR {

      imlink in_field;
      xlate => {0.0, 0.0, (in_field[0].coordinates.min_vec[2]+in_field[0].coordinates.max_vec[2])/2};
      plane {
         points+nres => {in_field[0].coordinates.min_vec[0], in_field[0].coordinates.min_vec[1], 
                         in_field[0].coordinates.max_vec[0], in_field[0].coordinates.max_vec[1]};
      };

      DVnode_data_labels {
         in => in_field[0];
      };
      DVcell_data_labels {
         in => in_field[0];
      };
      PlaneProjectUI {
         in_fld => in_field[0];
      };
      plane_proj2[array_size(in_field)] {
         in => in_field[index_of(plane_proj2)];
      };

      DataObjects objs {
         in_fields => plane_proj2.out;
         dos.Obj.xform_mode = 1;
         dos {
            Obj {
               name+nres => name_of(<-.<-.<-.<-)+index_of(dos);
            };
         };
      };
      DataObjects objs_2d {
         in_fields => plane_proj2.out_2d;
         dos.Obj.xform_mode = 1;
         dos {
            Obj {
               name+nres => name_of(<-.<-.<-.<-)+index_of(dos);
            };
         };
      };

      GroupObject slice_obj {
         Top.name => name_of(<-.<-.<-);
         child_objs => objs.dos.obj;
         Top.xform_mode = 2;
         Top.alt_xform+nres => <-.<-.plane.xform;
      };
      GroupObject slice_obj_2d {
         Top.name => name_of(<-.<-.<-);
         child_objs => objs_2d.dos.obj;
         Top.xform_mode = 2;
         Top.alt_xform+nres => <-.<-.plane.xform;
      };

      omlink out_fld => plane_proj2.out;
   };



   APPS.MultiWindowApp PlaneProjectEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,
                  <-.<-.<-.plane_project.out_obj_slice,
                  <-.<-.<-.plane_project.out_obj_plane
               };
               Xform {
                  mat = {
                     -0.284432,-0.115532,0.259401,0.,
                     -0.2653,0.239032,-0.184439,0.,
                     -0.101257,-0.301753,-0.245422,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {2.32003,1.26831,0.315435};
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,};
            };
         };
         Scene_Selector {
            input_views => {Scene.View.View, <-.Plane2DScene.View.View};
         };
         GDM.Uscene2D Plane2DScene {
            Top {
               child_objs<NEportLevels={4,1}> => {<-.<-.<-.plane_project.out_obj_2d};
            };
            View {
               View {
                  renderer = "OpenGL";
               };
            };
         };
      };


      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/bluntfin.fld";
         };
      };
      MODS.bounds bounds {
         in_field => <-.Read_Field.field;
      };


      XP_COMM_PROJ.PlaneProject.PlaneProjectMacs.plane_project plane_project {
         in_field => <-.Read_Field.field;
         SliceParam {
            component = {1};
         };
         plane {
            xform {
               xlate = {0.644271,-1.48889,4.66202};
               mat = {
                  0.196721,0.9292,0.312871,0.,
                  0.136751,0.289984,-0.947211,0.,
                  -0.970876,0.229121,-0.0700237,0.,
                  0.,0.,0.,1.
               };
            };
         };
         slice_obj {
            Props {
               jitter = 2;
               inherit = 0;
            };
         };
         plane_obj {
            Props {
               inherit = 0;
            };
         };
      };
   };




   APPS.MultiWindowApp PlaneProjectArrEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds_ARR.out_obj,
                  <-.<-.<-.plane_project_ARR.out_obj_slice,
                  <-.<-.<-.plane_project_ARR.out_obj_plane
               };
               Xform {
                  mat = {
                     0.305183,-12.8288,-3.11152,0.,
                     -3.69808,2.90465,-12.3386,0.,
                     12.6722,1.15661,-3.52579,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-3.15887,-1.61979,-7.70462};
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,};
            };
         };
         Scene_Selector {
            input_views => {Scene.View.View, <-.Plane2DScene.View.View};
         };
         GDM.Uscene2D Plane2DScene {
            Top {
               child_objs<NEportLevels={4,1}> => {<-.<-.<-.plane_project_ARR.out_obj_2d};
            };
            View {
               View {
                  renderer = "OpenGL";
               };
            };
         };
      };


      MODS.Plot3d_Multi_Block Plot3d_Multi_Block {
         filename_xyz = "$XP_PATH<0>/data/plot3d/multb_x.dat";
         filename_q = "$XP_PATH<0>/data/plot3d/multb_q.dat";
      };


      MODS_ARR.bounds_ARR bounds_ARR {
         in_field => <-.Plot3d_Multi_Block.fields;
      };


      XP_COMM_PROJ.PlaneProject.PlaneProjectMacs.plane_project_ARR plane_project_ARR {
         in_field => <-.Plot3d_Multi_Block.fields;
         SliceParam {
            component = {3};
         };
         plane {
            xform {
               xlate = {-0.0497116,0.0245799,0.348231};
               mat = {
                  0.994125,-0.0232146,0.105715,0.,
                  0.0802937,0.813138,-0.576506,0.,
                  -0.0725776,0.581607,0.810225,0.,
                  0.,0.,0.,1.
               };
            };
         };
         slice_obj {
            Props {
               jitter = 2;
               inherit = 0;
            };
         };
         plane_obj {
            Props {
               inherit = 0;
            };
         };
      };

   };


};

