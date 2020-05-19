/*
 * file: imgcache.c
 * date: 12-August-96
 * purpose: Capture and Replay images from/to Viewer framebuffer
 *
 */
#include "gen.h"

#include "avs/gd.h"

/* Define local structure for maintaining status etc.
 */
typedef struct {
  int mode;
  int capture;
  int playback;
  int frame_in;
  int num_frames;
#ifdef MSDOS
  HDC hdcMem;
#else
  Display *disp;
#endif
  void *pixmaps[500];
  OMobj_id object_id;
} CacheInfo;

/* routine: CreatePixmap
 * purpose: Create pixmap buffer to save frames into
 */
static void CreatePixmap( CacheInfo *cache, GDview *view, int frame_out )
{
  int width, height;
  GDwinfo *winfo;
#ifdef MSDOS
  FullBmpInfoT Info;
  LPBITMAPINFOHEADER Hdr = &Info.bmiHeader;
  int *ppvBits;
  int i;
#endif

  winfo = view->Winfo;
  width = winfo->w;
  height = winfo->h;

#ifdef MSDOS

  GdiFlush();
  if( cache->hdcMem == 0 )
    {
      cache->hdcMem = CreateCompatibleDC( winfo->hdc );
      SelectPalette(cache->hdcMem, view->VirtPal->mapinfo->hPalette, FALSE);
      RealizePalette(cache->hdcMem);
    }

  Hdr->biSize = sizeof(BITMAPINFOHEADER);
  Hdr->biPlanes = 1;
  Hdr->biCompression = BI_RGB;
  Hdr->biClrImportant = 0;
  Hdr->biWidth = width;
  Hdr->biHeight = height;
  Hdr->biBitCount = view->VirtPal->depth;
  Hdr->biXPelsPerMeter = winfo->xPPM;
  Hdr->biYPelsPerMeter = winfo->yPPM;

  if( view->VirtPal->depth == 8 )
    {
      Hdr->biSizeImage = width * height;
      Hdr->biClrUsed = 256;
      for (i=0; i<256; i++)
	Info.bmiColors[i] = i;

      cache->pixmaps[frame_out] =
	CreateDIBSection(cache->hdcMem, (LPBITMAPINFO)&Info,
			 DIB_PAL_COLORS, &ppvBits,
			 NULL, 0);
    }
  else
    {
      Hdr->biSizeImage = width * height * 3;
      Hdr->biClrUsed = 0;

      cache->pixmaps[frame_out] =
	CreateDIBSection(cache->hdcMem, (LPBITMAPINFO)&Info,
			 DIB_RGB_COLORS, &ppvBits,
			 NULL, 0);
    }

  if( !cache->pixmaps[frame_out] )
    fprintf( stderr, "CacheProcessTrigger: Unable to allocate pixmap, err = %d\n", GetLastError() );
#else
  cache->disp = winfo->disp;
  cache->pixmaps[frame_out] = (void *)XCreatePixmap(winfo->disp, winfo->window,
					   width, height, view->VirtPal->depth);
#endif

  return;
}

/* routine: SavePixmap
 * purpose: Copy pixels from screen to buffer
 */
static void SavePixmap( CacheInfo *cache, GDview *view, int frame_out )
{
  int width, height;
  GDwinfo *winfo;

  winfo = view->Winfo;
  width = winfo->w;
  height = winfo->h;

#ifdef MSDOS
  SelectObject(cache->hdcMem, cache->pixmaps[frame_out]);
  if( !BitBlt(cache->hdcMem, 0, 0, width, height, winfo->hdc, 0, 0, SRCCOPY) )
    fprintf( stderr, "CacheProcessTrigger: Unable to perform bitblt, err = %d\n", GetLastError() );
#else
  XCopyArea(winfo->disp, winfo->window, (Drawable)cache->pixmaps[frame_out],
	    view->State->draw_gc, 0, 0, width, height, 0, 0);
#endif
  return;
}

/* routine: DisplayPixmap
 * purpose: Copy pixels from buffer to screen
 */
static void DisplayPixmap( CacheInfo *cache, GDview *view, int frame_out )
{
  int width, height;
  GDwinfo *winfo;

  winfo = view->Winfo;
  width = winfo->w;
  height = winfo->h;

#ifdef MSDOS
  SelectObject(cache->hdcMem, cache->pixmaps[frame_out]);
  if( !BitBlt(winfo->hdc, 0, 0, width, height, cache->hdcMem, 0, 0, SRCCOPY) )
    fprintf( stderr, "CacheProcessTrigger: Unable to perform bitblt, err = %d\n", GetLastError() );
#else
  XCopyArea(winfo->disp, (Drawable)cache->pixmaps[frame_out], winfo->window,
	    view->State->draw_gc, 0, 0, width, height, 0, 0);
#endif
  return;
}

/* routine: DestroyPixmap
 * purpose: Free pixmap buffer
 */
