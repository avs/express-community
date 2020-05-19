
flibrary SetCursorApps<compile_subs=0>
{
   // working set cursor example application structure
   // This small application shows the naming conventions and
   // usage structure for the set cursor utility module, to be used by
   // application developers. A C api call is made from within 
   // the CursorExampleMod module. The utility module then pushes the
   // current OM state, and invokes the Cursor UI macro.
   // This can be used to change the current mouse cursor icon
   // to a busy or waiting icon, such an hourglass.

   APPS.DefaultApplication SetCursorEg {
      // application window responsible for cursor behavior
      UIapp CursorExample {

         // Note: you should do this to all UIshell/UItemplateDialogs
         // in your application.
         cursor+IPort2 => <-.Cursor;
      };

      UItoggle CursorBusyToggle {
         parent => <-.CursorExample;
         label = "Display Busy Cursor";
         y => (parent.clientHeight / 2) - (height / 2);
         x => (parent.clientWidth / 4);
         width => (parent.clientWidth / 2);
      };

      XP_COMM_PROJ.SetCursor.SetCursorMods.SetCursorExampleMod SetCursorExampleMod {
         busy_enable => <-.CursorBusyToggle.set;
      };


      // The 'working' cursor
#ifdef MSDOS
      UIpixmapCursor Cursor {
         attach_cursor = 0;
         cursor_pixmap {
            // replace this with your favourite cursor.
            filename = "c:\\Winnt\\Cursors\\hourglas.ani";
         };
      };
#else
      UIfontCursor Cursor {
         attach_cursor = 0;
         cursor_name = "watch";
      };
#endif
   };

};

