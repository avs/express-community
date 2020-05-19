macro Crop_Atoms<NEhelpContextID=1,NEhelpFile="STM3/Crop_Atoms.html"> {
   link molecule<NEportLevels={2,1}>;
   XP_COMM_PROJ.STM3.UTILS.CropAtoms CropAtoms<NEx=77.,NEy=462.> {
      in => <-.molecule;
      radius = 1.;
      center = {0.,0.,0.};
      sides = {1.,1.,1.};
      shape = "sphere";
		cross_bonds = "no";
   };
   UImod_panel UImod_panel<NEx=473.,NEy=33.> {
      title => name_of(<-.<-,1);
   };
   UIlabel title_1<NEx=594.,NEy=99.> {
      parent => <-.UImod_panel;
      label => "Crop type";
      y = 0;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   UIlist UIlist_shape<NEx=594.,NEy=143.> {
      parent => <-.UImod_panel;
      height => 110;
      selectedText = "sphere";
      strings = {
         "block","cylinder x","cylinder y","cylinder z","sphere"
      };
      y = 26;
      selectedItem<NEportLevels={0,2}> => <-.CropAtoms.shape;
      width => parent.clientWidth;
      listIsEditable = 0;
   };
   UItoggle reverse<NEx=594.,NEy=198.> {
      parent => <-.UImod_panel;
      y = 140;
      set => <-.CropAtoms.reverse;
   };
   UItoggle show_geometry<NEx=748.,NEy=198.> {
      parent => <-.UImod_panel;
      y = 140;
      x = 120;
      width = 150;
      set = 1;
   };
	UItoggle add_cross_bonds<NEx=902.,NEy=198.> {
   	parent => <-.UImod_panel;
   	y = 170;
   	width = 200;
      set => <-.CropAtoms.cross_bonds;
	};
   UIlabel title_2<NEx=539.,NEy=253.> {
      parent => <-.UImod_panel;
      label => "Geometry";
      y = 200;
      width => parent.clientWidth;
      color {
         foregroundColor = "white";
         backgroundColor = "blue";
      };
   };
   macro block_UI<NEx=484.,NEy=462.> {
      UIpanel UIpanel_block<NEx=66.,NEy=33.> {
         visible => (<-.<-.UIlist_shape.selectedItem == 0);
         parent<NEportLevels={3,0}> => <-.<-.UImod_panel;
         y = 230;
         width => parent.clientWidth;
      };
      UIlabel UIlabel_center<NEx=198.,NEy=132.> {
         parent => <-.UIpanel_block;
         label => "center:";
         y = 3;
         x = 5;
         width = 63;
         alignment = "left";
      };
      UIfield c_x<NEx=385.,NEy=132.> {
         parent => <-.UIpanel_block;
         value => <-.<-.CropAtoms.center[0];
         y = 0;
         x = 70;
         width = 60;
         updateMode = 7;
      };
      UIfield c_y<NEx=572.,NEy=132.> {
         parent => <-.UIpanel_block;
         value => <-.<-.CropAtoms.center[1];
         y = 0;
         x = 130;
         width = 60;
         updateMode = 7;
      };
      UIfield c_z<NEx=770.,NEy=132.> {
         parent => <-.UIpanel_block;
         value => <-.<-.CropAtoms.center[2];
         y = 0;
         x = 190;
         width = 60;
         updateMode = 7;
      };
      UIlabel UIlabel_sides<NEx=198.,NEy=187.> {
         parent => <-.UIpanel_block;
         label => "sides:";
         y = 33;
         x = 5;
         width = 63;
         alignment = "left";
      };
      UIfield s_x<NEx=385.,NEy=187.> {
         parent => <-.UIpanel_block;
         value => <-.<-.CropAtoms.sides[0];
         y = 30;
         x = 70;
         width = 60;
         updateMode = 7;
         min = 0.;
      };
      UIfield s_y<NEx=572.,NEy=187.> {
         parent => <-.UIpanel_block;
         value => <-.<-.CropAtoms.sides[1];
         y = 30;
         x = 130;
         width = 60;
         updateMode = 7;
         min = 0.;
      };
      UIfield s_z<NEx=770.,NEy=187.> {
         parent => <-.UIpanel_block;
         value => <-.<-.CropAtoms.sides[2];
         y = 30;
         x = 190;
         width = 60;
         updateMode = 7;
         min = 0.;
      };
   };
   macro cyl_UI<NEx=638.,NEy=462.> {
      UIpanel UIpanel_cyl<NEx=55.,NEy=55.> {
         visible => ((<-.<-.UIlist_shape.selectedItem > 0) && (<-.<-.UIlist_shape.selectedItem < 4));
         parent<NEportLevels={3,0}> => <-.<-.UImod_panel;
         y = 230;
         width => parent.clientWidth;
      };
      UIlabel UIlabel_center<NEx=198.,NEy=132.> {
         parent => <-.UIpanel_cyl;
         label => "center:";
         y = 3;
         x = 5;
         width = 63;
         alignment = "left";
      };
      UIfield c_x<NEx=385.,NEy=132.> {
         parent => <-.UIpanel_cyl;
         value => <-.<-.CropAtoms.center[0];
         y = 0;
         x = 70;
         width = 60;
         updateMode = 7;
      };
      UIfield c_y<NEx=572.,NEy=132.> {
         parent => <-.UIpanel_cyl;
         value => <-.<-.CropAtoms.center[1];
         y = 0;
         x = 130;
         width = 60;
         updateMode = 7;
      };
      UIfield c_z<NEx=770.,NEy=132.> {
         parent => <-.UIpanel_cyl;
         value => <-.<-.CropAtoms.center[2];
         y = 0;
         x = 190;
         width = 60;
         updateMode = 7;
      };
      UIlabel UIlabel_radius<NEx=198.,NEy=187.> {
         parent => <-.UIpanel_cyl;
         label => "radius:";
         y = 33;
         x = 5;
         width = 63;
         alignment = "left";
      };
      UIfield radius<NEx=385.,NEy=187.> {
         parent => <-.UIpanel_cyl;
         value => <-.<-.CropAtoms.radius;
         y = 30;
         x = 70;
         width = 60;
         updateMode = 7;
         min = 0.;
      };
   };
   macro sphere_UI<NEx=792.,NEy=462.> {
      UIpanel UIpanel_sphere<NEx=77.,NEy=44.> {
         visible => (<-.<-.UIlist_shape.selectedItem == 4);
         parent<NEportLevels={3,0}> => <-.<-.UImod_panel;
         y = 230;
         width => parent.clientWidth;
      };
      UIlabel UIlabel_center<NEx=198.,NEy=132.> {
         parent => <-.UIpanel_sphere;
         label => "center:";
         y = 3;
         x = 5;
         width = 63;
         alignment = "left";
      };
      UIfield c_x<NEx=385.,NEy=132.> {
         parent => <-.UIpanel_sphere;
         value => <-.<-.CropAtoms.center[0];
         y = 0;
         x = 70;
         width = 60;
         updateMode = 7;
      };
      UIfield c_y<NEx=572.,NEy=132.> {
         parent => <-.UIpanel_sphere;
         value => <-.<-.CropAtoms.center[1];
         y = 0;
         x = 130;
         width = 60;
         updateMode = 7;
      };
      UIfield c_z<NEx=770.,NEy=132.> {
         parent => <-.UIpanel_sphere;
         value => <-.<-.CropAtoms.center[2];
         y = 0;
         x = 190;
         width = 60;
         updateMode = 7;
      };
      UIlabel UIlabel_radius<NEx=198.,NEy=187.> {
         parent => <-.UIpanel_sphere;
         label => "radius:";
         y = 33;
         x = 5;
         width = 63;
         alignment = "left";
      };
      UIfield radius<NEx=385.,NEy=187.> {
         parent => <-.UIpanel_sphere;
         value => <-.<-.CropAtoms.radius;
         y = 30;
         x = 70;
         width = 60;
         updateMode = 7;
         min = 0.;
      };
   };
   macro Sphere<NEx=803.,NEy=550.> {
		FLD_MAP.point_mesh point_mesh<NEx=396.,NEy=198.> {
   		coord[1][3] = {0.,0.,0.};
			out {
      		xform {
         		xlate => {
            		<-.<-.<-.<-.CropAtoms.center[0],<-.<-.<-.<-.CropAtoms.center[1],
            		<-.<-.<-.<-.CropAtoms.center[2]};
         		mat => {
            		<-.<-.<-.<-.CropAtoms.radius,0.,0.,0.,0.,
            		<-.<-.<-.<-.CropAtoms.radius,0.,0.,0.,0.,
            		<-.<-.<-.<-.CropAtoms.radius,0.,0.,0.,0.,1.
         		};
      		};
			};
		};
		FLD_MAP.radius_data radius_data<NEx=550.,NEy=198.> {
   		in_data = {1};
		};
		FLD_MAP.combine_mesh_data combine_mesh_data<NEx=473.,NEy=264.> {
   		in_mesh => <-.point_mesh.out;
   		in_nd => <-.radius_data.out;
   		DataObject {
      		Props {
         		trans = 0.4;
         		inherit = 0;
         		subdiv = 16;
      		};
         	Obj {
            	visible => (<-.<-.<-.<-.UIlist_shape.selectedItem == 4);
            	name => name_of(<-.<-.<-.<-);
         	};
   		};
		};
		link out_obj<NEx=473.,NEy=561.,NEportLevels={1,2}> => .combine_mesh_data.obj;
   };
   FLD_MAP.hex_mesh Block<NEx=484.,NEy=550.> {
      coord[8][3] => {
         -1,-1,1,1,-1,1,1,1,1,-1,1,1,-1,-1,-1,1,-1,-1,1,1,-1,-1,1,-1
      };
      connect = {0,1,2,3,4,5,6,7};
      out {
         xform {
            xlate => {
               <-.<-.<-.CropAtoms.center[0],<-.<-.<-.CropAtoms.center[1],
               <-.<-.<-.CropAtoms.center[2]};
            mat => {
               (<-.<-.<-.CropAtoms.sides[0] / 2.),0.,0.,0.,0.,
               (<-.<-.<-.CropAtoms.sides[1] / 2.),0.,0.,0.,0.,
               (<-.<-.<-.CropAtoms.sides[2] / 2.),0.,0.,0.,0.,1.
            };
         };
      };
      GDM.DataObjectLite obj {
         in => <-.out;
         Obj {
            visible => (<-.<-.<-.UIlist_shape.selectedItem == 0);
            name => name_of(<-.<-.<-);
         };
         Props {
            trans = 0.4;
         };
         obj<NEportLevels={1,3}>;
      };
   };
   macro Cylinder<NEx=638.,NEy=550.> {
      float+nres min_coord<NEportLevels={0,1},NEx=462.,NEy=11.>[3] => cache(min_array(<-.molecule.xyz_lst,0,0));
      float+nres max_coord<NEportLevels={0,1},NEx=462.,NEy=44.>[3] => cache(max_array(<-.molecule.xyz_lst,0,0));
      float coords_x<NEportLevels={0,1},NEx=66.,NEy=66.>[2][3] => {
         min_coord[0],<-.CropAtoms.center[1],<-.CropAtoms.center[2],
         max_coord[0],<-.CropAtoms.center[1],<-.CropAtoms.center[2]
      };
      float coords_y<NEportLevels={0,1},NEx=66.,NEy=99.>[2][3] => {
         <-.CropAtoms.center[0],min_coord[1],<-.CropAtoms.center[2],
         <-.CropAtoms.center[0],max_coord[1],<-.CropAtoms.center[2]
      };
      float coords_z<NEportLevels={0,1},NEx=66.,NEy=132.>[2][3] => {
         <-.CropAtoms.center[0],<-.CropAtoms.center[1],min_coord[2],
         <-.CropAtoms.center[0],<-.CropAtoms.center[1],max_coord[2]
      };
      int conn<NEportLevels={0,1},NEx=242.,NEy=44.>[] => {0,1};
      FLD_MAP.polyline_mesh polyline_mesh<NEx=264.,NEy=143.> {
         coord => switch(<-.<-.UIlist_shape.selectedItem,<-.coords_x,<-.coords_y,<-.coords_z);
         connect => <-.conn;
      };
      XP_COMM_PROJ.STM3.SUPPORT.tubes tubes<NEx=352.,NEy=253.> {
         in+nres => switch(is_valid(<-.<-.molecule.num_atoms),<-.polyline_mesh.out);
         bondradius => <-.<-.CropAtoms.radius;
         omethod+notify_val+notify_inst update = "makeTubes";
      };
      GDM.DataObjectLite DataObjectLite<NEx=385.,NEy=352.> {
         in => <-.tubes.out;
         Obj {
            visible => ((<-.<-.<-.UIlist_shape.selectedItem > 0) && (<-.<-.<-.UIlist_shape.selectedItem < 4));
            name => name_of(<-.<-.<-);
         };
         Props {
            trans = 0.4;
         };
         obj<NEportLevels={1,3}>;
      };
   };
   GDM.GroupObject GroupObject<NEx=660.,NEy=627.> {
      child_objs => {<-.Sphere.out_obj,<-.Block.obj.obj,<-.Cylinder.DataObjectLite.obj};
      Top {
         visible => <-.<-.show_geometry.set;
      };
   };
   link inside<NEportLevels={1,2}> => CropAtoms.inside;
   link outside<NEportLevels={1,2}> => CropAtoms.outside;
	link geometry<NEportLevels={1,2}> => GroupObject.obj;
};
