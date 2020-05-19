flibrary AnimatedImageGeneratorMacs {
	//
	// Macro that connects to a Viewer, and writes a sequence of
	// images on every view refresh, either from the network of modules
	// generating new content, or mouse interaction with the window.
	// Image numbering sequence is automatic, incrementing by 1 on each update.
	// Prefix and postfix strings are used for image name construction.
	// The macro uses lower-level parts only available for the Developer Edition.
	// 
	// Author: August 10, 2000 I. Curington
	//
	macro AnimatedImageGenerator {

		GDview_templ+IPort2 &view_in;

		macro filename_sequence {

			int+OPort2 index => counter_with_reset.output;
			string+OPort2 filename =>
					 str_format("%s%04d%s",UItext_prefix.text,index,UItext_append.text);

			UIframe UIframe_filename {
				parent => Write_Image.write_image_ui.panel;
				height = 150;
				y = 0;
				width = 240;
				shadowType = "shadow_etched_out";
				shadowThickness = 5;
			};
			UIlabel UIlabel_prefix {
				parent => <-.UIframe_filename;
				label => "Prefix";
				y = 0;
				x = 10;
				alignment = "left";
			};
			UItext UItext_append {
				parent => <-.UIframe_filename;
				text = ".gif";
				y = 50;
				x = 120;
			};
			UItext UItext_prefix {
				parent => <-.UIframe_filename;
				text = "A";
				x = 120;
				y = 0;
			};
			UIlabel UIlabel_append {
				parent => <-.UIframe_filename;
				label => "Extension";
				y = 50;
				x = 10;
				alignment = "left";
			};
			UIlabel UIlabel_string {
				parent => <-.UIframe_filename;
				label => <-.filename;
				y = 100;
				width = 110;
				x = 10;
				alignment = "left";
			};
			UIbutton UIbutton_reset {
				parent => <-.UIframe_filename;
				label => "Reset Counter";
				y = 100;
				x = 120;
			};
			macro counter_with_reset {
				int+IPort2 up => <-.<-.output_fld.output_field.View.trigger;
				int+IPort2 reset => <-.UIbutton_reset.do;
				GMOD.parse_v on_trigger {
					trigger => <-.up;
					v_commands = "output = output+1;";
					relative => <-.<-;
				};
				GMOD.parse_v off_trigger {
					trigger => <-.reset;
					v_commands = "output = 0;";
					relative => <-.<-;
				};
				int+OPort2 output = 0;
			};
		};
		GDM.OutputField output_fld {
			view_in => <-.view_in;
			macro output_field {
				UIrenderView+nres+IPort2 &render_view => <-.view_in.render_view;
				GDview_templ+IPort2 &view_in => <-.view_in;
				UIpanel UIpanel {
					parent<NEportLevels={4,0}> => <-.<-.<-.Write_Image.write_image_ui.panel;
					y = 160;
					height = 100;
				};
				UIslider UIslider {
					y = 5;
					parent => <-.UIpanel;
					max = 4096.;
					value = 500.;
					mode = "integer";
					title => "Width";
				};
				UIslider UIslider#1 {
					y => ((<-.UIslider.y + <-.UIslider.height) + 5);
					parent => <-.UIpanel;
					max = 4096.;
					value = 500.;
					mode = "integer";
					title => "Height";
				};
				UIrenderView UIrenderView {
					y = 0;
					width => <-.UIslider.value;
					height => <-.UIslider#1.value;
					visible = 0;
					parent => <-.render_view.parent;
				};
				BestVirtPal VirtPal {
					vclass => view_in.virtpal.vclass;
					vid => view_in.virtpal.vid;
					share = 0;
					gamma = 1.;
					cube_size => view_in.virtpal.cube_size;
				};
				DefaultFieldOutput Output {
					output<NEportLevels={0,4}>;
				};
				DefaultView View {
					back_col => view_in.back_col;
					light_info => view_in.light_info;
					cameras => view_in.cameras;
					output => <-.Output;
					handle => <-.UIrenderView.handle;
					virtpal => <-.VirtPal;
					output_enabled = 1;
					trigger = 0;
					refresh = 1;
				};
			};
		};
		MODS.Write_Image Write_Image {
			in+nres => <-.output_fld.output_field.Output.output;
			write_image_ui {
				panel+OPort3 {
					option {
						set = 1;
					};
					title => name_of(<-.<-.<-.<-,1);
				};
				Image_Filename {
					visible = 0;
					y = 170;
				};
				file_name {
					visible = 0;
				};
				visible {
					visible = 0;
				};
				write_oneshot {
					visible = 0;
				};
				format_gif {
					set = 1;
				};
				flip_toggle {
					set = 0;
				};
				overwrite_toggle {
					set = 1;
				};
				format_rb {
					selectedItem = 2;
				};
				bpp_8 {
					set = 1;
				};
				color_rgb {
					set = 1;
				};
				compression_lzw {
					set = 1;
				};
				reduction_fs {
					set = 1;
				};
				filetype_binary {
					set = 1;
				};
			};
			DVwrite_image {
				filename+IPort3 => <-.<-.filename_sequence.filename;
			};
		};
	};


};
