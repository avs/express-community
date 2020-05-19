/*****************************************************************************

                        Copyright (c) 1998 by
                        Advanced Visual Systems Inc.
                        All Rights Reserved

        This software comprises unpublished confidential information of
        Advanced Visual Systems Inc. and may not be used, copied or made
        available to anyone, except in accordance with the license
        under which it is furnished.

******************************************************************************

This file contains the implementation of the custom renderer for the VLI.

******************************************************************************
*****************************************************************************/

#include "express.h"
#include <avs/gd.h>

#define VLI_GEOM_NAME  "VLI Volume Render"

const int XP_NUM_CROP_OBJS = 14;
const int XP_NUM_CROP_TERMS = 3;
const int XP_CROP_TERM0_INDEX = 6;  // refer to V definition of VLICropBounds

/************************  Public Global Variables  *************************/

/* <none> */

/************************  Private Type Definitions  ************************/

extern "C" {
typedef void (*XPUpdateMatrixFunc)(GDview *view);
typedef void (*XPPolyTriRendFunc)(GDgeom_data *geomData, GDstate *state);
typedef void (*XPSetTextureFunc)(GDview *view, GDtexture *texture,
                                 float mat[4][4]);
typedef void (*XPFreeTextureFunc)(GDtexture *texture);
typedef void (*XPFreeGeomFunc)(GDgeom_data *geom);
}


// The following enumerated values specify the order in which material
// properties are stored in the "material" array in the GDprops structure.
//
enum XPMaterial
{
   XP_kAmbient     = GD_MATERIAL_AMBIENT,
   XP_kDiffuse     = GD_MATERIAL_DIFFUSE,
   XP_kSpecular    = GD_MATERIAL_SPECULAR,
   XP_kSpecularExp = GD_MATERIAL_SPEC_EXP
};


// The following enumerated values correspond to the enum subobject in
// the "predefined" subobject in the VLICropBounds.  If the V code is changed,
// these values must be changed in parallel.
//
enum XPVLICropFlags
{
   XP_kSubVolume = 0,
   XP_k3DCross,
   XP_k3DCrossInvert,
   XP_k3DFence,
   XP_k3DFenceInvert,
   XP_kUserDefined
};


// The following enumerated values correspond to the enum subobject in
// the "slice_flags" subobject in the VLIVolumeInfo.  If the V code is
// changed, these values must be changed in parallel.
//
enum XPVLISliceFlags
{
   XP_kDisable = 0,
   XP_kInside,
   XP_kOutside
};

// The following enumerated values correspond to the enum subobject in
// the "cursor" subobject in the VLIVolumeInfo.  If the V code is
// changed, these values must be changed in parallel.
//
enum XPVLICursorType
{
   XP_kCrossHair = 0,
   XP_kPlane
};

// The following enumerated values correspond to the enum subobject in
// the "blend_mode" subobject in the VLIVolumeInfo.  If the V code is
// changed, these values must be changed in parallel.
//
enum XPVLIBlendMode
{
   XP_kFTB = 0,
   XP_kMIP,
   XP_kMINIP
};


// The following enumerated values correspond to the enum subobject in
// the "texture_interp" subobject in the VLIVolumeInfo.  If the V code is
// changed, these values must be changed in parallel.
//
enum XPVLITextureInterpolation
{
   XP_kPoint = 0,
   XP_kBilinear
};


// The following enumerated values correspond to the enum subobject in
// the "texture_type" subobject in the VLIVolumeInfo.  If the V code is
// changed, these values must be changed in parallel.
//
enum XPVLITextureType
{
   XP_kUnfiltered = 0,
   XP_kMipmap
};

// The following enumerated values correspond to the enum subobject in
// the "supersample_space" subobject in the VLIVolumeInfo.  If the V code is
// changed, these values must be changed in parallel.
//
enum XPVLISuperSampleSpace
{
   XP_kCameraSpace = 0,
   XP_kObjectSpace
};

// The following enumerated values correspond to the enum subobject in
// the "base_plane_accum" subobject in the VLIVolumeInfo.  If the V code is
// changed, these values must be changed in parallel.
//
enum XPVLIBasePlaneAccum
{
   XP_kGrowBasePlane = 0,
   XP_kBlendBasePlane
};


// The following constants define the default values for some of the
// AVS/Express enumerations.
//
const int XP_DEFAULT_TEXTURE_INTERP = XP_kBilinear;
const int XP_DEFAULT_TEXTURE_TYPE   = XP_kUnfiltered;


// Class:  XPVLIState
// Models: Low-level state information that needs to be preserved across
//         successive calls to the render method.
// Info:   This class stores the VLI context.  It also stores information
//         used to determine whether the AVS/Express volume data has changed,
//         so that the data must be uploaded to the VolumePro card only as
//         often as necessary.
// Notes:  ---
//
class XPVLIState
{
public:
   XPVLIState(const GDobject *obj);
   ~XPVLIState();

   static XPVLIState *Get(const GDobject *obj);

   int IsValid() const  { return Context && (!OMis_null_obj(ModuleId)); }

   OMobj_id GetModuleId() const  { return ModuleId; }

   VLIContext  *GetContext() { return Context; }
   VLIVolume   *GetVolume()  { return Volume; }
 
   int UpdateVolume(float transform[4][4]);

   int GetNx() const    { return Nx; }
   int GetNy() const    { return Ny; }
   int GetNz() const    { return Nz; }
   int GetSize() const  { return Nx * Ny * Nz; }

   int CropTermsHaveChanged(OMobj_id cropId);
   int SetCropObjs(OMobj_id cropId);
   int SetCropFlags(int cropFlags);
   int GetCropFlags();

   int LightsHaveChanged(int newSeqNum);

   int LUTHasChanged(int newSeqNum) const  { return newSeqNum != LUTSeqNum; }
   int SetLUT(int size, const VLIuint8 *r, const VLIuint8 *g,
              const VLIuint8 *b, const VLIuint16 *a, int newSeqNum);

   // REMOVE THIS! Hack to get around supersampling bug using ARGB images 
   unsigned char *GetBasePlaneARGBImage(int cur_bix, int cur_biy);

private:
   OMobj_id ModuleId;
   OMobj_id CropObjs[XP_NUM_CROP_OBJS];
   VLIContext *Context;
   VLIVolume *Volume;
   void *XPVoxels;
   int DataComponent;
   int DataSeqNum;
   int LUTSeqNum;
   int LightSeqNum;
   int CropTermSeqNums[XP_NUM_CROP_TERMS];
   int CropPredefinedSeqNum;
   int VoxFileNameSeqNum;
   int Nx;
   int Ny;
   int Nz;

   int GetNewDataSeqNum() const;
   void ClearVolume();

   void SetDims(const int *dim)  { Nx = dim[0]; Ny = dim[1]; Nz = dim[2]; }

   // REMOVE THIS! Hack to get around supersampling bug using ARGB images 
   unsigned char *argbImage;
   int prev_bix;
   int prev_biy;
 
};


/*******************  Private (Static) Global Variables  ********************/

/* <none> */

/***********************  Private (Static) Functions  ***********************/

int             XPDrawHexagon(OMobj_id meshId, float viewportScale,
			      VLIVector3D *hexagon,
			      VLIVector2D *textureCoords,
			      int textureType, int textureInterp,
			      unsigned char *image, unsigned char *argbImage,
			      unsigned iw, unsigned ih,
			      GDstate *state);

static void     SetConfigInfo(OMobj_id moduleId);
static int      SetCameraInfo(GDstate *state, VLIContext *vliContext);
static int      SetMaterialProperties(GDstate *state, VLIContext *vliContext);
static int      SetModes(OMobj_id moduleId, VLIContext *vliContext);
static int      SetCutPlaneInfo(OMobj_id moduleId, VLIContext *vliContext,
				float volObjMat[4][4]);
static int      SetLightInfo(GDlight_info *xpLightInfo,XPVLIState *xpvliState);
static int      SetCropInfo(XPVLIState *xpvliState);
static int      SetCursorInfo(XPVLIState *xpvliState);
static int      SetLookupTable(XPVLIState *xpvliState);

static int      GetXPInt(OMobj_id parent, const char *name, int dflt = 0);
static double   GetXPDouble(OMobj_id parent, const char *name,
			    double dflt = 0.0);
inline short    GetXPShort(OMobj_id parent, const char *name, short dflt = 0)
{ 
  return (short)GetXPInt(parent, name, dflt); 
}
inline float    GetXPFloat(OMobj_id parent, const char *name, float dflt = 0)
{ 
  return (float)GetXPDouble(parent, name, dflt); 
}

static OMobj_id FindXPSubobj(OMobj_id parent, const char *name,
                             int mode = OM_OBJ_RD);

inline float    MinFloat(float a, float b)  { return (a < b) ? a : b; }
inline float    MaxFloat(float a, float b)  { return (a > b) ? a : b; }
inline void     MinMaxFloat(float &mn, float &mx, float a)
{ 
  mn = MinFloat(mn, a); mx = MaxFloat(mx, a); 
}

inline float    XPColorToIntensity(float *color)
{ 
  return MaxFloat(color[0], MaxFloat(color[1], color[2])); 
}

/*****************************************************************************
*****************************---   Methods   ---******************************
*****************************************************************************/


