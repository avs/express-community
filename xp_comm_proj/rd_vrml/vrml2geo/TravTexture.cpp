//
// travTexture.C
// Texture Mapping Utils for QvTraverse
//

#include <stdlib.h>
#ifdef _VISUALC_
#  include <io.h>
#else
#  include <unistd.h>
#endif

#include <QvElement.h>
#include <QvNodes.h>
#include <QvUnknownNode.h>
#include <QvState.h>
#include <avs/avs.h>
#include <avs/port.h>
#include <avs/geom.h>
#include <travUtil.h>
#include <travTexture.h>

#define AVSerror printf

// disable texture mapping
static void
disableTexture()
{
    texSW = 0;
    //AVSmodify_parameter("texture", AVS_VALUE, 0, 0, 0);
}


// get directory name
void
getdirname(const char *path, char *dir)
{
    if ( path == NULL ) {
	*dir = '\0';
	return;
    }

    strcpy(dir, path);

    char *cp;
    for ( cp = dir; *cp != '\0'; cp++ );
    for ( cp--; *cp != '/' && cp != dir ; cp-- );

    if ( cp == dir ) {
	strcpy(dir, "./");
	return;
    }

    *++cp = '\0';
    return;
}


// get file name
void
getfilename(const char *path, char *base)
{
    if ( path == NULL ) {
	*base = '\0';
	return;
    }

    char *cp;
    for ( cp = (char *)path; *cp != '\0'; cp++ );
    for ( cp--; *cp != '/' && cp != path ; cp-- );

    if ( *cp == '/' ) cp++;
    strcpy(base, cp);
    return;
}


// convert filename: RGB -> AVS Image
void
convfn(const char *rgb, char *x)
{
    char	*cp;
    char	*tmp;
    char	base[128];
    static char	*tmpbuf = "/tmp";

    tmp = getenv("RGBTMP");
    if ( !tmp ) tmp = tmpbuf;


    if ( *rgb == '/' || !strncmp(rgb, "http:", 5) ) {
	getfilename(rgb, base);
    } else {
	strcpy(base, rgb);
    }

    for ( cp = (char *)base; *cp != '\0'; cp++ );
    for ( cp--; *cp != '.' && cp != base ; cp-- );
    if ( cp++ != base &&
	(!strcmp(cp, "rgb") || !strcmp(cp, "sgi") ||
	 !strcmp(cp, "gif") ||
	 !strcmp(cp, "jpg") || !strcmp(cp, "jpeg"))
	) {	// has some suffix I know.
	strcpy(x, tmp);
	strcat(x, "/");
	strcat(x, base);
	for ( cp = x; *cp != '\0'; cp++ );
	cp--; cp--; cp--;
	*cp = '\0';
	strcat(x, "x");
    } else { // no valid suffix
	strcpy(x, tmp);
	strcat(x, "/");
	strcat(x, base);
	strcat(x, ".x");
    }

}

// get image type from suffix
int
getimgtype(const char *file)
{
    /* Returns: 0  Unknown
     *		1  IRIS RGB
     *		2  GIF
     *		3  JPEG
     */

    char	*cp;

    for ( cp = (char *)file; *cp != '\0'; cp++ );
    for ( cp--; *cp != '.' && cp != file ; cp-- );
    if ( cp != file ) {
	cp++;
	if ( !strcmp(cp, "rgb") || !strcmp(cp, "sgi") ) {
	    return(1); /* IRIS RGB Image */
	} else if ( !strcmp(cp, "gif") ) {
	    return(2); /* GIF Image */
	} else if ( !strcmp(cp, "jpg") || !strcmp(cp, "jpeg") ) {
	    return(3); /* JPEG Image */
	} else {
	    return(0); /* I don't know.... */
	}
    } else { /* no suffix, I can't guess. */
	return(0);
    }
}



// Traverse Texture2 image
int
travTexture(QvState *state, char *tf, char wrap[2])
{
    QvElement	*elm = state->getTopElement(QvState::Texture2Index);

    if ( elm != NULL ) {

	QvTexture2 *tex2 = (QvTexture2 *)elm->data;
	char *rgbf = (char *)tex2->filename.value.getString();
	if ( strlen(rgbf) > 1 ) {
	    char	tempfile[64];

	    convfn(rgbf, tempfile);
#ifndef _VISUALC_
	    if ( access(tempfile, R_OK) == 0 )
#endif /*!_VISUALC_*/
            {
		strcpy(tf, tempfile);

		// Wrapping
		wrap[0] = (tex2->wrapS.value == QvTexture2::REPEAT)?0:1;
		wrap[1] = (tex2->wrapT.value == QvTexture2::REPEAT)?0:1;

		return ( 1 );
	    }
	}
    }
    return ( 0 );
}

