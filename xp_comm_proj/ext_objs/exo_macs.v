
flibrary ExtractObjectsMacs <compile_subs=0> {

   macro ExtractObjectsUI {
      int    &inc_objects<NEportLevels={2,1}>[];
      string &objectnames<NEportLevels={2,1}>[];

      UImod_panel panel {
         parent<NEportLevels={4,0}>;
         message = "Select the objects to extract";
         title => name_of(<-.<-.<-);
      };

      UIoptionBoxLabel objectsOptionBox {
         parent => <-.panel;
         labels+IPort2 => <-.objectnames;
         &selectedItems+IPort2 => <-.inc_objects;
         title = "Select Objects";
         x = 0;
         y = 0;
         width => parent.clientWidth;
      };
   };


   macro extract_objects {
      imlink in_objs;

      int+OPort    inc_objects[];
      group gen_objectnames[array_size(.in_objs)] {
         string+OPort2 text => "Object[" + index_of(<-.gen_objectnames) + "]";
      };
      string+Port &objectnames[] => gen_objectnames.text;

      ExtractObjectsUI ExtractObjectsUI {
         inc_objects => <-.inc_objects;
         objectnames => <-.objectnames;
      };

      XP_COMM_PROJ.ExtractObjects.ExtractObjectsMods.ExtractObjectsCore ExtractObjectsCore {
         in_objs  => <-.in_objs;
         inc_objs => <-.inc_objects;
      };

      omlink out_objs => ExtractObjectsCore.out_objs;
   };


   macro extract_single_object {
      imlink in_objs;

      int+OPort    inc_objects[];
      group gen_objectnames[array_size(.in_objs)] {
         string+OPort2 text => "Object[" + index_of(<-.gen_objectnames) + "]";
      };
      string+Port &objectnames[] => gen_objectnames.text;

      ExtractObjectsUI ExtractObjectsUI {
         inc_objects => <-.inc_objects;
         objectnames => <-.objectnames;
         objectsOptionBox.max = 1;
      };

      XP_COMM_PROJ.ExtractObjects.ExtractObjectsMods.ExtractObjectsCore ExtractObjectsCore {
         in_objs  => <-.in_objs;
         inc_objs => <-.inc_objects;
      };

      olink out_obj => ExtractObjectsCore.out_objs[0];
   };


};

