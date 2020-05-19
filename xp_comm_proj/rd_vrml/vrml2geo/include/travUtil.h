// TravUtil.h
//	Traversal Utilities header
//
#ifndef _TRAV_UTIL_
#define _TRAV_UTIL_

#include <math.h>

#undef	fsin
#undef	fcos
#undef	frint
#undef	ffabs
#undef	fsqr
#undef	fsqrt
#undef	ffloor
#define	fsin(x)		(float)sin((double)(x))
#define	fcos(x)		(float)cos((double)(x))
#define	frint(x)	(float)rint((double)(x))
#define	ffabs(x)	(float)fabs((double)(x))
#define	fsqr(x)		(float)((x)*(x))
#define	fsqrt(x)	(float)sqrt((double)(x))
#define	ffloor(x)	(float)floor((double)(x))

#define	MAX3(x)	\
((x)[0] > (x)[1] ? (x)[0] : (x)[1]) > (x)[2] ? \
((x)[0] > (x)[1] ? (x)[0] : (x)[1]) : (x)[2]

#define MIN3(x) \
((x)[0] < (x)[1] ? (x)[0] : (x)[1]) < (x)[2] ? \
((x)[0] < (x)[1] ? (x)[0] : (x)[1]) : (x)[2]

#define MAX3I(x) \
((x)[0] > (x)[1] ? (x)[0] : (x)[1]) > (x)[2] ? \
((x)[0] > (x)[1] ? 0 : 1) : 2

#define MIN3I(x) \
((x)[0] < (x)[1] ? (x)[0] : (x)[1]) < (x)[2] ? \
((x)[0] < (x)[1] ? 0 : 1) : 2

#define MED3I(x) \
(((x)[0]>(x)[1]&&(x)[1]>(x)[2])||((x)[0]<(x)[1]&&(x)[1]<(x)[2]))? 1 :\
((((x)[0]>(x)[2]&&(x)[2]>(x)[1])||((x)[0]<(x)[2]&&(x)[2]<(x)[1]))? 2 : 0 )

#define	NORM(x)	(((x)>1.0)?1.0:(x))


typedef	float	FLOAT3[3];
typedef	float	MATRIX[4][4];

// for convert float_color to packed_color
typedef union {
    unsigned char	ccol[4];
    unsigned int	lcol;
} PACKCOLOR;


// Matrix4 :	4x4 float matrix
class Matrix4
{
  public:
    Matrix4();
    Matrix4(MATRIX);
    ~Matrix4();

    MATRIX	value;

    Matrix4&	operator*=(const Matrix4& a);

    void	scale(float sx, float sy, float sz);
    void	trans(float tx, float ty, float tz);
    void	rot_x(float th);
    void	rot_y(float th);
    void	rot_z(float th);
    void	glRotMatrix(float a, FLOAT3 v);
    void	reset();
};

// Matrix :	Sub-class of Matrix4 
class Matrix : public Matrix4
{
  public:
    Matrix();
    Matrix(MATRIX);
    ~Matrix();

    void	addchild();
    void	delchild();

    Matrix	*parent;
    Matrix	*child;
};

extern Matrix	*getLast(Matrix*);
extern void	multim34(FLOAT3, Matrix4);
extern void	normVec3(FLOAT3, FLOAT3);

extern Matrix		*gmatrix;
extern GEOMedit_list	*editList;
#define	GNLENGTH	1024
extern char		geomName[GNLENGTH];
extern char		geomNBody[16];

extern void	InitGMatrix();
extern void	DelGMatrix();

extern void	InitGName();
extern void	pushGName();
extern void	incGName();
extern void	setGeomParent();
extern void	popGName();

extern void	simpleMaterial(QvState *);

// Delete Geometory objects
extern int	IVdestroy();

#endif // _TRAV_UTIL_
//EOF
