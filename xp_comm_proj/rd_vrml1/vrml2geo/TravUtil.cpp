// travUtil.C
//	Utilities for QvTraverse
//
#include <QvElement.h>
#include <QvNodes.h>
#include <QvUnknownNode.h>
#include <QvState.h>
#include <avs/avs.h>
#include <avs/port.h>
#include <avs/geom.h>

#include <stdlib.h>
#include <string.h> 
#include <travUtil.h>
#include <travCamera.h>

#define AVSerror printf

#define	AVS_ROTATION

//----------------------------------------------------------------------//
//		Matrix4
//----------------------------------------------------------------------//

// Constructor
Matrix4::Matrix4()
{
    for ( int i = 0; i < 4; i++ ) {
	for ( int j = 0; j < 4; j++ )
	    value[i][j] = 0.0;
	value[i][i] = 1.0;
    }
}
Matrix4::Matrix4(MATRIX v)
{
    for ( int i = 0; i < 4; i++ )
	for ( int j = 0; j < 4; j++ )
	    value[i][j] = v[i][j];
}

// Destractor
Matrix4::~Matrix4()
{
}

// Operator `*='
Matrix4&
Matrix4::operator*=(const Matrix4& a)
{
    Matrix4 b = *this;

    for ( int i = 0; i < 4; i++ ) {
	for ( int j = 0; j < 4; j++ ) {
	    value[i][j] = 0.0;
	    for ( int k = 0; k < 4; k++)
		value[i][j] += ( b.value[i][k] * a.value[k][j] );
	}
    }

    return *this;
}

// scale Matrix4
void
Matrix4::scale(float sx, float sy, float sz)
{
    Matrix4	a;

    a.value[0][0] = sx;
    a.value[1][1] = sy;
    a.value[2][2] = sz;

    *this *= a;
}

// translate Matrix4
void
Matrix4::trans(float tx, float ty, float tz)
{
    Matrix4	a;

    a.value[3][0] = tx;
    a.value[3][1] = ty;
    a.value[3][2] = tz;

    *this *= a;
}

// rotate (X)
void
Matrix4::rot_x(float th)
{
    Matrix4	a;

    a.value[1][1] = fcos(th);
    a.value[1][2] = fsin(th);
    a.value[2][1] = -fsin(th);
    a.value[2][2] = fcos(th);

    *this *= a;
}

// rotate (Y)
void
Matrix4::rot_y(float th)
{
    Matrix4	a;

    a.value[0][0] = fcos(th);
    a.value[0][2] = -fsin(th);
    a.value[2][0] = fsin(th);
    a.value[2][2] = fcos(th);

    *this *= a;
}

// rotate (Z)
void
Matrix4::rot_z(float th)
{
    Matrix4	a;

    a.value[0][0] = fcos(th);
    a.value[0][1] = -fsin(th);
    a.value[1][0] = fsin(th);
    a.value[1][1] = fcos(th);

    *this *= a;
}

// reset Matrix
void
Matrix4::reset()
{
    for ( int i = 0; i < 4; i++ ) {
	for ( int j = 0; j < 4; j++ )
	    value[i][j] = 0.0;
	value[i][i] = 1.0;
    }
}


//----------------------------------------------------------------------//
//		Matrix
//----------------------------------------------------------------------//
// Constractor for Matrix
Matrix::Matrix()
{
    parent = (Matrix *)0;
    child = (Matrix *)0;
}

Matrix::Matrix(MATRIX v)
{
    for ( int i = 0; i < 4; i++ )
	for ( int j = 0; j < 4; j++ )
	    value[i][j] = v[i][j];
    parent = (Matrix *)0;
    child = (Matrix *)0;
}

Matrix::~Matrix()
{
    delchild();
}

void
Matrix::delchild()
{
    if ( child ) {
	child->delchild();
	delete child;
	child = (Matrix *)0;
    }
}

void
Matrix::addchild()
{
    if ( child != (Matrix *)0 ) {
	child->addchild();
    } else {
	child = new Matrix(value);
	child->parent = this;
    }
}

