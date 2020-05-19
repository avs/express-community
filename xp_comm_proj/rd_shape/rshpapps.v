
flibrary ReadShapeApps <compile_subs=0> {

   APPS.SingleWindowApp ReadShapeUserEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.read_shape_rural.out_obj,
                  <-.<-.<-.read_shape_urban.out_obj,
                  <-.<-.<-.read_shape_roads.out_obj
               };
            };
         };
      };

      XP_COMM_PROJ.ReadShape.ReadShapeMacs.read_shape read_shape_rural {
         ReadShapeParams {
            filename = "xp_comm_proj/rd_shape/data/mdworld3.shp";
         };
         ReadShape {
            PolygonShape {
               polyhedron_mesh {
                  DataObject {
                     Props {
                        col = {0.11232,0.52,0.104};
                     };
                  };
               };
            };
         };
      };

      XP_COMM_PROJ.ReadShape.ReadShapeMacs.read_shape read_shape_urban {
         ReadShapeParams {
            filename = "xp_comm_proj/rd_shape/data/pppolyp.shp";
         };
         ReadShape {
            PolygonShape {
               polyhedron_mesh {
                  DataObject {
                     Props {
                        col = {1.,0.3,0.342};
                     };
                  };
               };
            };
         };
      };

      XP_COMM_PROJ.ReadShape.ReadShapeMacs.read_shape read_shape_roads {
         ReadShapeParams {
            filename = "xp_comm_proj/rd_shape/data/rdline.shp";
         };
      };

   };


   APPS.SingleWindowApp ReadShapeProgramEg {
      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.CittaPolygonShape.out_obj,
                  <-.<-.<-.HighwaysArcShape.out_obj,
                  <-.<-.<-.RailroadsArcShape.out_obj,
                  <-.<-.<-.RiverArcShape.out_obj,
                  <-.<-.<-.CityNames_glyph.out_obj
               };
            };
         };
      };

      string+Port citta_str     = "xp_comm_proj/rd_shape/data/pppolyp";
      string+Port highways_str  = "xp_comm_proj/rd_shape/data/rdline";
      string+Port railroads_str = "xp_comm_proj/rd_shape/data/rrline";
      string+Port river_str     = "xp_comm_proj/rd_shape/data/ponetl";
      string+Port cities_str    = "xp_comm_proj/rd_shape/data/ppanno";

      XP_COMM_PROJ.ReadShape.ReadShapeMacs.Program.PolygonShape CittaPolygonShape {
         filename => <-.citta_str;
         polyhedron_mesh {
            DataObject {
               Obj {
                  name => "citta";
               };
               Props {
                  col = {0.2949,0.75,0.135};
               };
            };
         };
      };
      XP_COMM_PROJ.ReadShape.ReadShapeMacs.Program.ArcShape HighwaysArcShape {
         filename => <-.highways_str;
         polyline_mesh {
            DataObject {
               Obj {
                  name => "highways";
               };
               Props {
                  col = {1.,0.3,0.342};
               };
            };
         };
      };
      XP_COMM_PROJ.ReadShape.ReadShapeMacs.Program.ArcShape RailroadsArcShape {
         filename => <-.railroads_str;
         polyline_mesh {
            DataObject {
               Obj {
                  name => "railroads";
               };
               Props {
                  col = {0.89,0.89,0.89};
               };
            };
         };
      };
      XP_COMM_PROJ.ReadShape.ReadShapeMacs.Program.ArcShape RiverArcShape {
         filename => <-.river_str;
         polyline_mesh {
            DataObject {
               Obj {
                  name => "river";
               };
            };
         };
      };


      XP_COMM_PROJ.ReadShape.ReadShapeMacs.Program.PointShape PointShape {
         filename => <-.cities_str;
      };

      string+Port column = "NAME";
      XP_COMM_PROJ.ReadDbase.ReadDbaseMods.ReadDBFString ReadDBFString {
         DBFFileName => <-.cities_str;
         ColumnName => <-.column;
      };

      MODS.text_glyph CityNames_glyph {
         in_mesh => <-.PointShape.out_mesh;
         in_text => <-.ReadDBFString.ColumnValues;
         TextUI {
            Font {
               text = "-adobe-times-bold-r-*-*-12-*-*-*-*-*-*-*";
            };
         };
      };

   };

};

