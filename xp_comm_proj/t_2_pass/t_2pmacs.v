
flibrary Transparency2PassMacs <compile_subs=0> {

//
// Transparency1Pass - One Pass Transparency Rendering Macro
//
// Takes a single field, and renders it
// with single-pass transparency.

   macro Transparency1Pass {
      ilink in_fld;
      float transparency<NEportLevels={2,1}> = 0.4;

      GDM.DataObject DataObject {
         in=> <-.in_fld;
         Props {
            inherit = 0;
            cull = "Normal";
            trans => <-.<-.transparency;
         };
         Obj.name => name_of(<-.<-.<-);
      };

      olink out_obj => DataObject.obj;
   };



//
// Transparency2Pass - Two Pass Transparency Rendering Macro
//
// Takes a single field, and renders it
// with two-pass transparency. First
// render back faces, then render front faces.
//
// Object is effectively split into two halves, each with
// different settings for "culling". They are
// then combined back together into a group object.
// This is a very simple method, and does not take care of
// all transparency sorting issues. It works for a single
// convex object, such as a teapot. It will be less effective
// for complex concave objects, and does deal with multiple
// overlapping objects in the same scene.
//
// I. Curington, 3 August 2000, Advanced Visual Systems Inc.
//

   macro Transparency2Pass {
      ilink in_fld;
      float transparency<NEportLevels={2,1}> = 0.4;

      GDM.DataObject DataObjectBack {
         in=> <-.in_fld;
         Props {
            inherit = 0;
            cull = "Front";
            trans => <-.<-.transparency;
         };
         Obj.xform_mode = "Parent";
      };
      GDM.DataObject DataObjectFront {
         in=> <-.in_fld;
         Props {
            inherit = 0;
            cull = "Back";
            trans => <-.<-.transparency;
         };
         Obj.xform_mode = "Parent";
      };
      GDM.GroupObject GroupObject {
         child_objs => { <-.DataObjectBack.obj, <-.DataObjectFront.obj };
         Top.name => name_of(<-.<-.<-);
      };

      olink out_obj => GroupObject.obj;
   };


//
// ReverseTransparency2Pass - Reverse Two Pass Transparency Rendering Macro
//
// Takes a single field, and renders it
// with two-pass transparency. First
// render front faces, then render back faces.
// Useful for the situation when face normals
// point inwards not outwards as would be expected.

   Transparency2Pass ReverseTransparency2Pass {
      GroupObject {
         child_objs => { <-.DataObjectFront.obj, <-.DataObjectBack.obj };
      };
   };


   // example application user interface
   macro TransparencyEgUI {
      int   &transparency_type<NEportLevels={2,1}>;
      float &transparency<NEportLevels={2,1}>;

      UImod_panel panel {
         title => "Transparency Example";
         parent<NEportLevels={3,0}>;
      };

      UIradioBoxLabel TransTypeRadioBox {
         parent => <-.panel;
         title = "Transparency Type";
         labels => {"1 Pass Transparency", "2 Pass Transparency", "Reverse 2 Pass Transparency"};
         x = 0;
         y = 0;
         selectedItem<NEportLevels={2,2}> => <-.transparency_type <+> -1;
      };

      UIslider TransparencySlider {
         x => <-.TransTypeRadioBox.x;
         y => <-.TransTypeRadioBox.y + <-.TransTypeRadioBox.height + 20;
         parent => <-.panel;
         title = "Transparency...";
         min = 0.;
         max = 1.;
         value => <-.transparency;
         mode = "real";
         decimalPoints = 2;
      };
   };



   // object ordering example application user interface
   macro TransparencyOrderingEgUI {
      int   &object_order<NEportLevels={2,1}>;
      float &transparency<NEportLevels={2,1}>;

      UImod_panel panel {
         title => "Transparency Ordering Example";
         parent<NEportLevels={3,0}>;
      };

      UIradioBoxLabel ObjectOrderRadioBox {
         parent => <-.panel;
         title = "Object Order";
         labels => {"Small then Large", "Large then Small"};
         x = 0;
         y = 0;
         selectedItem<NEportLevels={2,2}> => <-.object_order <+> -1;
      };

      UIslider TransparencySlider {
         x => <-.ObjectOrderRadioBox.x;
         y => <-.ObjectOrderRadioBox.y + <-.ObjectOrderRadioBox.height + 20;
         parent => <-.panel;
         title = "Transparency...";
         min = 0.;
         max = 1.;
         value => <-.transparency;
         mode = "real";
         decimalPoints = 2;
      };
   };


};