Matrix *
getLast(Matrix *a)
{
    Matrix *p = a;

    while ( p->child )
	p = p->child;

    return p;
}


// vector3 x Matrix4
void
multim34(FLOAT3 v, Matrix4 m)
{
    float	vv[4];

    vv[0] = v[0];
    vv[1] = v[1];
    vv[2] = v[2];
    vv[3] = 1.0;

    for ( int i = 0; i < 3; i++ ) {
	v[i] = 0.0;
	for ( int j = 0; j < 4; j++ )
	    v[i] += vv[j]*m.value[j][i];
    }
}

// Normalize Vector3
void
normVec3(FLOAT3 v, FLOAT3 nv)
{
    float	a = fsqr(v[0]) + fsqr(v[1]) + fsqr(v[2]);

    if ( a < 1e-10 ) {
	nv[0] = nv[1] = nv[2] = fsqrt(3)/3.0;
	return;
    }

    nv[0] = v[0] / fsqrt(a);
    nv[1] = v[1] / fsqrt(a);
    nv[2] = v[2] / fsqrt(a);

}

// OpenGL liked rotation
void
Matrix4::glRotMatrix(float a, FLOAT3 v)
{
    FLOAT3	u;
    Matrix	uut, ciu, s;
    float	sa = fsin(a);
    float	ca = fcos(a);
    register int	i, j;

    // Normalize v
    normVec3(v, u);
#ifdef AVS_ROTATION
    u[0] = -u[0]; u[1] = -u[1]; u[2] = -u[2];
#endif

    // Construct uut
    uut.value[0][0] = fsqr(u[0]);
    uut.value[1][1] = fsqr(u[1]);
    uut.value[2][2] = fsqr(u[2]);
    uut.value[0][1] = uut.value[1][0] = u[0] * u[1];
    uut.value[0][2] = uut.value[2][0] = u[0] * u[2];
    uut.value[1][2] = uut.value[2][1] = u[1] * u[2];

    // Construct cos(a)*(I-uut)
    for ( i = 0; i < 3; i++ )
	for ( j = 0; j < 3; j++ ) {
	    ciu.value[i][j] -= uut.value[i][j];
	    ciu.value[i][j] *= ca;
	}

    // Construct sin(a)*S
    s.value[0][0] = s.value[1][1] = s.value[2][2] = 0.0;
    s.value[0][1] = -u[2] * sa;
    s.value[0][2] =  u[1] * sa;
    s.value[1][0] =  u[2] * sa;
    s.value[1][2] = -u[0] * sa;
    s.value[2][0] = -u[1] * sa;
    s.value[2][1] =  u[0] * sa;

    // Construct M = uut + cos(a)*(I-uut) + sin(a)*S
    for ( i = 0; i < 3; i++ )
	for ( j = 0; j < 3; j++ )
	    value[i][j] = uut.value[i][j]
		+ ciu.value[i][j] + s.value[i][j];

}

//----------------------------------------------------------------------//
//		Misc.
//----------------------------------------------------------------------//

//	Matrix stack
Matrix	*gmatrix;

//	Global output edit list
GEOMedit_list	*editList = (GEOMedit_list *)NULL;

//	Geometry object name stack 
char		geomName[GNLENGTH];

//
//	Initialize transformation matrix stack
//
void
InitGMatrix()
{
    gmatrix = new Matrix;
}

//
//	Destroy transformation matrix stack
//
void
DelGMatrix()
{
    delete gmatrix;
}

//
// Geometry object name stack utils.
//

// Initialize Geometry Name
void
InitGName()
{
    strcpy(geomName, geomNBody);
}

// Push Geometry Name
void
pushGName()
{
    void setGeomParent();
    GEOMobj *obj = GEOMcreate_obj(GEOM_POLYTRI, NULL);

    GEOMadd_polyline(obj, NULL, NULL, 0, GEOM_COPY_DATA);
    GEOMedit_geometry(*editList, geomName, obj);
    GEOMdestroy_obj(obj);

    setGeomParent();
    
    strcat(geomName, "_0");
}