/*****************************************************************************
Function: VLIVolumeRender()
Purpose:  This is the implementation of the custom renderer that interfaces
          AVS/Express with the VLI simulator.
Params:   obj    A pointer to a GDobject structure provided by the GD kit.
          state  A pointer to a GDstate structure provided by the GD kit.
State:    This function is a render method, and is invoked whenever the view
          needs to be refreshed.
Returns:  ---
*****************************************************************************/
void VLIVolumeRender(GDobject *obj, GDstate *state)
{
  XPVLIState *xpvliState = XPVLIState::Get(obj);
  
  if (xpvliState && xpvliState->IsValid())
  {
    OMobj_id moduleId = xpvliState->GetModuleId();
    
    VLIContext *vliContext = xpvliState->GetContext();
    if (!vliContext)
    {
      ERRverror("VLIVolumeRender", ERR_ERROR, 
		"Unable to get VLI context\n");
      return;
    }
    
    if (!xpvliState->UpdateVolume(state->objmat))
    {
      ERRverror("VLIVolumeRender", ERR_ERROR,
		"Unable to get VLI volume\n");
      return;
    }
    
    // There may be no volume data to render, even if the volume update
    // was successful.  In that case we can bail out early (with no error
    // message).
    //
    if (!xpvliState->GetVolume())
    {
      return;
    }
    
    // Set up the camera.  Technically, this only needs to be done if
    // the corresponding parameter(s) in the AVS/Express camera have
    // changed, but this is probably just as efficient given the
    // overhead of determining what has changed.
    // 		  		 
    if (!SetCameraInfo(state, vliContext))
    {
      ERRverror("VLIVolumeRender", ERR_ERROR,
		"Unable to set VLI camera\n");
      return;
    }
    
    if (!SetModes(moduleId, vliContext))
    {
      ERRverror("VLIVolumeRender", ERR_ERROR,
		"Unable to set VLI modes\n");
      return;
    }
    
    if (!SetMaterialProperties(state, vliContext))
    {
      ERRverror("VLIVolumeRender", ERR_WARNING,
		"Unable to set VLI material properties\n");
    }
    
    if (!SetLightInfo(state->view->LightInfo, xpvliState))
    {
      ERRverror("VLIVolumeRender", ERR_WARNING,
		"Unable to set VLI lights\n");
    }
    
    if (!SetCropInfo(xpvliState))
    {
      ERRverror("VLIVolumeRender", ERR_WARNING,
		"Unable to set VLI crop slabs\n");
    }
    
    if (!SetCursorInfo(xpvliState))
    {
      ERRverror("VLIVolumeRender", ERR_WARNING,
		"Unable to set VLI cursor\n");
    }
    
    if (!SetCutPlaneInfo(obj->field_id, vliContext, state->objmat))
    {
      ERRverror("VLIVolumeRender", ERR_WARNING,
		"Unable to set VLI slice plane\n");
    }
    
    if (!SetLookupTable(xpvliState))
    {
      ERRverror("VLIVolumeRender", ERR_WARNING,
		"Unable to set VLI lookup table\n");
    }
    
    int textureType;
    switch (GetXPInt(obj->field_id, "texture_type",
		     (int)XP_DEFAULT_TEXTURE_TYPE))
    {
      case XP_kMipmap:
        textureType = GD_TYPE_MIPMAP;
        break;
      
      case XP_kUnfiltered:
      default:
        textureType = GD_TYPE_UNFILTERED;
        break;
    }
    
    int textureInterp;
    switch (GetXPInt(obj->field_id, "texture_interp",
		     (int)XP_DEFAULT_TEXTURE_INTERP))
    {
      case XP_kPoint:
        textureInterp = GD_FILTER_POINT;
        break;
      
      case XP_kBilinear:
      default:
        textureInterp = GD_FILTER_BILINEAR;
        break;
    }
    
    // If there were texture coordinates specified, use those instead of
    // the ones obtained with the base plane.
    //
    VLIVector2D *tex = 0;
    tex = (VLIVector2D *)OMret_name_array_ptr(obj->field_id,
					      OMstr_to_name("tex_coords"),
					      OM_GET_ARRAY_RD, 0, 0);
    
    int bix, biy, imgx, imgy;
    VLIPixel *vliBasePlane;
    VLIVector3D vliHexagon[6];
    VLIVector2D vliTexCoords[6];
    OMobj_id meshId = FindXPSubobj(moduleId, "volume");
    float viewportScale = GetXPFloat(obj->field_id, "viewport_scale", 1.0);
    
    // Allow the VLI and hardware to do all the hard work
    vliContext->RenderBasePlane(xpvliState->GetVolume(), 0);
    
    vliContext->FetchBasePlane(0, bix, biy, imgx, imgy, vliBasePlane, 
			       vliHexagon, vliTexCoords);

    printf( "base plane: w = %d, h = %d  image: w = %d, h = %d\n", bix, biy, imgx, imgy );
    // Diagnostic output
/*
    OMobj_id parentId;
    OMget_obj_parent( moduleId, &parentId );
    int *bp_dims = (int *)OMret_name_array_ptr(parentId,
			     OMstr_to_name("base_plane_dims"),
			     OM_GET_ARRAY_RW, 0, 0);
    if( bp_dims )
    {
      printf( "bix = %d, biy = %d\n", bix, biy );
      bp_dims[0] = bix;    
      bp_dims[1] = biy;    
      ARRfree( bp_dims );
    }

    int *image_dims = (int *)OMret_name_array_ptr(parentId,
		  	       OMstr_to_name("image_dims"),
			       OM_GET_ARRAY_RW, 0, 0);
    if( image_dims )
    {
      printf( "image_x = %d, image_y = %d\n", imgx, imgy );
      image_dims[0] = imgx;    
      image_dims[1] = imgy;    
      ARRfree( image_dims );
    }

    unsigned char *bp_image = (unsigned char *)OMret_name_array_ptr(parentId,
			       OMstr_to_name("base_plane_argb"),
			       OM_GET_ARRAY_RW, 0, 0);
    if( bp_image )
    {
      memcpy( (char *)bp_image, (char *)vliBasePlane, bix*biy*4 );      
      ARRfree( bp_image );
    }

    float *hex_coords = (float *)OMret_name_array_ptr(parentId,
			           OMstr_to_name("hex_coords"),
				   OM_GET_ARRAY_RW, 0, 0);
    if( hex_coords )
    {
      vliHexagon[0].CopyTo(hex_coords + 0);
      vliHexagon[1].CopyTo(hex_coords + 3);
      vliHexagon[5].CopyTo(hex_coords + 6);
      vliHexagon[2].CopyTo(hex_coords + 9);
      vliHexagon[4].CopyTo(hex_coords + 12);
      vliHexagon[3].CopyTo(hex_coords + 15);
      ARRfree( hex_coords );
    }
    
    float *tex_coords = (float *)OMret_name_array_ptr(parentId,
			           OMstr_to_name("tex_coords"),
				   OM_GET_ARRAY_RW, 0, 0);
    if( tex_coords )
    {
      vliTexCoords[0].CopyTo(tex_coords + 0);
      vliTexCoords[1].CopyTo(tex_coords + 2);
      vliTexCoords[5].CopyTo(tex_coords + 4);
      vliTexCoords[2].CopyTo(tex_coords + 6);
      vliTexCoords[4].CopyTo(tex_coords + 8);
      vliTexCoords[3].CopyTo(tex_coords + 10);
      ARRfree( tex_coords );
    }
*/
    
    // Draw the base plane in the Express viewer
    // REMOVE THIS! Hack to get around supersampling bug using ARGB images 
    unsigned char *argbImage = xpvliState->GetBasePlaneARGBImage( bix, biy );
    if( argbImage ) 
    {
      XPDrawHexagon(meshId, viewportScale, vliHexagon, 
		    tex ? tex : vliTexCoords, 
		    textureType, textureInterp, 
		    (unsigned char *)vliBasePlane, argbImage, 
		    bix, biy, state);
    }
     
    // Clean up
    vliContext->ReleaseBasePlane(0);
    
    if (tex)
    {
      ARRfree(tex);
    }
    
  }
  else   // Unable to get xpvliState
  {
    ERRverror("VLIVolumeRender", ERR_ERROR, 
	      "Unable to get xpvliState\n");
  }
  
}	  