static void DestroyPixmap( CacheInfo *cache, int index )
{
#ifdef MSDOS
  DeleteObject( cache->pixmaps[index] );
#else
  XFreePixmap( cache->disp, (Pixmap)cache->pixmaps[index] );
#endif
}

/* routine: GetCacheInfo
 * purpose: Get pointer to internal CacheInfo structure
 */
static CacheInfo *GetCacheInfo( OMobj_id object )
{
  CacheInfo *cache = NULL;
  OMget_name_ptr_val( object, OMstr_to_name("hidden"), (void *)&cache, 0 );
  if( cache == NULL )
    fprintf( stderr, "GetCacheInfo: ptr value = 0\n" );

  return( cache );
}

/* routine: SetCacheCapture
 * purpose: Start or Stop capture, cleaning up as required
 */
static void SetCacheCapture( int capture, CacheInfo *cache )
{
  /* For mode 0 clean up from previous capture if required
   */
  if( capture && (cache->mode == 0) && !cache->capture )
    {
      /* Clean up from previous capture
       */
      int i;
      for( i = 0; i < cache->num_frames; i++ )
	DestroyPixmap( cache, i );

      memset( cache->pixmaps, 0, sizeof(cache->pixmaps) );
      cache->num_frames = 0;
    }

  /* For mode 1 clean up as soon as we leave capture mode
   */
  if( !capture && (cache->mode == 1) && cache->capture )
    {
      /* Clean up from previous capture
       */
      int i;
      for( i = 0; i < cache->num_frames; i++ )
	DestroyPixmap( cache, i );

      memset( cache->pixmaps, 0, sizeof(cache->pixmaps) );
      cache->num_frames = 0;
    }

  /* If currently in playback mode, turn it off
   */
  if( capture && cache->playback )
    {
      OMset_name_int_val( cache->object_id, OMstr_to_name("playback"), 0 );
      cache->playback = 0;
    }

  cache->capture = capture;
}

/* routine: CacheCreate
 * purpose: Instance Cacheator module,
 *    create and initialize internal structures etc.
 */
int CacheCreate(OMobj_id obj_id, OMevent_mask event_mask, int seq_num)
{
  CacheInfo *cache;

  cache = (CacheInfo *)malloc( sizeof(CacheInfo) );
  if( cache )
    {
      memset( cache, 0, sizeof(CacheInfo) );

      cache->object_id = obj_id;
      OMset_name_ptr_val( obj_id, OMstr_to_name("hidden"), (void *)cache, 0 );

      CacheSetMode(obj_id, event_mask, seq_num);
      CacheCapture(obj_id, event_mask, seq_num);
      CachePlayback(obj_id, event_mask, seq_num);

      return( 1 );
    }
  else
    {
      fprintf( stderr, "CacheCreate: Unable to allocate storage for cache info\n" );
      OMset_name_ptr_val( obj_id, OMstr_to_name("hidden"), NULL, 0 );
      return( 0 );
    }
}

/* routine: CacheSetMode
 * purpose: Set cacheator mode (interactive or loop)
 */
int CacheSetMode(OMobj_id obj_id, OMevent_mask event_mask, int seq_num)
{
  CacheInfo *cache;
  int newmode;

  /* Get pointer to control structure
   */
  if( (cache = GetCacheInfo( obj_id )) == NULL )
    return( 0 );

  newmode = 0;
  OMget_name_int_val( obj_id, OMstr_to_name("mode"), &newmode );

  if( (newmode != 0) && (newmode != 1) )
    {
      fprintf( stderr, "CacheSetMode: Invalid mode\n" );
      return( 0 );
    }

  /* Do not let someone change modes while in capture or
   *  playback mode since managing this would get overwhelming
   */
  if( (newmode != cache->mode) && (cache->capture || cache->playback) )
    {
      OMset_name_int_val( obj_id, OMstr_to_name("mode"), cache->mode );
      fprintf( stderr, "CacheSetMode: Can't change mode while active\n" );
      return( 0 );
    }

  cache->mode = newmode;
  return( 1 );
}

/* routine: CacheCapture
 * purpose: Turn on/off frame capture
 */
int CacheCapture(OMobj_id obj_id, OMevent_mask event_mask, int seq_num)
{
  CacheInfo *cache;
  int capture;

  /* Get pointer to control structure
   */
  if( (cache = GetCacheInfo( obj_id )) == NULL )
    return( 0 );

  /* Get new value of capture input
   */
  capture = 0;
  OMget_name_int_val( obj_id, OMstr_to_name("capture"), &capture );

  SetCacheCapture( capture, cache );
  return( 1 );
}

/* routine: CachePlayback
 * purpose: Turn on or off playback
 */
