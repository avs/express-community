#ifndef INDEXABLE_PARAPARA
#define INDEXABLE_PARAPARA
#include <stdio.h>
#include <fstream.h>
//classe base per oggetti temporanei (in lettura) dotati di identificatore

class Indexable
{
public:
	int ID;
	Indexable(): ID(-1){};
	Indexable(int a): ID(a){};
	virtual void Serialize(ofstream &fp);
};

//classe container per Indexable

class IndexableArray
{
	public:
	int Size;
	Indexable** TheArray; 
	IndexableArray(): Size(0){};
	IndexableArray(int a);
	~IndexableArray();
	void Sort(int,int);
	Indexable* operator[](int);
	Indexable* Sequential(int);//funziona male, spesso accedo direttamente TheArray. Brutto ma funziona
	int Index_Of(int);
	void ReturnID_Array(int*);//serve per avere l'array di ID all'esterno
	virtual void Serialize(ofstream &fp);
};

#endif