/*****************************************************************************
Function: XPDrawHexagon()
Purpose:  Draws the hexagonal baseplane, sets the uv coordinates for each
          vertex, and specifies the texture.
Params:   viewportScale  The scale factor used to size the baseplane on the
                         AVS/Express viewport.
          points         The vertices of the hexagon.
          textureCoords  The uv coordinates for each vertex.
          image          The texture to map onto the hexagon.
          iw, ih         The width and height, in pixels, of the texture
                         image.
          state          A pointer to a GDstate structure provided by the
                         GD kit.
State:    ---
Returns:  ---
*****************************************************************************/
int XPDrawHexagon(OMobj_id meshId, float viewportScale,
		  VLIVector3D *hexagon,
                  VLIVector2D *textureCoords,
                  int textureType, int textureInterp,
                  unsigned char *image, unsigned char *argbImage,
		  unsigned iw, unsigned ih,
                  GDstate *state)
{
  int i;
  GDtexture texture;
  GDobject *obj = state->obj;
  GDview   *view = state->view;
  
  // In order to keep the baseplane from being transformed with the rest
  // of the objects in the view, we use "Locked" mode on that geometry.
  // This means that the baseplane will always be rendered into the range
  // (-1, -1) to (1, 1), regardless of the aspect ratio of the viewport
  // (i.e. a square will turn into a rectangle with the same aspect ratio
  // as the viewport). This means that we need to correct for aspect 
   // ratios other than 1.0 by asymetrically scaling the hexagon.
  //
  float xf, yf;
  if( view->renderer != 0)
  {
    float w, h;
    GDwinfo *winfo = view ? view->Winfo : 0;
    
    if (winfo && winfo->w && winfo->h)
    {
      w = (float)winfo->w;
      h = (float)winfo->h;
    }
    else
       return 0;   // this is an error condition
    
    xf = (h > w) ? (float)1.0 : h / w;
    yf = (h < w) ? (float)1.0 : w / h;
  }
  else
  {
    xf = 1.0;
    yf = 1.0;
  }
  
  // Apply a viewport scale factor.  Only needed when displaying a 'vox' file
  xf *= viewportScale;
  yf *= viewportScale;
  
  // Get the bounding box coordinate array from Express
  float *bboxPoints = (float *)OMret_name_array_ptr(meshId,
						    OMstr_to_name("points"),
						    OM_GET_ARRAY_RD, 0, 0);
  
  // Calculate the center of the volume in world coordinates
  VLIVector3D volWorldCenter;
  if( bboxPoints )
  {
    volWorldCenter.X() = ((bboxPoints[0] + bboxPoints[3]) / 2.0);
    volWorldCenter.Y() = ((bboxPoints[1] + bboxPoints[4]) / 2.0); 
    volWorldCenter.Z() = ((bboxPoints[2] + bboxPoints[5]) / 2.0); 
  }
  else
  {
    volWorldCenter.X() = 0.5;
    volWorldCenter.Y() = 0.5; 
    volWorldCenter.Z() = 0.0; 
  }
  
  // Convert volume center point from world to screen coordinates
  VLIVector3D volScreenCenter;
  VLIMatrix fullMat( (float *)state->fullmat );   
  fullMat.TransformPoint( volWorldCenter, volScreenCenter ); 
  
  //  Adjust the volume center for the window aspect ratio
  volScreenCenter.X() *= xf;  
  volScreenCenter.Y() *= yf; 
  volScreenCenter.Z() = 0.0; 
  
  // We're going to add a temporary texture object (the baseplane image)
  // and various other things to the view that we don't want to persist.
  // Therefore, we remember the current context before adding those things.
  // Then, after rendering, we can restore the original context, so that
  // the GD kit "forgets" about our added stuff.  We have to clean up any
  // allocated memory ourselves, but in this case we allocate almost
  // everything on the stack so this is easy.
  //
  
  GDstack_push(view);
  
  // If we set the transform mode of the data object to "Locked" statically,
  // we can never obtain the full object transform, because the low-level
  // renderer code always sets it to the identity matrix.  Instead, we
  // can change the transform mode on the fly (which has the added benefit
  // of not requiring any special changes to the data object), and obtain
  // the transform matrix normally.
  //
  int oldXformMode = obj->xform_mode;
  
  if (view->state_update_matrix)
  {
    XPUpdateMatrixFunc updateMatrix;
    obj->xform_mode = GD_XFORM_MODE_LOCKED;
    updateMatrix = (XPUpdateMatrixFunc)view->state_update_matrix;
    (*updateMatrix)(view);
  }
  
  // Calulate the center of the hexagon
  VLIVector3D hexCenter;
  double t;
  
  if (hexagon[0] + (hexagon[0]-hexagon[3]) == hexagon[3])
  {
    t = 0.5;
  }
  else
  {
    t = -0.5;
  }
  hexCenter = hexagon[0] + (t*(hexagon[0] - hexagon[3]));
  
  // Translate the hexagon origin to its center
  for (i=0;i<6;i++) {
    hexagon[i] -= hexCenter;
  }
  
  // Correct for the viewport aspect ratio
  for (i=0;i<6;i++) {
    hexagon[i].X() *= xf;
    hexagon[i].Y() *= yf;
  }
  
  // Translate	the new hexagon origin to the center of the bounding box
  for (i=0;i<6;i++) {
    hexagon[i] += volScreenCenter;
  }
  
  // When in 'Locked' xform_mode the Z values must be between 0 and 1
  for (i=0;i<6;i++) {
    hexagon[i].Z() = 0.0;
  }
  
  // Set up the points of the hexagon in an order appropriate for a tri
  // strip.  That is, numbered like this:
  //
  //     input order            desired order
  //
  //   0----1--------           0----1--------
  //   |     \                  |     \
  //   |      \                 |      \
  //   5       \                2       \
  //   |\       2       ===>    |\       3
  //   | \      |               | \      |
  //   |  \     |               |  \     |
  //   |   4----3               |   4----5
  //   |                        |
  //   |                        |
  //
   
  float xpPoints[6*3];
  float xpTexCoords[6*2];
  
  hexagon[0].CopyTo(xpPoints + 0);
  hexagon[1].CopyTo(xpPoints + 3);
  hexagon[5].CopyTo(xpPoints + 6);
  hexagon[2].CopyTo(xpPoints + 9);
  hexagon[4].CopyTo(xpPoints + 12);
  hexagon[3].CopyTo(xpPoints + 15);
  
  if (textureCoords)
  {
    textureCoords[0].CopyTo(xpTexCoords +  0);
    textureCoords[1].CopyTo(xpTexCoords +  2);
    textureCoords[5].CopyTo(xpTexCoords +  4);
    textureCoords[2].CopyTo(xpTexCoords +  6);
    textureCoords[4].CopyTo(xpTexCoords +  8);
    textureCoords[3].CopyTo(xpTexCoords + 10);
  }
  
  // Initialize the GDgeom_data structure.  This seems to just set all of
  // the fields in the structure to 0.
  //
  GDgeom_data geomData;
  GDinit_geom_data(&geomData, state);
  strcpy(geomData.name, VLI_GEOM_NAME);
  
  geomData.geom_type = GD_GEOM_TRISTRIP;
  
  geomData.ngeoms = 1;        // only one tristrip
  geomData.nspace = 3;        // because xpPoints is (effectively) [][3]
  geomData.rspace = 3;        // camera space - todo: guarantee 3D above
  geomData.have_surface = 1;  // a surface type cell set
  geomData.have_lines = 0;    // not a line type cell set
  geomData.n = 6;             // six vertices total
  
  geomData.vert_base = geomData.vert = xpPoints;
  
  // Setup the extents
  if( bboxPoints )
  {
    geomData.have_vert_extents = 1;
    
    geomData.vert_min_ext[0] = bboxPoints[0];
    geomData.vert_min_ext[1] = bboxPoints[1];
    geomData.vert_min_ext[2] = bboxPoints[2];

    geomData.vert_max_ext[0] = bboxPoints[3];
    geomData.vert_max_ext[1] = bboxPoints[4];
    geomData.vert_max_ext[2] = bboxPoints[5];

    ARRfree( bboxPoints );
  }

  // Set up the texture information, if provided.
  //
  if (textureCoords && image && view->state_set_texture)
  {
    // Initialize the GDtexture structure..
    //
    static float mat[4][4] = { { 1, 0 ,0 ,0 },
			       { 0, 1, 0, 0 },
			       { 0, 0, 1, 0 },
			       { 0, 0, 0, 1 } };
    
    // REMOVE THIS! Hack to get around supersampling bug using ARGB images 

    // The baseplane image is in RGBA format, but we're expecting ARGB.
    // Therefore, we have to allocate a buffer in which to swap it around.
    // This is a little unfortunate, because internally Express will just
    // swap it back to RGBA.  Ultimately, we should modify the Express
    // render code so that we can specify that it shouldn't bother swapping
    // the image bytes around.
    //
    if( argbImage )
    {
      int i;
      unsigned char *ip = image;
      unsigned char *op = argbImage;

      for (i = 0; i < iw*ih; i++, ip += 4, op += 4)
      {
	op[0] = ip[3];
	op[1] = ip[0];
	op[2] = ip[1];
	op[3] = ip[2];
      }
    }
      
    memset(&texture, 0, sizeof(GDtexture));
    
    texture.changed = 1;
    texture.data_changed = 1;
    
    texture.data = argbImage;
    texture.ndims = 2;
    texture.ncomps = 4;   // A, R, G, and B

    // The VLI will always return a texture image size to a power of 2...
    texture.width = iw;
    texture.height = ih;
    
    texture.tile = GD_TILE_CLAMP;
    texture.alpha = GD_ALPHA_REPLACE;
    texture.filter = textureInterp;
    texture.type = textureType;
    texture.stage = GD_STAGE_PRESPECULAR;
    
    XPSetTextureFunc setTexture;
    setTexture = (XPSetTextureFunc)view->state_set_texture;
    (*setTexture)(view, &texture, mat);
    
    geomData.uv = geomData.uv_base = xpTexCoords;
  }
  
  // Now render the tri strip.  Note that in order to keep the hexagon
  // facing the camera (i.e. parallel to the viewplane), the xform_mode
  // of the object has been set to "Locked".
  //
  if (view->prim_3d_polytri) 
  {
    XPPolyTriRendFunc rendFunc;
    rendFunc = (XPPolyTriRendFunc)view->prim_3d_polytri;
    (*rendFunc)(&geomData, state);
  }
  
  GDstack_pop(view);   // restore the original GD state
  obj->xform_mode = oldXformMode;
  
  // Not sure about this call.  Was getting a OGL out of memory
  // error in glTexImage2D after multiple renders of large datasets.
  // This clears it up.  
  //
  XPFreeTextureFunc freeTxtrFunc;
  freeTxtrFunc = (XPFreeTextureFunc)texture.txtr_free_func;
  if( freeTxtrFunc )
  {
    (*freeTxtrFunc)( &texture );
  }
  
  // Clean up geom data object (free the name etc.)
  XPFreeGeomFunc freeGeomFunc;
  freeGeomFunc = (XPFreeGeomFunc)geomData.geom_free_func;
  if( freeGeomFunc )
  {
    (*freeGeomFunc)( &geomData );
  }
  
  return 1;
}


/*****************************************************************************
Function: SetConfigInfo()
Purpose:  Obtains configuration information from the VLI, and sets the
          configuration information subobjects of the render module
          accordingly.
Params:   moduleId   The OMobj_id of the render module.
State:    This function sets the values of all subobjects in the
          "config_info" subobject.
Returns:  ---
*****************************************************************************/
static void SetConfigInfo(OMobj_id moduleId)
{
  OMobj_id configInfo = FindXPSubobj(moduleId, "config_info", OM_OBJ_RW);
  
  if (!OMis_null_obj(configInfo))
  {
    int b;
    int boardCount;
    OMobj_id boardArray;
    OMobj_id boardInfo;
    VLIConfiguration vliConfig;
    
    OMset_name_int_val(configInfo, OMstr_to_name("major_version"),
		       vliConfig.GetBoardMajorVersion());
    OMset_name_int_val(configInfo, OMstr_to_name("minor_version"),
		       vliConfig.GetBoardMinorVersion());
    OMset_name_int_val(configInfo, OMstr_to_name("vli_major_version"),
		       vliConfig.GetVLIMajorVersion());
    OMset_name_int_val(configInfo, OMstr_to_name("vli_minor_version"),
		       vliConfig.GetVLIMinorVersion());
    OMset_name_int_val(configInfo, OMstr_to_name("max_cut_planes"), 
		       vliConfig.GetMaxCutPlanes());
    OMset_name_int_val(configInfo, OMstr_to_name("gradient_table_length"), 
		       vliConfig.GetGradientTableLength());
    
    boardCount = vliConfig.GetNumberOfBoards();
    OMset_name_int_val(configInfo, OMstr_to_name("board_count"),
		       boardCount);
    
    boardArray = FindXPSubobj(configInfo, "board_info", OM_OBJ_RW);
    
    if (!OMis_null_obj(boardArray))
    {
      for (b = 0; b < boardCount; b++)
      {
	if (OMget_array_val(boardArray, b, &boardInfo,
			    OM_OBJ_RW) == OM_STAT_SUCCESS)
	{
	  OMset_name_int_val(boardInfo,
			     OMstr_to_name("available_memory"),
			     (int)vliConfig.GetAvailableMemory(b));
	}
      }
    }
  }
}


/*****************************************************************************
Function: SetCameraInfo()
Purpose:  Sets up the VLI camera so that it has the same orientation as the
          AVS/Express camera.
Params:   state       A pointer to the GDstate structure that contains the
                      AVS/Express camera xform matrix.
          vliContext  A pointer to the VLI context containing the VLI camera.
State:    Any current settings for the view matrix of the VLI camera are 
          replaced.
Returns:  1  if successful,
          0  otherwise.
*****************************************************************************/
static int SetCameraInfo(GDstate *state, VLIContext *vliContext)
{
  if (state)
  {
    VLIMatrix viewMat( (float *)state->viewmat );
    vliContext->GetCamera().SetViewMatrix( viewMat );
    
    return 1;
  }
  
  return 0;
}


