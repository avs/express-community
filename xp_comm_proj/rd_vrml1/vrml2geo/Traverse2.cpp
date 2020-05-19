//
// Traverse.C
// Local VRML traversal functions for AVS Module (read_VRML)
//
#include <QvElement.h>
#include <QvNodes.h>
#include <QvUnknownNode.h>
#include <QvState.h>
#include "gobject.h"
#include <avs/geom.h>

#include <travUtil.h>
#include <travLight.h>
#include <travCamera.h>
#include <travXForm.h>
#include <travTexture.h>

// #define	_DEBUG_ANNOUNCE_

// #define DEBUG

// prototype missing
extern "C" int GEOMget_font_number(char *, int, int);

//
//	Default traversal function
//

static int	indent = 0;

#if 000
static void
announce(const char *className)
{
#ifdef _DEBUG_ANNOUNCE_
    for (int i = 0; i < indent; i++)
	printf("\t");
    printf("Traversing a %s\n", className);
#endif
}
#define ANNOUNCE(className) announce(QV__QUOTE(className))
#endif

#ifdef _DEBUG_ANNOUNCE_
#  define ANNOUNCE(className) \
    { for (int i = 0; i < indent; i++) printf("\t"); \
      printf("Traversing a %s\n", QV__QUOTE(className)); }
#else
#  define ANNOUNCE(className)
#endif


#define DEFAULT_TRAVERSE(className)	\
void					\
className::traverse(QvState *, geom_objects *geometries,int sdiv)		\
{					\
    ANNOUNCE(className);		\
}


//
//	Group Nodes.
//
void
QvGroup::traverse(QvState *state, geom_objects *geometries, int sdiv)
{
    ANNOUNCE(QvGroup);
    indent++;

    //pushGName();

    for (int i = 0; i < getNumChildren(); i++)
	getChild(i)->traverse(state, geometries, sdiv);
    indent--;

    //popGName();
    //incGName();
}

void
QvLevelOfDetail::traverse(QvState *state, geom_objects *geometries,int sdiv)
{
    ANNOUNCE(QvLevelOfDetail);
    indent++;

    if (getNumChildren() > 0)
    	getChild(0)->traverse(state, geometries,sdiv);	// Traverse top-node only.

    indent--;
}

void
QvSeparator::traverse(QvState *state, geom_objects *geometries, int sdiv)
{
    ANNOUNCE(QvSeparator);
    state->push();
    indent++;

    //pushGName();

    for (int i = 0; i < getNumChildren(); i++)
	getChild(i)->traverse(state, geometries, sdiv);
    indent--;

    //popGName();
    //incGName();

    state->pop();
}

void
QvSwitch::traverse(QvState *state, geom_objects *geometries, int sdiv)
{
    ANNOUNCE(QvSwitch);
    indent++;

    //pushGName();


    int which = whichChild.value;

    if (which == QV_SWITCH_NONE)
	;
    else if (which == QV_SWITCH_ALL)
	for (int i = 0; i < getNumChildren(); i++)
	    getChild(i)->traverse(state, geometries, sdiv);
    else
	if ( which >= 0 && which < getNumChildren() )
	    getChild(which)->traverse(state, geometries, sdiv);

    indent--;

    //popGName();
    //incGName();
}


//
//	Property Nodes.
//

#define DO_PROPERTY(className, stackIndex)		\
void							\
className::traverse(QvState *state, geom_objects *geometries, int sdiv)	\
{							\
    ANNOUNCE(className);				\
    QvElement *elt = new QvElement;			\
    elt->data = this;					\
    state->addElement(QvState::stackIndex, elt);	\
}

#define DO_TYPED_PROPERTY(className, stackIndex, eltType)	\
void								\
className::traverse(QvState *state, geom_objects *geometries, int sdiv)				\
{								\
    ANNOUNCE(className);					\
    QvElement *elt = new QvElement;				\
    elt->data = this;						\
    elt->type = QvElement::eltType;				\
    state->addElement(QvState::stackIndex, elt);		\
}

DO_PROPERTY(QvCoordinate3,		Coordinate3Index)
DO_PROPERTY(QvMaterial,			MaterialIndex)
DO_PROPERTY(QvMaterialBinding,		MaterialBindingIndex)
DO_PROPERTY(QvNormal,			NormalIndex)
DO_PROPERTY(QvNormalBinding,		NormalBindingIndex)
DO_PROPERTY(QvShapeHints,		ShapeHintsIndex)
DO_PROPERTY(QvTextureCoordinate2,	TextureCoordinate2Index)
DO_PROPERTY(QvTexture2Transform,	Texture2TransformationIndex)

//DO_PROPERTY(QvTexture2,		Texture2Index)
void
QvTexture2::traverse(QvState *state, geom_objects *geometries, int sdiv)
{
    ANNOUNCE(QvTexture2);
    QvElement *elt = new QvElement;
    elt->data = this;
    state->addElement(QvState::Texture2Index, elt);
    if ( texSW )
	goTexture(state);
}


#define DO_LIGHT_PROPERTY(className, stackIndex, eltType)	\
void								\
className::traverse(QvState *state, geom_objects *geometries, int sdiv)	\
{								\
    ANNOUNCE(className);					\
    QvElement *elt = new QvElement;				\
    elt->data = this;						\
    elt->type = QvElement::eltType;				\
    state->addElement(QvState::stackIndex, elt);		\
    /*goLight(state);*/						\
}

DO_LIGHT_PROPERTY(QvDirectionalLight,	LightIndex, DirectionalLight)
DO_LIGHT_PROPERTY(QvPointLight,		LightIndex, PointLight)
DO_LIGHT_PROPERTY(QvSpotLight,		LightIndex, SpotLight)

#define DO_CAMERA_PROPERTY(className, stackIndex, eltType)	\
void								\
className::traverse(QvState *state, geom_objects *geometries, int sdiv)	\
{								\
    ANNOUNCE(className);					\
    QvElement *elt = new QvElement;				\
    elt->data = this;						\
    elt->type = QvElement::eltType;				\
    state->addElement(QvState::stackIndex, elt);		\
    /*goCamera(state);*/						\
}

DO_CAMERA_PROPERTY(QvOrthographicCamera,	CameraIndex, OrthographicCamera)
DO_CAMERA_PROPERTY(QvPerspectiveCamera,		CameraIndex, PerspectiveCamera)

DO_TYPED_PROPERTY(QvTransform,	     TransformationIndex, Transform)
DO_TYPED_PROPERTY(QvRotation,	     TransformationIndex, Rotation)
DO_TYPED_PROPERTY(QvMatrixTransform, TransformationIndex, MatrixTransform)
DO_TYPED_PROPERTY(QvTranslation,     TransformationIndex, Translation)
DO_TYPED_PROPERTY(QvScale,	     TransformationIndex, Scale)

