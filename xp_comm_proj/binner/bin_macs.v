
flibrary BinnerMacs<compile_subs=0> {

   macro BinnerUI {
      XP_COMM_PROJ.Binner.BinnerMods.BinnerParams+IPort2 &BinnerParams;
      ilink in_fld;

      int+nres+Port &nspace => in_fld.nspace;

      UImod_panel UImod_panel {
         title = "binner";
         message = "Select Binner control panel.";
         parent<NEportLevels={4,0}>;
      };

      UIlabel coord_sys_label {
         parent => <-.UImod_panel;
         label = "Type of coordinate binning:";
         x = 5;
         y = 5;
         width => parent.width;
         alignment = "left";
      };
      UIoption option_cart {
         label = "Cartesian";
      };
      UIoption option_polar {
         label = "Polar";
      };
      UIoption option_polar_n {
         label = "Polar normalized";
      };
      UIradioBox coord_system_RadioBox {
         parent => <-.UImod_panel;
         cmdList => {<-.option_cart, <-.option_polar, <-.option_polar_n};
         selectedItem => <-.BinnerParams.coordinate_system;
         x => parent.width/3.;
         y => <-.coord_sys_label.y + coord_sys_label.height + 10;
         width => parent.width - x;
      };


      UIlabel ncoord_bins_label {
         parent => <-.UImod_panel;
         alignment = "left";
         label = "Number of coordinate bins:";
         x => <-.coord_sys_label.x;
         y => <-.coord_system_RadioBox.y + <-.coord_system_RadioBox.height + 20;
         width => parent.width - x;
      };
      string lbl_axis[] = {
         "X:", "Y:", "Z:",
         "R:", "rho:", "error",
         "R:", "rho:", "error"
      };
      UIlabel coord_labels[nspace] {
         parent => <-.UImod_panel;
         x => parent.width/3.;
         y => <-.ncoord_bins_label.y + <-.ncoord_bins_label.height + 10 + (index_of(coord_labels) * 32);
         width => parent.width/3.;
         label => lbl_axis[BinnerParams.coordinate_system*3 + index_of(coord_labels)];
         alignment = "left";
      };
      UIfield coord_fields[nspace] {
         parent => <-.UImod_panel;
         x => (2 * parent.width)/3.;
         y => <-.ncoord_bins_label.y + <-.ncoord_bins_label.height + 10 + (index_of(coord_fields) * 32);
         width => parent.width/3.;
         height = 30;
         value = 10.;
         min = 1.;
         mode = "integer";
         updateMode = 7;
      };


      UItoggle toggle_data {
         parent => <-.UImod_panel;
         x => <-.coord_sys_label.x;
         y => <-.ncoord_bins_label.y + <-.ncoord_bins_label.height + 10 + (3 * 32) + 20;
         width => parent.width - x;
         label = "also bin data values...";
      };

      UIlabel data_label {
         parent => <-.UImod_panel;
         x => <-.coord_sys_label.x;
         y => <-.toggle_data.y + <-.toggle_data.height + 10;
         width => (2 * parent.width)/3. - x - 3;
         visible => <-.toggle_data.set;
         label = "Number of data bins:";
         alignment = "left";
      };
      UIfield data_field {
         parent => <-.UImod_panel;
         x => (2 * parent.width)/3.;
         y => <-.toggle_data.y + <-.toggle_data.height + 10;
         width => parent.width/3.;
         height = 30;
         visible => <-.toggle_data.set;
         value = 1.;
         min = 1.;
         mode = "integer";
         updateMode = 7;
      };
      UIslider comp_slider {
         parent => <-.UImod_panel;
         value => <-.BinnerParams.comp;
         title = "Component:";
         visible => <-.toggle_data.set;
         x => parent.width/3.;
         y => <-.data_field.y + <-.data_field.height + 10;
         width => parent.width - x;
         min = 0.;
         max+nres => <-.in_fld.nnode_data - 1;
         mode = "integer";
      };

      int+OPort2 ncoord_bins[] => coord_fields.value;
      int+OPort2 ndata_bins => switch(toggle_data.set+1, 1, data_field.value);
   }; 


   macro BinnerFunc {
      ilink in_fld;

      int &ncoord_bins<NEportLevels={2,1}>[];
      int &ndata_bins<NEportLevels={2,1}>;

      XP_COMM_PROJ.Binner.BinnerMods.BinnerParams &BinnerParams<NEportLevels={2,1}>;

      XP_COMM_PROJ.Binner.BinnerMods.BinnerCore BinnerCore {
         fld => <-.in_fld;
         BinnerParams => <-.BinnerParams;
         ncoord_bins => <-.ncoord_bins;
         ndata_bins => <-.ndata_bins;
      };
      GDM.DataObject DataObject {
         in => <-.BinnerCore.out_fld;
         Obj.name => name_of(<-.<-.<-);
      };

      olink out_fld => BinnerCore.out_fld;
      olink out_obj => DataObject.obj;
   };


   macro binner {
      ilink in_fld;

      XP_COMM_PROJ.Binner.BinnerMods.BinnerParams BinnerParams {
         coordinate_system = "cartesian";
         comp = 0;
      };

      XP_COMM_PROJ.Binner.BinnerMacs.BinnerUI BinnerUI {
         BinnerParams => <-.BinnerParams;
         in_fld => <-.in_fld;

         UImod_panel {
            title => name_of(<-.<-.<-,1);
         };
      };

      XP_COMM_PROJ.Binner.BinnerMacs.BinnerFunc BinnerFunc {
         in_fld => <-.in_fld;
         BinnerParams => <-.BinnerParams;
         ncoord_bins => <-.BinnerUI.ncoord_bins;
         ndata_bins => <-.BinnerUI.ndata_bins;
      	DataObject.Obj.name => name_of(<-.<-.<-.<-);
      };

      olink out_fld => BinnerFunc.out_fld;
      olink out_obj => BinnerFunc.out_obj;
   };
    
};