/*****************************************************************************
Function: SetMaterialProperties()
Purpose:  Sets the material (i.e. reflectance) properties for the volume
          object from the corresponding attributes of the AVS/Express object.
Params:   state       A pointer to the GDstate structure that contains the
                      material properties for this object from AVS/Express.
          vliContext  A pointer to the VLI context.
State:    The diffuse, emissive, and specular coefficients are set, as are
          the specular exponent and color.
Returns:  1  if successful,
          0  otherwise.
*****************************************************************************/
static int SetMaterialProperties(GDstate *state, VLIContext *vliContext)
{
  if (state)
  {
    float ambientInten;
    float *material = state->material;
    double diffuse = (double)(material[XP_kDiffuse]);
    double specular = (double)(material[XP_kSpecular]);
    double emissive;
    double specularExp = (double)(material[XP_kSpecularExp]);
    
    // AVS/Express uses the notion of ambient lighting, to which the
    // various objects in the scene are more or less responsive.  The
    // VLI uses emissive light (objects in the scene are modeled as
    // giving off light, or glowing).  The emissive light coefficient
    // for an object is its ambient coefficient times the intensity of
    // the ambient light.
    //
    if (state->view->LightInfo->amb_state)   // if ambient lighting is on
       ambientInten = XPColorToIntensity(state->view->LightInfo->amb_col);
    else
       ambientInten = 0;
    
    emissive = (double)(material[XP_kAmbient] * ambientInten);
    
    if (vliContext->SetReflectionProperties(diffuse, specular, emissive,
					    specularExp) == kVLIOK)
    {
      if (vliContext->SetSpecularColor((double)state->spec_col[0],
				       (double)state->spec_col[1],
				       (double)state->spec_col[2]) == kVLIOK)
	 return 1;
    }
  }
  
  return 0;
}


/*****************************************************************************
Function: SetModes()
Purpose:  Sets the gradient modulation and blend modes according to the user
          settings in the VLIVolume module.
Params:   moduleId    The OMobj_id of the VLIVolume module, which contains
                      the user-supplied values for these modes.
          vliContext  A pointer to the VLI context.
State:    The alpha and color modulation modes are set, as is the blend mode.
Returns:  1  if successful,
          0  otherwise.
*****************************************************************************/
static int SetModes(OMobj_id moduleId, VLIContext *vliContext)
{
  // Add correct opacity
  int correctOpacity = GetXPInt(moduleId, "correct_opacity", VLItrue);
  vliContext->SetCorrectOpacity(correctOpacity);
  
  // Set the gradient table
  double *gradTable = (double *)OMret_name_array_ptr(moduleId, 
		 		   OMstr_to_name("gradient_table"),
				   OM_GET_ARRAY_RD, 0, 0);
  if( gradTable ) 
  {
    if (vliContext->SetGradientTable( gradTable ) != kVLIOK)
    {
      return 0;
    }
    ARRfree( gradTable );
  }
  
  // Set gradient opacity modulation
  int gradOpacityMod = GetXPInt(moduleId, "grad_opacity_mod", VLIfalse);
  vliContext->SetGradientOpacityModulation(gradOpacityMod);
  
  // Set gradient diffuse illumination
  int gradDiffuseIllumMod = GetXPInt(moduleId, "grad_diffuse_illum_mod", 
				     VLIfalse);
  if (vliContext->SetGradientDiffuseIlluminationModulation(
    gradDiffuseIllumMod) != kVLIOK)
  {
    return 0;
  }
  
  // Set gradient specular illumination
  int gradSpecularIllumMod = GetXPInt(moduleId, "grad_specular_illum_mod",
				      VLIfalse);
  if (vliContext->SetGradientSpecularIlluminationModulation(
    gradSpecularIllumMod) != kVLIOK)
  {
    return 0;
  }
  
  // Set the supersample space
  VLICoordinateSpace supersampleSpace;
  switch( GetXPInt(moduleId, "supersample_space", XP_kCameraSpace ))
  {
    case XP_kObjectSpace:
      supersampleSpace = kVLIObjectSpace;
      break;

    case XP_kCameraSpace:
    default:
      supersampleSpace = kVLICameraSpace;
      break;
  }
  if (vliContext->SetSuperSamplingSpace( supersampleSpace ) != kVLIOK)
  {
    return 0;
  }
  
  // Set the supersample base plane accumulation
  switch( GetXPInt(moduleId, "base_plane_accum", XP_kGrowBasePlane ))
  {
    case XP_kBlendBasePlane:
      if (vliContext->SetBasePlaneAccumulation(VLIContext::kBlendBasePlane) 
	  != kVLIOK)
      {
        return 0;
      }
      break;

    case XP_kGrowBasePlane:
    default:
      if (vliContext->SetBasePlaneAccumulation(VLIContext::kGrowBasePlane) 
	  != kVLIOK)
      {
	return 0;
      }
      break;
  }
  
  // Set the supersample factors
  double xFactor = GetXPDouble(moduleId, "x_supersample", 1.0);
  double yFactor = GetXPDouble(moduleId, "y_supersample", 1.0);
  double zFactor = GetXPDouble(moduleId, "z_supersample", 1.0);
  if (vliContext->SetSuperSamplingFactor( xFactor, yFactor, zFactor ) 
      != kVLIOK)
  {
    return 0;
  }   
  
  // Set the blend mode
  VLIBlendMode blendMode;
  switch (GetXPInt(moduleId, "blend_mode", XP_kFTB))
  {
    case XP_kMIP:
      blendMode = kVLIBlendMIP;
      break;
    
    case XP_kMINIP:
      blendMode = kVLIBlendMINIP;
      break;
    
    case XP_kFTB:
    default:
      blendMode = kVLIBlendFTB;
      break;
  }
  
  if (vliContext->SetBlendMode(blendMode) != kVLIOK)
  {
    return 0;
  }
  
  // Successful return
  return 1;
}


/*****************************************************************************
Function: SetCutPlaneInfo()
Purpose:  Sets the VLI cut plane according to the input slice plane.
          The slice plane is a 2D, 2-Space unit plane lying in XY.  It is
          moved into 3-space using its xform.  We construct the coefficients
          for the VLI cut plane from the point-normal form of the
          transformed plane.
Params:   moduleId    The OMobj_id of the render module.
          vliContext  A pointer to the VLI context.
State:    All VLI cut plane parameters are updated.
Returns:  1  if successful,
          0  otherwise.
*****************************************************************************/
static int SetCutPlaneInfo(OMobj_id moduleId, VLIContext *vliContext,
			   float volObjMat[4][4])
{
  int ok = 0;
  int enabled = 1;
  VLICutPlane *cutPlane = 0;
  VLICutPlane::Flags sliceFlags;
  OMobj_id sliceId = FindXPSubobj(moduleId, "slice_plane");
  
  // First of all, determine if the slice plane should be enabled or
  // disabled.  If there is no AVS/Express plane object connected to the
  // "slice_plane" input, or if the slice flags are set to "disabled",
  // we should disable the slice plane and remove it from the contest.
  // Otherwise, we should enabled it and add it to the context (if
  // necessary).
  //
  if (OMis_null_obj(sliceId))   // if there is no AVS/Express slice plane
  {
    enabled = 0;
  }
  else
  {
    switch (GetXPInt(moduleId, "slice_flags"))
    {
      case XP_kInside:   // inside
        sliceFlags = VLICutPlane::kInside;
        break;
      
      case XP_kOutside:   // outside
        sliceFlags = VLICutPlane::kOutside;
        break;
      
      case XP_kDisable:   // disabled; remove cut plane from VLI context
      default:
        enabled = 0;
        break;
    }
  }
  
  // If there is currently a slice plane in the context, get a pointer
  // to it.
  //
  if (vliContext)
  {
    if (vliContext->GetCutPlaneCount() > 0)
    {
      cutPlane = vliContext->GetCutPlane(0);
    }
    else 
    {
      cutPlane = 0;
    }
    
    // If the slice plane is enabled, obtain the coefficients from the
    // AVS/Express slice plane object and set up the VLI slice plane so
    // that it corresponds.
    //
    if (enabled)
    {
      float planeRelMat[4][4];
      float fullMat[4][4];
      float xfVerts[2][3];
      
      // The xform of the plane is relative to the volume.  Since the
      // VLI wants the xform in 'world coordinates' we concat the volume
      // 'object' xform with the plane xform.
      static float planeVerts[2][3] = { { 0, 0, 0 }, { 0, 0, 1 } };
      OMobj_id xformId = FindXPSubobj(sliceId, "xform");
      GDxform_get_comp_matrix(xformId, (float *)planeRelMat);
      MATmat4_multiply( planeRelMat, volObjMat, fullMat );
      MATxform_verts3(2, planeVerts, fullMat, xfVerts);
      
      // Calculate the plane coefficients
      double a = xfVerts[1][0] - xfVerts[0][0];
      double b = xfVerts[1][1] - xfVerts[0][1];
      double c = xfVerts[1][2] - xfVerts[0][2]; 
      double d = -((a * xfVerts[0][0]) + (b * xfVerts[0][1]) + 
		   (c * xfVerts[0][2]));
      
      /*  Comment out for now, error in compile project link with 'sqrt'
	  
      // Normalize the plane equation
      double length = sqrt(a*a + b*b + c*c);
      if (length > VLIGetEpsilon() )
      {
        double k = 1.0/length;
	a *= k;
	b *= k;
	c *= k;
	d *= k;
      }

      end of comment */

      double sliceFallOff = GetXPDouble(moduleId, "slice_falloff");
      double sliceOffset = GetXPDouble(moduleId, "slice_offset");
		   
      if (cutPlane)   // if there is already a cut plane in the VLI context
      {
	if (cutPlane->SetPlane(a, b, c, d) == kVLIOK)
	   if (cutPlane->SetThickness(sliceOffset) == kVLIOK)
	      if (cutPlane->SetFallOff(sliceFallOff) == kVLIOK)
		 ok = (cutPlane->SetFlags(sliceFlags) == kVLIOK);
      }
      else   // create a new cut plane and add it to the VLI context
      {
	cutPlane = VLICutPlane::Create(a, b, c, d, sliceOffset,
				       sliceFallOff, sliceFlags);
	
	if (cutPlane)
	{
	  ok = (vliContext->AddCutPlane(cutPlane) == kVLIOK);
	  
	  cutPlane->Release();
	}
      }
    }
    else   // the slice plane is disabled; remove it from the VLI context
    {
      if (cutPlane)   // if there is a cut plane to remove
      {
	ok = (vliContext->RemoveCutPlane(cutPlane) == kVLIOK);
      }
      else        // no cut plane to remove
      {
	ok = 1;
      }
    }
  }
  else        // no context
  {
    ok = 1;
  }
  
  return ok;
}