void
QvTransformSeparator::traverse(QvState *state, geom_objects *geometries, int sdiv)
{
    ANNOUNCE(QvTransformSeparator);

    // We need to "push" just the transformation stack. We'll
    // accomplish this by just pushing a no-op transformation onto
    // that stack. When we "pop", we'll restore that stack to its
    // previous state.

    QvElement *markerElt = new QvElement;
    markerElt->data = this;
    markerElt->type = QvElement::NoOpTransform;
    state->addElement(QvState::TransformationIndex, markerElt);

    indent++;
    for (int i = 0; i < getNumChildren(); i++)
	getChild(i)->traverse(state, geometries, sdiv);
    indent--;

    // Now do the "pop"
    while (state->getTopElement(QvState::TransformationIndex) != markerElt)
	state->popElement(QvState::TransformationIndex);

}


//
//	Shape Nodes.
//

// Cone 
void
QvCone::traverse(QvState *state, geom_objects *geometries, int sdiv)
{
//#define	DIVISION_CONE	12
//#define NV      	DIVISION_CONE*2 +2 +2
//#define NI      	DIVISION_CONE*4 *2 +1

    register int	i, j;
    FLOAT3	*verts, temp;
    float	(*uvs)[2];
    int		*indice;
    GEOMobj	*obj;
    char	textureFile[64];
    char	wrap[2];

	int DIVISION_CONE = sdiv;
	int NV = sdiv*2 +2 +2;
	int NI = sdiv*4 *2 +1;

	verts = new FLOAT3 [NV];
	indice = new int [NI];
	uvs	= new float [NV][2];

    ANNOUNCE(QvCone);

    InitGMatrix();
    goTrans(state);
    Matrix	*mat = getLast(gmatrix);

    if ( texSW && travTexture(state, textureFile, wrap) ) {

	verts[0][0] = 0.0;
	verts[0][1] = -height.value * 0.5;
	verts[0][2] = 0.0;

	verts[NV-1][0] = 0.0;
	verts[NV-1][1] = height.value * 0.5;
	verts[NV-1][2] = 0.0;

	printf(" sin = %lf\n",fsin(1.0));
	for ( i = 1; i < DIVISION_CONE +2; i++ ) {
	    temp[0] = bottomRadius.value * fsin(2*M_PI*(i-1)/DIVISION_CONE);
	    temp[1] = -height.value * 0.5;
	    temp[2] = -bottomRadius.value * fcos(2*M_PI*(i-1)/DIVISION_CONE);

	    verts[i][0] = temp[0];
	    verts[i][1] = temp[1];
	    verts[i][2] = temp[2];

	    verts[i+DIVISION_CONE+1][0] = temp[0];
	    verts[i+DIVISION_CONE+1][1] = temp[1];
	    verts[i+DIVISION_CONE+1][2] = temp[2];
	}

	uvs[0][0] = 0.5; uvs[0][1] = 0.5;
	uvs[NV-1][0] = 0.5; uvs[NV-1][1] = 0.0;
	for ( i=1; i<DIVISION_CONE +2; i++) {
	    uvs[i][0] = 0.5 * fsin(2*M_PI*(i-1)/DIVISION_CONE) + 0.5;
	    uvs[i][1] = 0.5 * fcos(2*M_PI*(i-1)/DIVISION_CONE) + 0.5;

	    uvs[i+DIVISION_CONE+1][0] = (float)(i-1)/DIVISION_CONE;
	    uvs[i+DIVISION_CONE+1][1] = 1.0;
	}

	obj = GEOMcreate_obj(GEOM_POLYHEDRON, NULL);
	GEOMadd_vertices(obj, (float *)verts, NV, GEOM_COPY_DATA);

	/* Bottom face */
	/* set Indices */
	for ( j = 0; j < DIVISION_CONE; j++ ) {
	    indice[j*4] = 3;
	    indice[j*4 +1] = 1;
	    indice[j*4 +2] = j+2;
	    indice[j*4 +3] = j+3;
	}

	/* Side face */
	/* set Indices */
	for ( j = DIVISION_CONE; j < DIVISION_CONE*2; j++ ) {
	    indice[j*4] = 3;
	    indice[j*4 +1] = NV;
	    indice[j*4 +2] = j+4;
	    indice[j*4 +3] = j+3;
	}
	indice[NI-1] = 0;

	GEOMadd_polygons(obj, indice, 0, GEOM_COPY_DATA);
	GEOMgen_normals(obj, 0);

	GEOMadd_uvs(obj, (float *)uvs, NV, GEOM_COPY_DATA);
	//GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
//	GEOMdestroy_obj(obj);

	//GEOMedit_texture(*editList, geomName, textureFile);
	//GEOMedit_texture_options(*editList, geomName, GEOM_TEXTURE_FILTER, 1);

    } else {

	// Set vertex //
	verts[0][0] = 0.0;
	verts[0][1] = -height.value * 0.5;
	verts[0][2] = 0.0;
	for ( i = 1; i < DIVISION_CONE +1; i++ ) {
	    verts[i][0] = bottomRadius.value * fcos(2*M_PI*(i-1)/DIVISION_CONE);
	    verts[i][1] = -height.value * 0.5;
	    verts[i][2] = bottomRadius.value * fsin(2*M_PI*(i-1)/DIVISION_CONE);
	}
	verts[DIVISION_CONE+1][0] = 0.0;
	verts[DIVISION_CONE+1][1] = height.value * 0.5;
	verts[DIVISION_CONE+1][2] = 0.0;


	// Bottom face //
	obj = GEOMcreate_obj(GEOM_POLYHEDRON, NULL);
	GEOMadd_vertices(obj, (float *)verts, DIVISION_CONE+2, GEOM_COPY_DATA);

	// set Indices
	for ( j = 0; j < DIVISION_CONE; j++ ) {
	    indice[j*4] = 3;	// Triangle
	    indice[j*4 +1] = 1;
	    indice[j*4 +2] = j+2;
	    if ( j == DIVISION_CONE-1 )
		indice[j*4 +3] = 2;
	    else
		indice[j*4 +3] = j+3;
	}
	indice[DIVISION_CONE *4] = 0; // Terminater
	GEOMadd_polygons(obj, indice, 0, GEOM_COPY_DATA);
	GEOMgen_normals(obj, GEOM_FACET_NORMALS);

	//GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
//	GEOMdestroy_obj(obj);

	// Side face //
	obj = GEOMcreate_obj(GEOM_POLYHEDRON, NULL);
	GEOMadd_vertices(obj, (float *)verts, DIVISION_CONE+2, GEOM_COPY_DATA);

	// set Indices
	for ( j = 0; j < DIVISION_CONE; j++ ) {
	    indice[j*4] = 3;	// Triangle
	    indice[j*4 +1] = DIVISION_CONE+2;
	    if ( j == DIVISION_CONE-1 )
		indice[j*4 +2] = 2;
	    else
		indice[j*4 +2] = j+3;
	    indice[j*4 +3] = j+2;
	}
	indice[DIVISION_CONE *4] = 0; // Terminater
	GEOMadd_polygons(obj, indice, 0, GEOM_COPY_DATA);

	GEOMgen_normals(obj, 0);
	//GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
//	GEOMdestroy_obj(obj);

    }

    //simpleMaterial(state);
    //GEOMedit_set_matrix(*editList, geomName, (float *)mat->value);

    //setGeomParent();
    //incGName();
    DelGMatrix();

	delete verts;
	delete indice;
	delete uvs;
	
//#undef	DIVISION_CONE
//#undef NV
//#undef NI
}
//