int CachePlayback(OMobj_id obj_id, OMevent_mask event_mask, int seq_num)
{
  CacheInfo *cache;
  int playback;

  /* Get pointer to control structure
   */
  if( (cache = GetCacheInfo( obj_id )) == NULL )
    return( 0 );

  /* Get new value of playback
   */
  playback = 0;
  OMget_name_int_val( obj_id, OMstr_to_name("playback"), &playback );

  /* If in capture mode, disable this before starting playback
   */
  if( playback && cache->capture )
    {
      OMset_name_int_val( obj_id, OMstr_to_name("capture"), 0 );
      SetCacheCapture( 0, cache );
    }

  cache->playback = playback;
  return( 1 );
}

/* routine: CacheSetFrame
 * purpose: Set new frame
 */
int CacheSetFrame(OMobj_id obj_id, OMevent_mask event_mask, int seq_num)
{
  OMobj_id view_id;
  CacheInfo *cache;
  GDview *view;
  int frame_in;

  /* Get pointer to control structure
   */
  if( (cache = GetCacheInfo( obj_id )) == NULL )
    return( 0 );

  /* Get GDview structure
   */
  view_id = OMfind_subobj(obj_id, OMstr_to_name("view"), OM_OBJ_RD);
  if( OMis_null_obj(view_id) )
    {
      fprintf( stderr, "CacheUpdate: Unable to get object id for view\n" );
      return( 0 );
    }

  view = (GDview *)GDget_local(view_id, (OMpfi)GDview_init);

  /* Get new value of frame_in
   */
  frame_in = 0;
  OMget_name_int_val( obj_id, OMstr_to_name("frame_in"), &frame_in );
  cache->frame_in = frame_in;

  /* Don't do anything without a view connected
   */
  if( view )
    {
      /* Process playback for mode 0
       */
      if( cache->playback && (cache->mode == 0) )
        {
          if( (frame_in < 0) || (frame_in >= cache->num_frames) )
            {
              fprintf( stderr, "Invalid frame to display, frame_in = %d, num_frames = %d\n",
                       frame_in, cache->num_frames );

              return( 0 );
            }

	  DisplayPixmap( cache, view, frame_in );

          /* Update frame_out to reflect current frame
           */
          OMset_name_int_val(obj_id, OMstr_to_name("frame_out"), frame_in);
        }

      /* Process capture/playback for mode 1
       */
      if( cache->capture && (cache->mode == 1) )
        {
          if( cache->pixmaps[frame_in] )
            {
	      DisplayPixmap( cache, view, frame_in );
            }
          else
            {
              OMset_name_int_val(obj_id, OMstr_to_name("frame_out"), frame_in);
            }
        }
    }

  return( 1 );
}

/* routine: CacheProcessTrigger
 * purpose: Capture new frame if required
 */
int CacheProcessTrigger(OMobj_id obj_id, OMevent_mask event_mask, int seq_num)
{
  OMobj_id view_id;
  CacheInfo *cache;
  GDview *view;

  /* Get pointer to control structure
   */
  if( (cache = GetCacheInfo( obj_id )) == NULL )
    return( 0 );

  /* Get GDview structure
   */
  view_id = OMfind_subobj(obj_id, OMstr_to_name("view"), OM_OBJ_RD);
  if( OMis_null_obj(view_id) )
    {
      fprintf( stderr, "CacheUpdate: Unable to get object id for view\n" );
      return( 0 );
    }

  view = (GDview *)GDget_local(view_id, (OMpfi)GDview_init);

  /* Don't do anything without a view connected
   */
  if( view )
    {
      if( cache->capture )
        {
	  /* Mode 0: Capture frame and update frame_out and num_frames
	   */
          if( cache->mode == 0 )
            {
              int frame_out = cache->num_frames;
	      CreatePixmap( cache, view, frame_out );
	      SavePixmap( cache, view, frame_out );

              cache->num_frames += 1;
              OMset_name_int_val(obj_id, OMstr_to_name("frame_out"), frame_out);
              OMset_name_int_val(obj_id, OMstr_to_name("num_frames"), cache->num_frames);
            }
	  /* Mode 1: Capture frame
	   */
          else
            {
              if( cache->pixmaps[cache->frame_in] == NULL )
                {
		  CreatePixmap( cache, view, cache->frame_in );
		  SavePixmap( cache, view, cache->frame_in );
                }
              else
                fprintf( stderr, "CacheProcessTrigger: trigger with pixmap already cached\n" );
            }
        }
    }

  return(1);
}

/* routine: CacheDestroy
 * purpose: Cleanup alloced memory etc..
 */
int CacheDestroy(OMobj_id obj_id, OMevent_mask event_mask, int seq_num)
{
  CacheInfo *cache;

  cache = NULL;
  OMget_name_ptr_val( obj_id, OMstr_to_name("hidden"), (void *)&cache, 0 );

  if( cache )
    {
      /* Clean up from previous capture
       */
      int i;
      for( i = 0; i < cache->num_frames; i++ )
	DestroyPixmap( cache, i );

      memset( cache->pixmaps, 0, sizeof(cache->pixmaps) );
      free( cache );
    }

  return(1);
}