/*****************************************************************************
Function: SetLightInfo()
Purpose:  Sets up the VLI lights to emulate lights set AVS/Express.
Params:   xpLightInfo  A pointer to the GDlight_info object which describes
                       the lights currently set in AVS/Express.
          vliContext   A pointer to the VLI context.
State:    Updates VLI lights to AVS/Express settings.
Returns:  1 if successful
          0 otherwise 
*****************************************************************************/
static int SetLightInfo(GDlight_info *xpLightInfo,XPVLIState *xpvliState)
{
  
  // Check sequence number to see if anything in the lights has changed. 
  // Don't want to have VLI recalculate reflectance map if nothing has changed
  //
  int newSeqNum = OMget_obj_seq(xpLightInfo->light_info_id, OMnull_obj, 
				OM_SEQ_VAL);
  if (!xpvliState->LightsHaveChanged(newSeqNum)) return 1;
  
  
  // Get light information from GD state then set the lights in the VLI
  // correspondingly.
  // 
  if (xpLightInfo)
  {
    VLIContext *vliContext = xpvliState->GetContext();
    int numXPLights = xpLightInfo->nlights;
    int numVLILights = vliContext->GetLightCount();
    int xpLightIndex;
    unsigned int vliLightIndex;
    GDlight *curXPLight;
    VLILight *tmpLight;
    double vliLightIntensity;
    float vliLightPos[2][3];
    static float xpLightPos[2][3] = {{ 0.0, 0.0, 0.0 }, { 0.0, 0.0, -1.0 }};
    
    // Since we can't set light type in VLI, we must remove all old VLI
    // lights and then set the current values.  Probably no more overhead
    // than checking what has changed.
    //
    for (vliLightIndex = 0; vliLightIndex < numVLILights; vliLightIndex++)
    {
      tmpLight = vliContext->GetLight(vliLightIndex);
      
      if (tmpLight)
      {
	vliContext->RemoveLight(tmpLight);
	tmpLight->Release();
      }
    }
    
    // Now add a VLI light to the VLI context for every AVS/Express light.
    // Because the VLI has no notion of bidirectional lights, we add two
    // opposed directional lights to simulate a single bidirectional
    // light.
    // Note that the ambient light is handled separately, as part of the
    // material properties (because the VLI models this light as emissive,
    // rather than reflective).
    //
    for (xpLightIndex = 0; xpLightIndex < numXPLights; xpLightIndex++)
    {
      curXPLight = xpLightInfo->lights[xpLightIndex];
      
      // Susan points out that to light the scene accurately, we should
      // probably turn off the AVS/Express lights (otherwise an artifact
      // of the rendering process -- the VLI baseplane -- will be shaded
      // as though it were a normal object in the scene).  There are two
      // problems with this:
      //  1. We can't be turning the lights on and off without first
      //     pushing the graphics state.  Therefore, this function might
      //     better be called from within the XPDrawHexagon() function.
      //  2. We might want to have a directional light on the positive
      //     Z axis directed at the origin, so that the baseplane can be
      //     lit by both diffuse and ambient light.  Obviously the
      //     plane on which the baseplane is texture mapped should be
      //     unresponsive to specular illumination.
      //
      // curXPLight->state = 0;   // can't do this here; see note above
      
      if (curXPLight->state)   // if the light is on
      {
	// AVS/Express lights have a type, a transform, and a color.
	// VLI lights have a type, a position, and an intensity.  We
	// map the AVS/Express light attributes onto their VLI analogs
	// as follows:
	// 1. The type maps directly, except that a single AVS/Express
	//    bidirectional light is modeled by 2 VLI directional lights.
	// 2. The VLI light position is obtained by subjecting the
	//    default position for AVS/Express lights to the light
	//    transform.  This also provides the direction.
	// 3. The VLI light intensity is obtained from the AVS/Express
	//    light color using the function XPColorToIntensity().
	//
	vliLightIntensity = XPColorToIntensity(curXPLight->col);
	
	MATxform_verts3(2, xpLightPos, curXPLight->Xform->matrix,
			vliLightPos);
	
	vliLightPos[1][0] -= vliLightPos[0][0];   // convert the direction
	vliLightPos[1][1] -= vliLightPos[0][1];   // point to a vector
	vliLightPos[1][2] -= vliLightPos[0][2];
	
	VLIVector3D position(vliLightPos[0]);
	VLIVector3D direction(vliLightPos[1]);
	
	switch (curXPLight->type)
	{
	  case GD_LIGHT_DIR:    // Directional light
	    tmpLight = VLILight::CreateDirectional(
	      direction,
	      vliLightIntensity);
	    break;
	  
	  case GD_LIGHT_BIDIR:  // Bi-directional light (2 VLI lights)
	    tmpLight = VLILight::CreateDirectional(
	      direction,
	      vliLightIntensity);
	    vliContext->AddLight(tmpLight);
	    
	    direction *= -1.0;
	  
	    tmpLight = VLILight::CreateDirectional(
	      direction,
	      vliLightIntensity);
	    break;
	  
	  default:   // Default directional light
	    tmpLight = VLILight::CreateDirectional();
	}
	
	vliContext->AddLight(tmpLight);
      }
    }
    
    return 1;
  }
  
  return 0;
}


/*****************************************************************************
Function: SetCropInfo()
Purpose:  Sets up the crop info (slabs) according to the crop parameters in
          the AVS/Express VLIVolume object.
Params:   xpvliState   A pointer to the XPVLIState object associated with
                       the GDobject being rendered.
State:    This method modifies the current crop settings in the VLI context.
Returns:  1  if the cropping was set up successfully,
          0  otherwise.
*****************************************************************************/
static int SetCropInfo(XPVLIState *xpvliState)
{
  int ok = 1;
  int enabled = 0;
  VLIContext *vliContext = xpvliState->GetContext();
  OMobj_id moduleId = xpvliState->GetModuleId();
  OMobj_id cropId = FindXPSubobj(moduleId, "crop_bounds", OM_OBJ_RW);
  VLICrop &vliCrop = vliContext->GetCrop();
  
  if (!OMis_null_obj(cropId))
  {
    enabled = GetXPInt(cropId, "crop_enabled");
  }
  
  if (enabled)
  {
    double minx = GetXPDouble(cropId, "min_x", 0.0);
    double maxx = GetXPDouble(cropId, "max_x",
			      (double)(xpvliState->GetNx() - 1));
    double miny = GetXPDouble(cropId, "min_y", 0.0);
    double maxy = GetXPDouble(cropId, "max_y",
			      (double)(xpvliState->GetNy() - 1));
    double minz = GetXPDouble(cropId, "min_z", 0.0);
    double maxz = GetXPDouble(cropId, "max_z",
			      (double)(xpvliState->GetNz() - 1));
    
    // Set the slab info in the VLI
    if (vliCrop.SetSlabs(minx, maxx, miny, maxy, minz, maxz) != kVLIOK)
    {
      ok = 0;
    }
    
    // Determine the crop terms
    //
    int cropFlags;
    int flagType = GetXPInt(cropId, "predefined", (int)XP_kSubVolume);
    
    switch (flagType)
    {
      case XP_kSubVolume:
        cropFlags = VLICrop::kSubVolume;
	break;
      
      case XP_k3DCross:
        cropFlags = VLICrop::k3DCross;
	break;
		  
      case XP_k3DCrossInvert:
        cropFlags = VLICrop::k3DCrossInvert;
	break;
      
      case XP_k3DFence:
        cropFlags = VLICrop::k3DFence;
	break;
      
      case XP_k3DFenceInvert:
        cropFlags = VLICrop::k3DFenceInvert;
	break;
      
      case XP_kUserDefined:
      default:
        xpvliState->SetCropObjs(cropId);
	cropFlags = xpvliState->GetCropFlags();
	break;
    }
    
    // Set the crop flags in the VLI
    if (vliCrop.SetFlags(cropFlags) != kVLIOK)
    {
      ok = 0;
    }
    
    // Something went wrong
    if (!ok)   
    {
      vliCrop.SetFlags(VLICrop::kDisable);
    }
    
  }
  else   // cropping is disabled
  {
    vliCrop.SetFlags(VLICrop::kDisable);
  }
  
  return ok;
}