// Cube
void
QvCube::traverse(QvState *state, geom_objects *geometries, int sdiv)
{
    FLOAT3	verts[24];

    static FLOAT3	v1[8] = {	// No texture
	{-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0},
	{-1.0,  1.0, 1.0}, {1.0,  1.0, 1.0}, {1.0,  1.0, -1.0}, {-1.0,  1.0, -1.0}
    };
    static FLOAT3	v2[24] = {	// has texture
        {-1.0, -1.0,  1.0}, { 1.0, -1.0,  1.0}, { 1.0,  1.0,  1.0}, {-1.0,  1.0,  1.0},
        { 1.0, -1.0,  1.0}, { 1.0, -1.0, -1.0}, { 1.0,  1.0, -1.0}, { 1.0,  1.0,  1.0},
        { 1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}, {-1.0,  1.0, -1.0}, { 1.0,  1.0, -1.0},
        {-1.0, -1.0, -1.0}, {-1.0, -1.0,  1.0}, {-1.0,  1.0,  1.0}, {-1.0,  1.0, -1.0},
        {-1.0,  1.0,  1.0}, { 1.0,  1.0,  1.0}, { 1.0,  1.0, -1.0}, {-1.0,  1.0, -1.0},
        {-1.0, -1.0, -1.0}, { 1.0, -1.0, -1.0}, { 1.0, -1.0,  1.0}, {-1.0, -1.0,  1.0}
    };

    static int	ind1[] = {	// No texture
	4,  1, 2, 6, 5,
	4,  3, 4, 8, 7,
	4,  1, 5, 8, 4,
	4,  2, 3, 7, 6,
	4,  2, 1, 4, 3,
	4,  5, 6, 7, 8,  0
    };
    static int	ind2[] = {	// has texture
        4,   1,  2,  3,  4,
        4,   5,  6,  7,  8,
        4,   9, 10, 11, 12,
        4,  13, 14, 15, 16,
        4,  17, 18, 19, 20,
        4,  21, 22, 23, 24,  0
    };

    static float	uvs[][2] = {	// texture map
        {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0},
        {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0},
        {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0},
        {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0},
        {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0},
        {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0}
    };


    GEOMobj	*obj;
    char	textureFile[64];
    char	wrap[2];
    register int	i;


    ANNOUNCE(QvCube);

    InitGMatrix();
    goTrans(state);
    Matrix	*mat = getLast(gmatrix);

    obj = GEOMcreate_obj(GEOM_POLYHEDRON, NULL);

    if ( texSW && travTexture(state, textureFile, wrap) ) {

	for ( i = 0; i < 24; i++ ) {
	    verts[i][0] = v2[i][0] * width.value * 0.5;
	    verts[i][1] = v2[i][1] * height.value * 0.5;
	    verts[i][2] = v2[i][2] * depth.value * 0.5; 
	}

	GEOMadd_vertices(obj, (float *)verts, 24, GEOM_COPY_DATA);
	GEOMadd_polygons(obj, ind2, 0, GEOM_COPY_DATA);
	GEOMgen_normals(obj, GEOM_FACET_NORMALS);

	GEOMadd_uvs(obj, (float *)uvs, 24, GEOM_COPY_DATA);
	//GEOMedit_geometry(*editList, geomName, obj);
#ifdef DEBUG
		fprintf(stderr, "Adding object\n");fflush(stderr);
#endif
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);

	//GEOMedit_texture(*editList, geomName, textureFile);
	//GEOMedit_texture_options(*editList, geomName, GEOM_TEXTURE_FILTER, 1);

    } else {

	for ( i = 0; i < 8; i++ ) {
	    verts[i][0] = v1[i][0] * width.value * 0.5;
	    verts[i][1] = v1[i][1] * height.value * 0.5;
	    verts[i][2] = v1[i][2] * depth.value * 0.5; 
	}

	GEOMadd_vertices(obj, (float *)verts, 8, GEOM_COPY_DATA);
	GEOMadd_polygons(obj, ind1, 0, GEOM_COPY_DATA);
	GEOMgen_normals(obj, GEOM_FACET_NORMALS);

	//GEOMedit_geometry(*editList, geomName, obj);
#ifdef DEBUG
		fprintf(stderr, "Adding object2\n");
		fflush(stderr);
		fprintf(stderr,"readVRML: geometries = %lp\n",geometries);
		fflush(stderr);
#endif
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
    }

//    GEOMdestroy_obj(obj);

    //simpleMaterial(state);
    //GEOMedit_set_matrix(*editList, geomName, (float *)mat->value);

    //setGeomParent();
    //incGName();
    DelGMatrix();
}
//

