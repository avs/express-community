#include "Indexable.h"

//funzioni di utilita' per il parsing generico
int DetectBlank(char* line);
int DetectBlankN(char* line,int Size);
int DetectDigit(char* line);
int DetectEndt(char* line);
int GFstrset(char* line,char ch, int size);
void GFcheckE(char* buf);

float VectorModule(float F,float X,float Y,float Z);

//banale binsearch
int SimpleBinSearch(int* theArr,int lo,int hi,int &searchID);