/*****************************************************************************
Function: SetCursorInfo()
Purpose:  Sets up the cursor info according to the cursor parameters in
          the AVS/Express VLIVolume object.
Params:   xpvliState   A pointer to the XPVLIState object associated with
                       the GDobject being rendered.
State:    This method modifies the current cursor settings in the VLI context.
Returns:  1  if the cursor was set up successfully,
          0  otherwise.
*****************************************************************************/
static int SetCursorInfo(XPVLIState *xpvliState)
{
  int ok = 1;
  int enabled = 0;

  OMobj_id  moduleId = xpvliState->GetModuleId();
  OMobj_id  cursorId = FindXPSubobj( moduleId, "cursor", OM_OBJ_RW );

  VLIContext *vliContext = xpvliState->GetContext();
  VLICursor  &vliCursor  = vliContext->GetCursor();
  
  if( !OMis_null_obj( cursorId ) )
  {
    enabled = GetXPInt( cursorId, "enabled" );
  }
  
  if( enabled )
  {
    // Set the cursor type
    int type = GetXPInt( cursorId, "type", (int)XP_kCrossHair );
    VLICursor::Types cursorType;
    switch( type )
    {
      case XP_kPlane:
        cursorType = VLICursor::kPlane;
	break;		  

      case XP_kCrossHair:
      default:
        cursorType = VLICursor::kCrossHair;
	break;
    }
    if( vliCursor.SetType( cursorType ) != kVLIOK )
    {
      ok = 0;
    }
    
    // Set the cursor position in VLI "object" coordinates
    double *position = (double *)OMret_name_array_ptr( cursorId, 
				    OMstr_to_name("position"),
				    OM_GET_ARRAY_RD, 0, 0);
    if( position )
    {
      vliCursor.SetPosition( position[0], position[1], position[2] );
      ARRfree( position );
    }  

    // Set the cursor width and length
    double width = GetXPDouble( cursorId, "width", 1.0 );
    if( vliCursor.SetWidth( width ) != kVLIOK )
    {
      ok = 0;
    }

    double length = GetXPDouble( cursorId, "length", 150.0 );
    if( vliCursor.SetLength( length ) != kVLIOK )
    {
      ok = 0;
    }

    // Set cursor color and alpha terms
    OMobj_id AxisId;
    VLICursor::Axes  curAxis;
    VLICursor::Flags curFlag;
    int attributes = 0;
    for( int i = 0; i < 3; i++ )
    {
      switch( i )
      {
	case 0:
	  AxisId = FindXPSubobj( cursorId, "x_axis", OM_OBJ_RW );
	  curAxis = VLICursor::kXAxis;
          curFlag = VLICursor::kEnableX;
	  break;
	case 1:
	  AxisId = FindXPSubobj( cursorId, "y_axis", OM_OBJ_RW );
	  curAxis = VLICursor::kYAxis;
          curFlag = VLICursor::kEnableY;
          break;
	case 2:
	  AxisId = FindXPSubobj( cursorId, "z_axis", OM_OBJ_RW );
	  curAxis = VLICursor::kZAxis;
          curFlag = VLICursor::kEnableZ;
          break;
      }

      double red   = GetXPDouble( AxisId, "red",   (double)(i == 0) );    
      double green = GetXPDouble( AxisId, "green", (double)(i == 1) );    
      double blue  = GetXPDouble( AxisId, "blue",  (double)(i == 2) );    
      if( vliCursor.SetColor( curAxis, red, green, blue ) != kVLIOK )
      {
	ok = 0;
      }

      double alpha = GetXPDouble( AxisId, "alpha ", 1.0 );    
      if( vliCursor.SetOpacity( curAxis, alpha ) != kVLIOK )
      {
	ok = 0;
      }

      int axis_enabled = GetXPInt( AxisId, "enabled", 1 );
      if( axis_enabled ) 
      {
	attributes |= curFlag;
      }
    }

    // Add on the crop attribute
    int disable_crop = GetXPInt( cursorId, "disable_crop", 0 );
    if( disable_crop )
    {
      attributes |= VLICursor::kDisableCrop;
    }

    // Set the cursor attributes in the VLI
    if( vliCursor.SetAttributes( attributes ) != kVLIOK )
    {
      ok = 0;
    }
    
    // Something went wrong
    if (!ok)   
    {
      vliCursor.SetAttributes( VLICursor::kDisable );
    }
    
  }
  else   // cursor is disabled
  {
    vliCursor.SetAttributes( VLICursor::kDisable );
  }
  
  return ok;
}


/*****************************************************************************
Function: SetLookupTable()
Purpose:  Sets the lookup table according to the user settings in the
          VLIVolume module.
Params:   moduleId    The OMobj_id of the VLIVolume module, which contains
                      the user-supplied lookup table.
          vliContext  A pointer to the VLI context.
State:    The VLI lookup table is set.
Returns:  1  if successful,
          0  otherwise.
*****************************************************************************/
static int SetLookupTable(XPVLIState *xpvliState)
{
  int ok = 0;
  OMobj_id moduleId = xpvliState->GetModuleId();
  OMobj_id xpLUTId = FindXPSubobj(moduleId, "lookup_table");
  
  // If a VLILookUpTable does not exist replace the old one with a default
  if (OMis_null_obj(xpLUTId))
  {
    if (xpvliState->LUTHasChanged(0))
    {
      ok = xpvliState->SetLUT(0, 0, 0, 0, 0, 0);
    }
    else  // the LUT hasn't changed (still a default type); nothing to do
    {
      ok = 1;   
    }
  }
  
  else
  {
    int newSeqNum = OMget_obj_seq(xpLUTId, OMnull_obj, OM_SEQ_VAL);
    
    if (xpvliState->LUTHasChanged(newSeqNum))
    {
      int size = GetXPInt(xpLUTId, "size");
      VLIuint8 *r;
      VLIuint8 *g;
      VLIuint8 *b;
      VLIuint16 *a;
      
      r = (VLIuint8 *)OMret_name_array_ptr(xpLUTId, OMstr_to_name("red"),
					   OM_GET_ARRAY_RD, 0, 0);
      g = (VLIuint8 *)OMret_name_array_ptr(xpLUTId, OMstr_to_name("green"),
					   OM_GET_ARRAY_RD, 0, 0);
      b = (VLIuint8 *)OMret_name_array_ptr(xpLUTId, OMstr_to_name("blue"),
					   OM_GET_ARRAY_RD, 0, 0);
      a = (VLIuint16 *)OMret_name_array_ptr(xpLUTId,
					    OMstr_to_name("alpha"),
					    OM_GET_ARRAY_RD, 0, 0);
      
      if (r && g && b && a)
      {
	ok = xpvliState->SetLUT(size, r, g, b, a, newSeqNum);
	ARRfree(r);
	ARRfree(g);
	ARRfree(b);
	ARRfree(a);
      }
    }
    else    // the LUT hasn't changed; nothing to do
       ok = 1;
  }
  
  return ok;
}


/*****************************************************************************
Function: GetXPInt()
Purpose:  A simple utility function to make getting the value of an
          AVS/Express integer object a little more convenient.
Params:   parent  The OMobj_id of the parent group that contains the target
                  subobject.
          name    The name of the subobject whose value to get.
          dflt    An optional default value, to return if the value of the
                  specified object cannot be obtained.  If not supplied,
                  0 is assumed.
State:    ---
Returns:  The value of the specified AVS/Express object, or the specified
          default value if there is an error obtaining the object's value.
*****************************************************************************/
static int GetXPInt(OMobj_id parent, const char *name, int dflt)
{
  int a;
  
  if (OMget_name_int_val(parent,
			 OMstr_to_name((char *)name), &a) != OM_STAT_SUCCESS)
     a = dflt;

  return a;
}


/*****************************************************************************
Function: GetXPDouble()
Purpose:  A simple utility function to make getting the value of an
          AVS/Express double object a little more convenient.
Params:   parent  The OMobj_id of the parent group that contains the target
                  subobject.
          name    The name of the subobject whose value to get.
          dflt    An optional default value, to return if the value of the
                  specified object cannot be obtained.  If not supplied,
                  0 is assumed.
State:    ---
Returns:  The value of the specified AVS/Express object, or the specified
          default value if there is an error obtaining the object's value.
*****************************************************************************/
static double GetXPDouble(OMobj_id parent, const char *name, double dflt)
{
  double a;
  
  if (OMget_name_real_val(parent, OMstr_to_name((char *)name),
			  &a) != OM_STAT_SUCCESS)
     a = dflt;
  
  return a;
}


/*****************************************************************************
Function: FindXPSubobj()
Purpose:  A simple utility function to find the specified subobject, then
          follow any references from that subobject to the object at the
          end of the chain of references.
Params:   parent  The OMobj_id of the parent group that contains the target
                  subobject.
          name    The name of the subobject to get.
          mode    An optional parameter indicating how the found object will
                  be used.  The default value is OM_OBJ_RD, which specifies
                  that the caller intends to read, but not modify, the object.
                  The other legal value is OM_OBJ_RW, which indicates that
                  the caller intends to read and/or write to the object.
State:    ---
Returns:  The OMobj_id of the specified AVS/Express subobject, or
          OMnull_obj if the named object could not be found.
*****************************************************************************/
static OMobj_id FindXPSubobj(OMobj_id parent, const char *name, int mode)
{
  OMobj_id objId = OMfind_subobj(parent, OMstr_to_name((char *)name), mode);
  
  if (!OMis_null_obj(objId))
     if (OMget_obj_val(objId, &objId) != OM_STAT_SUCCESS)
	objId = OMnull_obj;
  
  return objId;
}


/*****************************************************************************
*********************  XPVLIState Method Implementations  ********************
*****************************************************************************/


/*****************************************************************************
Method:  XPVLIState::Get()
Purpose: Gets the XPVLIState object associated with the VLIVolume object
         being rendered.  If this object doesn't yet have an XPVLIState
         object attached, a new one is created and attached.
Params:  obj   The GDobject being rendered.
State:   This method will create and attach a new XPVLIState object if none
         currently exists.
Returns: A pointer to the XPVLIState object associated with this GDobject.
*****************************************************************************/
XPVLIState *XPVLIState::Get(const GDobject *obj)
{
  XPVLIState *xpvliState = 0;
  
  if (OMget_name_ptr_val(obj->field_id, OMstr_to_name("state"),
			 (void **)&xpvliState, 0) != OM_STAT_SUCCESS)
     xpvliState = 0;
  
  // If we were unable to get the state from our local pointer, attempt to
  // create new one.
  //
  if (!xpvliState)
  {
    xpvliState = new XPVLIState(obj);
    
    if (xpvliState->IsValid())
    {
      OMset_name_ptr_val(obj->field_id, OMstr_to_name("state"),
			 xpvliState, 0);
    }
    else
    {
      delete xpvliState;
      xpvliState = 0;
    }
  }
  
  return xpvliState;
}