// Cylinder
void
QvCylinder::traverse(QvState *state, geom_objects *geometries,int sdiv)
{
//#define	DIVISION_CYL	12
//#define NV		(DIVISION_CYL+1)*4 +2
//#define NI		DIVISION_CYL*13 +1

    FLOAT3	*verts, temp;
    int		*indice;
    float   (*uvs)[2];
    GEOMobj	*obj;
    char	textureFile[64];
    char	wrap[2];
    register int	i, j;

	int NV = (sdiv+1)*4 +2;
	int NI = sdiv*13 +1;
	int DIVISION_CYL = sdiv;
	
	verts = new FLOAT3 [NV];
	indice = new int [NI];
	uvs	= new float [NV][2];

    ANNOUNCE(QvCylinder);

    InitGMatrix();
    goTrans(state);
    Matrix	*mat = getLast(gmatrix);


    if ( texSW && travTexture(state, textureFile, wrap) ) {

	verts[0][0] = 0.0;
	verts[0][1] = -height.value * 0.5;
	verts[0][2] = 0.0;

	verts[NV-1][0] = 0.0;
	verts[NV-1][1] = height.value * 0.5;
	verts[NV-1][2] = 0.0;

	for ( i = 1; i < DIVISION_CYL +2; i++ ) {
	    temp[0] = radius.value * fsin(2*M_PI*(i-1)/DIVISION_CYL);
	    temp[1] = height.value * 0.5;
	    temp[2] = -radius.value * fcos(2*M_PI*(i-1)/DIVISION_CYL);

	    verts[i][0] = temp[0];
	    verts[i][1] = -temp[1];
	    verts[i][2] = temp[2];

	    verts[i+DIVISION_CYL+1][0] = temp[0];
	    verts[i+DIVISION_CYL+1][1] = -temp[1];
	    verts[i+DIVISION_CYL+1][2] = temp[2];

	    verts[i+(DIVISION_CYL+1)*2][0] = temp[0];
	    verts[i+(DIVISION_CYL+1)*2][1] = temp[1];
	    verts[i+(DIVISION_CYL+1)*2][2] = temp[2];

	    verts[i+(DIVISION_CYL+1)*3][0] = temp[0];
	    verts[i+(DIVISION_CYL+1)*3][1] = temp[1];
	    verts[i+(DIVISION_CYL+1)*3][2] = temp[2];
	}

	uvs[0][0] = uvs[0][1] = 0.5;
	uvs[NV-1][0] = uvs[NV-1][1] = 0.5;
	for ( i=1; i<DIVISION_CYL +2; i++) {
	    uvs[i][0] = 0.5 * fsin(2*M_PI*(i-1)/DIVISION_CYL) + 0.5;
	    uvs[i][1] = 0.5 * fcos(2*M_PI*(i-1)/DIVISION_CYL) + 0.5;

	    uvs[i+DIVISION_CYL+1][0] = (float)(i-1)/DIVISION_CYL;
	    uvs[i+DIVISION_CYL+1][1] = 1.0;

	    uvs[i+(DIVISION_CYL+1)*2][0] = (float)(i-1)/DIVISION_CYL;
	    uvs[i+(DIVISION_CYL+1)*2][1] = 0.0;

	    uvs[i+(DIVISION_CYL+1)*3][0] = 0.5 * fsin(2*M_PI*(i-1)/DIVISION_CYL) + 0.5;
	    uvs[i+(DIVISION_CYL+1)*3][1] =-0.5 * fcos(2*M_PI*(i-1)/DIVISION_CYL) + 0.5;
	}


	/* set Indices */
	for ( j = 0; j < DIVISION_CYL; j++ ) {
	    /* Bottom face */
	    indice[j*4] = 3;
	    indice[j*4 +1] = 1;
	    indice[j*4 +2] = j+2;
	    indice[j*4 +3] = j+3;

	    /* Side face */
	    indice[(DIVISION_CYL*4)+j*5] = 4;
	    indice[(DIVISION_CYL*4)+j*5 +1] = j+1+(DIVISION_CYL+2);
	    indice[(DIVISION_CYL*4)+j*5 +2] = j+DIVISION_CYL+2+(DIVISION_CYL+2);
	    indice[(DIVISION_CYL*4)+j*5 +3] = j+DIVISION_CYL+3+(DIVISION_CYL+2);
	    indice[(DIVISION_CYL*4)+j*5 +4] = j+2+(DIVISION_CYL+2);

	    /* Top face */
	    indice[(DIVISION_CYL*9)+j*4] = 3;
	    indice[(DIVISION_CYL*9)+j*4 +1] = NV;
	    indice[(DIVISION_CYL*9)+j*4 +2] = j+DIVISION_CYL+3+(DIVISION_CYL+1)*2+1;
	    indice[(DIVISION_CYL*9)+j*4 +3] = j+DIVISION_CYL+2+(DIVISION_CYL+1)*2+1;
	}

	indice[NI-1] = 0;

	obj = GEOMcreate_obj(GEOM_POLYHEDRON, NULL);
	GEOMadd_vertices(obj, (float *)verts, NV, GEOM_COPY_DATA);

	GEOMadd_polygons(obj, indice, 0, GEOM_COPY_DATA);
	GEOMgen_normals(obj, 0);

	GEOMadd_uvs(obj, (float *)uvs, NV, GEOM_COPY_DATA);
	//GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
//	GEOMdestroy_obj(obj);

	//GEOMedit_texture(*editList, geomName, textureFile);
	//GEOMedit_texture_options(*editList, geomName, GEOM_TEXTURE_FILTER, 1);

    } else {

	// Set vertex //
	verts[0][0] = 0.0;
	verts[0][1] = -height.value * 0.5;
	verts[0][2] = 0.0;
	for ( i = 1; i < DIVISION_CYL +1; i++ ) {
	    verts[i][0] = radius.value * fcos(2*M_PI*(i-1)/DIVISION_CYL);
	    verts[i][1] = -height.value * 0.5;
	    verts[i][2] = radius.value * fsin(2*M_PI*(i-1)/DIVISION_CYL);
	}
	for ( i = DIVISION_CYL +1; i < DIVISION_CYL*2 +1; i++ ) {
	    verts[i][0] = radius.value * fcos(2*M_PI*(i-1)/DIVISION_CYL);
	    verts[i][1] = height.value * 0.5;
	    verts[i][2] = radius.value * fsin(2*M_PI*(i-1)/DIVISION_CYL);
	}
	verts[DIVISION_CYL*2+1][0] = 0.0;
	verts[DIVISION_CYL*2+1][1] = height.value * 0.5;
	verts[DIVISION_CYL*2+1][2] = 0.0;

	// Bottom face //
	obj = GEOMcreate_obj(GEOM_POLYHEDRON, NULL);
	GEOMadd_vertices(obj, (float *)verts, DIVISION_CYL*2+2, GEOM_COPY_DATA);

	// set Indices
	for ( j = 0; j < DIVISION_CYL; j++ ) {
	    indice[j*4] = 3;	// Triangle
	    indice[j*4 +1] = 1;
	    indice[j*4 +2] = j+2;
	    if ( j == DIVISION_CYL-1 )
		indice[j*4 +3] = 2;
	    else
		indice[j*4 +3] = j+3;
	}
	indice[DIVISION_CYL*4] = 0; // Terminater
	GEOMadd_polygons(obj, indice, 0, GEOM_COPY_DATA);
	GEOMgen_normals(obj, GEOM_FACET_NORMALS);
	//GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);

//	GEOMdestroy_obj(obj);

	// Side face //
	obj = GEOMcreate_obj(GEOM_POLYHEDRON, NULL);
	GEOMadd_vertices(obj, (float *)verts, DIVISION_CYL*2+2, GEOM_COPY_DATA);

	// set Indices //
	for ( j = 0; j < DIVISION_CYL; j++ ) {
	    indice[j*5] = 4;	// Square
	    indice[j*5 +1] = j+2;
	    indice[j*5 +2] = j+DIVISION_CYL+2;
	    if ( j == DIVISION_CYL-1 ) {
		indice[j*5 +3] = DIVISION_CYL+2;
		indice[j*5 +4] = 2;
	    } else {
		indice[j*5 +3] = j+DIVISION_CYL+3;
		indice[j*5 +4] = j+3;
	    }
	}
	indice[DIVISION_CYL*5] = 0; // Terminater
	GEOMadd_polygons(obj, indice, 0, GEOM_COPY_DATA);
	GEOMgen_normals(obj, 0);
	//GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
//	GEOMdestroy_obj(obj);

	// Top face //
	obj = GEOMcreate_obj(GEOM_POLYHEDRON, NULL);
	GEOMadd_vertices(obj, (float *)verts, DIVISION_CYL*2+2, GEOM_COPY_DATA);

	// set Indices //
	for ( j = 0; j < DIVISION_CYL; j++ ) {
	    indice[j*4] = 3;	// Triangle
	    indice[j*4 +1] = DIVISION_CYL*2+2;
	    if ( j == DIVISION_CYL-1 )
		indice[j*4 +2] = DIVISION_CYL+2;
	    else
		indice[j*4 +2] = j+DIVISION_CYL+3;
	    indice[j*4 +3] = j+DIVISION_CYL+2;
	}
	indice[DIVISION_CYL*4] = 0; // Terminater
	GEOMadd_polygons(obj, indice, 0, GEOM_COPY_DATA);
	GEOMgen_normals(obj, GEOM_FACET_NORMALS);
	//GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
//	GEOMdestroy_obj(obj);
    }

    //simpleMaterial(state);
    //GEOMedit_set_matrix(*editList, geomName, (float *)mat->value);

    //setGeomParent();
    //incGName();
    DelGMatrix();

	delete verts;
	delete indice;
	delete uvs;

//#undef	DIVISION_CYL
//#undef NV
//#undef NI
}
//


