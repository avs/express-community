group XYZ_UIfields {

   link parent<NEportLevels={2,1}>;
   int  x_top_left = 0;
   int  y_top_left = 0;
   int  label_width = 50;
   int  label_height = 26;
   int  field_width = 50;
   int  field_height = 20;

   UIlabel X_label {
      parent => <-.parent;
      x => <-.x_top_left;
      y => <-.y_top_left;
      width => <-.label_width;
      height => <-.label_height;
      label => ("X " + name_of(<-.<-));
   };
   UIlabel Y_label {
      parent => <-.parent;
      x => <-.x_top_left;
      y => (<-.X_label.y + <-.X_label.height);
      width => <-.label_width;
      height => <-.label_height;
      label => ("Y " + name_of(<-.<-));
   };
   UIlabel Z_label {
      parent => <-.parent;
      x => <-.x_top_left;
      y => (<-.Y_label.y + <-.Y_label.height);
      width => <-.label_width;
      height => <-.label_height;
      label => ("Z " + name_of(<-.<-));
   };
   UIfield X_field {
      parent => <-.parent;
      x => (<-.X_label.width + <-.X_label.x);
      y => (<-.X_label.y - 3);
      width => <-.field_width;
      height => <-.field_height;
      message => X_label.label;
      nullString = "?";
      value => x_value;
   };
   UIfield Y_field {
      parent => <-.parent;
      x => <-.X_field.x;
      y => (<-.Y_label.y - 3);
      width => <-.field_width;
      height => <-.field_height;
      message => Y_label.label;
      nullString = "?";
      value => y_value;
   };
   UIfield Z_field {
      parent => <-.parent;
      x => <-.X_field.x;
      y => (<-.Z_label.y - 3);
      width => <-.field_width;
      height => <-.field_height;
      message => Z_label.label;
      nullString = "?";
      value => z_value;
   };

   int   x_bot_right => Z_field.x + Z_field.width;
   int   y_bot_right => Z_field.y + Z_field.height;
   float x_value<NEportLevels={2,2}>;
   float y_value<NEportLevels={2,2}>;
   float z_value<NEportLevels={2,2}>;
};
