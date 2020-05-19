
flibrary ReadDXFMacs<compile_subs=0> {

   macro ReadDXF_UI {
      XP_COMM_PROJ.ReadDXF.ReadDXFMods.ReadDXFParams &params<NEportLevels={2,1}>;
      string &layernames<NEportLevels={2,1}>[];

      UImod_panel panel {
         parent<NEportLevels={4,0}>;
         message = "Select the DXF file to read";
         title => "DXF File Reader";
      };

      UItoggle fillPolyToggle {
         parent => <-.panel;
         label = "Fill Polygons";
         &set+IPort2 => <-.params.fill_polygons;
         x = 0;
         y = 0;
      };
      UItoggle revNormToggle {
         parent => <-.panel;
         label = "Reverse Normals";
         &set+IPort2 => <-.params.reverse_normals;
         x => <-.fillPolyToggle.x;
         y => <-.fillPolyToggle.y + <-.fillPolyToggle.height + 5;
      };
      UIlabel fileLabel {
         parent => <-.panel;
         label => "DXF file";
         alignment = "left";
         x => <-.fillPolyToggle.x;
         y => <-.revNormToggle.y + <-.revNormToggle.height + 10;
      };
      UItext fileText {
         parent => <-.panel;
         &text<NEportlevels={2,0}> => <-.params.file_name;
         x => <-.fillPolyToggle.x;
         y => <-.fileLabel.y + <-.fileLabel.height + 5;
         width => parent.width - x - <-.fileButton.width - 4;
      };
      UIbutton fileButton {
         parent => <-.panel;
         label = "Browse ...";
         x => parent.width - width - 2;
         y => <-.fileText.y;
      };

      UIfileSB fileBrowser {
         &filename+IPort2 => <-.params.file_name;
         title = "Read DXF file";
         searchPattern = "*.dxf";
         visible+IPort2 => fileButton.do;
      };


      UIoptionBoxLabel layersOptionBox {
         parent => <-.panel;
         labels+IPort2 => <-.layernames;
         &selectedItems+IPort2 => <-.params.layers;
         title = "Select DXF Layers";
         x => <-.fillPolyToggle.x;
         y => <-.fileText.y + <-.fileText.height + 20;
         width => parent.width;
      };

   };



   macro ReadDXFFunc {
      XP_COMM_PROJ.ReadDXF.ReadDXFMods.ReadDXFParams &params<NEportLevels={2,1}>;

      XP_COMM_PROJ.ReadDXF.ReadDXFMods.ReadDXFCore ReadDXFCore {
         params => <-.params;
         layernames+OPort2;
      };

      XP_COMM_PROJ.CatCellSets.CatCSMacs.cat_cell_sets cat_cell_sets[array_size(ReadDXFCore.out)] {
         in => ReadDXFCore.out[index_of(cat_cell_sets)];
      };

      XP_COMM_PROJ.ExtractObjects.ExtractObjectsMods.ExtractObjectsCore ExtractObjectsCore {
         in_objs  => <-.cat_cell_sets.out;
         inc_objs => <-.params.layers;
      };

      GDM.DataObject GeomObject[array_size(ExtractObjectsCore.out_objs)] {
         in => ExtractObjectsCore.out_objs[index_of(GeomObject)];
         Obj.name => name_of(<-.<-.<-) + "_Geom_" + index_of(GeomObject);
         Obj.xform_mode = "Parent";
      };
      GDM.GroupObject GeomGroupObject {
         child_objs => <-.GeomObject.obj;
         Top.name => name_of(<-.<-.<-) + "_GeomObj";
      };

      GDM.DataObject TextObject[array_size(ReadDXFCore.Text)] {
         in => ReadDXFCore.Text[index_of(TextObject)];
         Obj.name => name_of(<-.<-.<-) + "_Text_" + index_of(TextObject);
         Obj.xform_mode = "Parent";
      };
      GDM.GroupObject TextGroupObject {
         child_objs => <-.TextObject.obj;
         Top.name => name_of(<-.<-.<-) + "_TextObj";
      };

      omlink out_flds => .ExtractObjectsCore.out_objs;
      olink  out_geom => .GeomGroupObject.obj;
      olink  out_text => .TextGroupObject.obj;
      omlink layernames => .ReadDXFCore.layernames;
   };


   macro readDXF {
      XP_COMM_PROJ.ReadDXF.ReadDXFMods.ReadDXFParams params {
         reverse_normals = 0;
         fill_polygons   = 0;
      };

      XP_COMM_PROJ.ReadDXF.ReadDXFMacs.ReadDXFFunc ReadDXFFunc {
         params => <-.params;
         GeomObject.Obj.name      => name_of(<-.<-.<-.<-) + "_Geom_" + index_of(GeomObject);
         GeomGroupObject.Top.name => name_of(<-.<-.<-.<-) + "_GeomObj";
         TextObject.Obj.name      => name_of(<-.<-.<-.<-) + "_Text_" + index_of(TextObject);
         TextGroupObject.Top.name => name_of(<-.<-.<-.<-) + "_TextObj";
      };


      GMOD.parse_v parse_v {
         on_inst = 0;
         trigger => <-.ReadDXFFunc.ReadDXFCore.nOut;
         relative => <-.params;
         v_commands = "layers = init_array(<-.ReadDXFFunc.ReadDXFCore.nOut, 0, (<-.ReadDXFFunc.ReadDXFCore.nOut-1) );";
         parse_v_relative<weight=0>;
      };

      XP_COMM_PROJ.ReadDXF.ReadDXFMacs.ReadDXF_UI ReadDXF_UI {
         params => <-.params;
         layernames => <-.ReadDXFFunc.layernames;
         panel.title => name_of(<-.<-.<-);
      };

      omlink out_flds => .ReadDXFFunc.out_flds;
      olink out_geom => .ReadDXFFunc.out_geom;
      olink out_text => .ReadDXFFunc.out_text;
   };

};