void
goTexture(QvState *state)
{
    static char	commbuff[][8] = {"0",
				 "rgb2avs",
				 "gif2avs",
				 "jpg2avs"};
    char	*comm;
    char	tf[64], rgbfile[128];


    QvElement   *elm = state->getTopElement(QvState::Texture2Index);

    if ( elm != NULL ) {

	QvTexture2 *tex2 = (QvTexture2 *)elm->data;
	char *rgbf = (char *)tex2->filename.value.getString();
	if ( strlen(rgbf) > 1 ) {
	    char	sysbuf[128];
	    char	*conv = "%s %s %s";

	    // get Texture file name
	    if ( *rgbf == '/' )
		strcpy(rgbfile, rgbf);
            else if ( !strncmp(rgbf, "http:", 5) ) {
		char	base[64];
		getfilename(rgbf, base);
		getdirname(infile, rgbfile);
		strcat(rgbfile, base);
	    }
	    else {
		getdirname(infile, rgbfile);
		strcat(rgbfile, rgbf);
	    }

	    // Check Texture image
#ifdef ZERO
	    if ( access(rgbfile, R_OK) != 0 ) {
		AVSerror("Can't access image:\n%s", rgbfile);
		disableTexture();
		return;
	    }
#endif

	    // get AVS file name
	    convfn(rgbf, tf);

	    // get Image type of Texture
	    int ImgType = getimgtype(rgbf);
	    switch ( ImgType ) {
	      case 1:
		comm = getenv("RGBCONV");
		break;
	      case 2:
		comm = getenv("GIFCONV");
		break;
	      case 3:
		comm = getenv("JPGCONV");
		break;
	      default:
		AVSerror("Unknown Image type:\n%s", rgbf);
		disableTexture();
		return;
	    }

	    // Check Image converter
	    if ( !comm ) comm = commbuff[ImgType];
#ifdef ZERO
	    if ( access(comm, X_OK) != 0 ) {
		AVSerror("Can't exec Image converter:\n%s", comm);
		disableTexture();
		return;
	    }
#endif

	    // Convert Image
	    sprintf(sysbuf, conv, comm, rgbfile, tf);
	    system(sysbuf);

	} else {    // Inline Texture Image

	    convfn(geomName, tf);
	    if ( !imgAVS(tex2->image, (const char *)tf) )
		tex2->filename.value = (const char *)geomName;

	}
    }
}


// generate BBOX
void
genBBOX(FLOAT3 *verts, int size, FLOAT3 *bbox)
{
    register int	i, j;

    // bbox[0][] : MINIMUM POINT
    // bbox[1][] : MAXIMUM POINT

    bbox[0][0] = bbox[1][0] = verts[0][0];
    bbox[0][1] = bbox[1][1] = verts[0][1];
    bbox[0][2] = bbox[1][2] = verts[0][2];

    for ( i = 1; i < size; i++ ) {
	for ( j = 0; j < 3; j++ ) {
	    if ( bbox[0][j] > verts[i][j] ) bbox[0][j] = verts[i][j];
	    if ( bbox[1][j] < verts[i][j] ) bbox[1][j] = verts[i][j];
	}
    }
}

// write down to AVS Image
int
imgAVS(QvSFImage img, const char *file)
{
    FILE	*stream;
    char	header[8];
    int		xsize, ysize;
    int		x, y;
    register int	zsize;
    unsigned char	b[4];

    /* open AVS Image file to output */
    if ( (stream = fopen(file, "w")) == NULL )
	return(-1);

    xsize = (int)img.size[0];
    ysize = (int)img.size[1];
    zsize = img.numComponents;

    /**  Write AVS Image header  **/
    header[0] = 0;
    header[1] = 0;
    header[2] = (xsize >> 8) & 0xff;
    header[3] = xsize & 0xff;
    header[4] = 0;
    header[5] = 0;
    header[6] = (ysize >> 8) & 0xff;
    header[7] = ysize & 0xff;
    fwrite(header, sizeof(header), 1, stream);

    /** Write AVS Image : alpha/red/green/blue **/
    b[0] = (unsigned char)0;
    for ( y = (ysize-1); y > -1; y-- ) {
	for ( x = 0; x < xsize; x++ ) {
	    switch ( zsize ) {
	      case 2:
		b[0] = (unsigned char)255 - img.bytes[y*ysize*zsize + x*zsize +1];
	      case 1:
		b[1] = b[2] = b[3] = img.bytes[y*ysize*zsize + x*zsize];
		break;
	      case 4:
		b[0] = (unsigned char)255 - img.bytes[y*ysize*zsize + x*zsize +3];
	      case 3:
		b[1] = img.bytes[y*ysize*zsize + x*zsize];
		b[2] = img.bytes[y*ysize*zsize + x*zsize +1];
		b[3] = img.bytes[y*ysize*zsize + x*zsize +2];
		break;
	    }
#ifdef sun
	    fwrite((char *)b, sizeof(b), 1, stream);
#else
	    fwrite(b, sizeof(b), 1, stream);
#endif
	}
    }

    fclose(stream);
    return(0);
}



