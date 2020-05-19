flibrary HiVisMacs {
  macro HiVis_Grid_Interp_Block
    {
      HiVisMods.HiVis_Grid_Interp HiVis_Grid_Interp_Block {
	id = "CellInterpBlock";
      };
    };

  macro HiVis_Grid_Interp_OLinear
    {
      HiVisMods.HiVis_Grid_Interp HiVis_Grid_Interp_OLinear {
	id = "CellInterpOLinear";
      };
    };

  macro HiVis_Grid_Interp_MLinear
    {
      HiVisMods.HiVis_Grid_Interp HiVis_Grid_Interp_MLinear {
	id = "CellInterpMLinear";
      };
    };

  macro HiVis_Attr_Comb_Epsilon {
    UIpanel HiVis_Attr_Comb_Epsilon {
      width = 600;
      height = 800;
      parent<NEportLevels={3,0}>;
    };
    UIradioBoxLabel UIradioBoxLabel {
      parent => <-.HiVis_Attr_Comb_Epsilon;
      labels => <-.HiVis_Grid.attributeName;
      selectedItem<NEportLevels={0,2}> = 0;
      title => "Attribute";
      label_cmd {
	cmd[6] = {
                  
	  {
	    set=1,,,,,,,,,,,,,,,,,,,,
	  },
	  ,,,,
	};
	outItem => inItem;
      };
    };
    HiVisMods.HiVis_Attr_Comb_Epsilon 
      HiVis_Attr_Comb_Epsilon_Mod<NEportLevels={0,2}> {
	epsilon => <-.UIdial.value;
	attribute => <-.UIradioBoxLabel.selectedItem;
	epsCalculateBy => <-.Calculate_By.selectedItem;
	epsApplyOn => <-.Apply_On.selectedItem;
      };
    HiVisMods.HiVis_Grid &HiVis_Grid<NEportLevels={2,0},export=1>;
    UIdial UIdial {
      parent => <-.HiVis_Attr_Comb_Epsilon;
      value<NEportLevels={0,2}> = 0.;
      title => "Epsilon";
      y => ((<-.UIradioBoxLabel.y + <-.UIradioBoxLabel.height) + <-.Y_Pad);
      x = 20;
      width = 180;
      height = 140;
      min = 0.;
      mode = "real";
      decimalPoints = 6;
      immediateMode = 0;
      valuePerRev<NEportLevels={2,0}> => (<-.HiVis_Grid.maxScalarValue[<-.UIradioBoxLabel.selectedItem] - <-.HiVis_Grid.minScalarValue[<-.UIradioBoxLabel.selectedItem]);
      showValue = 1;
      style => ;
    };
    UItoggle Manual_Mode {
      parent => <-.HiVis_Attr_Comb_Epsilon;
      y => ((<-.Apply_On.y + <-.Apply_On.height) + <-.Y_Pad);
      width = 300;
    };
    UIoptionMenu Calculate_By {
      parent => <-.HiVis_Attr_Comb_Epsilon;
      cmdList => {
	<-.Scalar_Difference,<-.Vector_Difference};
      selectedItem = 1;
      y => ((<-.UIdial.y + <-.UIdial.height) + <-.Y_Pad);
      width = 250;
    };
    UIoptionMenu Apply_On {
      parent => <-.HiVis_Attr_Comb_Epsilon;
      cmdList => {
	<-.Maximum_Difference,<-.Average_Difference};
      selectedItem = 1;
      y => ((<-.Calculate_By.y + <-.Calculate_By.height) + <-.Y_Pad);
      width = 250;
    };
    UIoptionBox UIoptionBox {
      y = 346;
    };
    UIbutton Update {
      parent => <-.HiVis_Attr_Comb_Epsilon;
      y => ((<-.Manual_Mode.y + <-.Manual_Mode.height) + <-.Y_Pad);
    };
    int Y_Pad<NEportLevels=1> = 10;
    UIoption Scalar_Difference {
      set = 1;
    };
    UIoption Vector_Difference;
    UIoption Maximum_Difference {
      set = 1;
    };
    UIoption Average_Difference;
  };
  macro Field_To_HiVis {
    UImod_panel Field_To_HiVis {
      parent<NEportLevels={3,0}>;
      option {
	set = 1;
      };
      title => name_of(<-.<-, 1);
    };
    HiVisMods.Field_To_HiVis Field_To_HiVis_Mod {
      in<NEportLevels={3,0}>;
      assoc => (<-.Attribute_Association.selectedItem);
      out<NEportLevels={0,3}>;
    };
    UIoption Inherit { set = 1; };
    UIoption Node;
    UIoption Cell;
    UIoptionMenu Attribute_Association {
      parent => <-.Field_To_HiVis;
      cmdList => {<-.Inherit,<-.Node,<-.Cell};
      selectedItem = 0;
      y = 0;
    };
  };
  macro AG_Octree {
    UImod_panel AG_Octree {
      parent<NEportLevels={3,0}>;
      title => name_of(<-.<-, 1);
    };
    HiVisMods.HiVis_Grid &in<NEportLevels={2,1}>;
    UIslider Max_tree_level {
      y = 0;
      active = 1;
      parent => <-.AG_Octree;
      min<NEportLevels={0,0},export=1> = 0.;
      max<NEportLevels={2,0},export=1> => <-.int;
      value<NEportLevels={1,2}> = 100.;
      decimalPoints = 0;
      horizontal = 1;
      increment = 1.;
    };
    HiVisMods.AG_Octree AG_Octree_Mod {
      level => <-.Max_tree_level.value;
      out<NEportLevels={0,3}>;
      in => <-.in;
      attr_Comb => <-.HiVis_Attr_Comb_Epsilon.HiVis_Attr_Comb_Epsilon_Mod;
    };
    HiVisMacs.HiVis_Attr_Comb_Epsilon HiVis_Attr_Comb_Epsilon {
      HiVis_Grid => <-.in;
      HiVis_Attr_Comb_Epsilon {
      	parent => <-.<-.AG_Octree;
      };
    };
    int int<NEportLevels=1> => .AG_Octree_Mod.maxLevel;
  };
  macro HiVis_Render_SparseGrid {
    HiVisMods.HiVis_Render_SparseGrid HiVis_Render_SparseGrid {
      in<NEportLevels={3,0}>;
      attribute = 0;
      ldScale = 0;
      renderType = 0;
      out<NEportLevels={0,3}>;
    };
    DataObject DataObject {
      in => <-.HiVis_Render_SparseGrid.out;
      obj<NEportLevels={1,3}>;
    };
  };
  macro HiVis_Read_Octree {
    UImod_panel UImod_panel {
      parent<NEportLevels={3,0}>;
      title => name_of(<-.<-, 1);
      option {
	set = 1;
      };
    };
    UIbutton Browse {
      parent => <-.UImod_panel;
      y => <-.UItext.y;
      height => <-.UItext.height;
      x => ((UItext.width + UItext.x) + 4);
      width = 75;
    };
    UIfileDialog File_Dialog {
      visible => <-.Browse.do;
      cancel = 1;
      ok = 1;
      searchPattern = "*.oc";
    };
    HiVisMods.HiVis_Read_Octree HiVis_Read_Octree {
      fileName => <-.UItext.text;
      out<NEportLevels={0,3}>;
    };
    UIlabel Octree_Filename {
      parent => <-.UImod_panel;
      y = 0;
      width = 200;
    };
    UItext UItext {
      parent => <-.UImod_panel;
      text<NEportLevels={2,2}> => <-.File_Dialog.filename;
      y => <-.Octree_Filename.y + <-.Octree_Filename.height + 10;
      width = 150;
    };
    link Filename<NEportLevels={1,2}> => .UItext.text;
  };
  macro HiVis_Read_SparseGrid {
    UImod_panel UImod_panel {
      parent<NEportLevels={3,0}>;
      title => name_of(<-.<-, 1);
      option {
	set = 1;
      };
    };
    UIbutton Browse {
      parent => <-.UImod_panel;
      y => <-.UItext.y;
      height => <-.UItext.height;
      x => ((UItext.width + UItext.x) + 4);
      width = 75;
    };
    UIslider Max_Cell_Size_Ratio {
      parent => <-.UImod_panel;
      min<NEportLevels={0,0}> = 1.;
      max<NEportLevels={2,0}> = 10.;
      value<NEportLevels={0,2}> = 1.;
      decimalPoints = 0;
      active => <-.Specify_Cell_Size_Ratio.set;
      y => <-.Specify_Cell_Size_Ratio.y +
      <-.Specify_Cell_Size_Ratio.height + 4;
    };
    UItoggle Specify_Cell_Size_Ratio {
      parent => <-.UImod_panel;
      y => <-.Browse.y + <-.Browse.height + 10;
      width = 300;
      set = 0;
    };
    UIfileDialog File_Dialog {
      visible => <-.Browse.do;
      cancel = 1;
      ok = 1;
      searchPattern = "*.sg";
    };
    HiVisMods.HiVis_Read_SparseGrid HiVis_Read_SparseGrid {
      fileName => <-.UItext.text;
      out<NEportLevels={0,3}>;
      maxLdScopeDiff =>
	<-.Max_Cell_Size_Ratio.value * <-.Specify_Cell_Size_Ratio.set;
    };
    UIlabel SparseGrid_Filename {
      parent => <-.UImod_panel;
      y = 0;
      width = 200;
    };
    UItext UItext {
      parent => <-.UImod_panel;
      text<NEportLevels={2,2}> => <-.File_Dialog.filename;
      y => <-.SparseGrid_Filename.y + <-.SparseGrid_Filename.height + 10;
      width = 150;
    };
    link Filename<NEportLevels={1,2}> => .UItext.text;
  };
  macro HiVis_Read_Field {
    UImod_panel UImod_panel {
      parent<NEportLevels={3,0}>;
      title => name_of(<-.<-, 1);
      option {
	set = 1;
      };
    };
    UIbutton Browse {
      parent => <-.UImod_panel;
      y => <-.UItext.y;
      height => <-.UItext.height;
      x => ((UItext.width + UItext.x) + 4);
      width = 75;
    };
    UIfileDialog File_Dialog {
      visible => <-.Browse.do;
      cancel = 1;
      ok = 1;
      searchPattern = "*.fld";
    };
    HiVisMods.HiVis_Read_Field HiVis_Read_Field {
      fileName => <-.UItext.text;
      out<NEportLevels={0,3}>;
    };
    UIlabel Field_Filename {
      parent => <-.UImod_panel;
      y = 0;
      width = 200;
    };
    UItext UItext {
      parent => <-.UImod_panel;
      text<NEportLevels={2,2}> => <-.File_Dialog.filename;
      y => <-.Field_Filename.y + <-.Field_Filename.height + 10;
      width = 150;
    };
    link Filename<NEportLevels={1,2}> => .UItext.text;
  };
  macro AG_SparseGrid {
    UImod_panel AG_SparseGrid {
      parent<NEportLevels={3,0}>;
      title => name_of(<-.<-, 1);
    };
    HiVisMods.HiVis_Grid &in<NEportLevels={2,1}>;
    UIslider Max_tree_level {
      y = 0;
      active = 1;
      parent => <-.AG_SparseGrid;
      min<NEportLevels={0,0},export=1> = 0.;
      max<NEportLevels={2,0},export=1> => <-.int;
      value<NEportLevels={1,2}> = 100.;
      decimalPoints = 0;
      horizontal = 1;
      increment = 1.;
    };
    HiVisMods.AG_SparseGrid AG_SparseGrid_Mod {
      level => <-.Max_tree_level.value;
      out<NEportLevels={0,3}>;
      in => <-.in;
      attr_Comb => <-.HiVis_Attr_Comb_Epsilon.HiVis_Attr_Comb_Epsilon_Mod;
    };
    HiVisMacs.HiVis_Attr_Comb_Epsilon HiVis_Attr_Comb_Epsilon {
      HiVis_Grid => <-.in;
      HiVis_Attr_Comb_Epsilon {
      	parent => <-.<-.AG_SparseGrid;
      };
    };
    int int<NEportLevels=1> => .AG_SparseGrid_Mod.maxLevel;
  };
  macro AG_OrthoSlice {
    UImod_panel AG_OrthoSlice {
      option {
	set = 1;
      };
      title => name_of(<-.<-, 1);
      parent<NEportLevels={3,0}>;
    };
    UItoggle Adjust_Min_Max {
      parent => <-.AG_OrthoSlice;
      label => "Adjust Min/Max";
      width = 300;
    };
    UIslider Pos {
      parent => <-.AG_OrthoSlice;
      min<NEportLevels={0,0}> = 0.;
      max<NEportLevels={2,0}> => <-.maxPos;
      value<NEportLevels={0,2}> = 0.;
      decimalPoints = 0;
    };
    HiVisMods.AG_OrthoSlice AG_OrthoSlice_Mod {
      in<NEportLevels={3,0}>;
      axis => <-.axis;
      pos => <-.Pos.value;
      adjust_Min_Max => <-.Adjust_Min_Max.set;
      out<NEportLevels={0,3}>;
    };
    int maxAxis<NEportLevels=1> =>
      (.AG_OrthoSlice_Mod.in.dim - 1);
    int maxPos<NEportLevels={1,1}>
			      => (.AG_OrthoSlice_Mod.in.size[.axis] - 1);
    int axis<NEportLevels=1> => .Axis.value;
    UIslider Axis {
      parent => <-.AG_OrthoSlice;
      value<NEportLevels={0,2}> = 0.;
      max<NEportLevels={2,0}> => <-.maxAxis;
      min<NEportLevels={0,0}> = 0.;
      decimalPoints = 0;
    };
  };
  macro HiVis_Bounds {
    UImod_panel UImod_panel {
      title => name_of(<-.<-, 1);
      parent<NEportLevels={3,0}>;
    };
    HiVis.HiVisMods.HiVis_Bounds HiVis_Bounds {
      in<NEportLevels={3,0}>;
      attribute = 0;
      renderType = 0;
      cropBorder = 0;
      out<NEportLevels={0,3}>;
    };
    DataObject DataObject {
      in => <-.HiVis_Bounds.out;
      obj<NEportLevels={1,3}>;
    };
    UIradioBoxLabel Color_Attribute {
      parent => <-.UImod_panel;
      labels => <-.HiVis_Bounds.in.attributeName;
      selectedItem<NEportLevels={2,2}> = 0;
      title => "Color Attribute";
      //label_cmd {
      //	cmd[];
      //};
      //active => (.value != 0);
      //y => ((Iso_Value.y + Iso_Value.height) + 10);
    };
  };
  macro HiVis_Iso {
    UImod_panel HiVis_Iso {
      option {
	set = 1;
      };
      title => name_of(<-.<-, 1);
      parent<NEportLevels={3,0}>;
    };
    UItoggle Adjust_Min_Max {
      parent => <-.HiVis_Iso;
      label => "Adjust Min/Max";
      width = 300;
      y => 0;
    };
    UItoggle Invert_Faces {
      parent => <-.HiVis_Iso;
      label => "Invert Faces";
      active => <-.HiVis_Iso_Mod.in.dim == 3;
      width = 300;
      y => ((Adjust_Min_Max.y + Adjust_Min_Max.height) + 10);
    };
    UIslider Iso_Value {
      parent => <-.HiVis_Iso;
      //value => 
      //<-.HiVis_Iso_Mod.in.minScalarValue[Iso_Attribute.selectedItem];
      min => HiVis_Iso_Mod.in.minScalarValue[Iso_Attribute.selectedItem];
      max =>
	HiVis_Iso_Mod.in.maxScalarValue[Iso_Attribute.selectedItem];
      y => ((Invert_Faces.y + Invert_Faces.height) + 10);
    };
    UIradioBoxLabel Iso_Attribute {
      parent => <-.HiVis_Iso;
      labels => <-.HiVis_Iso_Mod.in.attributeName;
      selectedItem<NEportLevels={2,2}> = 0;
      title => "Iso Attribute";
      //label_cmd {
      //cmd[];
      //};
      //active => (.value != 0);
      y => ((Iso_Value.y + Iso_Value.height) + 10);
    };

    UIradioBoxLabel Color_Attribute {
      parent => <-.HiVis_Iso;
      labels => <-.HiVis_Iso_Mod.in.attributeName;
      selectedItem<NEportLevels={2,2}> = 0;
      title => "Color Attribute";
      //y => ((Ranges.y + Ranges.height) + 10);
      //label_cmd {
      //	cmd[];
      //};
      //active => (.value != 0);
      y => ((Iso_Attribute.y + Iso_Attribute.height) + 10);
    };

    HiVisMods.HiVis_Iso HiVis_Iso_Mod {
      in<NEportLevels={3,0}>;
      attribute => <-.Iso_Attribute.selectedItem;
      colorattribute => <-.Color_Attribute.selectedItem;
      isoval => <-.Iso_Value.value;
      invertfaces => <-.Invert_Faces.set;
      out<NEportLevels={0,3}>;
    };
    DataObject DataObject {
      in => <-.HiVis_Iso_Mod.out;
      obj<NEportLevels={1,3}> {
	name => name_of(<-.<-.<-,1);
      };
    };
  };
  macro HiVis_Render_Field {
    DataObject DataObject {
      in => <-.HiVis_Render_Field_Mod.out;
      obj<NEportLevels={1,3}> {
	name => name_of(<-.<-.<-,1);
      };
    };
    olink out_fld<NEportLevels={1,2}> => .HiVis_Render_Field_Mod.out;
    UImod_panel HiVis_Render_Field {
      parent<NEportLevels={3,0}>;
      title => name_of(<-.<-, 1);
    };
    UIradioBoxLabel Attribute_to_show_for_color {
      parent => <-.HiVis_Render_Field;
      labels<NEportLevels={2,0}> => <-.string;
      selectedItem<NEportLevels={0,2}> = 0;
      label_cmd {
	//cmd[1];
	outItem => .inItem;
      };
    };
    HiVisMods.HiVis_Render_Field HiVis_Render_Field_Mod {
      in<NEportLevels={3,0}>;
      attribute => <-.Attribute_to_show_for_color.selectedItem;
    };
    string string<NEportLevels=1>[] => HiVis_Render_Field_Mod.in.attributeName;
  };


  macro HiVis_Render_Cells {
    UImod_panel HiVis_Render_Cells {
      title => name_of(<-.<-, 1);
      parent<NEportLevels={3,0}>;
      option {
	set = 1;
      };
    };

    // Module

    HiVisMods.HiVis_Render_Cells HiVis_Render_Cells_Mod {
      in<NEportLevels={3,0}>;
      attribute => <-.Color_Attribute.selectedItem;
      ipad => <-.Inner_Pad.value;
      renderType => <-.Cell_Render_Type.selectedItem;
      assoc => (<-.Attribute_Association.selectedItem);
      ranges => <-.Ranges.value;
      rangeMin = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
      rangeMax = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
      cropBorder = 0;
      rangeAttribute => <-.Range_Attribute.selectedItem;
    };

    olink out_fld => .HiVis_Render_Cells_Mod.out;
    DataObject DataObject {
      in => <-.HiVis_Render_Cells_Mod.out;
      obj<NEportLevels={1,3}> {
	name => name_of(<-.<-.<-,1);
      };
    };
    
    // UI Crop to interpolation field

    //UItoggle Crop_to_interpolation_field {
    //parent => <-.HiVis_Render_Cells;
    // y = 0;
    // width = 300;
    //};

    // UI Attribute Association

    UIoption Inherit { set = 1; };
    UIoption Node;
    UIoption Cell;
    UIoptionMenu Attribute_Association {
      parent => <-.HiVis_Render_Cells;
      cmdList => {<-.Inherit,<-.Node,<-.Cell};
      selectedItem = 0;
      y = 0;
    };

    UIradioBoxLabel Color_Attribute {
      parent => <-.HiVis_Render_Cells;
      labels => <-.AttributeName;
      selectedItem<NEportLevels={2,2}> = 0;
      title => "Color Attribute";
      y => ((<-.Attribute_Association.y + <-.Attribute_Association.height) + 10);
      //label_cmd {
      //	cmd[];
      //};
    };

    UIslider Inner_Pad {
      y => ((<-.Cell_Render_Type.y + <-.Cell_Render_Type.height) + 10);
      parent => <-.HiVis_Render_Cells;
      min = 0.;
      max = 1.;
      value = 0.;
    };
    string AttributeName[] => HiVis_Render_Cells_Mod.in.attributeName;
    UIoptionMenu Cell_Render_Type {
      parent => <-.HiVis_Render_Cells;
      cmdList => {<-.Edge,<-.Face};
      selectedItem = 0;
      x = 0;
      y => ((<-.Color_Attribute.y + <-.Color_Attribute.height) + 10);
    };
    UIoption Edge {
      set = 1;
    };
    UIoption Face;
    UIoptionMenu Range_Setting;
    UIslider Ranges {
      parent => <-.HiVis_Render_Cells;
      value = 0.;
      y => (Inner_Pad.y + Inner_Pad.height);
      max = 20.;
      decimalPoints = 0;
    };
    UIoptionMenu Range_selection {
      parent => <-.HiVis_Render_Cells;
      cmdList => <-.Range;
      selectedItem = 0;
      x = 0;
      y => ((Range_Attribute.y + Range_Attribute.height) + 10);
      active => (Ranges.value != 0);
    };
    UIoption Range[.Ranges.value];
    UIslider Min {
      parent => <-.HiVis_Render_Cells;
      y => ((Range_selection.y + Range_selection.height) + 10);
      value => HiVis_Render_Cells_Mod.rangeMin[<-.Range_selection.selectedItem];
      active => (Ranges.value != 0);
      min => HiVis_Render_Cells_Mod.in.minScalarValue[Range_Attribute.selectedItem];
      max => HiVis_Render_Cells_Mod.in.maxScalarValue[Range_Attribute.selectedItem];
    };
    UIslider Max {
      parent => <-.HiVis_Render_Cells;
      y => (Min.y + Min.height);
      value => HiVis_Render_Cells_Mod.rangeMax[<-.Range_selection.selectedItem];
      active => (Ranges.value != 0);
      min => HiVis_Render_Cells_Mod.in.minScalarValue[Range_Attribute.selectedItem];
      max => HiVis_Render_Cells_Mod.in.maxScalarValue[Range_Attribute.selectedItem];
    };
    UIradioBoxLabel Range_Attribute {
      parent => <-.HiVis_Render_Cells;
      labels => HiVis_Render_Cells_Mod.in.attributeName;
      selectedItem<NEportLevels={2,2}> = 0;
      title => "Range Attribute";
      y => ((Ranges.y + Ranges.height) + 10);
      UIradioBox;
      UIlabel;
      label_cmd {
	cmd[];
      };
      selectedString;
      active => (Ranges.value != 0);
    };
  };
  macro Render_Slice {
    macro AG_OrthoSlice {
      UImod_panel HiVis_Render_Slice<NEportLevels={0,2}> {
	parent<NEportLevels={4,0}>;
	option {
	  set = 1;
	};
      };
      UItoggle Adjust_Min_Max {
	y = 0;
	parent => <-.HiVis_Render_Slice;
	label => "Adjust Min/Max";
	width = 300;
      };
      UIslider Pos {
	y = 24;
	parent => <-.HiVis_Render_Slice;
	min<NEportLevels={0,0}> = 0.;
	max<NEportLevels={2,0}> => <-.maxPos;
	value<NEportLevels={0,2}> = 0.;
	decimalPoints = 0;
      };
      HiVisMods.AG_OrthoSlice AG_OrthoSlice_Mod {
	in<NEportLevels={4,0}>;
	axis => <-.axis;
	pos => <-.Pos.value;
	adjust_Min_Max => <-.Adjust_Min_Max.set;
	out<NEportLevels={0,3}>;
      };
      int maxAxis<NEportLevels=1> => (.AG_OrthoSlice_Mod.in.dim - 1);
      int maxPos<NEportLevels={1,1}> => (.AG_OrthoSlice_Mod.in.size[.axis] - 1);
      int axis<NEportLevels=1> => .Axis.value;
      UIslider Axis {
	y = 84;
	parent => <-.HiVis_Render_Slice;
	min<NEportLevels={0,0}> = 0.;
	max<NEportLevels={2,0}> => <-.maxAxis;
	value<NEportLevels={0,2}> = 0.;
	decimalPoints = 0;
      };
    };
    macro HiVis_Render_Field {
      olink out_fld<NEportLevels={1,2}> => .HiVis_Render_Field_Mod.out;
      DataObject DataObject {
	in => <-.HiVis_Render_Field_Mod.out;
	obj<NEportLevels={1,3}> {
	  name => name_of(<-.<-.<-,1);
	};
      };
      UIradioBoxLabel Attribute_to_show_for_color {
	parent<NEportLevels={3,1}> => <-.<-.AG_OrthoSlice.HiVis_Render_Slice;
	labels<NEportLevels={2,0}> => <-.string;
	selectedItem<NEportLevels={0,2}> = 0;
	y => (AG_OrthoSlice.Axis.y + AG_OrthoSlice.Axis.height);
	label_cmd {
	  cmd[1];
	  outItem => .inItem;
	};
      };
      HiVisMods.HiVis_Render_Field HiVis_Render_Field_Mod {
	in<NEportLevels={3,0}> => <-.<-.AG_OrthoSlice.AG_OrthoSlice_Mod.out;
	attribute => <-.Attribute_to_show_for_color.selectedItem;
      };
      string string<NEportLevels=1>[] => HiVis_Render_Field_Mod.in.attributeName;
    };
  };
  macro HiVis_Streamlines {
    macro HiVis_Streamline_UI {
      int Order<NEportLevels={1,1}> = 1;
      int comp_Color<NEportLevels={1,1}> => Color_Attribute.selectedItem;
      boolean ribbon_Inverse_Faces<NEportLevels={1,1}> = 0;
      float scale_Ribbon<NEportLevels={1,1}> = 0.5;
      int vis_Type<NEportLevels={1,1}> => (.Visualization_Type.selectedItem + 1);
      int comp_Vector<NEportLevels={1,1}> = 0;
      UImod_panel HiVis_Streamlines {
	parent<NEportLevels={4,0}>;
	title => name_of(<-.<-.<-, 1);
	option {
	  set = 1;
	};
      };
      UItoggle Adjust_Min_Max {
	y => ((Update.y + Update.height) + YPad);
	parent => <-.HiVis_Streamlines;
	label => "Adjust Min/Max";
	set = 1;
	width = 300;
      };
      UItoggle Go_Backward {
	y => ((Adjust_Min_Max.y + Adjust_Min_Max.height) + YPad);
	parent => <-.HiVis_Streamlines;
	set = 1;
      };
      boolean stream_Go_Backward<NEportLevels={1,1}> => .Go_Backward.set;
      int comp_ScaleX<NEportLevels=1> => ParticleUI.Scale_X_Attribute_Val;
      UIoptionMenu Visualization_Type {
	x = 0;
	y => ((<-.Color_Attribute.y + <-.Color_Attribute.height) + <-.YPad);
	parent => <-.HiVis_Streamlines;
	cmdList => {<-.Line,<-.Ribbon,
		    <-.Particle};
	selectedItem = 0;
      };
      UIoption Line;
      UIoption Ribbon;
      UIoption Particle {
	set = 1;
      };
      UIoption Tube;
      string attributes<NEportLevels=1>[] => <-.HiVis_Render_Streamline.in.attributeName;
      UIradioBoxLabel Color_Attribute {
	parent => <-.HiVis_Streamlines;
	labels => <-.attributes;
	selectedItem = 0;
	label_cmd {
	  cmd[4] = {
	    {
	      set=1,,,,,,,,,,,,,,,,,,,,
	    },
	    ,,};
	  outItem => inItem;
	};
	y => ((Go_Backward.y + Go_Backward.height) + YPad);
      };
      int YPad<NEportLevels=1> = 10;
      macro LineUI {
	UIpanel LinePanel {
	  visible<NEportLevels={3,0}> => (<-.<-.Visualization_Type.selectedItem == 0);
	  parent<NEportLevels={3,0}> => <-.<-.VisTypePanel;
	  y = 0;
	  height = 40;
	};
	int height<NEportLevels={1,2}> = 0;
      };
      macro RibbonUI {
	UIpanel UIpanel {
	  visible<NEportLevels={3,0}> => (<-.<-.Visualization_Type.selectedItem == 1);
	  parent<NEportLevels={3,0}> => <-.<-.VisTypePanel;
	  y = 0;
	  height => ((Stripe_Factor_Val.y + Stripe_Factor_Val.height) + YPad);
	};
	int height<NEportLevels={1,2}> => UIpanel.height;
	UIradioBoxLabel Scale_Attribute {
	  labels => <-.mlink;
	  selectedItem = 0;
	  label_cmd {
	    cmd[4] = {
	      {
		set=1,,,,,,,,,,,,,,,,,,,,
	      },
	      ,,};
	  };
	};
	UIradioBoxLabel Stripe_Attribute {
	  parent => <-.UIpanel;
	  labels => <-.mlink;
	  selectedItem = 0;
	  label_cmd {
	    cmd[4] = {
	      {
		set=1,,,,,,,,,,,,,,,,,,,,
	      },
	      ,,};
	  };
	  active => <-.Do_Stripe.set;
	  y => ((<-.Do_Stripe.y + <-.Do_Stripe.height) + YPad);
	};
	UIfield Stripe_Factor_Val {
	  parent => <-.UIpanel;
	  value = 1.;
	  y => Stripe_Factor.y;
	  x => (Stripe_Factor.x + Stripe_Factor.width);
	  active => <-.Do_Stripe.set;
	  message = "Stripe Scale";
	  min = 0.;
	  decimalPoints = 2;
	};
	mlink mlink<NEportLevels={2,1}> => <-.attributes;
	UIlabel Stripe_Factor {
	  parent => <-.UIpanel;
	  height => ((UIdata.UIfonts[0].lineHeight + 0) + Stripe_Factor_Val.height);
	  label => "Stripe Factor:";
	  y => ((Stripe_Attribute.y + Stripe_Attribute.height) + YPad);
	  active => <-.Do_Stripe.set;
	  alignment = "left";
	};
	UIfield Scale_Factor_Val {
	  value = 1.;
	  y => Scale_Factor.y;
	  x => (Scale_Factor.x + Scale_Factor.width);
	};
	UIlabel Scale_Factor {
	  label => "Scale Factor:";
	  y => ((Scale_Attribute.y + Scale_Attribute.height) + YPad);
	  alignment = "left";
	};
	UItoggle Do_Stripe {
	  parent => <-.UIpanel;
      width = 300;
	  y = 0;
	};
	int Stripe_Attribute_Val<NEportLevels=1> => (((.Do_Stripe.set == 0) * -1) + (.Stripe_Attribute.selectedItem * (.Do_Stripe.set == 1)));
      };
      macro ParticleUI {
	UIpanel UIpanel {
	  visible<NEportLevels={3,0}> => (<-.<-.Visualization_Type.selectedItem == 2);
	  parent<NEportLevels={3,0}> => <-.<-.VisTypePanel;
	  y = 0;
	  height => ((<-.Scale_X_Attribute.y + <-.Scale_X_Attribute.height) + <-.<-.YPad);
	};
	int height<NEportLevels={1,2}> => UIpanel.height;
	UIradioBoxLabel Scale_Attribute {
	  parent => <-.UIpanel;
	  labels => <-.mlink;
	  selectedItem = 0;
	  label_cmd {
	    cmd[4] = {
	      {
		set=1,
	      },,,};
	  };
	  active => <-.Do_Particle_Scale.set;
	  y => ((<-.Do_Particle_Scale.y + <-.Do_Particle_Scale.height) + <-.<-.YPad);
	  title => "Particle Scale Attribute";
	};
	UIradioBoxLabel Scale_X_Attribute {
	  parent => <-.UIpanel;
	  labels => <-.mlink;
	  selectedItem = 0;
	  label_cmd {
	    cmd[4] = {
	      {
		set=1,,,,,,,,,,,,,,,,,,,,
	      },
	      ,,};
	  };
	  active => <-.Do_Particle_Scale_X.set;
	  y => ((<-.Do_Particle_Scale_X.y + <-.Do_Particle_Scale_X.height) + <-.<-.YPad);
	  title => "Particle Scale X Attribute";
	};
	UIfield Distance_Factor_Val {
	  parent => <-.UIpanel;
	  value = 1.;
	  y => <-.Distance_Factor.y;
	  x => (<-.Distance_Factor.x + <-.Distance_Factor.width);
	  active = 1;
	  message = "Stripe Scale";
	  min = 0.;
	  decimalPoints = 2;
	};
	mlink mlink<NEportLevels={2,1}> => <-.attributes;
	UIlabel Distance_Factor {
	  parent => <-.UIpanel;
	  height => ((UIdata.UIfonts[0].lineHeight + 0) + <-.Distance_Factor_Val.height);
	  label => "Distance Factor:  ";
	  y => ((<-.Distance_Attribute.y + <-.Distance_Attribute.height) + <-.<-.YPad);
	  active = 1;
	  alignment = "right";
	  width = 150;
	};
	UIfield Scale_Factor_Val {
	  parent => <-.UIpanel;
	  value = 1.0;
	  y => Scale_Factor.y;
	  x => (Scale_Factor.x + Scale_Factor.width);
	  active = 1;
	};
	UIlabel Scale_Factor {
	  parent => <-.UIpanel;
	  label => "Particle Scale Factor:  ";
	  y => ((Scale_Attribute.y + Scale_Attribute.height) + YPad);
	  alignment = "right";
	  width = 150;
	  active = 1;
	};
	UItoggle Do_Particle_Scale_X {
	  parent => <-.UIpanel;
	  y => ((<-.Scale_Factor_Val.y + <-.Scale_Factor_Val.height) + <-.<-.YPad);
      width = 300;
	};
	UIradioBoxLabel Distance_Attribute {
	  parent => <-.UIpanel;
	  labels => <-.mlink;
	  selectedItem = 0;
	  label_cmd {
	    cmd[4] = {
	      {
		set=1,,,,,,,,,,,,,,,,,,,,
	      },
	      ,,};
	  };
	};
	UItoggle Do_Particle_Scale {
	  parent => <-.UIpanel;
	  label => "Do Particle Scale by Attribute";
	  y => ((<-.Distance_Factor.y + <-.Distance_Factor.height) + <-.<-.YPad);
	  width = 200;
	};
	int Scale_Attribute_Val<NEportLevels=1> => (((.Do_Particle_Scale.set == 0) * -1) + (.Scale_Attribute.selectedItem * (.Do_Particle_Scale.set == 1)));
	int Scale_X_Attribute_Val<NEportLevels=1> => (((.Do_Particle_Scale_X.set == 0) * -1) + (.Scale_X_Attribute.selectedItem * (.Do_Particle_Scale_X.set == 1)));
      };
      UIpanel VisTypePanel {
	parent => <-.HiVis_Streamlines;
	y => ((<-.Visualization_Type.y + <-.Visualization_Type.height) + <-.YPad);
	height => (((vtype == 0) * LineUI.height) + (((vtype == 1) * RibbonUI.height) + ((vtype == 2) * ParticleUI.height)));
      };
      int vtype<NEportLevels=1> => .Visualization_Type.selectedItem;
      UIoptionMenu Integration_Method {
	parent => <-.HiVis_Streamlines;
	cmdList => {<-.Euler};
	selectedItem = 0;
	y => ((<-.VisTypePanel.y + <-.VisTypePanel.height) + <-.YPad);
	x = 0;
      };
      UIoption Euler {
	set = 1;
      };
      UIbutton Update {
	parent => <-.HiVis_Streamlines;
	y = 0;
	active => Manual_Update.set;
	x => (Manual_Update.x + Manual_Update.width);
      };
      UItoggle Manual_Update {
	parent => <-.HiVis_Streamlines;
	y = 5;
	width = 300;
      };
      int Stripe_Comp<NEportLevels=1>[] => 
	{ -1,
	  RibbonUI.Stripe_Attribute_Val,
	  ParticleUI.Distance_Attribute.selectedItem};
      float Stripe_Factor<NEportLevels=1>[] =>
	{ 0,
	  .RibbonUI.Stripe_Factor_Val.value,
	  .ParticleUI.Distance_Factor_Val.value};
      int Scale_Comp<NEportLevels=1>[] =>
	{-1,-1,
	 ParticleUI.Scale_Attribute_Val};
      float Scale_Factor<NEportLevels=1>[] =>
	{0,0,
	 .ParticleUI.Scale_Factor_Val.value};
      UIfield Steps_Per_Cell_Val {
	parent => <-.HiVis_Streamlines;
	value = 20.;
	y => 
	  ((<-.Integration_Method.y + <-.Integration_Method.height) + <-.YPad);
	x => (<-.Steps_Per_Cell.x + <-.Steps_Per_Cell.width);
	min = 0.;
	decimalPoints = 0;
      };
      UIlabel Steps_Per_Cell {
	parent => <-.HiVis_Streamlines;
	label => "Steps Per Cell:  ";
	y => 
	  ((<-.Integration_Method.y + <-.Integration_Method.height) + <-.YPad);
	alignment = "right";
      };
      UIfield Max_Steps_Val {
	parent => <-.HiVis_Streamlines;
	value = 20000.;
	y => 
	  ((<-.Steps_Per_Cell_Val.y + <-.Steps_Per_Cell_Val.height) + <-.YPad);
	x => (<-.Steps_Per_Cell.x + <-.Steps_Per_Cell.width);
	min = 0.;
	decimalPoints = 0;
      };
      UIlabel Max_Steps {
	parent => <-.HiVis_Streamlines;
	label => "Max Steps:  ";
	y => 
	  ((<-.Steps_Per_Cell_Val.y + <-.Steps_Per_Cell_Val.height) + <-.YPad);
	alignment = "right";
      };
    };
    HiVisMods.HiVis_Render_Streamline HiVis_Render_Streamline {
      out<NEportLevels={0,3}>;
      order => <-.HiVis_Streamline_UI.Order;
      in<NEportLevels={3,0}>;
      probe<NEportLevels={3,0}>;
      particle_Shape<NEportLevels={3,0}>;
      max_Steps => <-.HiVis_Streamline_UI.Max_Steps_Val.value;
      steps_Per_Cell => <-.HiVis_Streamline_UI.Steps_Per_Cell_Val.value;
      stream_Go_Backward => <-.HiVis_Streamline_UI.stream_Go_Backward;
      vis_Type => <-.HiVis_Streamline_UI.vis_Type;
      scale_Ribbon => <-.HiVis_Streamline_UI.scale_Ribbon;
      scale_Object => <-.HiVis_Streamline_UI.Scale_Factor[<-.HiVis_Streamline_UI.vtype];
      scale_Stripe => <-.HiVis_Streamline_UI.Stripe_Factor[<-.HiVis_Streamline_UI.vtype];
      comp_Stripe => <-.HiVis_Streamline_UI.Stripe_Comp[<-.HiVis_Streamline_UI.vtype];
      comp_Color => <-.HiVis_Streamline_UI.comp_Color;
      comp_Scale => <-.HiVis_Streamline_UI.Scale_Comp[<-.HiVis_Streamline_UI.vtype];
      ribbon_Inverse_Faces => <-.HiVis_Streamline_UI.ribbon_Inverse_Faces;
      adjust_Min_Max => <-.HiVis_Streamline_UI.Adjust_Min_Max.set;
      comp_Vector => <-.HiVis_Streamline_UI.comp_Vector;
      comp_ScaleX => <-.HiVis_Streamline_UI.comp_ScaleX;
      manual_Update => <-.HiVis_Streamline_UI.Manual_Update.set;
      do_Update => <-.HiVis_Streamline_UI.Update.do;
    };
    DataObject DataObject {
      obj<NEportLevels={1,3}> {
	name => name_of(<-.<-.<-,1);
      };
      in => <-.HiVis_Render_Streamline.out;
    };
  };
  macro HiVis_To_Field {
    UImod_panel UImod_panel {
      option {
	set = 1;
      };
      parent<NEportLevels={3,0}>;
      title => name_of(<-.<-, 1);
    };
    HiVisMods.HiVis_To_Field HiVis_To_Field {
      in<NEportLevels={3,0}>;
      adjust_Min_Max => <-.Adjust_Min_Max.set;
      out<NEportLevels={0,3}>;
    };
    DataObject DataObject {
      in => <-.HiVis_To_Field.out;
      obj<NEportLevels={1,3}> {
	name => name_of(<-.<-.<-,1);
      };
    };
    UItoggle Adjust_Min_Max {
      parent => <-.UImod_panel;
      label => "Adjust Min/Max";
      width = 300;
    };
  };
  macro AG_Cell2Node {
    UImod_panel HiVis_Grid_Interp {
      parent<NEportLevels={3,0}>;
      title => name_of(<-.<-, 1);
    };
    HiVisMods.AG_Cell2Node AG_Cell2Node_Mod {
      in<NEportLevels={3,0}>;
      gridInterp => (<-.DVswitch.out);
      out<NEportLevels={0,3}>;
    };
    HiVisMacs.HiVis_Grid_Interp_Block HiVis_Grid_Interp_Block;
    HiVisMacs.HiVis_Grid_Interp_OLinear HiVis_Grid_Interp_OLinear;
    HiVisMacs.HiVis_Grid_Interp_MLinear HiVis_Grid_Interp_MLinear;
    UIoptionMenu Interpolate {
      parent => <-.HiVis_Grid_Interp;
      cmdList => {<-.Block,<-.Linear,<-.MLinear};
      selectedItem = 0;
      y = 0;
    };
    UIoption Block {
      set = 1;
    };
    UIoption Linear;
    UIoption MLinear;
    DVM.DVswitch DVswitch {
      in => {
	<-.HiVis_Grid_Interp_Block.HiVis_Grid_Interp_Block,
	<-.HiVis_Grid_Interp_OLinear.HiVis_Grid_Interp_OLinear,
	<-.HiVis_Grid_Interp_MLinear.HiVis_Grid_Interp_MLinear};
      index => <-.Interpolate.selectedItem;
      out;
    };
  };
  macro ArrowHead3D {
    Mesh ArrowHead {
      coordinates {
	values = {
	  1.,-0.5,1.,1.,-0.5,-1.,1.,0.5,-1.,1.,0.5,1.,-1.,0.,0.,
	  -1.,0.,0.
	};
      };
      nspace = 3;
      nnodes = 6;
      cell_set = {
	{
	  node_connect_list={2,5,3,0,4,1},
	  ncells=2,cell_nnodes=3,poly_flag=0,cell_order=1,,cell_ndim=2
	},
	{
	  node_connect_list=
	  {
	    0,3,5,4,0,1,2,3,1,2,5,4
	  },
	  ncells=3,cell_nnodes=4,poly_flag=0,cell_order=1,,cell_ndim=2
	}};
      ncell_sets = 2;
    };
    DataObject DataObject {
      in => <-.ArrowHead;
      obj<NEportLevels={1,3}> {
	name => name_of(<-.<-.<-,1);
      };
    };
    link link<NEportLevels={1,2}> => .ArrowHead;
  };
  macro Uniform_Vector_Field {
    FLD_MAP.extract_data_array extract_data_array#2 {
      UIradioBoxLabel_mode2 {
	label_cmd {
	  cmd[6] = {
	    ,
	    {
	      set=1,,,,,,,,,,,,,,,,,,,,
	    },
	    ,,,
	  };
	  outItem => .inItem;
	};
      };
      comp = 1;
      in => <-.Read_Field.field;
    };
    FLD_MAP.extract_data_array extract_data_array {
      UIradioBoxLabel_mode2 {
	label_cmd {
	  cmd[6] = {
	    ,,
	    {
	      set=1,,,,,,,,,,,,,,,,,,,,
	    },
	    ,,
	  };
	  outItem => .inItem;
	};
      };
      comp = 2;
      in => <-.Read_Field.field;
    };
    FLD_MAP.extract_data_array extract_data_array#1 {
      in => <-.Read_Field.field;
    };
    MODS.Read_Field Read_Field {
      read_field_ui {
	file_browser {
	  x = 495;
	  y = 289;
	  width = 300;
	  height = 100;
	  ok = 1;
	  //dirMaskCache = "/home/breitlip/projects/DA/samples/AVS/Nast/*.fld";
	};
	//filename = "$HIVIS_ROOT/samples/AVS/Nast/data.fld";
	panel {
	  parent<NEportLevels={5,0}>;
	};
      };
      DVread_field {
	Mesh_Unif+Node_Data Output_Field;
      };
      field;
    };
    FLD_MAP.interleave_3_arrays interleave_3_arrays {
      in1 => <-.extract_data_array#1.data;
      in2 => <-.extract_data_array#2.data;
      in3 => <-.extract_data_array.data;
    };
    int int<NEportLevels={1,1}>[] => Read_Field.field.dims;
    FLD_MAP.uniform_vector_field uniform_vector_field {
      data {
	out {
	  node_data = {
	    {
	      labels="Velocity",,,,,,,,,,
	    }};
	};
	in_data => <-.<-.interleave_3_arrays.out;
      };
      out<NEportLevels={0,3}>;
      mesh {
	in_dims => <-.<-.int;
	out {
	  coordinates;
	};
      };
    };
  };
  APPS.SingleWindowApp RenderSliceEg {
    UI {
      shell {
	x = 105;
	y = 143;
      };
      Modules {
	IUI {
	  optionList {
	    selectedItem = 2;
	    cmdList => {
	      <-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
	      <-.<-.<-.<-.bounds.UIpanel.option,
	      <-.<-.<-.<-.AG_OrthoSlice.AG_OrthoSlice.option,
	      <-.<-.<-.<-.HiVis_Render_Field.HiVis_Render_Field.option};
	  };
	};
      };
      Windows {
	IUI {
	  panel {
	    width = 652;
	    height = 552;
	  };
	  optionList {
	    selectedItem = 0;
	  };
	};
      };
      exit_dialog {
	ExitDialog {
	  cancel = 1;
	};
      };
      Editors {
	IUI {
	  optionList {
	    selectedItem = 0;
	  };
	};
      };
    };
    GDM.Uviewer3D Uviewer3D {
      Scene {
	View {
	  View {
	    trigger = 1;
	  };
	};
	Top {
	  Xform {
	    mat = {
	      -0.179467,0.000573229,0.277087,0.,-0.276982,0.00879465,-0.179417,
	      0.,-0.00769282,-0.330012,-0.0043002,0.,0.,0.,0.,1.
	    };
	    xlate = {1.34914,-5.93941,
		     -6.17413};
	  };
	  child_objs => {
	    <-.<-.<-.bounds.out_obj,<-.<-.<-.HiVis_Render_Field.DataObject};
	};
	Camera {
	  Camera {
	    perspec = 1;
	    front = 1.;
	  };
	};
      };
      Scene_Editor {
	View_Editor {
	  InfoPanel {
	    curobj_sel {
	      width = 238;
	      height = 245;
	      x = 0;
	      y = 0;
	    };
	  };
	};
	Camera_Editor {
	  GDcamera_edit {
	    front = 1.;
	  };
	};
      };
    };

    MODS.Read_Field Read_Field {
      read_field_ui {
	file_browser {
	  x = 495;
	  y = 289;
	  width = 300;
	  height = 100;
	  ok = 1;
	  dirMaskCache = "*.fld";
	};
	filename = "xp_comm_proj/HiVis/data/liquidCavity.fld";
      };
      DVread_field {
	Mesh_Unif+Node_Data Output_Field;
      };
      field;
    };
    HiVisMods.Field_To_HiVis Field_To_HiVis {
      out {
	attributeCount<NEportLevels={0,1},export=1>;
      };
      in => <-.Read_Field.field;
    };
    MODS.bounds bounds {
      in_field => <-.Read_Field.field;
      BoundsParam {
	edges = 1;
	kmax = 1;
	data = 1;
	component = 3;
      };
      BoundsUI {
	DVnode_data_labels {
	  labels[];
	};
	UIradioBoxLabel {
	  label_cmd {
	    cmd[] = {
	      ,,,
	      {
		set=1,
	      },,
	    };
	  };
	};
      };
    };
    AG_OrthoSlice AG_OrthoSlice {
      Adjust_Min_Max {
	y = 0;
      };
      Pos {
	value = 0.;
	y = 16;
      };
      AG_OrthoSlice_Mod {
	in => <-.<-.Field_To_HiVis.out;
      };
      Axis {
	y = 48;
      };
    };
    HiVis_Render_Field HiVis_Render_Field {
      HiVis_Render_Field_Mod {
	in => <-.<-.AG_OrthoSlice.AG_OrthoSlice_Mod.out;
      };
      Attribute_to_show_for_color {
	selectedItem = 3;
	label_cmd {
	  cmd[6] = {
	    ,,,
	    {
	      set=1,
	    },,
	  };
	};
      };
    };
  };
};
