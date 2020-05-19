#include "Indexable.h"

IndexableArray::IndexableArray(int a): Size(a){
		TheArray = new Indexable* [a];
};
IndexableArray::~IndexableArray(){
	if (TheArray) {
			delete [] TheArray;
	}
}

void Indexable::Serialize(ofstream &fp){
	
};


void IndexableArray::Serialize(ofstream &fp){
		for (int j=0;j< Size;j++ )
		{ 
			TheArray[j]->Serialize(fp);
		}
};
//volgare QuickSort ricorsivo
void IndexableArray::Sort(int lo0,int hi0){ 
      int lo = lo0;
      int hi = hi0;
      int mid;
	  Indexable* swpvar;
	if ( hi0 > lo0)
      {
         mid = TheArray[ ( lo0 + hi0 ) / 2 ]->ID;
         while( lo <= hi )
         {
	     while( ( lo < hi0 ) && ( (TheArray[lo]->ID) < mid ))
		 ++lo;
	     while( ( hi > lo0 ) &&  ( (TheArray[hi]->ID) > mid ))
		 --hi;
            if( lo <= hi )
            {
				swpvar=TheArray[hi];
				TheArray[hi]=TheArray[lo];
				TheArray[lo]=swpvar;
               ++lo;
               --hi;
            }
         }
         if( lo0 < hi )
            Sort(  lo0, hi );
         if( lo < hi0 )
            Sort( lo, hi0 );
      }
};

//volgare binsearch
int NodeBinSearch(Indexable** theArr,int lo,int hi,int &searchID){
	if ((hi<0) || (lo<0)) return(-1);
	if ((hi-lo)==1)
	{
			if (searchID==(theArr[lo]->ID))return (lo);
			if (searchID==(theArr[hi]->ID))return (hi);
			return (-1);
	} else 
		if (lo!=hi)
		{
			int mid=lo+(hi-lo)/2;
			if (searchID==(theArr[mid]->ID))return (mid);
			if (searchID>(theArr[mid]->ID)) return (NodeBinSearch(theArr,mid,hi,searchID));
				else return (NodeBinSearch(theArr,lo,mid,searchID));
		} else {
			if (searchID==(theArr[lo]->ID)) return (lo);
				else return (-1);
		}
};

//operatore per l'accesso alle righe via ID (e non via indice sequenziale)
Indexable* IndexableArray::operator[](int a){//assume array ORDINATO
	int res=NodeBinSearch(TheArray,0,(Size-1),a);
	if (res==-1) return (NULL);//lanciare eccezione? sarebbe carino
		else return (TheArray[res]);
};

//operatore per l'accesso alle righe via indice sequenziale (e non via ID)
Indexable* IndexableArray::Sequential(int a){	
	if ((a>=Size)||(a<0)) return (TheArray[0]);//lanciare eccezione? sarebbe carino
		else return (TheArray[a]);
};

//dato un ID, restituisce il suo indice sequenziale
int IndexableArray::Index_Of(int a){
	int res=-1;
	res=NodeBinSearch(TheArray,0,(Size-1),a);
	return (res);
};

//serve per avere l'array di ID all'esterno: assume array_rep (1) allocato (2) di dimensione giusta
void IndexableArray::ReturnID_Array(int* array_rep){
	for (int j=0;j< Size;j++ )
		{ 
			array_rep[j]=TheArray[j]->ID;
		}
};
