
#include <avs/f_utils.h>

#include "xp_comm_proj/rd_raw/gen.hxx"

#define EPS 1e-5
#define CHUNKSIZE 1024

// return 1 if A and B are very similar
int GFCmpValues(float A, float B) {
	float app = A-B;
	if (app>EPS) return 0;
	if (app<-EPS) return 0;
	return 1;
}
// return 1 if A and B are very similar
int GFCmpCoords(float* A, float* B) {
	int Res=0;
	Res = GFCmpValues(A[0],B[0]);
	if (!Res) return(0);
	Res = GFCmpValues(A[1],B[1]);
	if (!Res) return(0);
	Res = GFCmpValues(A[2],B[2]);
	if (!Res) return(0);
	return(1);
}

// append a Point3 (3 float) to the Arr array, reallocate the
// array is necessary
void GFAppendPoint3 (float*& Arr, int& Size, float* Point3) {
 if ((Size%CHUNKSIZE)==0) {
 	float* App = (float*)malloc((Size+CHUNKSIZE)*sizeof(float)*3);
	if (Arr==NULL) {
		Arr = App;
	} else {
		memcpy(App,Arr,sizeof(float)*Size*3);
		free(Arr);
		Arr = App;
	}
 }
 // Add the Point3
 memcpy(Arr+(Size*3),Point3,sizeof(float)*3);
 Size++;
// printf("Added a Point (%d)\n",Size);
}

// append a Conn3 (3 int) to the Arr array, reallocate the
// array is necessary
void GFAppendTri (int*& Arr, int& Size, int* Conn3) {
 if ((Size%CHUNKSIZE)==0) {
 	int* App = (int*)malloc((Size+CHUNKSIZE)*sizeof(int)*3);
	if (Arr==NULL) {
		Arr = App;
	} else {
		memcpy(App,Arr,sizeof(int)*Size*3);
		free(Arr);
		Arr = App;
	}
 }
 // Add the Conn3
 memcpy(Arr+(Size*3),Conn3,sizeof(int)*3);
 Size++;
}


// find a Point3 into a Coords3 array
//   coords are scanned backwards because matching coordinates are most
//   likely to be recent.
// return 1 if founded (Ndx will be the index)
// return 0 if not founded (Ndx is Size, the suggested index)
int GFFindVertex(float* Coords3,int Size,float* Point3, int& Ndx) {
	int Found=0;
	Ndx=Size;
	float* Coords3Ref = &Coords3[Size*3];
	while ((!Found) && (Ndx>0)) {
	   Ndx--;
	   Coords3Ref -= 3;
		Found = GFCmpCoords(Coords3Ref, Point3);
	}
	if (!Found) Ndx = Size;
	return Found;
}


int
ReadRaw_ReadRawMods_ReadRawCore::update(OMevent_mask event_mask, int seq_num)
{
   // filename (OMXstr read req notify)
   // nnodes (OMXint write)
   // coords (OMXfloat_array write)
   int coords_size;
   float *coords_arr;

   // ntri (OMXint write)
   // conn (OMXint_array write)
   int conn_size;
   int *conn_arr;

   char Line[1024];
   float Point3x3[9];
   float* Point3;
   int Tri3[3];
   int KK=0;

   //coords array
   float* Coords3=NULL;
   int Coords3Size=0;

   //connectivity array
   int* Conn3=NULL;
   int Conn3Size=0;
   int Ndx=0;
   
   char filename_buffer[AVS_PATH_MAX];
   
   if (!(char *)filename)  // ignore null file names
      return 1;

   // Use AVS/Express function to map enviroment variables into filename
   FILEmap_variables((char *)filename, filename_buffer);

	FILE* fp=NULL;
	char* file_app = (char*)filename_buffer;
	
	fp = fopen(file_app, "r");
	int scanres=0;	
	int Lines=0;
	int Found=0;
	char* res = file_app;

	printf("\nStart Reading from file %s\n",file_app);
	while (res) { 
		res = fgets(Line,1024,fp);
		Lines++;
		if ((Lines%1000)==0) printf("Processed %d Triangles\n",Lines);

		if (res) {
//			printf("Scan:[%s]\n",	Line);
			scanres = sscanf(Line,"%f %f %f %f %f %f %f %f %f",
							&Point3x3[0],&Point3x3[1],&Point3x3[2],
							&Point3x3[3],&Point3x3[4],&Point3x3[5],
							&Point3x3[6],&Point3x3[7],&Point3x3[8]
							);
			if (scanres!=9) {
				printf ("Syntax error at line [%s]",Line);
			} else {
				for (KK=0;KK<3;KK++) {
					Point3 = Point3x3+(KK*3);
					Found = GFFindVertex(Coords3, Coords3Size, Point3, Ndx);
					if (!Found) {	//add this point to Coords3
						GFAppendPoint3 (Coords3,Coords3Size,Point3);
					}
					Tri3[KK]=Ndx;
				}
			  	GFAppendTri (Conn3,Conn3Size,Tri3);
			}
		}
	}

	fclose(fp);
	printf("\nFinish Reading from file\n");
	
	printf("\nStart Copy of data\n");
   nnodes = Coords3Size;
	printf("nnodes:%d\n",(int)nnodes);	
   coords_arr = (float *)coords.ret_array_ptr(OM_GET_ARRAY_WR);
   if (coords_arr) {
   	memcpy(coords_arr,Coords3,sizeof(float)*3*Coords3Size);
      ARRfree(coords_arr);
   }
   if (Coords3) free(Coords3);

   ntri = Conn3Size;
	printf("ntri:%d\n",(int)ntri);	
   conn_arr = (int *)conn.ret_array_ptr(OM_GET_ARRAY_WR);
   if (conn_arr) {
   	memcpy(conn_arr,Conn3,sizeof(int)*3*Conn3Size);
      ARRfree(conn_arr);
   }
   if (Conn3) free(Conn3);
	printf("\nEnd Copy of data\n");

   // return 1 for success
   return(1);
}

