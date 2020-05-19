#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include "blistcpp.hxx"

/***************************************************************
listelement
***************************************************************/
template <class T>
LISTE<T>::LISTE (LISTE<T> *pBef, T* pI)
{
pItem=pI;
pNext=pBef->pNext;
pPrev=pBef;
pBef->pNext->pPrev=this;
pBef->pNext=this;
}

template <class T>
LISTE<T>::LISTE ()
{
pItem=NULL;
pNext=this;
pPrev=this;
}

template <class T>
LISTE<T>::~LISTE ()
{

pNext->pPrev=pPrev;
pPrev->pNext=pNext;
T *temp=pItem;
delete temp;

}

template <class T>
LISTE<T> *LISTE<T>::getNext()
{
return pNext;
}

template <class T>
LISTE<T> *LISTE<T>::getPrev()
{
return pPrev;
}

template <class T>
T *LISTE<T>::getItem()
{
return pItem;
}

template <class T>
void LISTE<T>::dump()
	{
	printf("Here I am\n");
	}

/*****************************************************************
listhead
*****************************************************************/

template <class T>
CLIST<T>::CLIST() : LISTE<T>()
{
pAktu=this;
}

template<class T>
CLIST<T>::~CLIST()
{
LISTE<T> *temp;
temp=pFirst;
while(temp && temp!=this)
	{
	delete temp;
	if(pFirst)temp=pFirst;
	}
}

template <class T>
int CLIST<T>::InsertBehind(T *pItemIns)
{
pAktu=new LISTE<T>(pAktu,pItemIns);
//pAktu=pAktu->getNext();
return 1;
}

template <class T>
int CLIST<T>::InsertBefore(T *pItemIns)
{
pAktu= new LISTE<T>(pAktu->getPrev(),pItemIns);
//pAktu=pAktu->getPrev();
return 1;
}

template <class T>
int CLIST<T>::InsertHead(T *pItemIns)
{
pAktu=new LISTE<T>(this,pItemIns);
//pAktu=pFirst;
return 1;
}

template <class T>
int CLIST<T>::InsertTail(T *pItemIns)
{
pAktu=new LISTE<T>(pLast,pItemIns);
//pAktu=pLast;
return 1;
}

template <class T>
void CLIST<T>::RemoveItem()
{
LISTE<T> *temp=pAktu;
delete temp;
pAktu=pFirst;
}

template <class T>
void CLIST<T>::chItemInList(T *pI)
{
new LISTE<T>(pAktu,pI);
LISTE<T>*temp=pAktu;
delete temp;
}

template <class T>
T *CLIST<T>::GetSelected()
{
T *temp=pAktu->getItem();
pAktu=pAktu->getNext();
return temp;
}

template <class T>
T *CLIST<T>::GetNext()
{
pAktu=pAktu->getNext();
return pAktu->getItem();
}

template <class T>
T *CLIST<T>::GetLast()
{
pAktu=pLast;
return pLast->getItem();
}

template <class T>
T *CLIST<T>::GetFirst()
{
pAktu=pFirst;
return pFirst->getItem();
}

template <class T>
T *CLIST<T>::GetIndexed(int Idx)
{
pAktu=pFirst;
for (int i=1; i<=Idx; i++) pAktu=pAktu->getNext();
return pAktu->getItem();
}

template <class T>
char CLIST<T>::Next()
{
if (pAktu->getNext()->getItem()) return 1;
return 0;
}

template <class T>
int CLIST<T>::addItemToList(T* pI,
					       int (*fcmp)(T *pItList,T *pItNew))
{
for (T* pItemAk=GetFirst();pItemAk!=NULL && fcmp(pItemAk,pI)<=0;pAktu=pAktu->getNext(),pItemAk=pAktu->getItem());
return InsertBefore(pI);
}

template <class T>
CLIST<T> &CLIST<T>::operator=(const CLIST<T> &other)
{
pAktu=other.pAktu;
pFirst=other.pFirst;
pLast=other.pLast;
pItem=NULL;
return *this;
}

/*void main()
{
char *t=new char [7];
strcpy(t,"erster");
char *z=new char [8];
strcpy(z,"zweiter");
char *o=new char [13];
strcpy(o,"eingeschoben");
char* u,*i,*p, *a, *b, *c;
CLIST <char *> R;
R.InsertBehind(&t);
R.InsertTail(&z);
R.InsertBefore(&o);
CLIST <char *> Q;
Q.InsertBehind(&t);
Q.InsertTail(&z);
Q.InsertBefore(&o);
u=*R.GetFirst();
a=*R.GetSelected();
b=*R.GetSelected();
i=*R.GetLast();
p=*R.GetIndexed(2);
cout<<u<<i<<p<<" "<<a<<b<<"\n";
R.RemoveItem();
p=*R.GetLast();
c=*R.GetSelected();
cout<<p<<c<<'\n';
} */
