
////////////////////////////////////////////////////////////////////////////////
//
// Project:      GFAView 
//
// Author:       A.Vanelli
//               ENI - Agip Division
// Date:         10-01-2000
//
////////////////////////////////////////////////////////////////////////////////


flibrary GFAViewMacs {

////////////////////////////////////////////////////////////////////////////////
//
// Module:       GFA_Uviewer 
//
// Description : Uviewer for a GFA format file.
//               The GFA_Uviewer is a customized version of the GDM.Uviewer 
//               with a simplified interface and with the editors for the not 
//               changeable attributes removed.
//               The only Editors available in a GFA_Uviewer are those that
//               interact with a GFA format file (flipbook animation generated
//               with the geom_capture module) in input.
//
//   Attributes that can be changed:
//   The following lists the object atributes you can change in frames that
//   have already been captured as part of the animation.
//   Changing any of these attributes changes the attributes saved in every 
//   frame within the animation, not just the object hierarchy in the current
//   frame.
//   For example, if you generate an animation containing a set  of Axis, an
//   Isosurface, and sphere, and then change the ambient color of the sphere,
//   the ambient color of the sphere changes in the current frame and in each
//   saved frame in which it appears.
//   
//   View   Editors: all attributes can be changed
//   Light  Editors: all attributes can be changed
//   Camera Editors: the following attributes can be changed:
//                   tripod
//                   lens
//                   clipping plane
//                   depth cueing
//   Object Editors: the object attributes can be changed are a subset
//                   of the object property attributes:
//                   General    - Primary, Secondary and Tertiary Colors
//                   Point/Line - Line Style, Line Thickness, Smooth Lines,
//                   Drawing Mode
//                   Surface    - Coloring : Ambient, Diffuse, and Specular
//                                Material : Glss, Opacity and Metal
//                                Culling Mode
//
// 
//   Attributes that cannot be changed:
//
//   Object transforms or trasformation mode
//     - You can tarnsform(rotate, translate, etc.) or change the transform mode of 
//       the entire capture frame, but you cannot interact with individual objects
//       within the frame. For example if a frame contains a set of Axes and an 
//       Isosurface you could not rotate the Isosurface independently, it would only
//       be possible to rotate the entire scene.
//   Objects modes        : Point, Line, Surface, Volume, Bounds and Normal
//   General properties   : jitter level
//   Point/Line properties: Glyph size, Subdivision
//   Object datamap
//   Object texture map
//   Field conversion techniques
//     - as field data is not stored within the flipbook any changed to this
//       attributes will not be reflected in the geometry flipbook.
//   Visibility status of an object
//     - if an object was saved as invisible or visible its status cannot be changed 
//       once in the geometry flipbook.
//   
////////////////////////////////////////////////////////////////////////////////

   GDM.Uviewer GFA_Uviewer {
      // Modified Scene_Editor to work with a GFA input file
      GDM.Mscene_editor Scene_Editor {
         GDM.Mview_link+GDM.Uview_link &in_view<NEportLevels={2,1}> => Scene_Selector.curr_view;
         GDM.Mview_editor+GDM.Uview_editor View_Editor<export=2,export_subs=1> {
            in_view<export=0> => <-.in_view;
            GDobj_sel {
               cur_obj<NEportLevels={0,5}>;
            };
         };
         // Default Transform Attributes
         GDM.Mtrack_editor+GDM.Utrack_editor Track_Editor<export=2,export_subs=1> {
            in_view<export=0> => <-.in_view;
            cur_obj<export=0> => <-.View_Editor.GDobj_sel.cur_obj;
         };
         // Default Light Attributes
         GDM.Mlights_editor+GDM.Ulight_editor Light_Editor<export=2,export_subs=1> {
            in_view<export=0> => <-.in_view;
         };
         // Deactivate the Camera General Attributes
         GDM.Mcamera_editor+GDM.Ucamera_editor Camera_Editor<export=2,export_subs=1> {
            in_view<export=0> => <-.in_view;
            macro IUI<instanced=0,hconnect=2,ui_global_class=1> {
               AU.AUI._AU_panel panel {
                  hconn_widget {
                     accept = "";
                     offer = "widget (stack)";
                     skip_levels = 3;
                  };
               };
               UIoptionMenu optionList<hconnect=2> {
                  y = 5;
                  label => "Camera";
                  AU.AUI._hconn_option_list hconn_list {
                     offer => ;
                  };
                  x => ((panel.width - width) / 2);
                  width = 250;
                  parent => <-.panel;
                  optionLabel = 1;
               };
               UIframe frame<hconnect=2> {
                  x = 5;
                  y => ((<-.optionList.y + <-.optionList.height) + 5);
                  width => (<-.panel.clientWidth - 10);
                  height => ((<-.panel.clientHeight - y) - 5);
                  parent => <-.panel;
                  AU.AUI._hconn_container hconn_widget<instanced=0> {
                     accept = "widget (panel stack)";
                     offer => ;
                  };
               };
               GDM.Ulens_editor Lens {
                  GDcamera_edit => <-.<-.GDcamera_edit;
               };
               GDM.Utripod_editor Tripod {
                  GDcamera_edit => <-.<-.GDcamera_edit;
               };
               GDM.Uclipping_editor Clipping_Planes {
                  GDcamera_edit => <-.<-.GDcamera_edit;
               };
               GDM.Udepth_cue_editor Depth_Cueing {
                  GDcamera_edit => <-.<-.GDcamera_edit;
               };
            };
         };
         // Define the available objects attributes
         AU.AUoptionMenuStack Object_Editor<export=2,export_subs=1> {
            GDM.Mview_link &in_view<NEportLevels={2,1},export=0> => <-.in_view;
            GDobject_templ &cur_obj<NEportLevels={2,1},export=0> {
               texture<NEportLevels={0,2}>;
            } => <-.View_Editor.GDobj_sel.cur_obj;
            GDobj_edit GDobj_edit<export=2,export_all=2> {
               shell_vis = 1;
               camera => <-.in_view.picked_camera;
               object => <-.cur_obj;
            };
            GDprops_edit GDprops_edit<NEportLevels={0,1},export=2,export_all=2> {
               shell_vis = 1;
               props => switch((edit_alt_props + 1),<-.cur_obj.props,<-.cur_obj.altobj.props);
               int edit_alt_props = 0;
            };
            UI {
               option {
                  label => "&Object";
                  message = "AU_EDIT_OBJ_MSG";
                  hconn_list {
                     order = 5;
                  };
               };
            };
            IUI {
               GDM.Uobject_props Properties {
                  IUI {
   		          EditAltProps {
                        x = 0;
                        y = 0;
                     };
                     // Deactive Alternate Obj Property
                     ObjectOptions = {,
                        {
                           active=0,,,,,,,,,,,,,,
                        }};
                     Type {
                        IUI {
                           // Remove the Volume properties
                           -Volume;
                           PointLine {
                             IUI {
                                // Remove GlyphSize and Subdivision attributes
                                -GlyphSize;   
                                -Subdivision;  
                                SmoothLines {
                                   y=> <-.LineThickness.y + <-.LineThickness.height +10;
                                };
                             };
                           };
                           General {
                              IUI {
                              // Remove the Jetter attributes
                                 -Jitter_Level;
                              };
                           };
                        };
                     };
                  };
                  GDprops_edit => <-.<-.GDprops_edit;
               };
            };
         };
         GDM.Ugraph_editor Graph_Editor {
            cur_obj => <-.View_Editor.GDobj_sel.cur_obj;
            list_changed => <-.View_Editor.GDobj_sel.obj_list_changed;
         };
         GDM.Uprint_editor Print_Editor {
            in_view => <-.in_view;
         };
      };
   };


//
// Module:       GFA_Viewer
//
// Description : GFA_Viewer plays back a GFA format file saved by the geom_capture module.
//               It extends from GFA_Uviewer by providing an integrated geom_replay module.
//
   GFA_Uviewer GFA_Viewer {
      ANIM_MODS.geom_replay geom_replay {
         in_view => <-.Scene_Selector.curr_view;
      };
   };


//
// Module:       GFA_SingleWindowApp 
//
// Description : GFA_SingleWindowApp for a GFA format file.
//               The GFA_SingleWindowApp is SingleWindowApp with
//               a GFA_Uviewer instead of a GDM.Uviewer.
//               It also integrates a geom_replay module
//
   APPS.SingleWindowApp GFA_SingleWindowApp {
      GFA_Uviewer GFA_Uviewer;
      ANIM_MODS.geom_replay geom_replay {
         in_view => <-.GFA_Uviewer.Scene_Selector.curr_view;
      };
   };


//
// Module:       GFA_MultiWindowApp 
//
// Description : GFA_MultiWindowApp for a GFA format file.
//               The GFA_MultiWindowApp is MultiWindowApp with
//               a GFA_Uviewer instead of a GDM.Uviewer.
//               It also integrates a geom_replay module
//
   APPS.MultiWindowApp GFA_MultiWindowApp {
      GFA_Uviewer GFA_Uviewer;
      ANIM_MODS.geom_replay geom_replay {
         in_view => <-.GFA_Uviewer.Scene_Selector.curr_view;
      };
   };


};