// Indexed Face Set
void
QvIndexedFaceSet::traverse(QvState *state, geom_objects *geometries,int sdiv)
{
    register int	i, j, k;
    int			nvs, npg;
    int			*indice;
    GEOMobj		*obj;
    char        	nflag, mflag, tflag;
    float       	*nvp;
    float		*uvs;
    QvElement		*elem;
    QvElement		*nelem, *nbelem;
    QvElement		*melem;
    QvElement		*telem;
    QvNormal    	*normal;
    QvMaterial		*material;
    QvTextureCoordinate2	*textureCoord;
    char       		wrap[2];
    unsigned int	*color;
    char		textureFile[64];


    ANNOUNCE(QvIndexedFaceSet);

    InitGMatrix();
    goTrans(state);
    Matrix *mat = getLast(gmatrix);

    elem = state->getTopElement(QvState::Coordinate3Index);
    if ( elem == (QvElement *)NULL )
	return;

    // Load Coordinates
    //	coords->point.num	: Number of Points
    //	coords->point.values	: Coordinates of Points
    QvCoordinate3 *coords = (QvCoordinate3 *)elem->data;
    float *pos = coords->point.values;

    // Load Normal
    nflag = 0;
    nbelem = state->getTopElement(QvState::NormalBindingIndex);
    if ( nbelem == (QvElement *)NULL ) {
	nflag = 0;
    } else {
	// Traverse NormalBinding
        QvNormalBinding *nbind = (QvNormalBinding *)nbelem->data;

	switch ( nbind->value.value ) {
	  case QvNormalBinding::NONE:
	  case QvNormalBinding::OVERALL:
	  case QvNormalBinding::PER_PART:
	  case QvNormalBinding::PER_PART_INDEXED:
	  case QvNormalBinding::PER_FACE:
	  case QvNormalBinding::PER_FACE_INDEXED:
	    break;
	  case QvNormalBinding::PER_VERTEX:
	    nflag = 1;
	    break;
	  case QvNormalBinding::PER_VERTEX_INDEXED:
	    nflag = 2;
	    break;
	  case QvNormalBinding::DEFAULT:
	  default:
	    nflag = (char)-1;
	}
    }
    // Check Normals
    if ( nflag > 0 ) {
	nelem = state->getTopElement(QvState::NormalIndex);
	if ( nelem == (QvElement *)NULL )
	    nflag = 0;
	else {
	    normal = (QvNormal *)nelem->data;
	    if ( nflag == 1 && normal->vector.num != coords->point.num ) {
		nflag = 0;
	    }
	    if ( nflag == 2 && normalIndex.num <= 1 ) {
		nflag = 0;
	    }
	    if ( nflag == (char)-1 ) {
		if ( normalIndex.num > 1 )
		    nflag = 2;
		else if ( normal->vector.num == coords->point.num )
		    nflag = 1;
		else
		    nflag = 0;
	    }
	}
    }

    // Load Material
    //	: only use Diffuse Color
    //
    mflag = 0;
    QvElement *mbelem = state->getTopElement(QvState::MaterialBindingIndex);
    if ( mbelem == (QvElement *)NULL ) {
	mflag = 0;
    } else {
	QvMaterialBinding *mbind = (QvMaterialBinding *)mbelem->data;

	switch ( mbind->value.value ) {
	  case QvMaterialBinding::PER_FACE:
	    mflag = 3;
	    break;
	  case QvMaterialBinding::PER_FACE_INDEXED:
	    mflag = 4;
	    break;
	  case QvMaterialBinding::PER_VERTEX:
	    mflag = 1;
	    break;
	  case QvMaterialBinding::PER_VERTEX_INDEXED:
	    mflag = 2;
	  default:
	    break;
	}
    }


    // Create Polygons
    indice = new int [coordIndex.num +2];

    nvs = 0;
    npg = 0;
    k = 0;
    for ( i = 0; i < coordIndex.num; i++ ) {
	if( coordIndex.values[i] != -1 ) {
	    if((coordIndex.values[i] >= 0) &&
	       (coordIndex.values[i] < coords->point.num)) {
		nvs++;
	    } else {
		fprintf(stderr, "Error: IndexedFaceSet: Out of Index: %d\n",
			coordIndex.values[i]);
	    }
	} else {
	    indice[k] = nvs;
	    for ( j = k; j < k+nvs; j++ ) {
		indice[j+1] = coordIndex.values[j] + 1;
	    }
	    nvs = 0;
	    npg++;
	    k = i + 1;
	}
    }
    if ( coordIndex.values[coordIndex.num -1] != -1 ) {
	indice[k] = nvs;
	for ( j = k; j < k+nvs; j++ )
	    indice[j+1] = coordIndex.values[j] + 1;
	npg++;
	indice[coordIndex.num +1] = 0;	// Terminator
    } else {
	indice[coordIndex.num] = 0;	// Terminator
    }

    // Create Normals
    if ( nflag == 1 ) {
	// Normal: PER_VERTEX
	nvp = normal->vector.values;
    } else if ( nflag == 2 ) {
	// Normal: PER_VERTEX_INDEXED
	nvp = new float[coords->point.num *3];
	for ( i = 0; i < normalIndex.num; i++ ) {
	    if( normalIndex.values[i] != -1 ) {
		nvp[coordIndex.values[i]*3  ] = normal->vector.values[normalIndex.values[i]*3];
		nvp[coordIndex.values[i]*3+1] = normal->vector.values[normalIndex.values[i]*3+1];
		nvp[coordIndex.values[i]*3+2] = normal->vector.values[normalIndex.values[i]*3+2];
	    }
	}
    } else
	nvp = (float *)NULL;

    // Create Colors
    if ( mflag > 0 ) {
	melem = state->getTopElement(QvState::MaterialIndex);
	if ( melem == (QvElement *)NULL )
	    mflag = 0;
	else {
	    material = (QvMaterial *)melem->data;
	    if ( material->diffuseColor.num <= 1 ) {
		mflag = 0;
	    } else
	    if ( mflag == 2 && materialIndex.num <= 1 ) {
		mflag = 0;
	    } else
	    if ( mflag == 4 && materialIndex.num <= 1 ) {
		mflag = 0;
	    }
	}
    }
    if ( mflag > 0 ) {
	PACKCOLOR	cwork;
	color = new unsigned int[coords->point.num];
	switch ( mflag ) {
	  case 1:	/* PER_VERTEX	*/
	    for ( i = 0; i < coords->point.num; i++ ) {
		cwork.ccol[0] = (unsigned char)0;
		cwork.ccol[1] = (unsigned char)
		    (NORM(material->diffuseColor.values[(i%material->diffuseColor.num)*3])*255);
		cwork.ccol[2] = (unsigned char)
		    (NORM(material->diffuseColor.values[(i%material->diffuseColor.num)*3+1])*255);
		cwork.ccol[3] = (unsigned char)
		    (NORM(material->diffuseColor.values[(i%material->diffuseColor.num)*3+2])*255);
		color[i] = cwork.lcol;
	    }
	    break;
	  case 2:	/* PER_VERTEX_INDEXED	*/
	    for ( i = 0; i < materialIndex.num; i++ )
		if ( materialIndex.values[i] != -1 ) {
		    cwork.ccol[0] = (unsigned char)0;
		    cwork.ccol[1] = (unsigned char)
			(NORM(material->diffuseColor.values[(materialIndex.values[i]%material->diffuseColor.num)*3])*255);
		    cwork.ccol[2] = (unsigned char)
			(NORM(material->diffuseColor.values[(materialIndex.values[i]%material->diffuseColor.num)*3+1])*255);
		    cwork.ccol[3] = (unsigned char)
			(NORM(material->diffuseColor.values[(materialIndex.values[i]%material->diffuseColor.num)*3+2])*255);
		    color[coordIndex.values[i]] = cwork.lcol;
		}
	    break;
	  case 3:	/* PER_FACE	*/
	    k = 0;
	    for ( i = 0; i < coordIndex.num; i++ )
		if ( coordIndex.values[i] != -1 ) {
		    cwork.ccol[0] = (unsigned char)0;
		    cwork.ccol[1] = (unsigned char)
			(NORM(material->diffuseColor.values[(k%material->diffuseColor.num)*3])*255);
		    cwork.ccol[2] = (unsigned char)
			(NORM(material->diffuseColor.values[(k%material->diffuseColor.num)*3+1])*255);
		    cwork.ccol[3] = (unsigned char)
			(NORM(material->diffuseColor.values[(k%material->diffuseColor.num)*3+2])*255);
		    color[coordIndex.values[i]] = cwork.lcol;
		} else {
		    k++;
		}
	    break;
	  case 4:	/* PER_FACE_INDEXED	*/
	    k = 0;
	    for ( i = 0; i < coordIndex.num; i++ )
		if ( coordIndex.values[i] != -1 ) {
		    cwork.ccol[0] = (unsigned char)0;
		    cwork.ccol[1] = (unsigned char)
			(NORM(material->diffuseColor.values[(materialIndex.values[k]%material->diffuseColor.num)*3])*255);
		    cwork.ccol[2] = (unsigned char)
			(NORM(material->diffuseColor.values[(materialIndex.values[k]%material->diffuseColor.num)*3+1])*255);
		    cwork.ccol[3] = (unsigned char)
			(NORM(material->diffuseColor.values[(materialIndex.values[k]%material->diffuseColor.num)*3+2])*255);
		    color[coordIndex.values[i]] = cwork.lcol;
		} else {
		    k++;
		}
	    break;
	}
    } else
	color = (unsigned int *)NULL;

    // Check Texture Mapping condition
    tflag = 0;
    if ( texSW && travTexture(state, textureFile, wrap) ) {
	if ( textureCoordIndex.num <= 1 )
	    tflag = 1;
	else {
	    telem = state->getTopElement(QvState::TextureCoordinate2Index);
	    if ( telem == (QvElement *)NULL )
		tflag = 1;
	    else {
		textureCoord = (QvTextureCoordinate2 *)telem->data;
		tflag = 2;
	    }
	}
    }

    // Create UV Map
    switch ( tflag ) {
      case 1:
	{
	    // Default Mapping
	    FLOAT3	bbox[2], bboxLen;
	    int		axes[2];
	    genBBOX((FLOAT3 *)pos, coords->point.num, (FLOAT3 *)bbox);
	    bboxLen[0] = bbox[1][0] - bbox[0][0];
	    bboxLen[1] = bbox[1][1] - bbox[0][1];
	    bboxLen[2] = bbox[1][2] - bbox[0][2];
	    axes[0] = MAX3I(bboxLen);
	    axes[1] = MED3I(bboxLen);
	    if ( axes[0] == axes[1] ) { axes[0] = 0; axes[1] = 1; }

	    uvs = new float[coords->point.num *2];

	    for ( i = 0; i < coords->point.num; i++ ) {
		uvs[i*2  ] = (pos[i*3+axes[0]] - bbox[0][axes[0]])/bboxLen[axes[0]];
		uvs[i*2+1] = (pos[i*3+axes[1]] - bbox[0][axes[1]])/bboxLen[axes[0]];
	    }

	    travTexture2Transform(state, uvs, coords->point.num, wrap);

	    break;
	}
      case 2:
	{
	    // Use specified index and UV coordinates
	    uvs = new float[coords->point.num *2];

	    for ( i = 0; i < textureCoordIndex.num; i++ ) {
		if ( textureCoordIndex.values[i] != -1 ) {
		    uvs[coordIndex.values[i]*2  ] =
			textureCoord->point.values[textureCoordIndex.values[i]*2];
		    uvs[coordIndex.values[i]*2+1] = 
			textureCoord->point.values[textureCoordIndex.values[i]*2+1];
		}
	    }

	    travTexture2Transform(state, uvs, coords->point.num, wrap);

	    break;
	}
      default:
	{
	    uvs = (float *)NULL;
	}
    }


    // Create Geometry Object
    obj = GEOMcreate_obj(GEOM_POLYHEDRON, 0);
    GEOMadd_vertices_with_data(obj, (float *)pos,
			       nvp,
			       color,
			       coords->point.num, GEOM_COPY_DATA);

    GEOMadd_polygons(obj, indice, 0, GEOM_COPY_DATA);

    // Default normals
    if ( nflag == 0 )
	GEOMgen_normals(obj, 0);

    // Add UV Map
    if ( tflag ) {
	GEOMadd_uvs(obj, (float *)uvs, coords->point.num, GEOM_COPY_DATA);
	delete uvs;
    }

    // Convert to poly-triangle sets
    // GEOMcvt_polyh_to_polytri(obj, GEOM_SURFACE|GEOM_WIREFRAME);

    //GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
//	GEOMdestroy_obj(obj);

    if ( tflag ) {
	//GEOMedit_texture(*editList, geomName, textureFile);
        //GEOMedit_texture_options(*editList, geomName, GEOM_TEXTURE_FILTER, 1);
    }

    // Set properties
    //simpleMaterial(state);

    // Transformation
    //GEOMedit_set_matrix(*editList, geomName, (float *)mat->value);

    // Post process
    //setGeomParent();
    //incGName();
    DelGMatrix();

    delete indice;

    if ( nvp != NULL )
	delete nvp;
    if ( color != NULL )
	delete color;

}
//