/*****************************************************************************
Method:  XPVLIState::XPVLIState()
Purpose: Constructor.  Initializes all attributes, obtains a new VLI context,
         and populates the configuration information in the VLIVolume object.
Params:  obj   A pointer to the GDobject being rendered.
*****************************************************************************/
XPVLIState::XPVLIState(const GDobject *obj) : Context(0), Volume(0),
                                              DataComponent(0), DataSeqNum(0),
                                              CropPredefinedSeqNum(-1),
                                              VoxFileNameSeqNum(0),
                                              XPVoxels(0),
                                              Nx(0), Ny(0), Nz(0),
                                              ModuleId(obj->field_id)
{
  DataSeqNum = GetNewDataSeqNum();
  
  // Open the VLI and check the status
  VLIStatus status;
  switch( status = VLIOpen() )
  {
    case kVLIErrNoHardware:
      ERRverror("VLIVolumeRender (XPVLIState constructor)", ERR_ERROR,
		"VolumePro hardware device not found.\n");
      return;
      
    case kVLIErrVersion:
      ERRverror("VLIVolumeRender (XPVLIState constructor)", ERR_ERROR,
		"Wrong version of VLI library.\n");
      return;
    
    case kVLIOK:
      break;
      
    default:
      ERRverror("VLIVolumeRender (XPVLIState constructor)", ERR_ERROR,
		"Unknown return value from VLIOpen.\n");
      return;
  }

  // TEMPORARY *******************************
  // There appears to be a bug in the VLI that prevents me from
  // setting a new VLILookupTable into the VLIContext after creation.
  //
  VLILookupTable *lut = VLILookupTable::Create(VLILookupTable::kSize256);
  
  Context = VLIContext::Create( lut );
  
  if (lut)             // TEMPORARY ******************************
  {
    lut->Release();   // TEMPORARY ******************************
  }
  
  if (!Context)
     ERRverror("VLIVolumeRender (XPVLIState constructor)", ERR_ERROR,
	       "Unable to create a new VLI context.\n");
  
  for (int cropTerm = 0; cropTerm < XP_NUM_CROP_TERMS; cropTerm++)
     CropTermSeqNums[cropTerm] = -1;
  
  // Now is a good time to get the configuration information from the VLI,
  // since this information won't change during the lifetime of this
  // instance.
  //
  SetConfigInfo(ModuleId);
  
  // Set the base plane image format to ARGB so that is meshs with Express
  if( Context )
  {
    // REMOVE THIS! Hack to get around supersampling bug using ARGB images 
    //    Context->SetBasePlaneFormat( kVLIPixelFormatARGB8 );
  }
   
  // REMOVE THIS! Hack to get around supersampling bug using ARGB images 
  prev_bix = 0;
  prev_biy = 0;
  argbImage = 0;
}


/*****************************************************************************
Method:  XPVLIState::~XPVLIState()
Purpose: Destructor.  Releases the VLI context, frees any memory held by
         this instance.
Params:  ---
*****************************************************************************/
XPVLIState::~XPVLIState()
{
  ClearVolume();

  if (Context)
  {
    Context->Release();
  }
  
  VLIClose();
}


/*****************************************************************************
Method:  XPVLIState::GetNewDataSeqNum()
Purpose: Gets the current sequence number for the "node_data" subobject of
         the "volume" (field) subobject of this module.  This can be used to
         determine if the volume data has changed since the last time this
         render method was invoked.
Params:  ---
State:   This method uses the ModuleId subobject, so the caller must ensure
         that this OMobj_id is set correctly before calling this method.
Returns: The current sequence number for the volume data, as described above.
*****************************************************************************/
int XPVLIState::GetNewDataSeqNum() const
{
  OMobj_id meshId = FindXPSubobj(ModuleId, "volume");
  OMobj_id dataId = FindXPSubobj(meshId, "node_data");
  OMobj_id nodeDataCompId;
  
  if (OMget_array_val(dataId, DataComponent, &nodeDataCompId,
		      OM_OBJ_RD) == OM_STAT_SUCCESS)
  {
    return OMget_obj_seq(nodeDataCompId, OMnull_obj, OM_SEQ_VAL);
  }
  
  return 0;
}


/*****************************************************************************
Method:  XPVLIState::CropTermsHaveChanged()
Purpose: Compares the sequence numbers of each crop term with those obtained
         during the previous invokation of this render method.  If any have
         changed, then we know that the value, and possibly the crop term
         object itself, has changed.
Params:  cropId   The OMobj_id of the crop object.  This must have been
                  obtained in read-write mode (i.e. OM_OBJ_RW).
State:   This method updates the internal sequence numbers, so it must only
         be called once per invokation of the render method (since the
         second call would always return false).
Returns: 1  if one or more of the crop terms has changed since the last
            invokation of this method.
         0  if none of the crop terms has changed.
*****************************************************************************/
int XPVLIState::CropTermsHaveChanged(OMobj_id cropId)
{
  int hasChanged = 0;
  int cropTerm;
  int termCount;
  int newSeqNum;
  OMobj_id termId;
  
  for (termCount = 0, cropTerm = XP_CROP_TERM0_INDEX;
       termCount < XP_NUM_CROP_TERMS;
       cropTerm++, termCount++)
  {
    if (OMget_array_subobj(cropId, cropTerm,
			   &termId, OM_OBJ_RD) == OM_STAT_SUCCESS)
    {
      newSeqNum = OMget_obj_seq(termId, OMnull_obj, OM_SEQ_VAL);
      
      if (newSeqNum != CropTermSeqNums[termCount])
      {
	CropTermSeqNums[termCount] = newSeqNum;
	hasChanged = 1;
      }
    }
    else
       hasChanged = 1;
  }
  
  newSeqNum = OMget_obj_seq(FindXPSubobj(cropId, "predefined"),
			    OMnull_obj, OM_SEQ_VAL);
  
  if (newSeqNum != CropPredefinedSeqNum)
  {
    CropPredefinedSeqNum = newSeqNum;
    hasChanged = 1;
  }
  
  return hasChanged;
}

// REMOVE THIS! Hack to get around supersampling bug using ARGB images 
/*****************************************************************************
Method:  XPVLIState::GetBasePlaneARGBImage()
Purpose: Allocates storage for the argb Image.
Params:  cur_bix  Current base plane width.
         cur_biy  Current base plane height.
State:   -
Returns: pointer to image array.
*****************************************************************************/
unsigned char * XPVLIState::GetBasePlaneARGBImage(int cur_bix, int cur_biy)
{
  // Check if the baseplane image size has changed
  if( (cur_bix != prev_bix) && (cur_biy != prev_biy) )
  {
    // Delete the previous image if there was one
    if( argbImage )
    {
      delete [] argbImage;
    }

    // Allocate a new array
    argbImage = new unsigned char [cur_bix * cur_biy * 4];    
  } 

  // Reset base plane sizes
  prev_bix = cur_bix;
  prev_bix = cur_bix;

  // Return the image pointer
  return argbImage;
}


/*****************************************************************************
Method:  XPVLIState::ClearVolume()
Purpose: Releases any VLIVolume object held by this instance, and sets the
         volume size (i.e. Nx, Ny, Nz) attributes to 0.  Also releases any
         AVS/Express volume data referenced by this instance.
Params:  ---
State:   This will force a re-render the next time the UpdateVolume method
         is called with valid volume data.
Returns: ---
*****************************************************************************/
void XPVLIState::ClearVolume()
{
  if (Volume)
  {
    Volume->Release();
    Volume = 0;
  }
  
  if (XPVoxels)
  {
    ARRfree(XPVoxels);
    XPVoxels = 0;
  }
  
  Nx = Ny = Nz = 0;
}


/*****************************************************************************
Method:  XPVLIState::UpdateVolume()
Purpose: This method is called each time the render method is invoked, so
         that any changes to the AVS/Express volume data are properly
         reflected in the VLIVolume.
         If the AVS/Expresss volume has not changed since the last time this
         method was invoked, then the VLIVolume object is not changed (though
         a new volume will be created if none currently exists).  If the
         AVS/Express volume has changed, then the current volume is released,
         and a new volume is created referring to the new volume data.
Params:  transform   The full transformation matrix from the GDstate.
State:   This method obtains the OMobj_id of the mesh from the field_id
         stored when it was constructed.
       * Note that this method updates the current data sequence number.
       * This method also updates the Nx, Ny, and Nz attributes.
Returns: 1  if successful,
         0  if:
              - the mesh information couldn't be obtained, because, for
                example, the user disconnected the mesh input
              - the VLIVolume couldn't be created for some reason
              - some other bad thing happened.
*****************************************************************************/
int XPVLIState::UpdateVolume(float transform[4][4])
{
  // If there is a volume file name (a .vox file) specified on the
  // "vox_file_name" input, attempt to load that file.  Otherwise, obtain
  // the volume data from the AVS/Express volume attached to the "volume"
  // input.
  //
  OMobj_id voxFileNameId = FindXPSubobj(ModuleId, "vox_file_name");
  
  if (OMvalid_obj(voxFileNameId, OMnull_obj, 0))
  {
    int newVoxFileSeqNum = OMget_obj_seq(voxFileNameId, OMnull_obj,
					 OM_SEQ_VAL);
    
    if (VoxFileNameSeqNum != newVoxFileSeqNum)
    {
      char *voxFileName = 0;
      
      ClearVolume();
      
      VoxFileNameSeqNum = newVoxFileSeqNum;
      
      if (OMget_str_val(voxFileNameId, &voxFileName, 0) == OM_STAT_SUCCESS)
      {
	if (voxFileName)
	{
	  Volume = VLIVolume::CreateFromFile(voxFileName);
	  
	  free(voxFileName);
	  
	  if (Volume)
	     return 1;
	  else
	  {
	    ERRverror("VLIVolumeRender", ERR_ERROR,
		      "VLIVolume::CreateFromFile() failed for "
		      "\"%s\".\n", voxFileName);
	    return 0;
	  }
	}
      }
      
      ERRverror("VLIVolumeRender", ERR_ERROR,
		"Unable to get the .vox file name.\n");
      return 0;
    }
    
    // If we made it to this point, then the user specified a vox file
    // name in the "vox_file_name" subobject, but that name hasn't changed
    // since the last time we were called.  Therefore, we can assume that
    // the VLIVolume object obtained when the name was initially specified
    // will still be valid.
    //
    return 1;
  }
  
  // There was no data file specified in the "vox_file_name" input, so we
  // attempt to obtain the volume data from the AVS/Express field attached
  // to the "volume" input.
  //
  OMobj_id meshId = FindXPSubobj(ModuleId, "volume");
  
  if (OMis_null_obj(meshId))
  {
    // No volume is attached to the "volume" input.  This is not an
    // error, so we return the success code.  On the other hand, we do
    // need to notify the caller that there is no data to render, so we
    // clear the volume information.
    //
    ClearVolume();
    return 1;
  }
  else if (GetXPInt(meshId, "nnodes") <= 0)
  {
    // In this case, there is a mesh connected to the input, but it is
    // invalid because it has no nodes.  This occurs frequently at
    // application load time, when an error message is annoying.
    // Therefore this specific case is ignored.
    //
    ClearVolume();
    return 1;
  }
  else
  {
    int newSeqNum = GetNewDataSeqNum();
    int dataComponent = GetXPInt(ModuleId, "data_component");
    
    // If the new and old sequence numbers don't match, the volume data
    // must have changed.  If this is the case, or if there is currently
    // no VLIVolume object, we need to create a new VLIVolume object.
    //
    if ((DataSeqNum != newSeqNum) ||
	(dataComponent != DataComponent) ||
	(!Volume))
    {
      int *dims = (int *)OMret_name_array_ptr(meshId,
					      OMstr_to_name("dims"),
					      OM_GET_ARRAY_RD, 0, 0);
      ClearVolume();
      
      if (dims)
      {
	int type;
	int size;
	
	SetDims(dims);
	
	ARRfree(dims);
	
	if (FLDget_node_data(meshId, dataComponent, &type,
			     (char **)&XPVoxels, &size,
			     OM_GET_ARRAY_RD) == OM_STAT_SUCCESS)
	{
	  if (XPVoxels)
	  {
	    switch (type)
	    {
	      case OM_TYPE_CHAR:
	      case OM_TYPE_BYTE:
	        Volume = VLIVolume::Create(kVLIVoxelFormatUINT8,
					   XPVoxels, Nx, Ny, Nz);
		break;
	      
	      case OM_TYPE_SHORT:
	        Volume = VLIVolume::Create(kVLIVoxelFormatUINT12L,
					   XPVoxels, Nx, Ny, Nz);
		break;
		
	      default:
	        ERRverror("VLIVolumeRender (XPVLIState::UpdateVolume)",
			  ERR_ERROR, "Bad volume type\n"
			  "Volume data must be byte, char, or "
			  "short.\n");
		break;
	    }
	  }
	}
      }
      
      DataSeqNum = newSeqNum;
    }
    
    if (Volume)
    {
      // Get the world coordinates array for the volume bounding box
      float *bboxPoints = (float *)OMret_name_array_ptr(meshId,
                                                       OMstr_to_name("points"),
                                                       OM_GET_ARRAY_RD, 0, 0);
      if( !bboxPoints )
	 return 0;       
      
      // Calculate the scale factor from i,j,k space to world space
      float xFactor = ((bboxPoints[3] - bboxPoints[0]) / (GetNx()-1));
      float yFactor = ((bboxPoints[4] - bboxPoints[1]) / (GetNy()-1));
      float zFactor = ((bboxPoints[5] - bboxPoints[2]) / (GetNz()-1));

      // Free the points array back to Express
      if( bboxPoints )
	 ARRfree( bboxPoints );
      
      // Copy the Express object matrix so we don't change the original
      float objectMat[4][4];
      MATmat4_copy( objectMat, transform );
      
      // Set the values in a new scale matrix
      float scaleMat[4][4];
      MATmat4_identity( scaleMat );
      MATmat4_scale( scaleMat, xFactor, yFactor, zFactor );
      
      // Concatenate the scale matrix onto the object matrix
      MATmat4_multiply( scaleMat, objectMat, objectMat );		 
      
      // Set the VLI object matrix
      VLIMatrix objmat( (float *)objectMat );
      Volume->SetModelMatrix( objmat );
      
      return 1;
    }
  }
  
  return 0;
}


