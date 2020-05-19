
#include "gen.h"

#include <stdio.h>
#include <fcntl.h>

#include <QvDB.h>
#include <QvInput.h>
#include <QvNode.h>
#include <QvState.h>
#include <avs/geom.h>

#include "gobject.h"
#include <string.h>
#ifdef _VISUALC_
#include <io.h>
#endif /*_VISUALC_*/

// #define DEBUG

extern void InitGName();

extern "C" {
	extern int geoms_to_field(OMobj_id,int,GEOMobj**,OMobj_id);
}

int	texSW;
int	labelSW;

FILE 	*fp =(FILE *)NULL;
char    *infile = (char *)NULL;
char    geomNBody[16];

int
ReadVRML1_ReadVRMLMods_ReadVRMLCore::readVRML(OMevent_mask event_mask, int seq_num)
{
    QvDB::init();

    QvInput	in;
    QvNode	*root;
    QvState state;
	int i;
	GEOMobj **objs;
	int nobjs;
	int ret;
	int sdiv=20;
	geom_objects geometries;
	
	/***********************/
	/* Function's Body     */
	/***********************/

#ifdef DEBUG
    fprintf(stderr,"Read_VRML:  Subdivision = %d\n",(int)Subdivision);fflush (stderr);
#endif

	if ((int)Subdivision>2)
		sdiv=(int) Subdivision;
		
#ifdef DEBUG
    fprintf(stderr,"Read_VRML:  Subdivision = %d\n",sdiv);fflush (stderr);
#endif
	

#ifdef DEBUG
	ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: ReadVRML1_ReadVRMLMods_ReadVRMLCore::readVRML\n");

	fprintf(stderr,"readVRML: geometries = %lp\n",&geometries);
    strcpy(geomNBody, "VRML2GEO");
#endif

    InitGName();
    fp = fopen(Filename,"r");
#ifdef DEBUG
    fprintf(stderr,"Read_VRML:  File = %lp\n",(int)fp);fflush (stderr);
#endif
	if (!fp) return (0);
    in.setFilePointer(fp);
    // Read VRML file
    ret=QvDB::read(&in, root);
	fflush(stderr);
	if (!ret) return (0);
#ifdef DEBUG
    fprintf(stderr,"QvDB::read : %d\n",ret);fflush (stderr);
#endif
	root->traverse(&state,&geometries,sdiv);
#ifdef DEBUG
    fprintf(stderr,"root->traverse\n");fflush (stderr);
#endif
	objs=geometries.get_all_objs();
	nobjs=geometries.get_nobjs();
#ifdef DEBUG
    fprintf(stderr,"QvDB::nobjs : %d\n",nobjs);fflush (stderr);
#endif
	if (!nobjs)
		return(0);
	
	for (i=0; i < nobjs; i++)
	{
		switch (objs[i]->type)
		{
			case GEOM_POLYHEDRON:
			{
				GEOMcvt_polyh_to_polytri(objs[i],GEOM_SURFACE|GEOM_WIREFRAME);
				break;
			}
			case GEOM_MESH:
			{
				GEOMcvt_mesh_to_polytri(objs[i],GEOM_SURFACE|GEOM_WIREFRAME);
				break;
			}
		}
#ifdef DEBUG
		fprintf(stderr,"obj[%i] : %lp \t type: %i \n",i,objs[i],objs[i]->type);
		fflush (stderr);
#endif
	}
	
	geoms_to_field(this->obj_id(),nobjs,objs,(OMobj_id)out.obj_id());
		
	// return 1 for success
	return(1);
}