// Indexed Line Set
void
QvIndexedLineSet::traverse(QvState *state, geom_objects *geometries,int sdiv)
{
    int		i, j, k, nvs;
    GEOMobj	*obj;
    float	*v;
    float	*color;
    char	mflag;
    QvElement	*melem;
    QvMaterial	*material;


    ANNOUNCE(QvIndexedLineSet);

    InitGMatrix();
    goTrans(state);
    Matrix *mat = getLast(gmatrix);

    QvElement *elem = state->getTopElement(QvState::Coordinate3Index);
    if ( elem == (QvElement *)NULL )
	return;

    QvCoordinate3 *coords = (QvCoordinate3 *)elem->data;
    float *pos = coords->point.values;

    // Load Material
    //	: only use Diffuse Color
    //
    mflag = 0;
    QvElement *mbelem = state->getTopElement(QvState::MaterialBindingIndex);
    if ( mbelem == (QvElement *)NULL ) {
	mflag = 0;
    } else {
	QvMaterialBinding *mbind = (QvMaterialBinding *)mbelem->data;
	QvSFEnum mb = mbind->value;
	switch ( mb.value ) {
	  case 7: /* PER_VERTEX	*/
	    mflag = 1;
	    break;
	  case 8: /* PER_VERTEX_INDEXED	*/
	    mflag = 2;
	  default:
	    break;
	}
    }
    if ( mflag > 0 ) {
	melem = state->getTopElement(QvState::MaterialIndex);
	if ( melem == (QvElement *)NULL )
	    mflag = 0;
	else {
	    material = (QvMaterial *)melem->data;
	    if ( mflag == 1 && material->diffuseColor.num != coords->point.num ) {
		mflag = 0;
	    }
	    if ( mflag == 2 && materialIndex.num <= 1 ) {
		mflag = 0;
	    }
	}
    }    


    // Create object
    obj = GEOMcreate_obj(GEOM_POLYTRI, 0);

    nvs = 0;
    k = 0;
    for ( i = 0; i < coordIndex.num; i++ ) {
	if( coordIndex.values[i] != -1 && i != coordIndex.num-1 ) {
	    if((coordIndex.values[i] >= 0) &&
	       (coordIndex.values[i] < coords->point.num)) {
		nvs++;
	    } else {
		fprintf(stderr, "Error: IndexedLineSet: Out of Index: %d\n",
			coordIndex.values[i]);
	    }
	} else {
	    if ( i == coordIndex.num-1 && coordIndex.values[coordIndex.num-1] != -1 )
		nvs++;

	    v = new float[nvs*3];
	    for ( j = 0; j < nvs; j++ ) {
		v[j*3  ] = pos[coordIndex.values[j+k]*3];
		v[j*3+1] = pos[coordIndex.values[j+k]*3+1];
		v[j*3+2] = pos[coordIndex.values[j+k]*3+2];
	    }

	    // Set material
	    if ( mflag == 1 ) {
		color = new float[nvs*3];
		for ( j = 0; j < nvs; j++ ) {
		    color[j*3  ] = material->diffuseColor.values[coordIndex.values[j+k]*3];
		    color[j*3+1] = material->diffuseColor.values[coordIndex.values[j+k]*3+1];
		    color[j*3+2] = material->diffuseColor.values[coordIndex.values[j+k]*3+2];
		}
		GEOMadd_polyline(obj, v, color, nvs, GEOM_COPY_DATA);
		delete color;
	    } else if ( mflag == 2 ) {
		color = new float[nvs*3];
		for ( j = 0; j < nvs; j++ ) {
		    color[j*3  ] = material->diffuseColor.values[materialIndex.values[j+k]*3];
		    color[j*3+1] = material->diffuseColor.values[materialIndex.values[j+k]*3+1];
		    color[j*3+2] = material->diffuseColor.values[materialIndex.values[j+k]*3+2];
		}
		GEOMadd_polyline(obj, v, color, nvs, GEOM_COPY_DATA);
		delete color;
	    } else
	    GEOMadd_polyline(obj, v, 0, nvs, GEOM_COPY_DATA);

	    delete v;
	    nvs = 0;
	    k = i + 1;
	}
    }
    //GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
//    GEOMdestroy_obj(obj);


    if ( mflag == 0 )
	//simpleMaterial(state);
    //GEOMedit_set_matrix(*editList, geomName, (float *)mat->value);

    //setGeomParent();
    //incGName();
    DelGMatrix();
}
//