/*****************************************************************************
Method:  XPVLIState::SetLUT()
Purpose: Sets the VLILookupTable in the VLIContext, and also adds it to the
         XPVLIState.  The current LUT, if any, is released.
Params:  size        The new VLILookupTable instance to set into this instance
                     and into the VLIContext.
         r, g, b, a  The desired new color and alpha values.
         newSeqNum   The new sequence number for the LUT.  This is used to
                     determine if the LUT has changed.
State:   The specified LUT is added to the VLIContext, and to this instance,
         replacing (in both cases) any existing LUT instance.
Returns: 1  if successful,
         0  otherwise.
*****************************************************************************/
int XPVLIState::SetLUT(int size, const VLIuint8 *r, const VLIuint8 *g,
                       const VLIuint8 *b, const VLIuint16 *a, int newSeqNum)
{
  int ok = 0;
  VLILookupTable::Size vliSize;
  
  // Make sure that the specified size is legal.  The VLI only allows
  // five specific power-of-2 sizes.
  //
  switch (size)
  {
    case 256:
      vliSize = VLILookupTable::kSize256;
      break;

    case 512:
      vliSize = VLILookupTable::kSize512;
      break;

    case 1024:
      vliSize = VLILookupTable::kSize1024;
      break;

    case 2048:
      vliSize = VLILookupTable::kSize2048;
      break;

    case 4096:
      vliSize = VLILookupTable::kSize4096;
      break;

    default:
      size = 0;
      break;
  }
  
  // Get the old lut (if any)
  VLILookupTable *lut = Context->GetLookupTable();
  
  if (size)   // if the size was valid
  {      
    int i;
    VLIuint8 colors[4096][3];
    
    for (i = 0; i < size; i++)
    {
      colors[i][0] = r[i];
      colors[i][1] = g[i];
      colors[i][2] = b[i];
    }
    
    
    // Create a new lut if one doesn't exist or the size changes
    if (lut) 
    {
      if( lut->GetSize() != vliSize)
      {
	lut->Release();
	lut = VLILookupTable::Create(vliSize);
	
	Context->SetLookupTable(lut);   
      }
    }
    else
    {
      lut = VLILookupTable::Create(vliSize);
      Context->SetLookupTable(lut);   
    }
    
    // Set up the alpha and color entries 
    if (lut)
    {
      if (lut->SetAlphaEntries(0, size, a) == kVLIOK)
      {
	if (lut->SetColorEntries(0, size, colors) == kVLIOK)
	{
	  LUTSeqNum = newSeqNum;
	  ok = 1;
	}
      }
    }
  }
  
  else  // if size was not valid create a default lut
  {
    if (lut) 
    {
      lut->Release();
    }
    lut = VLILookupTable::Create();
    
    if( Context->SetLookupTable(lut) == kVLIOK) 
    {
      ok = 1;
      LUTSeqNum = 0;
    }
  }
  
  return ok;
}


/*****************************************************************************
Method:  XPVLIState::LightsHaveChanged()
Purpose: Checks if the newly retrieved sequence number of XP GDlight_info 
         object equals the current LightSeqNum. If the lights have been 
         changed, sets LightSeqNum to the new vaule
Params:  newSeqNum   The new sequence number for the GDLight_info object. This
                     is used to determine if the lights have changed.
State:   If sequence numbers are not equal, the lights have been changed and
		 LightSeqNum to the new vaule
Returns: 1  if lights have changed
         0  otherwise.
*****************************************************************************/
int XPVLIState::LightsHaveChanged(int newSeqNum)
{
  int ok = (newSeqNum != LightSeqNum);
  
  if (ok) LightSeqNum = newSeqNum;
  
  return ok;
}


/*****************************************************************************
Method:  XPVLIState::SetCropObjs()
Purpose: 
         
Params:  
         
         
State:   
         
Returns: 1  if successful,
         0  otherwise.
*****************************************************************************/
int XPVLIState::SetCropObjs(OMobj_id cropId)
{
  static const int XP_NUM_SUBFLAGS = 4;  // 4 flags per crop term
  
  int ok = 0;
  OMobj_id termId;
  int termCount;
  int objIndex;
  int term;
  int flag;
  
  for (objIndex = 0; objIndex < XP_NUM_CROP_OBJS; objIndex++)
     CropObjs[objIndex] = OMnull_obj;
  
  // term0, term1, term2, combine, invert_result are items 6-10 in XP
  // crop_bounds object, cropId
  //
  for (objIndex = 0, termCount = 0, term = XP_CROP_TERM0_INDEX;
       termCount < XP_NUM_CROP_TERMS;
       termCount++, term++)
  {
    if (OMget_array_subobj(cropId, term,
			   &termId, OM_OBJ_RW) == OM_STAT_SUCCESS)
    {
      for (flag = 0; flag < XP_NUM_SUBFLAGS; flag++, objIndex++)
      {
	if (OMget_array_subobj(termId, flag, CropObjs + objIndex,
			       OM_OBJ_RW) != OM_STAT_SUCCESS)
	   break;
      }
    }
    else
       break;
    
    if (flag < XP_NUM_SUBFLAGS)   // if the inner loop failed
       break;
  }
  
  if (termCount >= XP_NUM_CROP_TERMS)  // if the above loop completed
  {
    int nextIndex = XP_CROP_TERM0_INDEX + XP_NUM_CROP_TERMS;
    
    if (OMget_array_subobj(cropId, nextIndex, CropObjs + objIndex,
			   OM_OBJ_RW) == OM_STAT_SUCCESS)
    {
      objIndex++;
      
      ok = (OMget_array_subobj(cropId, nextIndex + 1, CropObjs + objIndex,
			       OM_OBJ_RW) == OM_STAT_SUCCESS);
    }
  }
  
  return ok;
}


/*****************************************************************************
Method:  XPVLIState::SetCropFlags()
Purpose: 
         
Params:  
         
         
State:   
         
Returns: 1  if successful,
         0  otherwise.
*****************************************************************************/
int XPVLIState::SetCropFlags(int cropFlags)
{
  int mask;
  int bit;
  
  for (mask = 1, bit = 0; bit < XP_NUM_CROP_OBJS; bit++, mask <<= 1)
  {
    if (OMis_null_obj(CropObjs[bit]) ||
	(OMset_int_val(CropObjs[bit],
		       (mask & cropFlags) != 0) != OM_STAT_SUCCESS))
       break;
  }
  
  return bit >= XP_NUM_CROP_OBJS;
}


/*****************************************************************************
Method:  XPVLIState::GetCropFlags()
Purpose: 
         
Params:  
         
         
State:   
         
Returns: 1  if successful,
         0  otherwise.
*****************************************************************************/
int XPVLIState::GetCropFlags()
{
  int mask;
  int bit;
  int bitVal;
  int flags = 0;
  
  for (mask = 1, bit = 0; bit < XP_NUM_CROP_OBJS; bit++, mask <<= 1)
  {
    if (OMis_null_obj(CropObjs[bit]) ||
	(OMget_int_val(CropObjs[bit], &bitVal) != OM_STAT_SUCCESS))
       bitVal = 0;
    
    if (bitVal)
       flags |= mask;
  }
  
  return flags;
}


/*****************************************************************************
Function: VLIVolumeDestroy()
Purpose:  This method calls the XPVLIState destructor.
Params:   
State:    Called when the object is VLIVolume object is deleted.
Returns:  ---
*****************************************************************************/
int 
VLIVolumeDestroy(OMobj_id VLIVolume_id, OMevent_mask event_mask, int seq_num)
{
  XPVLIState *xpvliState;
  if (OMget_name_ptr_val(VLIVolume_id, OMstr_to_name("state"),
			 (void **)&xpvliState, 0) != OM_STAT_SUCCESS)
  {
    xpvliState = 0;
  }
  
  if( xpvliState )
  {
    delete xpvliState;   
  }
  
  return 1;
}