// Increment Geometry Name
void
incGName()
{
    char	*cp, next[8];

    for ( cp = geomName; *cp != '\0'; cp++);
    for ( ; *cp != '_'; cp-- );
    *cp++ = '\0';
    sprintf(next, "_%d", atoi(cp)+1);
    strcat(geomName, next);
}

// setGeomParent
void
setGeomParent()
{
    char	*cp;
    char	*parentN = strdup(geomName);

    for ( cp = parentN; *cp != '\0'; cp++);
    for ( ; *cp != '_'; cp-- );
    *cp = '\0';

    GEOMedit_parent(*editList, geomName, parentN);
}


// Pop Geometry Name
void
popGName()
{
    char	*cp;

    for ( cp = geomName; *cp != '\0'; cp++);
    for ( ; *cp != '_'; cp-- );
    *cp = '\0';
}    


//
//	Material utils.
//

// Simple Material
void
simpleMaterial(QvState *state)
{
    // Load Material
    QvElement *melem = state->getTopElement(QvState::MaterialIndex);
    if ( melem != (QvElement *)NULL ) {
	QvMaterial *m = (QvMaterial *)melem->data;

	// Set Color
	//	Base color = (diffuse_color + ambient_color + emissive_color)
	//
	float	color[3];
	color[0] = m->diffuseColor.values[0]
	    + m->ambientColor.values[0] + m->emissiveColor.values[0];
	color[1] = m->diffuseColor.values[1]
	    + m->ambientColor.values[1] + m->emissiveColor.values[1];
	color[2] = m->diffuseColor.values[2]
	    + m->ambientColor.values[2] + m->emissiveColor.values[2];
	if ( MAX3(color) > 1.0 ) {
	    color[0] /= MAX3(color);
	    color[1] /= MAX3(color);
	    color[2] /= MAX3(color);
	}
	GEOMedit_color(*editList, geomName, color);

	// Set Property
	float	ambient, diffuse, specular, spec_exp;
	float	transparency, spec_col[3];
	ambient = MAX3(m->ambientColor.values);
	color[0] = m->diffuseColor.values[0] + m->emissiveColor.values[0];
	color[1] = m->diffuseColor.values[1] + m->emissiveColor.values[1];
	color[2] = m->diffuseColor.values[2] + m->emissiveColor.values[2];
	diffuse = MAX3(color) > 1.0 ? 1.0 : MAX3(color);
	spec_exp = -1.0;
	transparency = 1.0 - m->transparency.values[0];
	spec_col[0] = m->specularColor.values[0];
	spec_col[1] = m->specularColor.values[1];
	spec_col[2] = m->specularColor.values[2];
	specular = MAX3(spec_col);
	GEOMedit_properties(*editList, geomName,
			    ambient,
			    diffuse,
			    specular,
			    spec_exp,
			    transparency,
			    spec_col);
    }
}


// Delete Geometory objects

int
IVdestroy()
{
#define	CLI(a)	sprintf(clibuf, a); \
 if ( ! AVScommand("kernel", clibuf, &cliout, &clierr) ) \
 { /* AVSerror(clierr); */ return(0); }
#define	MAX_EDIT_LIST	30

#if 0 /*CLI does NOT supported*/
    static char		clibuf[64], *clierr, *cliout;
    register int	i;

    /*
     *  CAUTION:
     *   Don't use CLI command: geom_lookup_obj_names
     *   to get object names, AVS will down with SEGV.
     *   Length of object names is very long,
     *   and over-run the buffer of AVScommand().
     */

    for ( i=0; i<MAX_EDIT_LIST; i++ ) {
	sprintf(clibuf, "geom_delete_obj -object %s.%d\n", geomNBody, i);
	CLI(clibuf);
    }

    CLI("geom_reset -camera 1");
    CLI("geom_refresh");
#endif /*0*/

    return(1);

#undef	MAX_EDIT_LIST
#undef	CLI
}

// EOF