// PointSet
void
QvPointSet::traverse(QvState *state, geom_objects *geometries,int sdiv)
{
#define	POINT_RADII	0.01
#define	POINT_SUBDIV	1

    int		j, nvs;
    GEOMobj	*obj;


    ANNOUNCE(QvIndexedPointSet);

    InitGMatrix();
    goTrans(state);
    Matrix *mat = getLast(gmatrix);

    QvElement *elem = state->getTopElement(QvState::Coordinate3Index);
    if ( elem == (QvElement *)NULL )
	return;

    QvCoordinate3 *coords = (QvCoordinate3 *)elem->data;

    if ( numPoints.value < 0 )
	return;

    nvs = numPoints.value;

    float *rad = new float[nvs];
    float *pos = new float[nvs *3];
    for ( j = 0; j < nvs *3; j++ )
	pos[j] = coords->point.values[j + startIndex.value*3];


    // Create object
    obj = GEOMcreate_obj(GEOM_SPHERE, 0);

    for ( j = 0; j < nvs; j++ ) {
	rad[j] = POINT_RADII;
    }
    GEOMadd_vertices(obj, (float *)pos, nvs, GEOM_COPY_DATA);
    GEOMadd_radii(obj, rad, nvs, GEOM_COPY_DATA);

    //GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
//    GEOMdestroy_obj(obj);
    delete rad;
    delete pos;

    //simpleMaterial(state);
    //GEOMedit_subdivision(*editList, geomName, POINT_SUBDIV);
    //GEOMedit_set_matrix(*editList, geomName, (float *)mat->value);

    //setGeomParent();
    //incGName();
    DelGMatrix();

#undef	POINT_RADII
#undef	POINT_SUBDIV
}
//

// Sphere
void
QvSphere::traverse(QvState *state, geom_objects *geometries,int sdiv)
{
//#define DIVISION_SPHERE	6
//#define NX      ((DIVISION_SPHERE-1)*4+1)
//#define NY      ((DIVISION_SPHERE-1)*2+1)

	

    FLOAT3	center;
    FLOAT3	*v;
    float	radii[1];
    GEOMobj	*obj;
    char	textureFile[64];
    static FLOAT3	ext[2] = { {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} };
    float	(*uvs)[2];
    float	*sb, *cb;
//    char		gflag = 0;
    register int	i, j;
    register float	d;
    char		wrap[2];

	int DIVISION_SPHERE=sdiv/2;
	int NX = (DIVISION_SPHERE-1)*4+1;
	int NY = (DIVISION_SPHERE-1)*2+1;

	v = new FLOAT3 [NX*NY];
	uvs	= new float [NX*NY][2];
	sb = new float [NX];
	cb = new float [NX];
	ANNOUNCE(QvSphere);


    InitGMatrix();
    goTrans(state);
    Matrix	*mat = getLast(gmatrix);

    // Transform center
    center[0] = center[1] = center[2] = 0.0;


    if ( texSW && travTexture(state, textureFile, wrap) ) {

//	if ( gflag == 0 ) {
	    for ( i=0; i<NX; i++) {
		d = 2.0*M_PI/(NX-1) * (float)i;
		sb[i] = fsin(d);
		cb[i] = fcos(d);
	        printf(" sin = %lf\n",sin(d));
	    }

	    for ( j=0; j<NY; j++ )
		for ( i=0; i<NX; i++ ) {
		    uvs[NX*j +i][0] = (float)i/(NX-1);
		    uvs[NX*j +i][1] = (float)j/(NY-1);
		}
//	    gflag = 1;
//	}

        for ( j=0; j<NY; j++ ) {
            d = radius.value*sb[j];
            for ( i=0; i<NX; i++ ) {
                v[NX*j +i][0] = -d*sb[i];
                v[NX*j +i][1] = radius.value*cb[j];
                v[NX*j +i][2] = -d*cb[i];
	    }
	}

	obj = GEOMcreate_mesh((float *)ext, (float *)v, NX, NY, GEOM_COPY_DATA);
	GEOMadd_uvs(obj, (float *)uvs, (NX*NY), GEOM_COPY_DATA);
	//GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
	//GEOMedit_texture(*editList, geomName, textureFile);
	//GEOMedit_texture_options(*editList, geomName, GEOM_TEXTURE_FILTER, 1);

    } else {

	// Construct object
	radii[0] = radius.value;
	obj = GEOMcreate_sphere((float *)ext, (float *)center, radii, 0, 0, 1, GEOM_COPY_DATA);
	//GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
	//GEOMedit_subdivision(*editList, geomName, DIVISION_SPHERE);

    }

    GEOMdestroy_obj(obj);    

    //simpleMaterial(state);
    //GEOMedit_set_matrix(*editList, geomName, (float *)mat->value);

    //setGeomParent();
    //incGName();
    DelGMatrix();

	delete v;
	delete sb;
	delete cb;
	delete uvs;
	
//#undef	DIVISION_SPHERE
//#undef NX
//#undef NY
}
//