//------------------------------------//
//		UVmatrix
//------------------------------------//

//Constractor
UVmatrix::UVmatrix()
{
    for ( int i = 0; i < 3; i++ ) {
	for ( int j = 0; j < 3; j++ )
	    value[i][j] = 0.0;
	value[i][i] = 1.0;
    }
}
UVmatrix::UVmatrix(float v[3][3])
{
    for ( int i = 0; i < 3; i++ )
	for ( int j = 0; j < 3; j++ )
	    value[i][j] = v[i][j];
}

//Destractor
UVmatrix::~UVmatrix()
{
}

// Operator `*='
UVmatrix&
UVmatrix::operator*=(const UVmatrix& a)
{
    UVmatrix b = *this;
    for ( int i = 0; i < 3; i++ ) {
	for ( int j = 0; j < 3; j++ ) {
	    value[i][j] = 0.0;
	    for ( int k = 0; k < 3; k++)
		value[i][j] += ( b.value[i][k] * a.value[k][j] );
	}
    }
    return *this;
}

//scale
void
UVmatrix::scale(float su, float sv)
{
    UVmatrix	a;
    a.value[0][0] = su;
    a.value[1][1] = sv;
    *this *= a;
}

//trans
void
UVmatrix::trans(float tu, float tv)
{
    UVmatrix	a;
    a.value[2][0] = tu;
    a.value[2][1] = tv;
    *this *= a;
}

//rotate
void
UVmatrix::rot(float r)
{
    UVmatrix	a;
    a.value[0][0] = fcos(r);
    a.value[1][0] = -fsin(r);
    a.value[0][1] = fsin(r);
    a.value[1][1] = fcos(r);
    *this *= a;
}

//reset
void
UVmatrix::reset()
{
     for ( int i = 0; i < 3; i++ ) {
	for ( int j = 0; j < 3; j++ )
	    value[i][j] = 0.0;
	value[i][i] = 1.0;
    }
}

//multim23
void
multim23(float v[2], UVmatrix m)
{
    float	vv[3];
    vv[0] = v[0];
    vv[1] = v[1];
    vv[2] = 1.0;

    for ( int i = 0; i < 2; i++ ) {
        v[i] = 0.0;
        for ( int j = 0; j < 3; j++ )
            v[i] += vv[j]*m.value[j][i];
    }
}


// Global Texture2Transform Matrix
UVmatrix	*guvmat;

// Inititialize Global Texture2Transform Matrix
void
InitUVGmat()
{
    guvmat = new UVmatrix;
}

// Delete Global Texture2Transform Matrix
void
DelUVGmat()
{
    delete guvmat;
}


//
//	Generate current Texture2Transform matrix
//
void
Texture2TransformTraverse(QvElement *elem)
{
    UVmatrix	*mat = guvmat;
    QvTexture2Transform	*t = (QvTexture2Transform *)elem->data;

    // center
    mat->trans(-t->center.value[0], -t->center.value[1]);

    // rotation
    mat->rot(t->rotation.value);

    // inverse center
    mat->trans(t->center.value[0], t->center.value[1]);

    // scale
    mat->scale(t->scaleFactor.value[0], t->scaleFactor.value[1]);

    // translation
    mat->trans(t->translation.value[0], t->translation.value[1]);


    // recurrent call
    if ( elem->next != NULL)
	Texture2TransformTraverse(elem->next);

}


//
//	Traverse Transform2Texture
//
void
goUVTrans(QvState *state)
{
    QvElement   *elt;

    // Traverse UV Transform Matrix //
    elt = state->getTopElement(QvState::Texture2TransformationIndex);
    if ( elt ) {
	Texture2TransformTraverse(elt);
    }

}

void
travTexture2Transform(QvState *state, float *uvs, int n, char wrap[2])
{
    register int	i;
    float		tuv[2];

    InitUVGmat();
    goUVTrans(state);

    for ( i = 0; i < n; i++ ) {
	tuv[0] = uvs[i*2];
	tuv[1] = uvs[i*2+1];
	multim23(tuv, *guvmat);

	if ( wrap[0] == 0 )
	    uvs[i*2] = REPEATUV(tuv[0]);
	else
	    uvs[i*2] = CLAMPUV(tuv[0]);

	if ( wrap[1] == 0 )
	    uvs[i*2+1] = 1.0 - REPEATUV(tuv[1]);
	else
	     uvs[i*2+1] = 1.0 - CLAMPUV(tuv[1]);
    }

    DelUVGmat();
}
// EOF
