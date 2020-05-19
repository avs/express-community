
#define THERMO_PI 3.1415926526

flibrary ThermometerMods <compile_subs=0> {

   group+OPort ThermometerParams {
      float+Port2 x_pos;
      float+Port2 y_pos;
      float+Port2 radius;
      float+Port2 length;
      float+Port2 foreground_col[3];
      float+Port2 background_col[3];
   };

   macro ThermometerCircle {
      float x_pos<NEportLevels={2,1}>;
      float y_pos<NEportLevels={2,1}>;
      float radius<NEportLevels={2,1}>;

      int+Port subdiv = 16;

      Field_Spher_Unif+OPort circleField {
         int nnodes => prod(.dims);
         int dims[.ndim] => {<-.subdiv, <-.subdiv};
         int ndim = 2;
         int nspace = 2;
         float points[.npoints][.nspace] => {0.0, 0.0, <-.radius, (2 * THERMO_PI) };
         xform {
            xlate => {<-.<-.x_pos, <-.<-.y_pos, 0.0};
         };
      };

      GDM.DataObject DataObject {
         in => <-.circleField;
         Props {
            col<NEportLevels={4,1}>;
            inherit = 0;
         };
         Modes {
            mode = {1,1,2,1,1};
            normals = "None";
         };
      };

      olink out_obj => .DataObject.obj;
   };


   macro ThermometerRectangle {
      float x_pos<NEportLevels={2,1}>;
      float y_pos<NEportLevels={2,1}>;
      float height<NEportLevels={2,1}>;
      float width<NEportLevels={2,1}>;

      Field_Unif+OPort rectangleField {
         dims => {2,2};
         ndim = 2;
         nspace = 2;
         points => {
            { -(<-.width / 2), 0.0},
            {  (<-.width / 2), <-.height}
         };
         xform {
            xlate => {<-.<-.x_pos, <-.<-.y_pos, 0.0};
         };
      };


      GDM.DataObject DataObject {
         in => <-.rectangleField;
         Props {
            col<NEportLevels={4,1}>;
            inherit = 0;
         };
         Modes {
            mode = {1,1,2,1,1};
            normals = "None";
         };
      };
      olink out_obj => .DataObject.obj;

   };


};