//
// WWW-specific nodes.
//

//DEFAULT_TRAVERSE(QvWWWAnchor)
void
QvWWWAnchor::traverse(QvState *state, geom_objects *geometries,int sdiv)
{
    ANNOUNCE(QvWWWAnchor);
    state->push();
    indent++;

    //pushGName();

    for (int i = 0; i < getNumChildren(); i++)
	getChild(i)->traverse(state, geometries,sdiv);
    indent--;

    //popGName();
    //incGName();

    state->pop();
}


//DEFAULT_TRAVERSE(QvWWWInline)
void
QvWWWInline::traverse(QvState *state, geom_objects *geometries,int sdiv)
{
    //
    //  Create only bounding box ( if exists )
    //
    static FLOAT3	v[8] = {
	{-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0},
	{-1.0,  1.0, 1.0}, {1.0,  1.0, 1.0}, {1.0,  1.0, -1.0}, {-1.0,  1.0, -1.0}
    };
    static int	ind[] = {
	4,  1, 2, 6, 5,
	4,  3, 4, 8, 7,
	4,  1, 5, 8, 4,
	4,  2, 3, 7, 6,
	4,  2, 1, 4, 3,
	4,  5, 6, 7, 8,  0
    };

    FLOAT3	verts[8];
    GEOMobj	*obj;
    register int	i;

    ANNOUNCE(QvWWWInline);

    if ( bboxSize.value[0]<0.1 && bboxSize.value[1]<0.1 && bboxSize.value[2]<0.1 )
	return;

    InitGMatrix();
    goTrans(state);
    Matrix	*mat = getLast(gmatrix);

    obj = GEOMcreate_obj(GEOM_POLYHEDRON, NULL);

    for ( i = 0; i < 8; i++ ) {
	verts[i][0] = v[i][0] * bboxSize.value[0] *0.5 + bboxCenter.value[0];
	verts[i][1] = v[i][1] * bboxSize.value[1] *0.5 + bboxCenter.value[1];
	verts[i][2] = v[i][2] * bboxSize.value[2] *0.5 + bboxCenter.value[2]; 
    }

    GEOMadd_vertices(obj, (float *)verts, 8, GEOM_COPY_DATA);
    GEOMadd_polygons(obj, ind, 0, GEOM_COPY_DATA);

    //GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
//    GEOMdestroy_obj(obj);

    //GEOMedit_render_mode(*editList, geomName, "lines");
    //GEOMedit_set_matrix(*editList, geomName, (float *)mat->value);

    //setGeomParent();
    //incGName();
    DelGMatrix();
}
//


//
//  Geometric Labels.
// 

//DEFAULT_TRAVERSE(QvAsciiText)
void
QvAsciiText::traverse(QvState *state, geom_objects *geometries,int sdiv)
{
#define LABEL_SCALE     0.15

    extern int	labelSW;

    GEOMobj	*obj;
    int		flag;
    int		bold, italic;
    int		align;
    int		fontNum = 0;
    QvFontStyle	*fontStyle, defaultStyle;
    static FLOAT3	locate = { 0.0, 0.0, 0.0 };
    static FLOAT3	offset = { 0.0, 0.0, 0.0 };


    ANNOUNCE(QvAsciiText);

    if ( !labelSW )
	return;

    InitGMatrix();
    goTrans(state);
    Matrix *mat = getLast(gmatrix);

    QvElement *elem = state->getTopElement(QvState::FontStyleIndex);
    if ( elem == (QvElement *)NULL )
	fontStyle = &defaultStyle;
    else
	fontStyle = (QvFontStyle *)elem->data;

    bold = italic = 0;
    if ( fontStyle->style.value & QvFontStyle::BOLD )
	bold = 1;
    if ( fontStyle->style.value & QvFontStyle::ITALIC )
	italic = 1;

    switch ( fontStyle->family.value ) {
      case QvFontStyle::SERIF:
	fontNum = GEOMget_font_number("Times", bold, italic);
	break;
      case QvFontStyle::SANS:
	fontNum = GEOMget_font_number("Helvetica", bold, italic);
	break;
      case QvFontStyle::TYPEWRITER:
      default:
	fontNum = GEOMget_font_number("Courier", bold, italic);
	break;
    }

    switch ( justification.value ) {
      case CENTER:
	align = GEOM_LABEL_CENTER;
	break;
      case RIGHT:
	align = GEOM_LABEL_RIGHT;
	break;
      case LEFT:
      default:
	align = GEOM_LABEL_LEFT;
	break;
    }

    // Create Label flag
    flag = GEOMcreate_label_flags(fontNum, 0, 0, 0, align, 0);

    // Create Label
    obj = GEOMcreate_label(NULL, flag);
    GEOMadd_label(obj, (char *)string.value.getString(),
		  (float *)locate, (float *)offset,
		  /* (fontStyle->size.value) */ LABEL_SCALE, NULL, -1);
    //GEOMedit_geometry(*editList, geomName, obj);
	if (geometries->add_obj(obj) == -1)
		fprintf(stderr, "Error Adding object\n");fflush(stderr);
//	GEOMwrite_obj(obj,fileno(stdout),GEOM_DO_ALL_DATA);
//    GEOMdestroy_obj(obj);

    //simpleMaterial(state);
    //GEOMedit_set_matrix(*editList, geomName, (float *)mat->value);

    //setGeomParent();
    //incGName();
    DelGMatrix();

#undef LABEL_SCALE
}



DO_PROPERTY(QvFontStyle,	FontStyleIndex)


//
// Default traversal methods. These nodes have no effects during traversal.
//

DEFAULT_TRAVERSE(QvInfo)
DEFAULT_TRAVERSE(QvUnknownNode)


#undef ANNOUNCE
#undef DEFAULT_TRAVERSE
#undef DO_PROPERTY
#undef DO_TYPED_PROPERTY
