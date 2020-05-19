flibrary BoundingboxesMacs {

   macro QuadBox {
      group+IPort2 &in_field {
	 int nspace = 3;
	 group coordinates {
	    prim min_vec[3];
	    prim max_vec[3];
	 };
	 group+opt xform;
      };
      
      Quad set1 {
	 ncells = 6;
	 node_connect_list = {0,1,2,3,4,5,6,7,0,1,5,4,3,2,6,7,0,3,7,4,1,5,6,2};
      };
      
      Mesh quadbox {
	 int nnodes = 8;
	 int nspace = 3;
	 coordinates {
	    float values[nvals][veclen] => {
	       {
		  <-.<-.in_field.coordinates.min_vec[0],
		  <-.<-.in_field.coordinates.min_vec[1],
		  <-.<-.in_field.coordinates.min_vec[2]
	       },
	       {
		  <-.<-.in_field.coordinates.max_vec[0],
		  <-.<-.in_field.coordinates.min_vec[1],
		  <-.<-.in_field.coordinates.min_vec[2]
	       },
	       {
		  <-.<-.in_field.coordinates.max_vec[0],
		  <-.<-.in_field.coordinates.max_vec[1],
		  <-.<-.in_field.coordinates.min_vec[2]
	       },
	       {
		  <-.<-.in_field.coordinates.min_vec[0],
		  <-.<-.in_field.coordinates.max_vec[1],
		  <-.<-.in_field.coordinates.min_vec[2]
	       },
	       {
		  <-.<-.in_field.coordinates.min_vec[0],
		  <-.<-.in_field.coordinates.min_vec[1],
		  <-.<-.in_field.coordinates.max_vec[2]
	       },
	       {
		  <-.<-.in_field.coordinates.max_vec[0],
		  <-.<-.in_field.coordinates.min_vec[1],
		  <-.<-.in_field.coordinates.max_vec[2]
	       },
	       {
		  <-.<-.in_field.coordinates.max_vec[0],
		  <-.<-.in_field.coordinates.max_vec[1],
		  <-.<-.in_field.coordinates.max_vec[2]
	       },
	       {
		  <-.<-.in_field.coordinates.min_vec[0],
		  <-.<-.in_field.coordinates.max_vec[1],
		  <-.<-.in_field.coordinates.max_vec[2]
	       }
	    };
	 };
	 int ncell_sets = 1;
	 cell_set[ncell_sets] => {set1};
      };
      
      GDM.DataObjectLite obj {
	 in => <-.quadbox;
	 Obj.name => name_of(<-.<-.<-);
	 Modes {
	    // Render the box as a wireframe
	    mode = {0, GD_LINES, GD_NO_SURF, 0, 0};
	 };
      };
      
      olink out_fld<export_all=2> => quadbox;
      olink out_obj => obj.Obj;
   };

   group+OPort param_group {
      int bounds = 1;
      int bounds_deformed = 1;
      int deformed_valid;
      enum mesh {
	 choices => {"model","offset"};
      } = "model";
      int slice = 1;
      int cut_above =0;
      int cut = 1;
      int slice_axis=0;
      float slice_plane = 26.89;
      int node_data_comp = 1;
      int cell_data_comp = 0;
      int offset_comp = 0;
      float offset_scale = -0.86;
      enum vec_comp {
	 choices => {"x","y","z"};
      } = "x";
      int+nres vector => (<-.in.node_data[.node_data_comp].veclen > 1);
      enum data {
	 choices => {"none","cell",
	 "scalar","component"};
      } = "scalar";
      group coord {
	 float min_vec[3] = 0;
	 float max_vec[3] = 100;
      };
   };
   
   macro Bounding_boxes_func {
      
      ilink in_field;
      ilink in_deformed;
      
      XP_COMM_PROJ.Boundingboxes.BoundingboxesMacs.param_group+IPort2 &param_group;
      XP_COMM_PROJ.Boundingboxes.BoundingboxesMacs.QuadBox UndeformedBounds {
	 in_field => <-.in_field;
	 obj {
	    Obj {
	       visible => <-.<-.<-.param_group.bounds;
	    };
	    Props {
	       col = {0.,0.,0.};
	       line_aa = 1;
	       line_style => switch((<-.<-.<-.param_group.slice + 1),"Solid","Dotted");
	       inherit = 0;
	    };
	 };
      };
      XP_COMM_PROJ.Boundingboxes.BoundingboxesMacs.QuadBox DeformedBounds {
	 in_field => <-.in_deformed;
	 obj {
	    Obj {
	       visible => <-.<-.<-.param_group.bounds_deformed;
	    };
	    Modes {
	       mode = {0,2,1,0,0};
	    };
	    Props {
	       col = {0.,0.,0.};
	       line_aa = 1;
	       line_style => "Dashed";
	       inherit = 0;
	    };
	 };
      };
      DVM.DVswitch select_bounds {
	 in => {
	 <-.UndeformedBounds.out_fld,<-.DeformedBounds.out_fld};
	 index => <-.param_group.mesh;
      };
      DVM.DVswitch select_bounds_unmodified {
	 in => {
	 <-.DeformedBounds.out_obj,<-.UndeformedBounds.out_obj};
	 index => <-.param_group.mesh;
      };
      macro slice_or_cut_bounds {
	 ilink in_field => <-.select_bounds.out;
	 int component[] = {0};
	 int cell_data[] = {0};
	 MODS.slice slice {
	    UIpanel SliceUI;
	    UIpanel UIpanel;
	    in_field => <-.in_field;
	    in_plane => <-.<-.plane;
	    DVslice {
	       dist = 0.;
	       map_comp => {0};
	       cell_data => {0};
	    };
	 };
	 MODS.cut cut{
	    in_field => <-.in_field;
	    in_plane => <-.<-.plane;
	    CutParam {
	       component[] = {0};
	       dist = 0.;
	       above => <-.<-.<-.param_group.cut_above;
	       int cell_data[];
	    };
	    UIpanel UIpanel;
	    UIpanel CutUI;
	 };
	 MODS.external_edges external_edges {
	    UIpanel ExtEdgeUI;
	    in_field => <-.cut.out_fld;
	    edge_angle = 5.;
	 };
	 DVM.DVswitch select_slice_or_cut {
	    in => {<-.slice.out_fld,
	    <-.external_edges.out_fld};
	    index => <-.<-.param_group.cut;
	 };
	 GDM.DataObjectLite SliceOrCutBoundsObj {
	    in => <-.select_slice_or_cut.out;
	    Obj {
	       visible => (<-.<-.<-.param_group.slice & ((<-.<-.<-.param_group.bounds_deformed & <-.<-.<-.param_group.cut) | (<-.<-.<-.param_group.bounds & (!<-.<-.<-.param_group.mesh))));
	       name => name_of(<-.<-.<-);
	    };
	    Modes {
	       mode = {0,2,1,0,0};
	    };
	    Props {
	       col = {0.,0.,0.};
	       line_aa = 1;
	       inherit = 0;
	    };
	 };
      };
      link undeformed_uncut_bounds_obj => .UndeformedBounds.out_obj;
      link deformed_uncut_bounds_obj => .DeformedBounds.out_obj;
      link sliced_or_cut_bounds_obj => .slice_or_cut_bounds.SliceOrCutBoundsObj.obj;
      int dim1 = 2;
      int dim2 = 2;
      int dim3 = 2;
      group SliceParam<export_all=2> {
	 int axis => <-.param_group.slice_axis;
	 float+nres plane => <-.param_group.slice_plane;
	 int component[] => switch(((<-.show_node_data | <-.process_all_comps) + <-.process_all_comps),{
	 <-.param_group.node_data_comp},<-.all_comps);
	 int cell_data[] => switch(<-.show_cell_data,{
	 <-.param_group.cell_data_comp});
      };
      Mesh_Unif plane_xy {
	 nspace = 2;
	 ndim = 2;
	 dims => {dim1,dim2};
	 points+nres => {
	    in_field.coordinates.min_vec[0],in_field.coordinates.min_vec[1],
	 in_field.coordinates.max_vec[0],in_field.coordinates.max_vec[1]};
	 xform{
	    xlate+nres => cache({0.,0.,SliceParam.plane});
	 };
      };
      Mesh_Unif plane_yz {
	 nspace = 2;
	 ndim = 2;
	 dims => {dim3,dim2};
	 points+nres => {
	    in_field.coordinates.min_vec[2],in_field.coordinates.min_vec[1],
	 in_field.coordinates.max_vec[2],in_field.coordinates.max_vec[1]};
	 xform {
	    mat = {
	       0.,0.,1.,0.,0.,1.,0.,0.,-1.,0.,0.,0.,0.,0.,0.,1.
	    };
	    xlate+nres => cache({
	    SliceParam.plane,0.,0.});
	 };
      };
      Mesh_Unif plane_xz {
	 nspace = 2;
	 ndim = 2;
	 dims => {<-.dim1,<-.dim3};
	 points+nres => {
	    in_field.coordinates.min_vec[0],in_field.coordinates.min_vec[2],
	 in_field.coordinates.max_vec[0],in_field.coordinates.max_vec[2]};
	 xform {
	    mat = {
	       1.,0.,0.,0.,0.,0.,1.,0.,0.,-1.,0.,0.,0.,0.,0.,1.
	    };
	    xlate+nres => cache({0.,SliceParam.plane,0.});
	 };
      };
      Mesh_Unif planes[] {
	 nspace = 2;
	 ndim = 2;
	 } => {.plane_yz,.plane_xz,.plane_xy,.plane_free};
	 Mesh_Unif+OPort2 &plane => planes[.SliceParam.axis];
	 Mesh_Unif+OPort plane_free {
	    nspace = 2;
	    ndim = 2;
	    xform {
	       xlate+nres => cache({0.,0.,0.});
	    };
	    dims => {dim1,dim2};
	    points+nres => {
	       in_field.coordinates.min_vec[0],in_field.coordinates.min_vec[1],
	    in_field.coordinates.max_vec[0],in_field.coordinates.max_vec[1]};
	 };
	 GDM.DataObjectLite FreePlaneObj {
	    in => <-.plane_free;
	    Obj {
	       visible = 0;
	    };
	 };
	 link free_plane_obj => .FreePlaneObj.obj;
	 GMOD.select_array select_boxes {
	    select => {(<-.param_group.bounds | <-.param_group.slice),1,1,1};
	    array_input<NEportLevels={2,0}> => {
	       <-.undeformed_uncut_bounds_obj,<-.deformed_uncut_bounds_obj,
	    <-.sliced_or_cut_bounds_obj,<-.free_plane_obj};
	    array_output+OPort2;
	 };
	 GDM.GroupObject bboxes {
	    child_objs => <-.select_boxes.array_output;
	 };
	 olink out_obj => bboxes.obj;
      };

      macro Bounding_boxes_UI {
	 ilink in_field;
	 float+nres min_vec[3] => in_field.coordinates.min_vec;
	 float+nres max_vec[3] => in_field.coordinates.max_vec;
	 UItoggle UIdeformedToggle {
	    x = 4;
	    y = 4;
	    parent => <-.Bounding_boxes;
	    width => (.parent.clientWidth - 8);
	    label => "Show Deformed Bounds";
	    active => <-.param_group.deformed_valid;
	    set => <-.param_group.bounds_deformed;
	 };
	 UItoggle UIsliceToggle {
	    x = 4;
	    y => ((<-.UIdeformedToggle.y + <-.UIdeformedToggle.height) + 4);
	    parent => <-.Bounding_boxes;
	    width => (.parent.clientWidth - 8);
	    label => "Enable Slice Plane";
	    set => <-.param_group.slice;
	 };
	 string slice_axes[] = {"x","y","z","Free"};
	 UIradioBoxLabel UIsliceAxisRadioBox {
	    x = 4;
	    y => ((<-.UIsliceToggle.y + <-.UIsliceToggle.height) + 4);
	    parent => <-.Bounding_boxes;
	    width => (.parent.clientWidth - 8);
	    labels => <-.slice_axes;
	    title => "Axis";
	    selectedItem => <-.param_group.slice_axis;
	    UIradioBox {
	       x => (<-.UIlabel.width + 2);
	       y => 0;
	       orientation = "horizontal";
	       width => ((.parent.clientWidth / 5) * 4);
	       itemWidth => ((.parent.clientWidth - 8) / 5);
	       height => UIdata.UIfonts[0].lineHeight+ 4;
	    };
	    UIlabel {
	       width => (.parent.clientWidth / 5);
	       y = 2;
	       alignment = "left";
	    };
	    label_cmd {
	       cmd[];
	    };
	    active => <-.param_group.slice;
	 };
	 UIslider UIslicePlaneSlider {
	    x = 4;
	    y => ((<-.UIsliceAxisRadioBox.y + <-.UIsliceAxisRadioBox.height) + 4);
	    parent => <-.Bounding_boxes;
	    width => (((9 * .parent.clientWidth) / 10) - 6);
	    min+nres => <-.min_vec[<-.param_group.slice_axis];
	    max+nres => <-.max_vec[<-.param_group.slice_axis];
	    mode = "real";
	    title => "Plane";
	    value => <-.param_group.slice_plane;
	    active => (<-.<-.param_group.slice && (<-.<-.param_group.slice_axis < 3));
	 };
	 VUIslider_typein UIslicePlaneTypein {
	    slider => <-.UIslicePlaneSlider;
	 };
	 UImod_panel Bounding_boxes;
	 XP_COMM_PROJ.Boundingboxes.BoundingboxesMacs.param_group+IPort2 &param_group;
	 UItoggle Cut {
	    parent => <-.Bounding_boxes;
	    active => <-.<-.param_group.slice;
	    set => <-.param_group.cut;
	    width => ((.parent.clientWidth - 8) / 2) ;
	    x = 4;
	    y => ((<-.UIslicePlaneSlider.y + <-.UIslicePlaneSlider.height) + 4);
	 };
	 UItoggle Cut_above {
	    parent => <-.Bounding_boxes;
	    active =>(<-.<-.param_group.slice && <-.<-.param_group.cut);
	    set => <-.param_group.cut_above;
	    x => (.parent.clientWidth / 2);
	    y => <-.Cut.y;
	    
	 };
      };

      macro Bounding_boxes {
	 ilink in_field;
	 ilink in_deformed;
	 int cut = 1;
	 int cut_above = 0;
	 int+nres deformed_valid => is_valid(in_deformed);
	 XP_COMM_PROJ.Boundingboxes.BoundingboxesMacs.param_group param_group {
	    deformed_valid =><-.deformed_valid;
	    cut => <-.cut;
	    cut_above => <-.cut_above;
	 };
	 XP_COMM_PROJ.Boundingboxes.BoundingboxesMacs.Bounding_boxes_func Bounding_boxes_func {
	    in_field => <-.in_field;
	    in_deformed => <-.in_deformed;
	    param_group => <-.param_group;
	 };

	 XP_COMM_PROJ.Boundingboxes.BoundingboxesMacs.Bounding_boxes_UI Bounding_boxes_UI {
	    param_group => <-.param_group;
	    in_field => <-.in_field;
	 };
	 olink out_boxes => .Bounding_boxes_func.out_obj;
	 olink out_plane => .Bounding_boxes_func.plane;

      };

   };
