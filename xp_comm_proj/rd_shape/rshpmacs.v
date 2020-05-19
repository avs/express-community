
flibrary ReadShapeMacs <compile_subs=0> {

   library Program {
      macro PointShape {
         ilink filename;

         XP_COMM_PROJ.ReadShape.ReadShapeMods.ReadShapePoint ReadShapePoint {
            ShapeFileName => <-.filename;
         };
         FLD_MAP.point_mesh point_mesh {
            coord => <-.ReadShapePoint.Coordinates;
         };

         olink out_mesh => point_mesh.out;
         olink out_obj => point_mesh.obj;
      };


      macro ArcShape {
         ilink filename;

         XP_COMM_PROJ.ReadShape.ReadShapeMods.ReadShapeArc ReadShapeArc {
            ShapeFileName => <-.filename;
         };
         FLD_MAP.polyline_mesh polyline_mesh {
            coord => <-.ReadShapeArc.Coordinates;
            connect => <-.ReadShapeArc.ConnectList;
         };

         olink out_mesh => polyline_mesh.out;
         olink out_obj => polyline_mesh.obj;
      };


      macro PolygonShape {
         ilink filename;

         XP_COMM_PROJ.ReadShape.ReadShapeMods.ReadShapePolygon ReadShapePolygon {
            ShapeFileName => <-.filename;
         };
         FLD_MAP.polyhedron_mesh polyhedron_mesh {
            coord => <-.ReadShapePolygon.Coordinates;
            poly_nodes => <-.ReadShapePolygon.PolyNodes;
            connect => <-.ReadShapePolygon.ConnectList;
         };

         olink out_mesh => polyhedron_mesh.out;
         olink out_obj => polyhedron_mesh.obj;
      };


      macro PolygonAsLineShape {
         ilink filename;

         XP_COMM_PROJ.ReadShape.ReadShapeMods.ReadShapePolygonAsLine ReadShapePolygonAsLine {
            ShapeFileName => <-.filename;
         };
         FLD_MAP.polytri_mesh polytri_mesh {
            coord => <-.ReadShapePolygonAsLine.Coordinates;
            connect => <-.ReadShapePolygonAsLine.ConnectList;
         };

         olink out_mesh => polytri_mesh.out;
         olink out_obj => polytri_mesh.obj;
      };
   };


   group+OPort ReadShapeParams {
      int+Port2    ShapeSelection;
      string+Port2 filename;
   };


   macro ReadShapeUI {
      ReadShapeParams &ReadShapeParams<NEportLevels={2,1}>;

      UImod_panel UImod_panel {
         title = "Read Shape";
         message = "Select Read Shape control panel.";
         parent<NEportLevels={4,0}>;
      };

      UIlabel UIlabel {
         parent => <-.UImod_panel;
         label = "Shape File Name:";
         x = 0;
         y = 0;
         width => parent.width;
      };

      UItext FilenameUItext {
         parent => <-.UImod_panel;
         text => <-.ReadShapeParams.filename;
         x => <-.UIlabel.x;
         y => <-.UIlabel.y + <-.UIlabel.height + 5;
         width => parent.width * (2./3.) - 3;
      };
      UIbutton BrowseUIbutton {
         parent => <-.UImod_panel;
         label = "Browse...";
         y => <-.FilenameUItext.y + ((<-.FilenameUItext.height - .height)/2);
         x => <-.FilenameUItext.x + <-.FilenameUItext.width + 3;
         width => parent.width * (1./3.) - 3;
      };

      UIfileDialog ShapeUIfileDialog {
         visible => <-.BrowseUIbutton.do;
         title = "Select Shape File";
         filename => <-.ReadShapeParams.filename;
         searchPattern = "*.shp";
      };

      UIradioBoxLabel ShapetypeUIradioBoxLabel {
         parent => <-.UImod_panel;
         labels = {"Point","Line","Polygon","Polygon as Line"};
         title = "Shape Type";
         selectedItem => <-.ReadShapeParams.ShapeSelection;
         x => <-.FilenameUItext.x;
         y => <-.FilenameUItext.y + <-.FilenameUItext.height + 10;
         width => parent.width;
      };
   };


   module Mesh_switch {
   	Mesh+IPort2 &in[];
   	int+IPort2   index = 0;
   	Mesh+OPort2 &out => in[index];
   };

   module DefaultObject_switch {
   	DefaultObject+IPort2 &in[];
   	int+IPort2   index = 0;
   	DefaultObject+OPort2 &out => in[index];
   };


   macro ReadShape {
      ReadShapeParams &ReadShapeParams<NEportLevels={2,1}>;

      string+OPort f0 => switch((.ReadShapeParams.ShapeSelection + 1),.ReadShapeParams.filename);
      string+OPort f1 => switch( .ReadShapeParams.ShapeSelection     ,.ReadShapeParams.filename);
      string+OPort f2 => switch((.ReadShapeParams.ShapeSelection - 1),.ReadShapeParams.filename);
      string+OPort f3 => switch((.ReadShapeParams.ShapeSelection - 2),.ReadShapeParams.filename);

      //GMOD.err_handler err_handler;

      Program.PointShape PointShape {
         filename => <-.f0;
         point_mesh {
            obj {
               name => name_of(<-.<-.<-.<-.<-);
            };
         };
      };
      Program.ArcShape ArcShape {
         filename => <-.f1;
         polyline_mesh {
            obj {
               name => name_of(<-.<-.<-.<-.<-);
            };
         };
      };
      Program.PolygonShape PolygonShape {
         filename => <-.f2;
         polyhedron_mesh {
            obj {
               name => name_of(<-.<-.<-.<-.<-);
            };
         };
      };
      Program.PolygonAsLineShape PolygonAsLineShape {
         filename => <-.f3;
         polytri_mesh {
            obj {
               name => name_of(<-.<-.<-.<-.<-);
            };
         };
      };

      Mesh_switch switch_mesh {
         in => {<-.PointShape.out_mesh,<-.ArcShape.out_mesh,
                <-.PolygonShape.out_mesh,<-.PolygonAsLineShape.out_mesh};
         index => <-.ReadShapeParams.ShapeSelection;
      };
      DefaultObject_switch switch_obj {
         in => {<-.PointShape.out_obj,<-.ArcShape.out_obj,
                <-.PolygonShape.out_obj,<-.PolygonAsLineShape.out_obj};
         index => <-.ReadShapeParams.ShapeSelection;
      };

      olink out_mesh => .switch_mesh.out;
      olink out_obj  => .switch_obj.out;
   };


   macro read_shape {

      ReadShapeParams ReadShapeParams {
         ShapeSelection => <-.ShapeSelection;
      };

      XP_COMM_PROJ.ReadShape.ReadShapeMods.ReadShapeHeader ReadShapeHeader {
         ShapeFileName => <-.ReadShapeParams.filename;
         ShapeType = 0;       // Ugly hack around Uviewer problem
                              // If a Uviewer is supplied a unresolved link to a DataObject
                              // it will refuse to connect.  Therefore ShapeType must have a definite value.
      };
      int+Port ShapeSelection => switch( (.ReadShapeHeader.ShapeType+1), 0,0,0,1,0,2,0,0,0);

      ReadShapeUI ReadShapeUI {
         ReadShapeParams => <-.ReadShapeParams;
         UImod_panel {
            title => name_of(<-.<-.<-,1);
         };
      };

      ReadShape ReadShape {
         ReadShapeParams => <-.ReadShapeParams;

         PointShape {
            point_mesh {
               obj {
                  name => name_of(<-.<-.<-.<-.<-.<-);
               };
            };
         };
         ArcShape {
            polyline_mesh {
               obj {
                  name => name_of(<-.<-.<-.<-.<-.<-);
               };
            };
         };
         PolygonShape {
            polyhedron_mesh {
               obj {
                  name => name_of(<-.<-.<-.<-.<-.<-);
               };
            };
         };
         PolygonAsLineShape {
            polytri_mesh {
               obj {
                  name => name_of(<-.<-.<-.<-.<-.<-);
               };
            };
         };
      };

      olink out_mesh => ReadShape.out_mesh;
      olink out_obj => ReadShape.out_obj;
   };

}; // end of lib

