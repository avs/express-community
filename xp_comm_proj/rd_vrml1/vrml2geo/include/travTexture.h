//
// Traverse Texture
//

#ifndef _TRAV_TEXTURE_
#define _TRAV_TEXTURE_

#include <QvSFImage.h>
#include <travUtil.h>
#include <math.h>

extern int	texSW;
extern char	*infile;

extern void	getdirname(const char *path, char *dir);
extern void	getfilename(const char *path, char *base);
extern void	convfn(const char *rgb, char *x);
extern int	getimgtype(const char *file);
extern int	travTexture(QvState *state, char *tf, char wrap[2]);
extern void	goTexture(QvState *state);
extern void	genBBOX(FLOAT3 *verts, int size, FLOAT3 *bbox);
extern int	imgAVS(QvSFImage img, const char *file);


#define	FODD(x)\
(fabs(ffloor((x))*0.5-(int)ffloor((x))/2)>0.1?1:0)

#define REPEATUV(x)\
(\
((x)<=1.0&&(x)>=0.0)?(x):\
(FODD((x))?(1.0-((x)-ffloor((x)))):((x)-ffloor((x))))\
)

#define	CLAMPUV(x)\
(\
((x)<=1.0&&(x)>=0.0)?(x):\
(((x)>1.0)?1.0:0.0)\
)

// UVmatrix :	3x3 float matrix for Texture2Transform
class	UVmatrix
{
  public:
    UVmatrix();
    UVmatrix(float[3][3]);
    ~UVmatrix();

    float	value[3][3];

    UVmatrix&	operator*=(const UVmatrix& a);

    void	scale(float su, float sv);
    void	trans(float tu, float tv);
    void	rot(float r);
    void	reset();
};

extern void	multim23(float[2], UVmatrix);

extern void	Texture2TransformTraverse(QvElement *elem);
extern void	goUVTrans(QvState *state);
extern void	travTexture2Transform(QvState *state, float *uvs, int n, char wrap[2]);

// Global Texture2Transform Matrix
extern UVmatrix	*guvmat;
extern void	InitUVGmat();
extern void	DelUVGmat();


#endif // _TRAV_TEXTURE_
// EOF
