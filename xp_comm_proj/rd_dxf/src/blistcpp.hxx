#ifndef blistcpp_h
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#define blistcpp_h

template <class T>
class LISTE
{
protected:
	LISTE<T> *pNext;
	LISTE<T> *pPrev;
	T *pItem;
public:
	LISTE (LISTE<T> *pBef, T* pI);
	LISTE ();
	virtual ~LISTE ();
	LISTE<T> *getNext();
	LISTE<T> *getPrev();
	T *getItem();
	void dump();
};

#define pFirst pNext
#define pLast pPrev

template <class T>
class CLIST : public LISTE<T>
{
private:
	LISTE<T> *pAktu;
public:
	CLIST();// : LISTE<T>()
	       /*	{
		pAktu=this;
		}*/
	virtual ~CLIST();

	int InsertBehind(T *pItemIns);
	int InsertBefore(T *pItemIns);
	int InsertHead(T *pItemIns);
	int InsertTail(T *pItemIns);
	void RemoveItem();

	void chItemInList(T *pI);
	T *GetSelected();
	T *GetNext();
	T *GetLast();
	T *GetFirst();
	T *GetIndexed(int Idx);
	char Next();
	int addItemToList(T *pI,
			 int (*fcmp)(T *pItList,T *pItNew));

	CLIST &operator=(const CLIST &other);
};

#endif
