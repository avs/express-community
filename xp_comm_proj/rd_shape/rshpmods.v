
flibrary ReadShapeMods <build_dir="xp_comm_proj/rd_shape"> {

   module ReadShapeHeader < src_file="shprdhdr.cxx",
                            cxx_src_files="gsshape.cxx
                                           gsshapeh.cxx
                                           gsbyteu.cxx",
                            out_src_file="hdr_gen.cxx",
                            out_hdr_file="hdr_gen.h" > {
      string+IPort2 ShapeFileName;

      cxxmethod+notify_inst+req ReadShapeHeader(
         .ShapeFileName+read+notify+req,
         .ShapeType+write,
         .NumberOfFeatues+write
      );

      int+OPort2    ShapeType;
      int+OPort2    NumberOfFeatures;
   };


   module ReadShapePoint < src_file="shprdpnt.cxx",
                           cxx_src_files="gsshape.cxx
                                          gsshapeh.cxx
                                          gsbyteu.cxx",
                           out_src_file="pnt_gen.cxx",
                           out_hdr_file="pnt_gen.h" > {
      string+IPort2 ShapeFileName;
      int+IPort2    FeatureNumber;

      cxxmethod+notify_inst+req ReadShapePoint(
         .ShapeFileName+read+notify+req,
         .FeatureNumber+read+notify,
         .Coordinates+write,
         .ShapeType+write,
         .NumberOfFeatures+write,
         .FeatureID+write
      );

      int NumberOfPoints;

      double+OPort2 Coordinates[NumberOfPoints][2];
      int+OPort2    ShapeType;
      int+OPort2    NumberOfFeatures;
      int+OPort2    FeatureID[NumberOfPoints];
   };


   module ReadShapeArc < src_file="shprdarc.cxx",
                         cxx_src_files="gsshape.cxx
                                        gsshapeh.cxx
                                        gsbyteu.cxx",
                         out_src_file="arc_gen.cxx",
                         out_hdr_file="arc_gen.h" > {

      string+IPort2 ShapeFileName;
      int+IPort2    FeatureNumber;

      cxxmethod+notify_inst+req ReadShapeArc(
         .ShapeFileName+read+notify+req,
         .FeatureNumber+read+notify,
         .Coordinates+write,
         .ConnectList+write,
         .ShapeType+write,
         .FeatureID+write
      );

      int NumberOfPoints;
      int NumberOfConnections;
      int NumberOfPolylines;

      double+OPort2 Coordinates[NumberOfPoints][2];
      int+OPort2    ConnectList[NumberOfConnections];
      int+OPort2    ShapeType;
      int+OPort2    FeatureID[NumberOfPolylines];
   };


   module ReadShapePolygon < src_file="shprdplg.cxx",
                             cxx_src_files="gsshape.cxx
                                            gsshapeh.cxx
                                            gspgtree.cxx
                                            gsnode.cxx
                                            gspgnode.cxx
                                            gsvrtlst.cxx
                                            gsbyteu.cxx",
                             cxx_hdr_files="gsshape.h
                                            gspgtree.h",
                             c_src_files="gsmisc.c
                                          gstri.c
                                          gscnstrc.c
                                          gsmntone.c",
                             out_src_file="plg_gen.cxx",
                             out_hdr_file="plg_gen.h" > {
      string+IPort2 ShapeFileName;
      int+IPort2    FeatureNumber;
      int+IPort2    TriangulateAllPolygons = 0;

      cxxmethod+notify_inst+req ReadShapePolygon(
         .ShapeFileName+read+notify+req,
         .FeatureNumber+read+notify,
         .TriangulateAllPolygons+read+notify,
         .Coordinates+write,
         .PolyNodes+write,.shape_type+write,
         .ConnectList+write,
         .ShapeType+write,
         .FeatureID+write
      );

      int NumberOfPoints;
      int NumberOfPolyNodes;
      int NumberOfConnections;
      int NumberOfPolygons;

      double+OPort2 Coordinates[NumberOfPoints][2];
      int+OPort2    ConnectList[NumberOfConnections];
      int+OPort2    PolyNodes[NumberOfPolyNodes];
      int+OPort2    ShapeType;
      int+OPort2    FeatureID[NumberOfPolygons];
   };


   module ReadShapePolygonAsLine < src_file="shprdpgl.cxx",
                                   cxx_src_files="gsshape.cxx
                                                  gsshapeh.cxx
                                                  gsbyteu.cxx",
                                   out_src_file="pgl_gen.cxx",
                                   out_hdr_file="pgl_gen.h" > {
      string+IPort2 ShapeFileName;
      int+IPort2    FeatureNumber;

      cxxmethod+notify_inst+req ReadShapePolygonAsLine(
         .ShapeFileName+read+notify+req,
         .FeatureNumber+read+notify,
         .Coordinates+write,
         .ConnectList+write,
         .ShapeType+write,
         .FeatureID+write
      );

      int NumberOfPoints;
      int NumberOfConnections;
      int NumberOfPolylines;

      double+OPort2 Coordinates[NumberOfPoints][2];
      int+OPort2    ConnectList[NumberOfConnections];
      int+OPort2    ShapeType;
      int+OPort2    FeatureID[NumberOfPolylines];
   };

};

