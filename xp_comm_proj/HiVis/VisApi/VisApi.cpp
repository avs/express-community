// Technische Universität München - Peter.Breitling@in.tum.de
// Please read the file LICENSE for conditions associated with this file.

#include "VisApi.h"

visSystem visSYSTEM;

char* visCell2NodeStrings[visCell2NodeTypes] =
{
  "CellInterpBlock",
  "CellInterpOLinear",
  "CellInterpMLinear"
};

/* -----------------------------------------------------------------------------
   Stream functions
   -------------------------------------------------------------------------- */

int visIsBlank(int c) {
  return c == '\t' || c == ' ';
}

char visTRY(istream &in, char c) {
  char d = in.get();
  if (d == c) {
    //visMsgDbg << "try ok - read char " << d << visMsgEnd;
    return true;
  } else {
    //visMsgDbg << "try not ok - read char " << d << visMsgEnd;
    in.putback(d);
    return false;
  }
}

bool visREADTOKEN(istream &in, int (*m)(int), char* s, int length) {
  char c;
  c = in.get();
  if (in.eof()) return false;
  //visMsgDbg << "read string char " << c << visMsgEnd;
  if (m(c)) {
    if (s && length > 0) s[0] = c;
    int i;
    for (i = 1; i < length, m(c); i++) {
      if (in.eof()) {
	return true;
      }
      c = in.get();
      if (s) s[i] = c;
      //visMsgDbg << "read string char " << c << visMsgEnd;
    }
    in.putback(c);
    //visMsgDbg << "read string put back: " << c << visMsgEnd;
    if (s && length > 0) s[i - 1] = '\0';
    return true;
  } else {
    in.putback(c);
    //visMsgDbg << "read string put back (2): " << c << visMsgEnd;
    return false;
  }
}

/* -----------------------------------------------------------------------------
   visGridLink Methods
   -------------------------------------------------------------------------- */

visGridLink::visGridLink(HiVis_HiVisMods_HiVis_Grid *hiVisFieldAccess)
  : myHiVisFieldAccess(hiVisFieldAccess), myFieldAccess(NULL)
{
};

visGridLink::~visGridLink()
{
};

visAdaptiveGrid *visGridLink::castToAdaptiveGrid()
{
  visMsgDbg << "c1-0 " << visMsgEnd;
  visMsgDbg << "c1-1 " << myFieldAccess->getNumberOfRefs() << visMsgEnd;
  visMsgDbg << "c1-2 " << visMsgEnd;
  if (myFieldAccess && myFieldAccess->instanceOf(visAdaptiveGridId)) {
    return (visAdaptiveGrid*) myFieldAccess;
  }
  return NULL;
}

visStructuredGrid *visGridLink::castToStructuredGrid() 
{
  if (myFieldAccess && myFieldAccess->instanceOf(visStructuredGridId)) {
    return (visStructuredGrid*) myFieldAccess;
  }
  return NULL;
}

visUnstructuredGrid *visGridLink::castToUnstructuredGrid() 
{
  if (myFieldAccess && myFieldAccess->instanceOf(visUnstructuredGridId)) {
    return (visUnstructuredGrid*) myFieldAccess;
  }
  return NULL;
}


/* -----------------------------------------------------------------------------
   visSystem Methods
   -------------------------------------------------------------------------- */

visSystem::visSystem() : myMinUpdatesDelay(0.1f), myLastMsg(NULL),
  myStatus(visStatOk)
{
  char *fname = getenv("VISAPI_LOGFILE");
  if (fname) {
    out = new ofstream(fname, ios::out | ios::app);
    if (!*out) {
      delete out;
      out = NULL;
    }
  } else {
    out = NULL;
  }
}

visSystem::~visSystem()
{
  if (out) {
    delete out;
  }
}

void visSystem::setStatus(char *msg, float progress)
{
  clock_t t = clock();
  if (progress > 1.0f || progress < 0.0f) progress = 0.0f;
  myLastProgress = progress;
  if (msg != myLastMsg 
      || 
      ((float)(t - myLastUpdateTime)) / CLOCKS_PER_SEC > myMinUpdatesDelay) {
    myLastMsg = msg;
    myLastUpdateTime = t;
    OMstatus_check((int)(progress * 100), msg, &myStatus);
  }
}

void visSystem::setStatus(char *msg)
{
  myLastProgress += myMinUpdatesDelay;
  this->setStatus(msg, myLastProgress);
}

int visSystem::getStatus()
{
  return myStatus;
}

/* -----------------------------------------------------------------------------
   visObject Methods
   -------------------------------------------------------------------------- */
visObject::visObject() : myIsAllocated(false), myRefCount(1)
{
}

visObject::~visObject()
{
  this->deallocate();
}

void visObject::operator delete(void *theObject)
{
  if (theObject) {
    ((visObject*)theObject)->decRef();
  }
}

bool visObject::allocate() 
{
  myIsAllocated = true;
  return true;
};

void visObject::deallocate() 
{
  myIsAllocated = false;
};

unsigned int visObject::getNumberOfRefs() const
{
  return myRefCount;
}

void visObject::incRef()
{
  myRefCount++;
}

void visObject::decRef()
{
  if (--myRefCount <= 0) {
    ::delete(this);
  };
}

/* -----------------------------------------------------------------------------
   visProperty Methods
   ---------------------------------------------------------------------------*/

visProperty::visProperty() : myName(NULL) 
{ 
};

visProperty::visProperty(visProperty &x) 
{ 
  int i;
  for (i = 0; i < myValue.getSize(); i++) {
    delete[] myValue.pop();
  }
  if (x.myName) {
    myName = new char[strlen(x.myName)];
    strcpy(myName, x.myName);
  } else {
    myName = NULL;
  }
  for (i = 0; i < x.myValue.getSize(); i++) {
    char *s = new char[strlen(x.myValue[i])];
    strcpy(s, x.myValue[i]);
    myValue.push(s);
  }
};

visProperty::visProperty(char *name, char *value) 
{ 
  visAssert(name);
  myName = new char[strlen(name)];
  strcpy(myName, name);
  myValue.empty();
  if (value) {
    char *s = new char[strlen(value)];
    strcpy(s, value);
    myValue.push(s);
  }
};

visProperty::~visProperty() 
{
  if (myName) { delete[] myName; myName = NULL; }
  int i;
  for (i = 0; i < myValue.getSize(); i++) {
    delete[] myValue.pop();
  }
};

char* visProperty::operator[](unsigned int i)
{
  if (i >= myValue.getSize()) {
    visMsgFatal << "Out of bounds in " << __FILE__ << ":" << __LINE__ 
		<< visMsgFatalEnd;
  }
  return myValue[i];
}

int visProperty::operator<(const visProperty &x) 
{ 
  return strcmp(myName, x.myName) < 0;
};

int visProperty::operator>(const visProperty &x)
{ 
  return strcmp(myName, x.myName) > 0;
};

int visProperty::operator<=(const visProperty &x) 
{ 
  return strcmp(myName, x.myName) <= 0;
};

int visProperty::operator>=(const visProperty &x) 
{ 
  return strcmp(myName, x.myName) >= 0;
};

int visProperty::operator==(const visProperty &x) 
{ 
  return strcmp(myName, x.myName) == 0;
};

void visProperty::operator=(visProperty &x) 
{ 
  if (myName) { delete[] myName; myName = NULL; }
  int i;
  for (i = 0; i < myValue.getSize(); i++) {
    delete[] myValue.pop();
  }
  if (x.myName) {
    myName = new char[strlen(x.myName)];
    strcpy(myName, x.myName);
  } else {
    myName = NULL;
  }
  for (i = 0; i < x.myValue.getSize(); i++) {
    char *s = new char[strlen(x.myValue[i])];
    strcpy(s, x.myValue[i]);
    myValue.push(s);
  }
};

void visProperty::setName(const char *name)
{
  visAssert(name);
  if (myName) { delete[] myName; myName = NULL; }
  myName = new char[strlen(name)];
  strcpy(myName, name);
}

int visProperty::getDim() 
{ 
  return myValue.getSize(); 
}

inline const char *visProperty::getName() { 
  return myName;
};

const char *visProperty::getValue(unsigned int n) 
{ 
  visAssert(n < myValue.getSize());
  return myValue[n]; 
};

void visProperty::pushValue(char *value) 
{ 
  if (value) {
    char *s = new char[strlen(value)];
    strcpy(s, value);
    myValue.push(s);
  }
};

void visProperty::setValue(int d, char **value) 
{ 
  visAssert(value != NULL);
  myValue.empty();
  int i;
  for (i = 0; i < d; i++) {
    visAssert(value[i]);
    char *s = new char[strlen(value[i])];
    strcpy(s, value[i]);
    myValue.push(s);
  }
};

/* -----------------------------------------------------------------------------
   visProperties Methods
   ---------------------------------------------------------------------------*/
visProperties::visProperties() 
{ 
  this->allocate();
};

visProperties::~visProperties() 
{ 
  this->deallocate();
};

bool visProperties::allocate() 
{
  this->deallocate();
  if (!visObject::allocate()) return false;
  return true;
}

bool visProperties::allocate(istream &in)
{
  this->deallocate();
  if (!visObject::allocate()) return false;
  return this->setProperties(in);
}

void visProperties::deallocate()
{
  if (!this->isAllocated()) return;
  visObject::deallocate();
  this->unsetProperties();
}

bool visProperties::setProperties(istream &in)
{
  if (!this->isAllocated()) return false; 
  
  // test if stream ok
  
  visMsgDbg << "setProperties called!" << visMsgEnd;
  if (in.bad() || in.eof() || in.fail()) {
    visMsgError << "bad istream!" << visMsgErrorEnd;
    return false;
  }
  
#define visMAXPROPSLEN 1024
  
  char name[visMAXPROPSLEN], value[visMAXPROPSLEN];
  if (in.bad() || in.eof() || in.fail()) {
    visMsgError << "bad istream!" << visMsgErrorEnd;
    return false;
  }
  
  int i;
  while (!in.bad() && !in.eof() && !in.fail()) {
    visEATWHITE(in);
    if (in.eof()) break;
    if (visTRY(in, '#')) {
      in.getline(name, visMAXPROPSLEN);
      visMsgDbg << "read comment: " << name << visMsgEnd;
    } else {
      if (!visREADTOKEN(in, isalnum, name, visMAXPROPSLEN)) break;
      visREADTOKEN(in, visIsBlank);
      if (!visTRY(in, '=')) {
	if (!strcmp(name, "variable")) {
	  if (!visREADTOKEN(in, isdigit, value, visMAXPROPSLEN)) return 1;
	  visMsgDbg << "read variable " << value << visMsgEnd;
	  in.getline(name, visMAXPROPSLEN);
	} else {
	  visMsgDbg << "unsupported keyword " << name << visMsgEnd;
	  in.getline(name, visMAXPROPSLEN);
	}
	continue;
      } else {
	visREADTOKEN(in, visIsBlank);
	if (!visREADTOKEN(in, isalnum, value, visMAXPROPSLEN)) {
	  return 1;
	}
      }
      visMsgDbg << "read name: " << name << visMsgEnd;
      visMsgDbg << "read value: " << value << visMsgEnd;
      visREADTOKEN(in, visIsBlank);
      while (!visTRY(in, '\n') && !visTRY(in, '#')) {
	visREADTOKEN(in, visIsBlank);
	if (!visREADTOKEN(in, isalnum, value, visMAXPROPSLEN)) {
	  return false;
	}
	visMsgDbg << "read value+: " << value << visMsgEnd;
      }
    }
  }

  return true;
}

bool visProperties::getProperties(ostream &out) 
{
  if (!this->isAllocated()) return false; 
  return true;
}

bool visProperties::hasProperty(char *name)
{
  if (!this->isAllocated()) return false;
  visProperty prop(name);
  return myProperties.exist(prop);
}

visProperty visProperties::getProperty(char *name)
{
  visProperty prop(name);
  myProperties.get(prop);
  return prop;
}

void visProperties::setProperty(visProperty &property)
{
  myProperties.put(property);
}

void visProperties::unsetProperty(char *name)
{
  visProperty prop(name);
  myProperties.remove(prop);
}

void visProperties::unsetProperties()
{
  myProperties.empty();
}

/* -----------------------------------------------------------------------------
   visAttribute Methods
   ---------------------------------------------------------------------------*/
visAttribute::visAttribute() 
{ 
};

visAttribute::~visAttribute() 
{ 
  this->deallocate();
};

bool visAttribute::allocate()
{
  this->deallocate();
  if (!visObject::allocate()) return false;
  return true;
}

void visAttribute::deallocate()
{
  if (!this->isAllocated()) return;
  visObject::deallocate();
}

double *visAttribute::lendBuffer(int dim) 
{
  if (!this->isAllocated()) return NULL;
  
  int i;
  double *buffer = new double[dim];
  for (i = 0; i < dim; i++) {
    buffer[i] = this->getValue(i);
  }
  return buffer;
}

void visAttribute::takebackBuffer(int dim, double *buffer)
{
  if (!this->isAllocated()) return;

  int i;
  for (i = 0; i < dim; i++) {
    this->setValue(i, buffer[i]);
  }
  delete[] buffer;
}


/* -----------------------------------------------------------------------------
   visScalarAttribute Methods
   ---------------------------------------------------------------------------*/

visScalarAttribute::visScalarAttribute()
{ 
};

visScalarAttribute::visScalarAttribute(double x)
{ 
  this->allocate(x);
};

visScalarAttribute::~visScalarAttribute() 
{
  this->deallocate();
};

bool visScalarAttribute::allocate(double x)
{
  this->deallocate();
  if (!visAttribute::allocate()) return false;
  myValue = x;
  return true;
}

void visScalarAttribute::deallocate()
{
  if (!this->isAllocated()) return;
  myValue = 0;
  visAttribute::deallocate();
}

double visScalarAttribute::getValue(int)
{
  return myValue;
}

void visScalarAttribute::setValue(int, visAttrVal val) 
{
  switch (val) {
  case visAttrAddNeutral:
    myValue = 0;
    break;
  case visAttrMultNeutral:
    myValue = 1;
    break;
  case visAttrMin:
    myValue = visMINDOUBLE;
    break;
  case visAttrMax:
    myValue = visMAXDOUBLE;
    break;
  default:
    myValue = 0;
    break;
  }
}

void visScalarAttribute::setValue(int, double x) 
{
  myValue = x;
}

void visScalarAttribute::setValue(int, visAttribute *attr)
{
  myValue = ((visScalarAttribute *)attr)->myValue;
}

double visScalarAttribute::getScalarValue(int)
{
  return myValue; 
}

double *visScalarAttribute::lendBuffer(int) 
{
  return &myValue;
}

void visScalarAttribute::takebackBuffer(int, double *)
{
}

bool visScalarAttribute::isEqual(int, visAttribute *x) 
{ 
  return myValue == x->getScalarValue(1);
}

bool visScalarAttribute::isLess(int dim, visAttribute *x) 
{ 
  return myValue < x->getScalarValue(dim);
}

void visScalarAttribute::add(int dim, visAttribute *x) 
{ 
  myValue += x->getScalarValue(dim);
}

void visScalarAttribute::sub(int dim, visAttribute *x) 
{ 
  myValue -= x->getScalarValue(dim);
}

void visScalarAttribute::mult(int, double x) 
{ 
  myValue *= x;
}

void visScalarAttribute::div(int, double x) 
{ 
  myValue /= x;
}


/* -----------------------------------------------------------------------------
   visVectorAttribute Methods
   ---------------------------------------------------------------------------*/

visVectorAttribute::visVectorAttribute()
{ 
};

visVectorAttribute::visVectorAttribute(int dim, double* x) 
{
  this->allocate(dim, x);
};

visVectorAttribute::~visVectorAttribute() 
{
  this->deallocate();
};

bool visVectorAttribute::allocate(int dim, double* x)
{
  this->deallocate();
  if (!visAttribute::allocate()) return false;
  myValue = new double[dim];
  int i;
  for (i = 0; i < dim; i++) {
    myValue[i] = x[i];
  }
  return true;
}

void visVectorAttribute::deallocate()
{
  if (!this->isAllocated()) return;
  delete[] myValue;
  visAttribute::deallocate();
}

double visVectorAttribute::getValue(int i)
{
  return myValue[i];
}

void visVectorAttribute::setValue(int dim, visAttrVal val) 
{
  int i;
  switch (val) {
  case visAttrAddNeutral:
    for (i = 0; i < dim; i++) {
      myValue[i] = 0;
    }
    break;
  case visAttrMultNeutral:
    for (i = 0; i < dim; i++) {
      myValue[i] = 1;
    }
    break;
  case visAttrMin:
    for (i = 0; i < dim; i++) {
      myValue[i] = 0;
    }
    break;
  case visAttrMax:
    for (i = 0; i < dim; i++) {
      myValue[i] = visMAXDOUBLE;
    }
    break;
  default:
    for (i = 0; i < dim; i++) {
      myValue[i] = 0;
    }
    break;
  }
}

void visVectorAttribute::setValue(int i, double x) 
{
  myValue[i] = x;
}

void visVectorAttribute::setValue(int dim, visAttribute *attr)
{
  int i;
  for (i = 0; i < dim; i++) {
    myValue[i] = ((visVectorAttribute*) attr)->myValue[i];
  }
}

double visVectorAttribute::getScalarValue(int dim)
{
  // initialize scalarvalue with vectorlength
  double myScalarValue = 0;
  int i;
  for (i = 0; i < dim; i++) {
    if (myScalarValue == visMAXDOUBLE) return visMAXDOUBLE;
    myScalarValue += myValue[i] * myValue[i];
  }
  myScalarValue = sqrt(myScalarValue);
  return myScalarValue;
}

double *visVectorAttribute::lendBuffer(int) 
{
  return myValue;
}

void visVectorAttribute::takebackBuffer(int, double *)
{
}


bool visVectorAttribute::isEqual(int dim, visAttribute *x) 
{ 
  int i;
  for (i = 0; i < dim; i++) {
    if (myValue[i] != x->getValue(i)) {
      return false;
    }
  }
  return true;
}

bool visVectorAttribute::isLess(int dim, visAttribute *x) 
{ 
  return this->getScalarValue(dim) < x->getScalarValue(dim);
}

void visVectorAttribute::add(int dim, visAttribute *x) 
{ 
  int i;
  for (i = 0; i < dim; i++) {
    myValue[i] += x->getValue(i);
  }
}

void visVectorAttribute::sub(int dim, visAttribute *x) 
{ 
  int i;
  for (i = 0; i < dim; i++) {
    myValue[i] -= x->getValue(i);
  }
}

void visVectorAttribute::mult(int dim, double x) 
{ 
  int i;
  for (i = 0; i < dim; i++) {
    myValue[i] *= x;
  }
}

void visVectorAttribute::div(int dim, double x) 
{ 
  int i;
  for (i = 0; i < dim; i++) {
    myValue[i] /= x;
  }
}

/* -----------------------------------------------------------------------------
   visScalarAttributeFactory Methods
   ---------------------------------------------------------------------------*/

visScalarAttributeFactory::visScalarAttributeFactory()
{
}

visScalarAttributeFactory::~visScalarAttributeFactory()
{
  this->deallocate();
}

int visScalarAttributeFactory::getDim()
{
  return 1;
}

visAttribute* visScalarAttributeFactory::getNewAttribute() 
{
  return new visScalarAttribute(0);
};

/* -----------------------------------------------------------------------------
   visVectorAttributeFactory Methods
   ---------------------------------------------------------------------------*/

visVectorAttributeFactory::visVectorAttributeFactory(int dim)
{
  myDim = dim;
}

visVectorAttributeFactory::~visVectorAttributeFactory()
{
  this->deallocate();
}

int visVectorAttributeFactory::getDim()
{
  return myDim;
}

visAttribute* visVectorAttributeFactory::getNewAttribute() 
{
  visVectorAttribute* attr = new visVectorAttribute();
  double *x = new double[myDim];
  attr->allocate(myDim, x);
  delete[] x;
  return attr;
};

/* -----------------------------------------------------------------------------
   visCompareAttrAccess Methods
   ---------------------------------------------------------------------------*/

visCompareAttrAccess::visCompareAttrAccess(visAdaptiveGrid *fieldAccess, visAttrVal val) :
    myAttr(NULL), myFieldAccess(fieldAccess) 
{
  if (!myFieldAccess) {
    visMsgError << "visCompareAttrAccess called with uninitialzed parameters"
		<< visMsgErrorEnd;
    myFieldAccess = NULL;
    return;
  }
  if (!myFieldAccess->isAllocated()) {
    visMsgError << "visCompareAttrAccess called with unallocated fieldAccess"
		<< visMsgErrorEnd;
    myFieldAccess = NULL;
    return;
  }
  myFieldAccess->incRef();
  myCompareAttr = new visAttribute*[myFieldAccess->getAttributes()];
  int i;
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    visAttributeFactory *factory = myFieldAccess->getAttrFactory(i);
    myCompareAttr[i] = factory->getNewAttribute();
    myCompareAttr[i]->setValue(factory->getDim(), val);
    factory->decRef();
  }
}

visCompareAttrAccess::~visCompareAttrAccess() 
{
  int i;
  if (myFieldAccess) {
    for (i = 0; i < myFieldAccess->getAttributes(); i++) {
      myCompareAttr[i]->decRef();
    }
    delete[] myCompareAttr;
    myFieldAccess->decRef();
  }
}

int visCompareAttrAccess::getSize() 
{
  return 2;
}

int visCompareAttrAccess::getAttributes() 
{
  if (!myFieldAccess) return 0;
  return myFieldAccess->getAttributes();
}

visAttribute *visCompareAttrAccess::getCellAttribute(int n, int t) 
{
  if (!myAttr) {
    visMsgError << "visCompareAttrAccess::getCellAttribute(int, int)"
		<< ": attr not initialized with setCompareAttribute()"
		<< visMsgErrorEnd;
    return NULL;
  }
  switch (n) {
  case 0:
    myCompareAttr[t]->incRef();
    return myCompareAttr[t];
  case 1:
    myAttr[t]->incRef();
    return myAttr[t];
  default:
    visMsgError << "visCompareAttrAccess::getCellAttribute(int, int)"
		<< ": 1. parameter out of bounds"
		<< visMsgErrorEnd;
    return NULL;
  }
}

visAttributeFactory *visCompareAttrAccess::getAttrFactory(int t)
{
  return myFieldAccess->getAttrFactory(t);
}

void visCompareAttrAccess::setCompareAttribute(visAttribute **attr) 
{
  myAttr = attr;
}

/* -----------------------------------------------------------------------------
   visAttrCombiner Methods
   ---------------------------------------------------------------------------*/

visAttrCombiner::visAttrCombiner(visAttributeFactory *factory) 
{ 
  myAttrFactory = factory;
  myAttrFactory->incRef();
}

visAttrCombiner::~visAttrCombiner() 
{ 
  this->deallocate();
  myAttrFactory->decRef();
}

bool visAttrCombiner::allocate(visAttribute *attr)
{
  this->deallocate();
  if (!visObject::allocate()) return false;
  myAttr = attr;
  if (myAttr) {
    myAttr->incRef();
  }
  return true;
}

void visAttrCombiner::deallocate()
{
  if (!this->isAllocated()) return;
  if (myAttr) {
    myAttr->decRef();
  }
  int i;
  for (i = 0; i < myAttrCombinerStack.getSize(); i++) {
    myAttrCombinerStack[i]->decRef();
  }
  myAttrCombinerStack.empty();
  visObject::deallocate();
}

void visAttrCombiner::addAttrCombiner(visAttrCombiner *attrCombiner)
{
  visAssert(attrCombiner);
  myAttrCombinerStack.push(attrCombiner);
}

visAttribute *visAttrCombiner::getCombinedAttr()
{
  return NULL;
}

visAttributeFactory *visAttrCombiner::getAttrFactory()
{
  myAttrFactory->incRef();
  return myAttrFactory;
}

/* -----------------------------------------------------------------------------
   visAttrCombDefault Methods
   ---------------------------------------------------------------------------*/

visAttrCombinerDefault::visAttrCombinerDefault(visAttributeFactory *factory) :
  visAttrCombiner(factory)
{
}

visAttrCombinerDefault::~visAttrCombinerDefault()
{
  this->deallocate();
}

bool visAttrCombinerDefault::allocate(visAttribute *attr)
{
  this->deallocate();
  if (!visAttrCombiner::allocate(attr)) return false;

  myIsEqual = true;
  return true;
}

void visAttrCombinerDefault::addAttrCombiner(visAttrCombiner *attrCombiner)
{
  visAttrCombiner::addAttrCombiner(attrCombiner);
  visAttribute *attr = attrCombiner->getCombinedAttr();
  visAssert(attr);
  int dim = myAttrFactory->getDim();

  // Check if attrCombiner has same dimension
  visAttributeFactory *factory = attrCombiner->getAttrFactory();
  visAssert(dim == factory->getDim());
  factory->decRef();

  if (!attr->isEqual(dim, myAttr)) {
    myIsEqual = false;
  }
  attr->decRef();
}

bool visAttrCombinerDefault::mayCombineAverage() 
{
  return myIsEqual;
};

bool visAttrCombinerDefault::mayCombineRemove() 
{
  int i;
  for (i = 0; i < myAttrFactory->getDim(); i++) {
    if (myAttr->getValue(i) != 0) {
      return false;
    }
  }
  return true;
};

visAttribute *visAttrCombinerDefault::getCombinedAttr()
{
  myAttr->incRef();
  return myAttr;
};

/* -----------------------------------------------------------------------------
   visAttrComb Methods
   ---------------------------------------------------------------------------*/

visAttrComb::visAttrComb() 
{ 
}

visAttrComb::~visAttrComb() 
{ 
  this->deallocate();
}

bool visAttrComb::allocate(visAttributeAccess *attrAccess)
{
  this->deallocate();
  if (!visObject::allocate()) return false;
  myAttrAccess = attrAccess;
  myAttrAccess->incRef();
  return true;
}

void visAttrComb::deallocate()
{
  if (!this->isAllocated()) return;
  myAttrAccess->decRef();
  visObject::deallocate();
}

visAttributeAccess *visAttrComb::getAttrAccess(void) 
{
  myAttrAccess->incRef();
  return myAttrAccess;
};

/* -----------------------------------------------------------------------------
   visAttrCombDefault Methods
   ---------------------------------------------------------------------------*/

visAttrCombDefault::visAttrCombDefault()
{
}

visAttrCombDefault::~visAttrCombDefault()
{
  this->deallocate();
}

bool visAttrCombDefault::allocate(visAttributeAccess *attrAccess)
{
  this->deallocate();
  if (!visAttrComb::allocate(attrAccess)) return false;

  // Check if equal
  
  int i, j;
  myIsEqual = true;
  for (j = 0; j < myAttrAccess->getAttributes(); j++) {
    visAttributeFactory* attrFactory = myAttrAccess->getAttrFactory(j);
    int dim = attrFactory->getDim();
    attrFactory->decRef();
    visAttribute *x = NULL;
    for (i = 0; i < myAttrAccess->getSize(), x == NULL; i++) {
      x = myAttrAccess->getCellAttribute(i, j);
    }
    for (; i < myAttrAccess->getSize(); i++) {
      visAttribute *y = myAttrAccess->getCellAttribute(i, j);
      if (y) {
	myIsEqual = x->isEqual(dim, y) && myIsEqual;
	y->decRef();
      }
    }
    if (x) x->decRef();
  }
  return true;
}

void visAttrCombDefault::deallocate()
{
  if (!this->isAllocated()) return;
  visAttrComb::deallocate();
}

bool visAttrCombDefault::isEqual() 
{
  return myIsEqual;
};

visAttribute *visAttrCombDefault::getAverageAttribute(int attr)
{
  visAttributeFactory* attrFactory = myAttrAccess->getAttrFactory(attr);
  visAttribute *average = attrFactory->getNewAttribute();
  int dim = attrFactory->getDim();
  attrFactory->decRef();
  average->setValue(dim, visAttrAddNeutral);
  int i, divisor = 0;
  for (i = 0; i < myAttrAccess->getSize(); i++) {
    visAttribute *x = myAttrAccess->getCellAttribute(i, attr);
    if (x) {
      average->add(dim, x);
      x->decRef();
      divisor++;
    }
  }
  if (divisor) {
    average->div(dim, divisor);
  }
  return average;
};

/* -----------------------------------------------------------------------------
   visAttrCombEpsilon Methods
   ---------------------------------------------------------------------------*/

visAttrCombEpsilon::visAttrCombEpsilon(int attr, double epsilon,
				       visAttrCombCalculateByType calcBy,
				       visAttrCombApplyOnType applyOn) 
{
  myEpsilon = epsilon;
  myAttr = attr;
  myCalcBy = calcBy;
  myApplyOn = applyOn;
}

visAttrCombEpsilon::~visAttrCombEpsilon()
{
  this->deallocate();
}

bool visAttrCombEpsilon::allocate(visAttributeAccess *attrAccess)
{
  this->deallocate();
  if (!visAttrComb::allocate(attrAccess)) return false;

  // Check if we may combine

  myDoCombine = true;
  
  // Check if maximum scalar difference to average is less than eps
  
  myIsEqual = myDoCombine;
  
  if (myAttr >= myAttrAccess->getAttributes()) {
    myAttr = 0;
  }
  visAttributeFactory* attrFactory = myAttrAccess->getAttrFactory(myAttr);
  int dim = attrFactory->getDim();

  if (myDoCombine) {
    if (myApplyOn == visAttrCombApplyOnAvgDiff) {
      visAttribute *cmp;
      cmp = getAverageAttribute(myAttr);
      if (myCalcBy == visAttrCombCalculateByScalarDiff) {
	double scalar = cmp->getScalarValue(dim);
	double diff;
	int i;
	for (i = 0; i < myAttrAccess->getSize(); i++) {
	  visAttribute *x = myAttrAccess->getCellAttribute(i, myAttr);
	  if (x) {
	    diff = scalar - x->getScalarValue(dim);
	    if (diff < 0) { diff = -diff; };
	    if (diff > myEpsilon) {
	       myIsEqual = false;
	      break;
	    }
	    x->decRef();
	  }
	}
      } else if (myCalcBy == visAttrCombCalculateByVectorDiff) {
	visAttribute *y = attrFactory->getNewAttribute();
	double diff;
	int i;
	for (i = 0; i < myAttrAccess->getSize(); i++) {
	  y->setValue(dim, cmp);
	  visAttribute *x = myAttrAccess->getCellAttribute(i, myAttr);
	  if (x) {
	    y->sub(dim, x);
	    diff = y->getScalarValue(dim);
	    if (diff < 0) { diff = -diff; };
	    if (diff > myEpsilon) {
	      myIsEqual = false;
	      break;
	    }
	    x->decRef();
	  }
	}
	y->decRef();
      } else {
	visMsgError << "visAttrCombEpsilon: unsupported Calculation Type"
		    << visMsgErrorEnd;
	myIsEqual = false;
	return false;
      }
      cmp->decRef();
    } else if (myApplyOn == visAttrCombApplyOnMaxDiff) {
      myIsEqual = false;
    } else {
      visMsgError << "visAttrCombEpsilon: unsupported Apply Type"
		  << visMsgErrorEnd;
      myIsEqual = false;
      return false;
    }
  }

  attrFactory->decRef();
  myDoCombine = myIsEqual;
  
  return true;
}

void visAttrCombEpsilon::deallocate()
{
  if (!this->isAllocated()) return;
  visAttrComb::deallocate();
}

bool visAttrCombEpsilon::isEqual()
{
  return myIsEqual;
};

visAttribute *visAttrCombEpsilon::getAverageAttribute(int attr) 
{
  // Calculate average
  
  visAttributeFactory* attrFactory = myAttrAccess->getAttrFactory(attr);
  visAttribute *average = attrFactory->getNewAttribute();
  int dim = attrFactory->getDim();
  attrFactory->decRef();
  average->setValue(dim, visAttrAddNeutral);
  int i, divisor = 0;
  for (i = 0; i < myAttrAccess->getSize(); i++) {
    visAttribute *x = myAttrAccess->getCellAttribute(i, attr);
    if (x) {
      average->add(dim, x);
      x->decRef();
      divisor++;
    }
  }
  if (divisor) {
    average->div(dim, divisor);
  }
  return average;
}

/* -----------------------------------------------------------------------------
   visGrid Methods
   ---------------------------------------------------------------------------*/
visGrid *visGrid::readField(istream &in)
{
  
  visMsgDbg << "(a)"  << visMsgEnd;
  visProperties *properties = new visProperties();
  visMsgDbg << "(b)"  << visMsgEnd;
  visProperty prop1("prop1_name");
  visMsgDbg << "(d)"  << visMsgEnd;
  prop1.pushValue("prop1_value1");
  visMsgDbg << "(e)"  << visMsgEnd;
  prop1.pushValue("prop1_value2");
  visMsgDbg << "(f)"  << visMsgEnd;
  visProperty prop2(prop1);
  visMsgDbg << prop2.getName() << "!!!"  << visMsgEnd;
  prop2.setName("prop2_name");
  prop2.pushValue("prop2_value2");
  visAssert(prop1 == visProperty("prop1_name"));
  visAssert(prop1.getDim() == 2);
  properties->setProperty(prop2);
  properties->setProperty(prop1);
  visAssert(properties->hasProperty("prop1_name"));
  visProperty propa("aaa");
  visProperty propb("aaa");
  visAssert(!(propa < propb));
  visAssert(!(propa > propb));
  visAssert((propa == propb));
  if(properties->hasProperty("prop1_nae")) {
    visMsgDbg << "has it!" << visMsgEnd;
  } else {
    visMsgDbg << "has not it!" << visMsgEnd;
  }
  visProperty prop3 = properties->getProperty("prop2_name");
  visAssert(prop3.getDim() == 3);
  
  visMsgDbg << prop1.getName() << "="
	    << prop1.getValue() << ","
	    << prop1.getValue(1) << visMsgEnd;
  visMsgDbg << prop3.getName() << "="
	    << prop3.getValue() << ","
	    << prop3.getValue(1) << visMsgEnd;
  visAssert(properties->hasProperty("prop1_name"));
  properties->unsetProperty("prop1_name");
  //visAssert(!properties->hasProperty("prop1_name"));
  visAssert(properties->hasProperty("prop2_name"));
  properties->unsetProperties();
  //visAssert(!properties->hasProperty("prop2_name"));
  
  if (!properties->allocate(in)) {
    return NULL;
  }
  
  return NULL;
}

bool visGrid::writeField(ostream &out)
{
  return false;
}

visGrid::visGrid()
{
};
  
visGrid::~visGrid() 
{
  this->deallocate();
};

bool visGrid::allocate(int dim, int *size,
		       int attrCount, visAttributeFactory** factory,
		       visProperties *properties)
{
  this->deallocate();
  if (!visObject::allocate()) return false;
  myDim = dim;
  mySize = new int[myDim];
  int i;
  for (i = 0; i < myDim; i++) {
    mySize[i] = size[i];
  }
  myAttrCount = attrCount;
  myAttrFactory = new visAttributeFactory*[attrCount];
  for (i = 0; i < myAttrCount; i++) {
    myAttrFactory[i] = factory[i];
    myAttrFactory[i]->incRef();
  }
  if (!properties) {
    myProperties = new visProperties();
  } else {
    myProperties = properties;
    myProperties->incRef();
  }
  return true;
}
  
void visGrid::deallocate()
{
  if (!this->isAllocated()) return;
  int i;
  for (i = 0; i < myAttrCount; i++) {
    myAttrFactory[i]->decRef();
  }
  delete[] myAttrFactory;
  myAttrCount = 0;
  delete[] mySize;
  myDim = 0;

  visObject::deallocate();
}

int visGrid::getCellTypes()
{
  return 1;
}

void visGrid::getSize(int *d)
{
  int i;
  for (i = 0; i < this->getDim(); i++) {
    d[i] = this->getSize(i);
  }
};

void visGrid::getCursor(int *p)
{
  int i;
  for (i = 0; i < this->getDim(); i++) {
    p[i] = this->getCursor(i);
  }
};

visAttributeFactory* visGrid::getAttrFactory(int comp)
{
  if (!this->isAllocated()) return NULL;
  myAttrFactory[comp]->incRef();
  return myAttrFactory[comp];
}

visAttribute* visGrid::getCellAttribute(int t)
{
  if (!this->isAllocated()) return NULL;
  
  visAttribute *attr = myAttrFactory[t]->getNewAttribute();
  this->getCellAttribute(t, attr);
  return attr;
}  

visAttribute *visGrid::getNodeAttribute(int t, int n)
{
  if (!this->isAllocated()) return NULL;
  visAttribute *attr = myAttrFactory[t]->getNewAttribute();
  this->getNodeAttribute(t, attr, n);
  return attr;
}

void visGrid::getNodeAttribute(int t, visAttribute *attr, int)
{
  if (!this->isAllocated()) return;
  this->getCellAttribute(t, attr);
}

/* -----------------------------------------------------------------------------
   visUnstructuredGrid Methods
   ---------------------------------------------------------------------------*/

visUnstructuredGrid::visUnstructuredGrid()
{
}

visUnstructuredGrid::~visUnstructuredGrid() 
{
  this->deallocate();
}

bool visUnstructuredGrid::allocate(int dim, int *size, int attrCount,
				   visAttributeFactory** factory) 
{
  this->deallocate();
  if (!visGrid::allocate(dim, size, attrCount, factory)) return false;
  return true;
}
  
void visUnstructuredGrid::deallocate()
{
  if (!this->isAllocated()) return;
  visGrid::deallocate();
}

/* -----------------------------------------------------------------------------
   visStructuredGrid Methods
   ---------------------------------------------------------------------------*/

visStructuredGrid::visStructuredGrid()
{
};
  
visStructuredGrid::~visStructuredGrid() 
{
  this->deallocate();
};

bool visStructuredGrid::allocate(int dim, int *size, int attrCount,
				   visAttributeFactory** factory) 
{
  this->deallocate();
  if (!visGrid::allocate(dim, size, attrCount, factory)) return false;
  return true;
}
  
void visStructuredGrid::deallocate()
{
  if (!this->isAllocated()) return;
  visGrid::deallocate();
}

/* -----------------------------------------------------------------------------
   visAdaptiveGrid Methods
   ---------------------------------------------------------------------------*/

visAdaptiveGrid::visAdaptiveGrid()
{
};
  
visAdaptiveGrid::~visAdaptiveGrid() 
{
  this->deallocate();
};

bool visAdaptiveGrid::allocate(int dim, int *size,
			       int attrCount, visAttributeFactory** factory) 
{
  this->deallocate();
  if (!visStructuredGrid::allocate(dim, size, attrCount, factory)) return false;
  myIntPos = new int[dim];
  myFloatPos = new float[dim];
  myFloatPosBuf = new float[dim];
  myEdgeAttribute = new visAttribute*[attrCount];
  int a;
  for (a = 0; a < attrCount; a++) {
    myEdgeAttribute[a] = factory[a]->getNewAttribute();
  }
  return true;
}
  
void visAdaptiveGrid::deallocate()
{
  if (!this->isAllocated()) return;
  int a;
  for (a = 0; a < this->getAttributes(); a++) {
    myEdgeAttribute[a]->decRef();
  }
  delete[] myEdgeAttribute;
  delete[] myFloatPosBuf;
  delete[] myFloatPos;
  delete[] myIntPos;
  visStructuredGrid::deallocate();
}

void visAdaptiveGrid::getCellPos(int *p)
{
  int i;
  for (i = 0; i < this->getDim(); i++) {
    p[i] = this->getCellPos(i);
  }
};

void visAdaptiveGrid::getCellSize(int *d)
{
  int i;
  for (i = 0; i < this->getDim(); i++) {
    d[i] = this->getCellSize(i);
  }
};

bool visAdaptiveGrid::setCursorF(float *pos) 
{
  int dim = this->getDim();
  int i;
  myNode = 0;
  for (i = 0; i < dim; i++) {
    myIntPos[i] = (int) pos[i];
    if (pos[i] >= this->getSize(i)) {
      myIntPos[i]--;
      myNode |= 1 << i;
    }
  }
  for (i = 0; i < dim; i++) {
    if (myIntPos[i] < 0 || myIntPos[i] >= this->getSize(i)) {
      return false;
    }
  }
  this->setCursor(myIntPos);
  return true;
}

bool visAdaptiveGrid::setCursorN(int *pos) 
{
  int i;
  myNode = 0;
  for (i = 0; i < myDim; i++) {
    myIntPos[i] =  pos[i];
    if (pos[i] >= this->getSize(i)) {
      myIntPos[i]--;
      myNode |= 1 << i;
    }
  }
  for (i = 0; i < myDim; i++) {
    visAssert(myIntPos[i] >= 0 && myIntPos[i] < this->getSize(i));
  }
  this->setCursor(myIntPos);
  return true;
}

void visAdaptiveGrid::getAttributeN(int comp, visAttribute *attr)
{
  this->getNodeAttribute(comp, attr, myNode);
}

void visAdaptiveGrid::getInterpAttribute(int comp, visAttribute *attr, 
					 float *pos)
{
  double div = 1;
  int i, j, dim = myDim, attrDim = myAttrFactory[comp]->getDim();
  float *&negPos = myFloatPosBuf;
  for (i = 0; i < dim; i++) {
    div *= this->getCellSize(i);
    negPos[i] = - pos[i] + this->getCellSize(i);
  }
  double mult;
  visAttribute *multAttr = myAttrFactory[comp]->getNewAttribute();
  attr->setValue(attrDim, visAttrAddNeutral);
  for (i = 0; i < (1 << dim); i++) {
    mult = 1;
    for (j = 0; j < dim; j++) {
      mult *= ((i & (1 << j)) != 0 ? pos[j] : negPos[j]);
    }
    this->getNodeAttribute(comp, myEdgeAttribute[comp], i);
    multAttr->setValue(attrDim, myEdgeAttribute[comp]);
    multAttr->mult(attrDim, mult);
    attr->add(attrDim, multAttr);
  }
  attr->div(attrDim, div);
  multAttr->decRef();
}

void visAdaptiveGrid::getCellAttributeF(int comp, visAttribute *attr,
					float *pos) 
{
  int i;
  int dim = this->getDim();
  for (i = 0; i < dim; i++) {
    myFloatPos[i] = pos[i] - this->getCellPos(i);
  }

  //  visMsgDbg << "getCellAttributeF called" << visMsgEnd;
  //  for (i = 0; i < dim; i++) {
    //visMsgDbg << "pos[" << i << "] = " << pos[i] << visMsgEnd;
    //    visMsgDbg << "cursorPos[" << i << "] = " << this->getCursor(i) << visMsgEnd;
    //    visMsgDbg << "cellPos[" << i << "] = " << this->getCellPos(i) << visMsgEnd;
    //    visMsgDbg << "cellSize[" << i << "] = " << this->getCellSize(i) << visMsgEnd;
    //visMsgDbg << "myFloatPos[" << i << "] = " << myFloatPos[i] << visMsgEnd;
  //}

  this->getInterpAttribute(comp, attr, myFloatPos);

  //  for (i = 0; i < dim; i++) {
  //visMsgDbg << "attr[" << i << "] = " << attr->getValue(i) << visMsgEnd;
  //}
}

/*void visAdaptiveGrid::setAttributeFactory(int comp, visAttributeFactory* factory)
{
  if (!this->isAllocated()) return;
  myAttrFactory[comp]->decRef();
  myAttrFactory[comp] = factory;
  myAttrFactory[comp]->incRef();
  }*/

/* -----------------------------------------------------------------------------
   visCell2Node Methods
   ---------------------------------------------------------------------------*/

bool visCell2Node::allocate(int dim, int *size,
			     int attrCount, visAttributeFactory** factory) 
{
  this->deallocate();
  if (!visAdaptiveGrid::allocate(dim, size,
				 attrCount, factory)) return false;
  return true;
}
  
void visCell2Node::deallocate()
{
  if (!this->isAllocated()) return;

  visAdaptiveGrid::deallocate();
}

/* -----------------------------------------------------------------------------
   visNode2Cell Methods
   ---------------------------------------------------------------------------*/

bool visNode2Cell::allocate(int dim, int *size,
			    int attrCount, visAttributeFactory** factory) 
{
  this->deallocate();
  if (!visAdaptiveGrid::allocate(dim, size,
				 attrCount, factory)) return false;
  return true;
}
  
/* -----------------------------------------------------------------------------
   visCell2NodeDirect Methods
   ---------------------------------------------------------------------------*/

visCell2NodeDirect::visCell2NodeDirect() 
{
}

visCell2NodeDirect::~visCell2NodeDirect() 
{
  this->deallocate();
}

bool visCell2NodeDirect::allocate(visAdaptiveGrid* fieldAccess)
{
  visMsgCall << "visCell2NodeDirect::allocate: called." << visMsgEnd;
  if (!fieldAccess || !fieldAccess->isAllocated()) {
    visMsgError << "visCell2NodeDirect::allocate"
		<< ": called with unallocated fieldAccess."
		<< visMsgErrorEnd;
    return false;
  }
  
  if (fieldAccess == this) {
    visMsgError << "visCell2NodeDirect::allocate"
		<< ": called with own instance."
		<< visMsgErrorEnd;
    return false;
  }
  
  this->deallocate();

  myFieldAccess = fieldAccess;
  myFieldAccess->incRef();

  int dim = myFieldAccess->getDim();
  int *size = new int[dim];
  int i;
  for (i = 0; i < dim; i++) {
    size[i] = myFieldAccess->getSize(i);
  }
  visAttributeFactory** factory =
    new visAttributeFactory*[myFieldAccess->getAttributes()];
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    factory[i] = myFieldAccess->getAttrFactory(i);
  }
  if (!visCell2Node::allocate(dim, size,
				myFieldAccess->getAttributes(), factory))
    return false;
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    factory[i]->decRef();
  }
  delete[] factory;
  delete[] size;

  // Set Cursor to origin
  {
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->setCursor(pos);
    delete[] pos;
  }

  return true;
}

void visCell2NodeDirect::deallocate() 
{
  visMsgCall << "visCell2NodeDirect::deallocate: called." << visMsgEnd;
  if (!this->isAllocated()) return;
  myFieldAccess->decRef();
  myFieldAccess = NULL;
  visAdaptiveGrid::deallocate();
}

void visCell2NodeDirect::setCursor(const int *pos) 
{
  if (!this->isAllocated()) return;
  myFieldAccess->setCursor(pos);
}

int visCell2NodeDirect::getCursor(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  return myFieldAccess->getCursor(i);
}

visAttribute* visCell2NodeDirect::getCellAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getCellAttribute(comp);
}

void visCell2NodeDirect::getCellAttribute(int comp, visAttribute* attr)
{
  if (!this->isAllocated()) return;
  myFieldAccess->getCellAttribute(comp, attr);
}

void visCell2NodeDirect::getInterpAttribute(int comp,
					    visAttribute *attr,
					    float *)
{
  if (!this->isAllocated()) return;
  myFieldAccess->getCellAttribute(comp, attr);
}

const char *visCell2NodeDirect::getAttrName(int comp) 
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getAttrName(comp);
};

const visAttribute* visCell2NodeDirect::getMinAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getMinAttribute(comp);
};

const visAttribute* visCell2NodeDirect::getMaxAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getMaxAttribute(comp);
};

int visCell2NodeDirect::getCellPos(unsigned int i)
{
  if (!this->isAllocated()) return 0;
  return myFieldAccess->getCellPos(i);
};

int visCell2NodeDirect::getCellSize(unsigned int i)
{
  if (!this->isAllocated()) return 1;
  return myFieldAccess->getCellSize(i);
};

int visCell2NodeDirect::getCells() 
{
  if (!this->isAllocated()) return 0;
  return myFieldAccess->getCells();
};

/* -----------------------------------------------------------------------------
   visCell2NodeBlock Methods
   ---------------------------------------------------------------------------*/

visCell2NodeBlock::visCell2NodeBlock()
{
}

visCell2NodeBlock::visCell2NodeBlock(visAdaptiveGrid* fieldAccess)
{
  this->allocate(fieldAccess);
}

visCell2NodeBlock::~visCell2NodeBlock()
{
  this->deallocate();
}

bool visCell2NodeBlock::allocate(visAdaptiveGrid* fieldAccess)
{
  visMsgCall << "visCell2NodeBlock::allocate: called." << visMsgEnd;

  // Check if cell field is allocated

  if (!fieldAccess || !fieldAccess->isAllocated()) {
    visMsgError << "visCell2NodeBlock::allocate"
		<< ": called with unallocated fieldAccess."
		<< visMsgErrorEnd;
    return false;
  }
  
  // Check if field is cell associated

  if (!(fieldAccess->getAttrAssoc() & visHasCellData)) {
    visMsgError << "visCell2NodeBlock::allocate"
		<< ": field must contain cell associated data."
		<< visMsgErrorEnd;
    return false;
  }
  
  // Check if call is recursive which is not allowed

  if (fieldAccess == this) {
    visMsgError << "visCell2NodeBlock::allocate"
		<< ": called with own instance."
		<< visMsgErrorEnd;
    return false;
  }
  
  // deallocate and allocate according to new field

  this->deallocate();

  myFieldAccess = fieldAccess;
  myFieldAccess->incRef();
  int dim = myFieldAccess->getDim();
  int *size = new int[dim];
  int i, j;
  int numberOfCellsMax = 1;
  for (i = 0; i < dim; i++) {
    size[i] = myFieldAccess->getSize(i) - 1;
    //size[i] = myFieldAccess->getSize(i);
    numberOfCellsMax *= size[i];
  }
  visAttributeFactory** factory =
    new visAttributeFactory*[myFieldAccess->getAttributes()];
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    factory[i] = myFieldAccess->getAttrFactory(i);
  }
  if (!visCell2Node::allocate(dim, size,
			      myFieldAccess->getAttributes(), factory))
    return false;
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    factory[i]->decRef();
  }
  delete[] factory;
  delete[] size;

  myCellPos = new int[dim];
  myCellSize = new int[dim];

  myAttrDim = new int[fieldAccess->getAttributes()];
  myEdgeAttribute = new visAttribute**[fieldAccess->getAttributes()];
  for (i = 0; i < fieldAccess->getAttributes(); i++) {
    myEdgeAttribute[i] = new visAttribute*[1 << dim];
    for (j = 0; j < (1 << dim); j++) {
      myEdgeAttribute[i][j] = myAttrFactory[i]->getNewAttribute();
    }
    myAttrDim[i] = myAttrFactory[i]->getDim();
  }
  myCursorBuf = new int[dim];
  myCellPosBuf = new int[dim];
  myCellSizeBuf = new int[dim];
  myFloatPosBuf = new float[dim];

  visAttributeEnumeration *e = new visAdaptiveGridCellEnumeration(this);
  
  // variables for percentage status
  
  int percent = 0;

  myCells = 0;
  e->setCursorFirstElement();
  while (e->isValidElement()) {
    myCells++;

    // update percentage status
    
    {
      int percentCMP = myCells * 100 / numberOfCellsMax;
      if (percentCMP >= percent - 1) {
	OMstatus_check(percent, "HiVis Grid Interp Block", NULL);
	percent += 10;
      }
    }
    
    e->setCursorNextElement();
  }
  e->decRef();

  visSETSTATUSIDLE;

  // Set Cursor to origin
  {
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->setCursor(pos);
    delete[] pos;
  }

  return true;
}

void visCell2NodeBlock::deallocate()
{
  if (!this->isAllocated()) return;
  int i, j;
  for (i = 0; i < this->getAttributes(); i++) {
    for (j = 0; j < (1 << this->getDim()); j++) {
      myEdgeAttribute[i][j]->decRef();
    }
    delete[] myEdgeAttribute[i];
  }
  delete[] myEdgeAttribute;
  delete[] myFloatPosBuf;
  delete[] myCellPosBuf;
  delete[] myCellSizeBuf;
  delete[] myCursorBuf;
  delete[] myAttrDim;
  myCells = 0;
  delete[] myCellSize;
  delete[] myCellPos;
  myFieldAccess->decRef();
  myFieldAccess = NULL;
  visAdaptiveGrid::deallocate();
}
  
void visCell2NodeBlock::setCursor(const int *pos)
{
  if (!this->isAllocated()) return;
  
  /*  visMsgDbg << "renderblock setCursor called:" << visMsgEnd;
  for (i = 0; i < this->getDim(); i++) {
    visMsgDbg << "  pos[" << i << "] = " << pos[i] << visMsgEnd;
  }*/

  int i, j, k, comp;

  int dim = this->getDim();
  
  myFieldAccess->setCursor(pos);
  for (i = 0; i < dim; i++) {
    myCellPosBuf[i] = myFieldAccess->getCellPos(i);
    myCellSizeBuf[i] = myFieldAccess->getCellSize(i) - 1;
  }

  for (i = 0; i < dim; i++) {
    myCellPos[i] = myFieldAccess->getCellPos(i);
    myCellSize[i] = myFieldAccess->getCellSize(i) - 1;
  }

  myHasEqualEdges = true;
  for (i = 0; i < dim; i++) {
    if (pos[i] >= myCellPosBuf[i] + myCellSizeBuf[i]) {
      myHasEqualEdges = false;
      for (j = 0; j < dim; j++) {
	myCursorBuf[j] = pos[j];
      }
      myCursorBuf[i]++;
      myCellPos[i] = pos[i];
      myCellSize[i] = 1;
      myFieldAccess->setCursor(myCursorBuf);
      for (j = 0; j < dim; j++) {
	if (myCellSize[j] > 1) {
	  if (pos[j] >= myFieldAccess->getCellPos(j) + 
	      myFieldAccess->getCellSize(j) - 1) {
	    myCellPos[j] = pos[j];
	    myCellSize[j] = 1;
	  } else {
	    int t = max(myCellPos[j], myFieldAccess->getCellPos(j));
	    myCellSize[j] = min(myCellPos[j] + myCellSize[j] - t,
				myFieldAccess->getCellPos(j) + 
				myFieldAccess->getCellSize(j) - 1 - t);
	    myCellPos[j] = t;
	  }
	}
      }
    }
  }
  
  for (i = 0; i < dim; i++) {
    visAssert(myCellSize[i] > 0);
    visAssert(myCellPos[i] >= myCellPosBuf[i]);
  }
  if (myHasEqualEdges) {
    for (comp = 0; comp < this->getAttributes(); comp++) {
      myFieldAccess->getCellAttribute(comp, myEdgeAttribute[comp][0]);
    }
  } else {
    for (k = (1 << dim) - 1; k >= 0; k--) {
      for (j = 0; j < dim; j++) {
	myCursorBuf[j] = myCellPos[j] + 
	  ((k & (1 << j)) != 0 ? myCellSize[j] : 0);
      }
      /*	for (j = 0; j < dim; j++) {
		visMsgDbg << "  myCursorBuf[" << j << "] = " << myCursorBuf[j] << visMsgEnd;
		}*/
      myFieldAccess->setCursor(myCursorBuf);
      for (comp = 0; comp < this->getAttributes(); comp++) {
	myFieldAccess->getCellAttribute(comp, myEdgeAttribute[comp][k]);
	/*	  visMsgDbg << "edge: " << k << ", dim: " 
		  << myEdgeAttribute[comp][k]->getDim()
		  << visMsgEnd;
		  visMsgDbg << "value: " << myEdgeAttribute[comp][k]->getScalarValue()
		  << visMsgEnd;*/
      }
    }
  }    
}

void visCell2NodeBlock::getNodeAttribute(int comp, 
					    visAttribute *attr,
					    int edge)
{
  if (!this->isAllocated()) return;

  if (myHasEqualEdges) {
    attr->setValue(myAttrDim[comp], myEdgeAttribute[comp][0]);
  } else {
    attr->setValue(myAttrDim[comp], myEdgeAttribute[comp][edge]);
  }
}

void visCell2NodeBlock::getInterpAttribute(int comp,
					   visAttribute *attr,
					   float *pos)
{
  if (!this->isAllocated()) return;

  if (myHasEqualEdges) {
    //    visMsgDbg << "visCell2NodeBlock: equal edges." << visMsgEnd;
    attr->setValue(myAttrDim[comp], myEdgeAttribute[comp][0]);
  } else {
    //    visMsgDbg << "visCell2NodeBlock: interpolating..." << visMsgEnd;
    double div = 1;
    int i, j, dim = this->getDim(), attrDim = myAttrDim[comp];
    float *&negPos = myFloatPosBuf;
    for (i = 0; i < dim; i++) {
      div *= myCellSize[i];
      negPos[i] = - pos[i] + myCellSize[i];
    }
    double mult;
    visAttribute *multAttr = myAttrFactory[comp]->getNewAttribute();
    attr->setValue(attrDim, visAttrAddNeutral);
    for (i = 0; i < (1 << dim); i++) {
      mult = 1;
      for (j = 0; j < dim; j++) {
	mult *= ((i & (1 << j)) != 0 ? pos[j] : negPos[j]);
      }
      //      visMsgDbg << "mult[" << i << "] = " << mult << visMsgEnd;
      
      multAttr->setValue(attrDim, myEdgeAttribute[comp][i]);
      multAttr->mult(attrDim, mult);
      attr->add(attrDim, multAttr);
    }
    attr->div(attrDim, div);
    multAttr->decRef();
  }
}

int visCell2NodeBlock::getCursor(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  return myFieldAccess->getCursor(i);
}

visAttribute* visCell2NodeBlock::getCellAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getCellAttribute(comp);
}

void visCell2NodeBlock::getCellAttribute(int comp, visAttribute* attr)
{
  if (!this->isAllocated()) return;
  myFieldAccess->getCellAttribute(comp, attr);
}

const char *visCell2NodeBlock::getAttrName(int comp) 
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getAttrName(comp);
};

const visAttribute* visCell2NodeBlock::getMinAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getMinAttribute(comp);
};

const visAttribute* visCell2NodeBlock::getMaxAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getMaxAttribute(comp);
};

int visCell2NodeBlock::getCellPos(unsigned int i)
{
  if (!this->isAllocated()) return 0;
  return myCellPos[i];
};

int visCell2NodeBlock::getCellSize(unsigned int i)
{
  if (!this->isAllocated()) return 1;
  return myCellSize[i];
};

int visCell2NodeBlock::getCells() 
{
  if (!this->isAllocated()) return 0;

  return myCells;
};

/* -----------------------------------------------------------------------------
   visCell2NodeOLinear Methods
   ---------------------------------------------------------------------------*/

visCell2NodeOLinear::visCell2NodeOLinear()
{
}

visCell2NodeOLinear::visCell2NodeOLinear(visAdaptiveGrid* fieldAccess)
{
  this->allocate(fieldAccess);
}

visCell2NodeOLinear::~visCell2NodeOLinear()
{
  this->deallocate();
}

bool visCell2NodeOLinear::allocate(visAdaptiveGrid* fieldAccess)
{
  visMsgCall << "visCell2NodeOLinear::allocate: called." << visMsgEnd;
  if (!fieldAccess || !fieldAccess->isAllocated()) {
    visMsgError << "visCell2NodeOLinear::allocate"
		<< ": called with unallocated fieldAccess."
		<< visMsgErrorEnd;
    return false;
  }
  
  if (fieldAccess == this) {
    visMsgError << "visCell2NodeOLinear::allocate"
		<< ": called with own instance."
		<< visMsgErrorEnd;
    return false;
  }
  
  this->deallocate();

  myFieldAccess = fieldAccess;
  myFieldAccess->incRef();
  int dim = myFieldAccess->getDim();
  int *size = new int[dim];
  int i;
  myCells = 1;
  for (i = 0; i < dim; i++) {
    size[i] = myFieldAccess->getSize(i) - 1;
    myCells *= size[i];
  }
  visAttributeFactory** factory =
    new visAttributeFactory*[myFieldAccess->getAttributes()];
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    factory[i] = myFieldAccess->getAttrFactory(i);
  }
  if (!visCell2Node::allocate(dim, size,
				myFieldAccess->getAttributes(), factory))
    return false;
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    factory[i]->decRef();
  }
  delete[] factory;
  delete[] size;

  myCellPos = new int[dim];

  myAttrDim = new int[fieldAccess->getAttributes()];
  for (i = 0; i < fieldAccess->getAttributes(); i++) {
    myAttrDim[i] = myAttrFactory[i]->getDim();
  }

  myAttribute = 
    new visAttribute*[myFieldAccess->getAttributes()];
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    myAttribute[i] = myAttrFactory[i]->getNewAttribute();
  }

  myAttributeBuffer = 
    new visAttribute*[myFieldAccess->getAttributes()];
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    myAttributeBuffer[i] = myAttrFactory[i]->getNewAttribute();
  }

  myCache = new visAttribute***[2 * dim];
  myCacheSize = new int[dim];
  myIsValidCache = false;
  myFieldCellPos = new int[dim];
  myFieldCellSize = new int[dim];

  myFloatPosBuf = new float[dim];

  // Set Cursor to origin
  {
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->setCursor(pos);
    delete[] pos;
  }

  return true;
}

void visCell2NodeOLinear::deallocate()
{
  if (!this->isAllocated()) return;
  int i;
  /*  for (i = 0; i < this->getAttributes(); i++) {
    for (j = 0; j < (1 << this->getDim()); j++) {
      myEdgeAttribute[i][j]->decRef();
    }
    delete[] myEdgeAttribute[i];
    }*/
  delete[] myFloatPosBuf;

  delete[] myFieldCellSize;
  delete[] myFieldCellPos;
  this->deallocateCache();
  delete[] myCache;
  delete[] myAttrDim;
  myCells = 0;
  delete[] myCellPos;
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    myAttribute[i]->decRef();
  }
  delete[] myAttribute;
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    myAttributeBuffer[i]->decRef();
  }
  delete[] myAttributeBuffer;
  myFieldAccess->decRef();
  myFieldAccess = NULL;
  visAdaptiveGrid::deallocate();
}
  
void visCell2NodeOLinear::deallocateCache()
{
  if (myIsValidCache) {
    /*    visMsgDbg << "deallocating cache" << visMsgEnd;*/
    int i, j, k;
    int dim = myFieldAccess->getDim();
    for (i = 0; i < dim; i++) {
      for (j = 0; j < myCacheSize[i]; j++) {
	if (myCache[2 * i + 0][j]) {
	  for (k = 0; k < myFieldAccess->getAttributes(); k++) {
	    myCache[2 * i + 0][j][k]->decRef();
	  }
	  delete[] myCache[2 * i + 0][j];
	  myCache[2 * i + 0][j] = NULL;
	}
	if (myCache[2 * i + 1][j]) {
	  for (k = 0; k < myFieldAccess->getAttributes(); k++) {
	    myCache[2 * i + 1][j][k]->decRef();
	  }
	delete[] myCache[2 * i + 1][j];
	myCache[2 * i + 1][j] = NULL;
	}
      };
      delete myCache[2 * i + 0];
    delete myCache[2 * i + 1];
    }
    myIsValidCache = false;
  }
}

void visCell2NodeOLinear::setCursor(const int *pos)
{
  if (!this->isAllocated()) return;
  
  int i, j;

  int dim = this->getDim();
  
  for (i = 0; i < dim; i++) {
    myCellPos[i] = pos[i];
  }

  if (myIsValidCache) {
    for (i = 0; i < dim; i++) {
      if (pos[i] < myFieldCellPos[i] 
	  ||
	  pos[i] >= myFieldCellPos[i] + myFieldCellSize[i]) {
	this->deallocateCache();
	break;
      }
    }
  }

  // allocate and initialize empty cache

  if (!myIsValidCache) {
    myFieldAccess->setCursor(pos);

    for (i = 0; i < dim; i++) {
      myFieldCellPos[i] = myFieldAccess->getCellPos(i);
      myFieldCellSize[i] = myFieldAccess->getCellSize(i);
    }
    
    for (i = 0; i < dim; i++) {
      int size = 1;
      for (j = 0; j < dim; j++) {
	if (j != i) {
	  size *= myFieldCellSize[j] + 1;
	}
      }
      myCacheSize[i] = size;
      myCache[2 * i + 0] = new visAttribute**[size];
      myCache[2 * i + 1] = new visAttribute**[size];
      for (j = 0; j < size; j++) {
	myCache[2 * i + 0][j] = NULL;
	myCache[2 * i + 1][j] = NULL;
      }
    }
    myIsValidCache = true;
    /*    for (i = 0; i < dim; i++) {
      visMsgDbg << "myCacheSize[" << i << "] = " << myCacheSize[i] << visMsgEnd;
      }*/

  }

  //
  
  this->getLinearInterpValue(pos, myAttribute);
}

void visCell2NodeOLinear::getLinearInterpValue(const int *pos,
						 visAttribute **resAttr)
{
  if (!this->isAllocated()) return;
  
  int i, j;

  int dim = this->getDim();
  
  // check if in cache
  
  int cacheBuffer = -1, cacheIndex, cacheSize;

  /*  visMsgDbg << "interp pos = "
      << pos[0] << "," << pos[1] << "," << pos[2] << visMsgEnd;*/

  for (i = 0; i < dim; i++) {
    if (pos[i] < myFieldCellPos[i] 
	||
	pos[i] > myFieldCellPos[i] + myFieldCellSize[i]) {
      break;
    }
    if (pos[i] == myFieldCellPos[i] 
	||
	pos[i] == myFieldCellPos[i] + myFieldCellSize[i]) {

      bool validPos = true;
      for (j = i + 1; j < dim; j++) {
	if (pos[j] < myFieldCellPos[j] 
	    ||
	    pos[j] > myFieldCellPos[j] + myFieldCellSize[j]) {
	  validPos = false;
	  break;
	}
      }
      if (!validPos) {
	break;
      }
      if (pos[i] == myFieldCellPos[i]) {
	cacheBuffer = 2 * i + 0;
      } else {
	cacheBuffer = 2 * i + 1;
      }
      cacheIndex = 0;
      cacheSize = 1;
      for (j = 0; j < dim; j++) {
	if (j != i) {
	  cacheIndex += (pos[j] - myFieldCellPos[j]) * cacheSize;
	  cacheSize *= myFieldCellSize[j] + 1;
	}
      }
      if (myCache[cacheBuffer][cacheIndex]) {
	
	/*	visMsgDbg << "found in cache - cacheIndex = " << cacheIndex
		  << " cacheBuffer = " << cacheBuffer 
		  << " value = " 
		  << myCache[cacheBuffer][cacheIndex][0]->getValue(0)
		  << visMsgEnd;*/
	// found in cache

	for (j = 0; j < this->getAttributes(); j++) {
	  resAttr[j]->setValue(myAttrFactory[j]->getDim(), 
			       myCache[cacheBuffer][cacheIndex][j]);
	}
	return;
      }
      break;
    }
  }

  // allocate space for intermediate calculations

  visAttribute **attr = 
    new visAttribute*[this->getAttributes()];
  for (i = 0; i < this->getAttributes(); i++) {
    attr[i] = myAttrFactory[i]->getNewAttribute();
  }

  // retrieve cell pos and size and calculate interpolation dimension
  
  myFieldAccess->setCursor(pos);

  int *fieldCellPos = new int[dim];
  int *fieldCellSize = new int[dim];
  int *interp = new int[dim];
  int interpDim = 0;
  for (i = 0; i < dim; i++) {
    fieldCellPos[i] = myFieldAccess->getCellPos(i);
    fieldCellSize[i] = myFieldAccess->getCellSize(i);
    
    if (pos[i] == this->getSize(i)) {
      fieldCellPos[i] = pos[i];
      fieldCellSize[i] = 0;
    }
    
    if (fieldCellPos[i] + fieldCellSize[i] > this->getSize(i)) {
      fieldCellSize[i]--;
    }
    
    if (pos[i] != fieldCellPos[i]) {
      interp[interpDim++] = i;
    }
  }

  //delta1[i] = pos[interp[i]] - myFieldCellPos[interp[i]];
  //delta2[i] = myEdgePos[i] - pos[interp[i]];
  
  /*  visMsgDbg << "cell pos = "
  	    << fieldCellPos[0] << "," << fieldCellPos[1] << "," << fieldCellPos[2] << visMsgEnd;
  visMsgDbg << "cell size = "
  	    << fieldCellSize[0] << "," << fieldCellSize[1] << "," << fieldCellSize[2] << visMsgEnd;
  visMsgDbg << "interp dim = "
  	    << interpDim << visMsgEnd;
  */
  if (interpDim > 0) {
    int *newPos1 = new int[dim];
    int *newPos2 = new int[dim];
    int mult;
    for (i = 0; i < this->getAttributes(); i++) {
      resAttr[i]->setValue(myAttrFactory[i]->getDim(), visAttrAddNeutral);
    }
    int nenner = 0;
    for (i = 0; i < interpDim; i++) {

      for (j = 0; j < dim; j++) {
	if (j == interp[i]) {
	  newPos1[j] = fieldCellPos[j];
	  newPos2[j] = fieldCellPos[j] + fieldCellSize[j];
	} else {
	  newPos1[j] = pos[j];
	  newPos2[j] = pos[j];
	}
      }

      mult = 1;
      for (j = 0; j < interpDim; j++) {
	if (i != j) {
	  mult *= fieldCellPos[interp[j]] + fieldCellSize[interp[j]] -
	    pos[interp[j]];
	  mult *= pos[interp[j]] - fieldCellPos[interp[j]];
	}
      }
      
      int mult1 = mult * (fieldCellPos[interp[i]] + fieldCellSize[interp[i]] -
			  pos[interp[i]]);
      int mult2 = mult * (pos[interp[i]] - fieldCellPos[interp[i]]);

      //      visMsgDbg << " --- calling recursively 2" << visMsgEnd;
      this->getLinearInterpValue(newPos2, attr);

      for (j = 0; j < this->getAttributes(); j++) {
	attr[j]->mult(myAttrFactory[j]->getDim(), mult2);
	resAttr[j]->add(myAttrFactory[j]->getDim(), attr[j]);
      }
      nenner += mult2;
      
      //      visMsgDbg << " --- calling recursively 1" << visMsgEnd;
      this->getLinearInterpValue(newPos1, attr);
      for (j = 0; j < this->getAttributes(); j++) {
	attr[j]->mult(myAttrFactory[j]->getDim(), mult1);
	resAttr[j]->add(myAttrFactory[j]->getDim(), attr[j]);
      }
      nenner += mult1;

    }
    //    visMsgDbg << "nenner = " << nenner << visMsgEnd;
    for (i = 0; i < this->getAttributes(); i++) {
      resAttr[i]->div(myAttrFactory[i]->getDim(), nenner);
    }
    delete[] newPos1;
    delete[] newPos2;
  } else {
    for (i = 0; i < this->getAttributes(); i++) {
      myFieldAccess->getCellAttribute(i, resAttr[i]);
    }
  }

  //  visMsgDbg << "result val = "
  //  	    << resAttr[0]->getValue(0) << visMsgEnd;
  delete[] interp;
  delete[] fieldCellSize;
  delete[] fieldCellPos;

  for (i = 0; i < this->getAttributes(); i++) {
    attr[i]->decRef();
  }
  delete[] attr;

  // check if we should cache the result
  
  if (cacheBuffer >= 0) {
    myCache[cacheBuffer][cacheIndex] = 
      new visAttribute*[this->getAttributes()];
    /*    visMsgDbg << "writing to cache - cacheIndex = " << cacheIndex
	      << " cacheBuffer = " << cacheBuffer 
	      << " value = " << resAttr[0]->getValue(0)
	      << visMsgEnd;*/
    for (i = 0; i < this->getAttributes(); i++) {
      myCache[cacheBuffer][cacheIndex][i] = myAttrFactory[i]->getNewAttribute();
      myCache[cacheBuffer][cacheIndex][i]->setValue(myAttrFactory[i]->getDim(),
						    resAttr[i]);
      //      myCache[cacheBuffer][cacheIndex][i] = resAttr[i];
    }
  }
}
  
void visCell2NodeOLinear::getNodeAttribute(int comp, 
					     visAttribute *attr,
					     int edge)
{
  if (!this->isAllocated()) return;

  int dim = this->getDim();
  int *newPos = new int[dim];
  int i;
  for (i = 0; i < dim; i++) {
    newPos[i] = (edge & (1 << i)) != 0 ? myCellPos[i] + 1 : myCellPos[i];
  }

  getLinearInterpValue(newPos, myAttributeBuffer);

  attr->setValue(myAttrFactory[comp]->getDim(), myAttributeBuffer[comp]);

  delete newPos;

}

void visCell2NodeOLinear::getInterpAttribute(int comp,
					     visAttribute *attr,
					     float *pos)
{
  if (!this->isAllocated()) return;
  
  //    visMsgDbg << "visCell2NodeOLinear: interpolating..." << visMsgEnd;
  double div = 1;
  int i, j, dim = this->getDim(), attrDim = myAttrDim[comp];
  float *&negPos = myFloatPosBuf;
  for (i = 0; i < dim; i++) {
    negPos[i] = 1 - pos[i];
  }
  double mult;
  visAttribute *multAttr = myAttrFactory[comp]->getNewAttribute();
  attr->setValue(attrDim, visAttrAddNeutral);
  for (i = 0; i < (1 << dim); i++) {
    mult = 1;
    for (j = 0; j < dim; j++) {
      mult *= ((i & (1 << j)) != 0 ? pos[j] : negPos[j]);
    }
    //      visMsgDbg << "mult[" << i << "] = " << mult << visMsgEnd;
    
    this->getNodeAttribute(comp, multAttr, i);
    multAttr->mult(attrDim, mult);
    attr->add(attrDim, multAttr);
  }
  attr->div(attrDim, div);
  multAttr->decRef();
}

int visCell2NodeOLinear::getCursor(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  return myCellPos[i];
}

visAttribute* visCell2NodeOLinear::getCellAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  myAttribute[comp]->incRef();
  return myAttribute[comp];
}

void visCell2NodeOLinear::getCellAttribute(int comp, visAttribute* attr)
{
  if (!this->isAllocated()) return;

  attr->setValue(myAttrDim[comp], myAttribute[comp]);
}

const char *visCell2NodeOLinear::getAttrName(int comp) 
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getAttrName(comp);
};

const visAttribute* visCell2NodeOLinear::getMinAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getMinAttribute(comp);
};

const visAttribute* visCell2NodeOLinear::getMaxAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getMaxAttribute(comp);
};

int visCell2NodeOLinear::getCellPos(unsigned int i)
{
  if (!this->isAllocated()) return 0;
  return myCellPos[i];
};

int visCell2NodeOLinear::getCellSize(unsigned int)
{
  return 1;
};

int visCell2NodeOLinear::getCells() 
{
  if (!this->isAllocated()) return 0;

  return myCells;
};

/* -----------------------------------------------------------------------------
   visCell2NodeMLinear Methods
   ---------------------------------------------------------------------------*/

visCell2NodeMLinear::visCell2NodeMLinear()
{
}

visCell2NodeMLinear::visCell2NodeMLinear(visAdaptiveGrid* fieldAccess)
{
  this->allocate(fieldAccess);
}

visCell2NodeMLinear::~visCell2NodeMLinear()
{
  this->deallocate();
}

bool visCell2NodeMLinear::allocate(visAdaptiveGrid* fieldAccess)
{
  visMsgCall << "visCell2NodeMLinear::allocate: called." << visMsgEnd;
  if (!fieldAccess || !fieldAccess->isAllocated()) {
    visMsgError << "visCell2NodeMLinear::allocate"
		<< ": called with unallocated fieldAccess."
		<< visMsgErrorEnd;
    return false;
  }
  
  if (fieldAccess == this) {
    visMsgError << "visCell2NodeMLinear::allocate"
		<< ": called with own instance."
		<< visMsgErrorEnd;
    return false;
  }
  
  this->deallocate();

  myFieldAccess = fieldAccess;
  myFieldAccess->incRef();
  int dim = myFieldAccess->getDim();
  int *size = new int[dim];
  int i, j;
  myCells = 1;
  for (i = 0; i < dim; i++) {
    size[i] = myFieldAccess->getSize(i) - 1;
    myCells *= size[i];
  }
  visAttributeFactory** factory =
    new visAttributeFactory*[myFieldAccess->getAttributes()];
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    factory[i] = myFieldAccess->getAttrFactory(i);
  }
  if (!visCell2Node::allocate(dim, size,
				myFieldAccess->getAttributes(), factory))
    return false;
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    factory[i]->decRef();
  }
  delete[] factory;
  delete[] size;

  myCellPos = new int[dim];

  myAttrDim = new int[fieldAccess->getAttributes()];
  for (i = 0; i < fieldAccess->getAttributes(); i++) {
    myAttrDim[i] = myAttrFactory[i]->getDim();
  }

  myAttribute = 
    new visAttribute*[myFieldAccess->getAttributes()];
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    myAttribute[i] = myAttrFactory[i]->getNewAttribute();
  }

  myAttributeBuffer = 
    new visAttribute*[myFieldAccess->getAttributes()];
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    myAttributeBuffer[i] = myAttrFactory[i]->getNewAttribute();
  }

  myCache = new visAttribute***[3 * dim];
  myCacheSize = new int[dim];
  myIsValidCache = false;
  myFieldCellPos = new int[dim];
  myFieldCellSize = new int[dim];

  myFloatPosBuf = new float[dim];

  // Alloc "first level" cache and initialize as empty

  myEdgeAttributeCache = new visAttribute**[1 << dim];
  for (i = 0; i < (1 << dim); i++) {
    myEdgeAttributeCache[i] = 
      new visAttribute*[myFieldAccess->getAttributes()];
    for (j = 0; j < myFieldAccess->getAttributes(); j++) {
      myEdgeAttributeCache[i][j] = NULL;
    }
  }

  // allocate space for intermediate calculations of getLinearInterpValue
  
  myIFieldCellPos = new int[dim * dim];
  myIFieldCellSize = new int[dim * dim];
  
  myIMult = new float[2 * dim * dim];
 
  myIAttr = new visAttribute**[2 * dim * dim];
  for (i = 0; i < 2 * dim * dim; i++) {
    myIAttr[i] = new visAttribute*[this->getAttributes()];
    for (j = 0; j < this->getAttributes(); j++) {
      myIAttr[i][j] = myAttrFactory[j]->getNewAttribute();
    }
  }
  myINewPos = new int[dim * dim];
  myINewInterp = new int[dim * dim];

  // invalidate cell pos since this is used to check of cursor has changed

  for (i = 0; i < dim; i++) {
    myCellPos[i] = this->getSize(i);
  }

  // Set Cursor to origin
  {
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->setCursor(pos);
    delete[] pos;
  }

  return true;
}

void visCell2NodeMLinear::deallocate()
{
  if (!this->isAllocated()) return;
  visMsgCall << "visCell2NodeMLinear::deallocate: called." << visMsgEnd;
  int i, j;

  int dim = this->getDim();
  delete[] myINewInterp;
  delete[] myINewPos;
  for (i = 0; i < 2 * dim * dim; i++) {
    for (j = 0; j < this->getAttributes(); j++) {
      myIAttr[i][j]->decRef();
    }
    delete[] myIAttr[i];
  }
  delete[] myIAttr;
  delete[] myIMult;
  delete[] myIFieldCellSize;
  delete[] myIFieldCellPos;


  // Free "first level" cache
  
  for (i = 0; i < (1 << this->getDim()); i++) {
    for (j = 0; j < myFieldAccess->getAttributes(); j++) {
      if (myEdgeAttributeCache[i][j]) {
	myEdgeAttributeCache[i][j]->decRef();
      }
    }
    delete[] myEdgeAttributeCache[i];
  }
  delete[] myEdgeAttributeCache;

  /*  for (i = 0; i < this->getAttributes(); i++) {
    for (j = 0; j < (1 << this->getDim()); j++) {
      myEdgeAttribute[i][j]->decRef();
    }
    delete[] myEdgeAttribute[i];
    }*/
  delete[] myFloatPosBuf;

  delete[] myFieldCellSize;
  delete[] myFieldCellPos;
  this->deallocateCache();
  delete[] myCache;
  delete[] myAttrDim;
  myCells = 0;
  delete[] myCellPos;
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    myAttribute[i]->decRef();
  }
  delete[] myAttribute;
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    myAttributeBuffer[i]->decRef();
  }
  delete[] myAttributeBuffer;
  myFieldAccess->decRef();
  myFieldAccess = NULL;
  visAdaptiveGrid::deallocate();
}
  
void visCell2NodeMLinear::deallocateCache()
{
  if (myIsValidCache) {
    /*    visMsgDbg << "deallocating cache" << visMsgEnd;*/
    int i, j, k;
    int dim = myFieldAccess->getDim();
    for (i = 0; i < dim; i++) {
      for (j = 0; j < myCacheSize[i]; j++) {
	if (myCache[3 * i + 0][j]) {
	  for (k = 0; k < myFieldAccess->getAttributes(); k++) {
	    myCache[3 * i + 0][j][k]->decRef();
	  }
	  delete[] myCache[3 * i + 0][j];
	}
	if (myCache[3 * i + 1][j]) {
	  for (k = 0; k < myFieldAccess->getAttributes(); k++) {
	    myCache[3 * i + 1][j][k]->decRef();
	  }
	  delete[] myCache[3 * i + 1][j];
	}
	if (myCache[3 * i + 2][j]) {
	  for (k = 0; k < myFieldAccess->getAttributes(); k++) {
	    myCache[3 * i + 2][j][k]->decRef();
	  }
	  delete[] myCache[3 * i + 2][j];
	}
      };
      delete[] myCache[3 * i + 0];
      delete[] myCache[3 * i + 1];
      delete[] myCache[3 * i + 2];
    }
    myIsValidCache = false;
  }
}

void visCell2NodeMLinear::setCursor(const int *pos)
{
  if (!this->isAllocated()) return;
  
  mySetCursorCalls++;
  
  int i, j, k;

  int dim = this->getDim();
  
  // if cursor doesn't change return

  for (i = 0; i < dim; i++) {
    if (pos[i] != myCellPos[i]) {
      goto posChanged;
    }
  }
  mySetCursorCachedCalls++;
  return;
 posChanged:
  
  // Clear "first level" cache and initialize as empty

  for (k = 0; k < dim; k++) {
    if (pos[k] == myCellPos[k]) {
      continue;
    } else if (pos[k] == myCellPos[k] - 1) {
      for (i = 0; i < (1 << dim); i++) {
	if (((1 << k) & i) != 0) {
	  int m = (1 << k) ^ i;
	  for (j = 0; j < myFieldAccess->getAttributes(); j++) {
	    if (myEdgeAttributeCache[i][j]) {
	      myEdgeAttributeCache[i][j]->decRef();
	    }
	    myEdgeAttributeCache[i][j] = myEdgeAttributeCache[m][j];
	    myEdgeAttributeCache[m][j] = NULL;
	  }
	}
      }
    } else if (pos[k] == myCellPos[k] + 1) {
      for (i = 0; i < (1 << dim); i++) {
	if (((1 << k) & i) == 0) {
	  int m = (1 << k) ^ i;
	  for (j = 0; j < myFieldAccess->getAttributes(); j++) {
	    if (myEdgeAttributeCache[i][j]) {
	      myEdgeAttributeCache[i][j]->decRef();
	    }
	    myEdgeAttributeCache[i][j] = myEdgeAttributeCache[m][j];
	    myEdgeAttributeCache[m][j] = NULL;
	  }
	}
      }
    } else {
      for (i = 0; i < (1 << dim); i++) {
	for (j = 0; j < myFieldAccess->getAttributes(); j++) {
	  if (myEdgeAttributeCache[i][j]) {
	    myEdgeAttributeCache[i][j]->decRef();
	    myEdgeAttributeCache[i][j] = NULL;
	  }
	}
      }
      break;
    }
  }

  for (i = 0; i < dim; i++) {
    myCellPos[i] = pos[i];
  }

  // Clear "second level" cache if we change position in related field

  if (myIsValidCache) {
    for (i = 0; i < dim; i++) {
      if (pos[i] < myFieldCellPos[i] 
          ||
          pos[i] >= myFieldCellPos[i] + myFieldCellSize[i]) {
	this->deallocateCache();
	break;
      }
    }
  }

  // allocate and initialize empty cache

  if (!myIsValidCache) {
    myFieldAccess->setCursor(pos);

    for (i = 0; i < dim; i++) {
      myFieldCellPos[i] = myFieldAccess->getCellPos(i);
      myFieldCellSize[i] = myFieldAccess->getCellSize(i);
    }
    
    for (i = 0; i < dim; i++) {
      int size = 1;
      for (j = 0; j < dim; j++) {
        if (j != i) {
          size *= myFieldCellSize[j] + 1;
        }
      }
      myCacheSize[i] = size;
      myCache[3 * i + 0] = new visAttribute**[size];
      myCache[3 * i + 1] = new visAttribute**[size];
      myCache[3 * i + 2] = new visAttribute**[size];
      for (j = 0; j < size; j++) {
        myCache[3 * i + 0][j] = NULL;
        myCache[3 * i + 1][j] = NULL;
        myCache[3 * i + 2][j] = NULL;
      }
    }
    myIsValidCache = true;

  }
  //visMsgDbg << "setCursor called: pos = " 
  //  	    << pos[0] << "," << pos[1] << "," << pos[2] << visMsgEnd;

  int interpDim = 0;
  int *interp = new int[dim];
  myFieldAccess->setCursor(pos);
  for (i = 0; i < dim; i++) {
    int s = myFieldAccess->getCellSize(i) - 1;
    if (((s & 1) || (s / 2 + myFieldAccess->getCellPos(i) != pos[i]))
	&& (myFieldAccess->getCellPos(i) > 0 || s / 2 < pos[i])
	&& (myFieldAccess->getCellPos(i) + myFieldAccess->getCellSize(i) <
	    myFieldAccess->getSize(i) || 
	    myFieldAccess->getCellPos(i) + s / 2 + 1 > pos[i])) 
      {
	interp[interpDim++] = i;
      }
  }
  this->getLinearInterpValue(interpDim, interp, pos, myAttribute, true);
  
  delete[] interp;
}

/**
   interpDim is the number of dimensions to be interpolated and interp is the
   array of axis indices that are not lying on the midpoint.

   Following additional conditions must be met when calling this function:
   Cursor lies on the cell which contains pos. pos[i] with i not element of
   interp need not be initialized.
   resAttr will be the interpolated value of the given pos
**/

void visCell2NodeMLinear::getLinearInterpValue(int interpDim, int *interp,
						const int *pos,
						visAttribute **resAttr,
						bool mayCache)
{
  if (!this->isAllocated()) return;
  
  int i, j;

  myGetLinearInterpValueCalls++;

  if (interpDim == 0) {
    for (i = 0; i < this->getAttributes(); i++) {
      myFieldAccess->getCellAttribute(i, resAttr[i]);
    }
    return;
  }

  int dim = this->getDim();
  
  // check if in cache
  
  int cacheBuffer = -1, cacheIndex, cacheSize;

  mayCache = false;
  bool checkIfCacheOk = false;
  if (interpDim < dim && mayCache) {
    int cacheBufferIndex = 0;
    for (i = 0; i < interpDim; i++) {
      if (cacheBufferIndex != interp[i]) {
	break;
      }
      cacheBufferIndex++;
    }
    if (pos[cacheBufferIndex] < myFieldCellPos[cacheBufferIndex]) {
      cacheBuffer = 3 * cacheBufferIndex + 0;
    } else if (pos[cacheBufferIndex] >= 
	       myFieldCellPos[cacheBufferIndex] + 
	       myFieldCellSize[cacheBufferIndex]) {
      cacheBuffer = 3 * cacheBufferIndex + 1;
    } else {
      cacheBuffer = 3 * cacheBufferIndex + 2;
    }

    cacheIndex = 0;
    cacheSize = 1;
    for (j = 0; j < dim; j++) {
      if (j != cacheBufferIndex) {
	cacheIndex += (pos[j] - myFieldCellPos[j]) * cacheSize;
	cacheSize *= myFieldCellSize[j] + 1;
      }
    }
    if (!checkIfCacheOk) {
      if (myCache[cacheBuffer][cacheIndex]) {
      
      //      visMsgDbg << "found in cache - cacheIndex = " << cacheIndex
      //	      << " cacheBuffer = " << cacheBuffer 
      //	      << " value = " 
      //	      << myCache[cacheBuffer][cacheIndex][0]->getValue(0)
      //		<< visMsgEnd;
      // found in cache
      
	for (j = 0; j < this->getAttributes(); j++) {
	  resAttr[j]->setValue(myAttrDim[j], 
			       myCache[cacheBuffer][cacheIndex][j]);
	}
	myGetLinearInterpValueCachedCalls++;
	return;
      }
    }
  }

  // retrieve cell pos, size and midpoint
  
  //int *fieldCellPos = new int[dim];
  //int *fieldCellSize = new int[dim];
  int *fieldCellPos = &myIFieldCellSize[dim * (interpDim - 1)];
  int *fieldCellSize = &myIFieldCellPos[dim * (interpDim - 1)];

  for (i = 0; i < dim; i++) {
    fieldCellPos[i] = myFieldAccess->getCellPos(i);
    fieldCellSize[i] = myFieldAccess->getCellSize(i);
  }
  
  // allocate space for intermediate calculations
  
  //visAttribute ***attr = new visAttribute**[2 * interpDim];
  //for (i = 0; i < 2 * interpDim; i++) {
  //  attr[i] = new visAttribute*[this->getAttributes()];
  //  for (j = 0; j < this->getAttributes(); j++) {
  //    attr[i][j] = myAttrFactory[j]->getNewAttribute();
  //  }
  //}
  //int *newPos = new int[dim];
  //int *newInterp = new int[interpDim - 1];
  //float *mult = new float[2 * interpDim];

  visAttribute ***attr = &myIAttr[2 * dim * (interpDim - 1)];
  int *newPos = &myINewPos[dim * (interpDim - 1)];
  int *newInterp = &myINewInterp[dim * (interpDim - 1)];
  float *mult = &myIMult[2 * dim * (interpDim - 1)];
 
  int newInterpDim;
  
  // initialize attr and mult arrays
  
  for (i = 0; i < interpDim; i++) {
    int interpI = interp[i];
    for (j = 0; j < dim; j++) {
      if (j != interpI) {
	newPos[j] = pos[j];
      }
    }


    if (float(fieldCellSize[interpI] - 1) / 2 +
	fieldCellPos[interpI] >= pos[interpI]) {
      newPos[interpI] = fieldCellPos[interpI] - 1;
      myFieldAccess->setCursor(newPos);
      mult[2 * i + 0] = 
	float(myFieldAccess->getCellSize(interpI) - 1) / 2 + 1 +
	pos[interpI] - fieldCellPos[interpI];
      newPos[interpI] = myFieldAccess->getCellPos(interpI) +
	(myFieldAccess->getCellSize(interpI) - 1) / 2;
      newInterpDim = 0;
      for (j = 0; j < interpDim; j++) {
	if (j != i) {
	  int s = myFieldAccess->getCellSize(interp[j]) - 1;
	  if (((s & 1) || (s / 2 + myFieldAccess->getCellPos(interp[j]) 
			   != pos[interp[j]]))
	      && (myFieldAccess->getCellPos(interp[j]) > 0 || s / 2 < 
		  pos[interp[j]])
	      && (myFieldAccess->getCellPos(interp[j])
		  + myFieldAccess->getCellSize(interp[j]) <
		  myFieldAccess->getSize(interp[j]) || 
		  myFieldAccess->getCellPos(interp[j]) + s / 2 + 1 >
		  pos[interp[j]])) 
	    {
	      newInterp[newInterpDim++] = interp[j];
	    }
	}
      }
    } else {
      mult[2 * i + 0] = 
	float(pos[interpI]) -
	(float(fieldCellSize[interpI] - 1) / 2 + fieldCellPos[interpI]);
      newPos[interpI] = fieldCellPos[interpI] +
	(fieldCellSize[interpI] - 1) / 2;
      newInterpDim = 0;
      for (j = 0; j < interpDim; j++) {
	if (j != i) {
	  newInterp[newInterpDim++] = interp[j];
	}
      }
    }
    
    //    visMsgDbg << " --- calling recursively 1" << visMsgEnd;
    this->getLinearInterpValue(newInterpDim, newInterp,
			       newPos, attr[2 * i + 0], interpDim == dim);
      
    
    if (float(fieldCellSize[interpI] - 1) / 2 + 
	fieldCellPos[interpI] <= pos[interpI]) {
      newPos[interpI] = fieldCellPos[interpI] + fieldCellSize[interpI];
      //	visMsgDbg << "2: setting to cursor: "
      //		  << newPos[0] << "," << newPos[1] << "," << newPos[2] 
      //		  << visMsgEnd;
      myFieldAccess->setCursor(newPos);
      mult[2 * i + 1] =
	float(myFieldAccess->getCellSize(interpI) - 1) / 2 +
	fieldCellPos[interpI] + fieldCellSize[interpI] - pos[interpI];
      newPos[interpI] = myFieldAccess->getCellPos(interpI) +
	(myFieldAccess->getCellSize(interpI) - 1) / 2;
      newInterpDim = 0;
      for (j = 0; j < interpDim; j++) {
	if (j != i) {
	  int s = myFieldAccess->getCellSize(interp[j]) - 1;
	  if (((s & 1) || (s / 2 + myFieldAccess->getCellPos(interp[j]) 
			   != pos[interp[j]]))
	      && (myFieldAccess->getCellPos(interp[j]) > 0 || s / 2 < 
		  pos[interp[j]])
	      && (myFieldAccess->getCellPos(interp[j])
		  + myFieldAccess->getCellSize(interp[j]) <
		  myFieldAccess->getSize(interp[j]) || 
		  myFieldAccess->getCellPos(interp[j]) + s / 2 + 1
		  > pos[interp[j]])) 
	    {
	      newInterp[newInterpDim++] = interp[j];
	    }
	}
      }
    } else {
      myFieldAccess->setCursor(pos);
      mult[2 * i + 1] =
	float(fieldCellSize[interpI] - 1) / 2 + fieldCellPos[interpI] -
	pos[interpI];
      newPos[interpI] = fieldCellPos[interpI] +
	(fieldCellSize[interpI] - 1) / 2;
      newInterpDim = 0;
      for (j = 0; j < interpDim; j++) {
	if (j != i) {
	  newInterp[newInterpDim++] = interp[j];
	}
      }
    }
    
    //    visMsgDbg << " --- calling recursively 2" << visMsgEnd;
    this->getLinearInterpValue(newInterpDim, newInterp,
			       newPos, attr[2 * i + 1], interpDim == dim);
  }

  for (i = 0; i < this->getAttributes(); i++) {
    resAttr[i]->setValue(myAttrDim[i], visAttrAddNeutral);
  }
  
  float nenner = 0;
  
  for (i = 0; i < interpDim; i++) {
    float m = 1;
    for (j = 0; j < interpDim; j++) {
      if (i != j) {
	m *= mult[2 * j + 0] * mult[2 * j + 1];
      }
    }
    
    float m1 = m * mult[2 * i + 0];
    for (j = 0; j < this->getAttributes(); j++) {
      attr[2 * i + 1][j]->mult(myAttrDim[j], m1);
      resAttr[j]->add(myAttrDim[j], attr[2 * i + 1][j]);
    }
    nenner += m1;
    
    float m2 = m * mult[2 * i + 1];
    for (j = 0; j < this->getAttributes(); j++) {
      attr[2 * i + 0][j]->mult(myAttrDim[j], m2);
      resAttr[j]->add(myAttrDim[j], attr[2 * i + 0][j]);
    }
    nenner += m2;
  }
  for (j = 0; j < this->getAttributes(); j++) {
    resAttr[j]->div(myAttrDim[j], nenner);
  }
  
  //delete[] newInterp;
  //delete[] newPos;
  //delete[] mult;
  //for (i = 0; i < 2 * interpDim; i++) {
  //  for (j = 0; j < this->getAttributes(); j++) {
  //    attr[i][j]->decRef();
  //  }
  //  delete[] attr[i];
  //}
  //delete[] attr;

  //delete[] fieldCellSize;
  //delete[] fieldCellPos;

  // check if we should cache the result
  
  if (cacheBuffer >= 0) {
    if (!myCache[cacheBuffer][cacheIndex]) {
      myCache[cacheBuffer][cacheIndex] = 
	new visAttribute*[this->getAttributes()];
      /*    visMsgDbg << "writing to cache - cacheIndex = " << cacheIndex
	    << " cacheBuffer = " << cacheBuffer 
	    << " value = " << resAttr[0]->getValue(0)
	    << visMsgEnd;*/
      for (i = 0; i < this->getAttributes(); i++) {
	myCache[cacheBuffer][cacheIndex][i] = myAttrFactory[i]->getNewAttribute();
	myCache[cacheBuffer][cacheIndex][i]->setValue(myAttrDim[i],
						      resAttr[i]);
      }
    } else {
      visMsgError << "oups" << visMsgErrorEnd;
      /*      for (i = 0; i < this->getAttributes(); i++) {
	if (!myCache[cacheBuffer][cacheIndex][i]
	    ->isEqual(myAttrFactory[i]->getDim(), resAttr[i])) {
	  visMsgError << "cache is wrong" << visMsgErrorEnd;
	}
	}*/
    }
  }
  /*  if (myCellPos[0] == 11 && myCellPos[1] == 11 && myCellPos[2] == 11) {
    visMsgDbg << "result(x)0 = " << resAttr[0]->getValue(0) << visMsgEnd;
    visMsgDbg << "result(x)1 = " << resAttr[0]->getValue(1) << visMsgEnd;
    visMsgDbg << "result(x)2 = " << resAttr[0]->getValue(2) << visMsgEnd;
    }*/

}
  
void visCell2NodeMLinear::getNodeAttribute(int comp, 
					     visAttribute *attr,
					     int edge)
{
  if (!this->isAllocated()) return;

  myGetEdgeAttributeCalls++;
  
  /*  if (myCellPos[0] == 11 && myCellPos[1] == 11 && myCellPos[2] == 11) {
    visMsgDbg << "getNodeAttribute called: edge = " << edge << visMsgEnd;
    }*/
  int dim = this->getDim();

  // Check if in first level cache

  if (myEdgeAttributeCache[edge][comp]) {
    attr->setValue(myAttrFactory[comp]->getDim(), 
		   myEdgeAttributeCache[edge][comp]);
    myGetEdgeAttributeCachedCalls++;
    /*    if (myCellPos[0] == 11 && myCellPos[1] == 11 && myCellPos[2] == 11) {
      visMsgDbg << "first hit" << visMsgEnd;
      }*/
    return;
  }

  int *newPos = new int[dim];
  int i;
  for (i = 0; i < dim; i++) {
    newPos[i] = (edge & (1 << i)) != 0 ? myCellPos[i] + 1 : myCellPos[i];
  }

  int interpDim = 0;
  int *interp = new int[dim];
  myFieldAccess->setCursor(newPos);
  for (i = 0; i < dim; i++) {
    int s = myFieldAccess->getCellSize(i) - 1;
    if (((s & 1) || (s / 2 + myFieldAccess->getCellPos(i) != newPos[i]))
	&& (myFieldAccess->getCellPos(i) > 0 || s / 2 < newPos[i])
	&& (myFieldAccess->getCellPos(i) + myFieldAccess->getCellSize(i) <
	    myFieldAccess->getSize(i) || 
	    myFieldAccess->getCellPos(i) + s / 2 + 1 > newPos[i])) 
      {
	interp[interpDim++] = i;
      }
  }  
  
  /*  if (myCellPos[0] == 11 && myCellPos[1] == 11 && myCellPos[2] == 11) {
    visMsgDbg << "calling rec (interpDim = " << interpDim << ")" << visMsgEnd;
    }*/
  this->getLinearInterpValue(interpDim, interp, newPos, myAttributeBuffer,
			     true);

  delete[] interp;
  delete newPos;

  attr->setValue(myAttrFactory[comp]->getDim(), myAttributeBuffer[comp]);

  // fill cache for this edge

  for (i = 0; i < this->getAttributes(); i++) {
    myEdgeAttributeCache[edge][i] = myAttrFactory[i]->getNewAttribute();
    myEdgeAttributeCache[edge][i]->setValue(myAttrFactory[i]->getDim(),
					    myAttributeBuffer[i]);
  }

}

void visCell2NodeMLinear::getInterpAttribute(int comp,
					     visAttribute *attr,
					     float *pos)
{
  if (!this->isAllocated()) return;
  
  /*  if (pos[0] == 11 && pos[1] == 11 && pos[2] == 11) {
    visMsgDbg << "visCell2NodeMLinear: interpolating..." << visMsgEnd;
    }*/
  double div = 1;
  int i, j, dim = this->getDim(), attrDim = myAttrDim[comp];
  float *&negPos = myFloatPosBuf;
  for (i = 0; i < dim; i++) {
    negPos[i] = 1 - pos[i];
  }
  double mult;
  visAttribute *multAttr = myAttrFactory[comp]->getNewAttribute();
  attr->setValue(attrDim, visAttrAddNeutral);
  for (i = 0; i < (1 << dim); i++) {
    mult = 1;
    for (j = 0; j < dim; j++) {
      mult *= ((i & (1 << j)) != 0 ? pos[j] : negPos[j]);
    }
    this->getNodeAttribute(comp, multAttr, i);
    /*    if (pos[0] == 11 && pos[1] == 11 && pos[2] == 11) {
      visMsgDbg << "mult[" << i << "] = " << mult << visMsgEnd;
    
      visMsgDbg << "multVal[" << i << "][0] = " << multAttr->getValue(0) <<
	visMsgEnd;
	}*/
    multAttr->mult(attrDim, mult);
    attr->add(attrDim, multAttr);
  }
  attr->div(attrDim, div);
  multAttr->decRef();
}

int visCell2NodeMLinear::getCursor(unsigned int i)
{
  if (!this->isAllocated()) return 0;
  return myCellPos[i];
}

visAttribute* visCell2NodeMLinear::getCellAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  myAttribute[comp]->incRef();
  return myAttribute[comp];
}

void visCell2NodeMLinear::getCellAttribute(int comp, visAttribute* attr)
{
  if (!this->isAllocated()) return;

  attr->setValue(myAttrDim[comp], myAttribute[comp]);
}

const char *visCell2NodeMLinear::getAttrName(int comp) 
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getAttrName(comp);
};

const visAttribute* visCell2NodeMLinear::getMinAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getMinAttribute(comp);
};

const visAttribute* visCell2NodeMLinear::getMaxAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getMaxAttribute(comp);
};

int visCell2NodeMLinear::getCellPos(unsigned int i)
{
  if (!this->isAllocated()) return 0;
  return myCellPos[i];
};

int visCell2NodeMLinear::getCellSize(unsigned int)
{
  return 1;
};

int visCell2NodeMLinear::getCells() 
{
  if (!this->isAllocated()) return 0;

  return myCells;
};

/* -----------------------------------------------------------------------------
   visAdaptiveGridCache Methods
   ---------------------------------------------------------------------------*/

visAdaptiveGridCache::visAdaptiveGridCache() : myName(NULL)
{
};

visAdaptiveGridCache::~visAdaptiveGridCache() 
{
  this->deallocate();
};

bool visAdaptiveGridCache::allocate(visAdaptiveGrid *source) {
  visMsgCall << "visAdaptiveGridCache::allocate: called." << visMsgEnd;

  this->deallocate();

  // check if source is allocated
  if (!source->isAllocated()) {
    visMsgError << "visAdaptiveGridCache::allocate: "
		<< "source field must be allocated."
		<< visMsgErrorEnd;
    return false;
  }

  int dim = source->getDim();
  int *size = new int[dim];
  int i;
  for (i = 0; i < dim; i++) {
    size[i] = source->getSize(i);
  }
  visAttributeFactory** factory =
    new visAttributeFactory*[source->getAttributes()];
  for (i = 0; i < source->getAttributes(); i++) {
    factory[i] = source->getAttrFactory(i);
  }

  // copy attribute association

  myAttrAssoc = source->getAttrAssoc();

  {
    if (!visAdaptiveGrid::allocate(dim, size, source->getAttributes(), factory))
      {
	for (i = 0; i < source->getAttributes(); i++) {
	  factory[i]->decRef();
	}
	delete[] factory;
	delete[] size;
	return false;
      }
  }

  delete[] factory;
  delete[] size;

  // Copy attribute names

  this->myName = new char*[source->getAttributes()];
  for (i = 0; i < source->getAttributes(); i++) {
    this->myName[i] = new char[strlen(source->getAttrName(i)) + 1];
    strcpy(this->myName[i], source->getAttrName(i));
  }

  myCursor = new int[source->getDim()];

  visAttribute *attr;

  // initialize Min Max

  myMinAttribute = new visAttribute*[source->getAttributes()];
  myMaxAttribute = new visAttribute*[source->getAttributes()];

  myAdjustMinMax = false;

  if (myAdjustMinMax) {
    for (i = 0; i < source->getAttributes(); i++) {
      myMinAttribute[i] = myAttrFactory[i]->getNewAttribute();
      myMinAttribute[i]->setValue(myAttrFactory[i]->getDim(), visAttrMax);
      myMaxAttribute[i] = myAttrFactory[i]->getNewAttribute();
      myMaxAttribute[i]->setValue(myAttrFactory[i]->getDim(), visAttrMin);
    }
  } else {
    for (i = 0; i < source->getAttributes(); i++) {
      attr = (visAttribute*)source->getMinAttribute(i);
      myMinAttribute[i] = myAttrFactory[i]->getNewAttribute();
      myMinAttribute[i]->setValue(myAttrFactory[i]->getDim(), attr);
      attr->decRef();
      attr = (visAttribute*)source->getMaxAttribute(i);
      myMaxAttribute[i] = myAttrFactory[i]->getNewAttribute();
      myMaxAttribute[i]->setValue(myAttrFactory[i]->getDim(), attr);
      attr->decRef();
    }
  }
  
  visSETSTATUSIDLE;

  // Set Cursor to origin
  {
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->setCursor(pos);
    delete[] pos;
  }
  return true;
}

void visAdaptiveGridCache::deallocate() {
  if (!this->isAllocated()) return;

  visMsgCall << "visAdaptiveGridCache::deallocate: called." << visMsgEnd;

  int i;
  for (i = 0; i < this->getAttributes(); i++) {
    delete[] myName[i];
  }
  delete[] myName;
  delete[] myCursor;;
  for (i = 0; i < this->getAttributes(); i++) {
    if (myMinAttribute[i]) { myMinAttribute[i]->decRef(); }
    if (myMaxAttribute[i]) { myMaxAttribute[i]->decRef(); }
  }
  delete[] myMinAttribute;
  delete[] myMaxAttribute;

  visAdaptiveGrid::deallocate();
}

void visAdaptiveGridCache::setCursor(const int *p)
{
  if (!this->isAllocated()) return;

  int i;

  for (i = 0; i < this->getDim(); i++) {
    visAssert(p[i] < mySize[i]);
    visAssert(p[i] >= 0);
    myCursor[i] = p[i];
  }

};

int visAdaptiveGridCache::getCursor(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  return myCursor[i];
}

void visAdaptiveGridCache::getCellAttribute(int comp, visAttribute *attr)
{
  if (!this->isAllocated()) return;
  
}

const char *visAdaptiveGridCache::getAttrName(int comp) 
{
  if (!this->isAllocated()) return "<unknown>";

  return myName[comp];
};

const visAttribute* visAdaptiveGridCache::getMinAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  visAttribute *attr = myAttrFactory[comp]->getNewAttribute();
  attr->setValue(myAttrFactory[comp]->getDim(), myMinAttribute[comp]);
  return attr;
};

const visAttribute* visAdaptiveGridCache::getMaxAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  myMaxAttribute[comp]->incRef();
  return myMaxAttribute[comp];
};

int visAdaptiveGridCache::getCellPos(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  return 0;
};

int visAdaptiveGridCache::getCellSize(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  return 1;
};

int visAdaptiveGridCache::getCells() 
{
  if (!this->isAllocated()) return 0;
  return myCells;
};

/* -----------------------------------------------------------------------------
   visMapAdaptiveGrid Methods
   ---------------------------------------------------------------------------*/
visMapAdaptiveGrid::visMapAdaptiveGrid()
{
};

visMapAdaptiveGrid::~visMapAdaptiveGrid()
{
  this->deallocate();
};

bool visMapAdaptiveGrid::allocate(visAdaptiveGrid *adaptiveGrid,
				  float ipad, int renderType,
				  visStack<float> *rangeMinMax,
				  int preferredAssoc)
{
  visMsgCall << "visMapAdaptiveGrid::allocate: called." << visMsgEnd;
  
  this->deallocate();
  return false;
}

void visMapAdaptiveGrid::deallocate()
{
};

/* -----------------------------------------------------------------------------
   visUniformGrid Methods
   ---------------------------------------------------------------------------*/

visUniformGrid::visUniformGrid() : myAttrAssoc(visHasNoData)
{
};

visUniformGrid::~visUniformGrid()
{
  this->deallocate();
};

bool visUniformGrid::allocate(FLD_Grid_Struct *grid_struct,
			      FLD_Node_Data *node_data,
			      FLD_Cell_Data *cell_data,
			      int attrAssoc) 
{
  visMsgCall << "visUniformGrid::allocate: called." << visMsgEnd;

  this->deallocate();

  visAssert(grid_struct);

  if (!node_data) {
    visMsgError << "visUniformGrid::allocate: "
		<< "no node data."
		<< visMsgErrorEnd;
    return false;
  }
  
  visMsgDbg << "a11" << visMsgEnd;
  int dim = (int) grid_struct->ndim;
  visMsgDbg << "a10" << visMsgEnd;

  if (dim <= 0) {
    visMsgError << "visUniformGrid::allocate: "
		<< "field dimension must be greater than one."
		<< visMsgErrorEnd;
    return false;
  }
  visMsgDbg << "a12" << visMsgEnd;

  // allocate factory for initialization of grid

  int attributeCount = (int) node_data->nnode_data;

  visMsgDbg << "a1" << visMsgEnd;
  visAttributeFactory** factory = new visAttributeFactory*[attributeCount];
  int i;
  for (i = 0; i < attributeCount; i++) {
    int attrDim = node_data->node_data[i].veclen;
    if (attrDim == 1) {
      factory[i] = new visScalarAttributeFactory();
    } else {
      factory[i] = new visVectorAttributeFactory(attrDim);
    }
  }

  // Get attr association

  myAttrAssoc = (attrAssoc == 0 ? visHasNodeData : attrAssoc);
  if (myAttrAssoc != visHasNodeData && myAttrAssoc != visHasCellData) {
    visMsgError << "visUniformGrid::allocate: "
		<< "only node or cell data association supported!" << visMsgEnd;
  }

  // copy size and adjust from node to cell size
  
  visMsgDbg << "a2" << visMsgEnd;
  int* size = new int[dim];
  {
    int *p = (int *) grid_struct->dims.ret_array_ptr(OM_GET_ARRAY_RD);
    for (i = 0; i < dim; i++) {
      if (myAttrAssoc == visHasCellData) {
	size[i] = p[i];
      } else {
	size[i] = p[i] - 1;
      }
    }
    ARRfree(p);
  }

  // init field size, index, maxindex and number of cells
  // and allocate grid
  
  {
    if (!size) return false;
    myIndex = 0;
    myCells = 1;
    myNodes = 1;
    for (i = 0; i < dim; i++) {
      myCells *= size[i];
      myNodes *= size[i] + 1;
    }
    myMaxIndex = new int[attributeCount];
    for (i = 0; i < attributeCount; i++) {
      myMaxIndex[i] = ((myAttrAssoc & visHasNodeData) ? myNodes : myCells) *
	factory[i]->getDim();
    }
    if (!visAdaptiveGrid::allocate(dim, size,
				   attributeCount, factory)) {
      delete[] size;
      return false;
    }
  }
  delete[] size;

  for (i = 0; i < attributeCount; i++) {
    factory[i]->decRef();
  }
  delete[] factory;

  // allocate offset table

  {
    myOffset = new int[dim];
    int j;
    for (i = 0; i < dim; i++) {
      myOffset[i] = 1;
      for (j = 0; j < i; j++) {
	if (myAttrAssoc & visHasNodeData) {
	  myOffset[i] *= this->getSize(j) + 1;
	} else {
	  myOffset[i] *= this->getSize(j);
	}
      }
    }
  }
  visMsgDbg << "a4" << visMsgEnd;
    
  // copy field data

  {
    myData = new double*[attributeCount];
    int a;
    for (a = 0; a < attributeCount; a++) {
      double *p = (double*) node_data->node_data[a].values.
	ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_DOUBLE);
      if (!p) {
	visMsgError << "visUniformGrid::allocate: couldn't allocate array."
		    << visMsgEnd;
	return false;
      }
      myData[a] = 
	new double[((myAttrAssoc & visHasNodeData) ? myNodes : myCells) *
		  myAttrFactory[a]->getDim()];
      memcpy(myData[a], p, sizeof(double) *
	     ((myAttrAssoc & visHasNodeData) ? myNodes : myCells) *
	     myAttrFactory[a]->getDim());
      ARRfree((void *) p);
    }
  }

  // initialize Min Max

  visMsgDbg << "a3" << visMsgEnd;

  {
    int o, a, d;
    myMinAttribute = new visAttribute*[attributeCount];
    myMaxAttribute = new visAttribute*[attributeCount];
    for (a = 0; a < attributeCount; a++) {
      myMinAttribute[a] = myAttrFactory[a]->getNewAttribute();
      myMaxAttribute[a] = myAttrFactory[a]->getNewAttribute();
      visAttribute *attr = myAttrFactory[a]->getNewAttribute();

      /*      double *minVector = (double*) node_data->node_data[a].
	min_vec.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_DOUBLE);
      double *maxVector = (double*)node_data->node_data[a].
      max_vec.ret_typed_array_ptr(OM_GET_ARRAY_RD, OM_TYPE_DOUBLE);*/
      
      myMinAttribute[a]->setValue(myAttrFactory[a]->getDim(), visAttrMax);
      myMaxAttribute[a]->setValue(myAttrFactory[a]->getDim(), visAttrMin);
      for (o = 0; o < myMaxIndex[a]; o += myAttrFactory[a]->getDim()) {
	for (d = 0; d < myAttrFactory[a]->getDim(); d++) {
	  attr->setValue(d, myData[a][o+d]);
	}
	if (attr->isLess(myAttrFactory[a]->getDim(), myMinAttribute[a])) {
	  myMinAttribute[a]->setValue(myAttrFactory[a]->getDim(), attr);
	}
	if (myMaxAttribute[a]->isLess(myAttrFactory[a]->getDim(), attr)) {
	  myMaxAttribute[a]->setValue(myAttrFactory[a]->getDim(), attr);
	}
      }

      /*ARRfree(minVector);
	ARRfree(maxVector);*/
      attr->decRef();
    }
  }

  // copy attribute names

  {
    int a;
    myName = new char*[attributeCount];
    for (a = 0; a < attributeCount; a++) {
      char *s = (char*) node_data->node_data[a].labels;
      if (s) {
	myName[a] = new char[strlen(s) + 1];
	strcpy(myName[a], s);
      } else {
	myName[a] = new char[strlen("<unknown>") + 1];
	strcpy(myName[a], "<unknown>");
      }
    }
  }
  
  // Allocate cursor and set to origin

  {
    myCursor = new int[dim];
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->setCursor(pos);
    delete[] pos;
  }

  visMsgDbg << "visUniformAccess: size = " << visMsgEnd;
  for (i = 0; i < myDim; i++) {
    visMsgDbg << "size[" << i << "] = " << mySize[i] << visMsgEnd;
  }

  // sign success

  return true;
};

void visUniformGrid::deallocate() 
{
  if (!this->isAllocated()) return;

  visMsgCall << "visUniformGrid::deallocate: called." << visMsgEnd;

  int i, a;
  visMsgDbg << "a8" << visMsgEnd;
  for (a = 0; a < this->getAttributes(); a++) {
    delete[] myMinAttribute[a];
    delete[] myMaxAttribute[a];
    delete[] myData[a];
    delete[] myName[a];
  }
  delete[] myMaxAttribute;
  visMsgDbg << "a7" << visMsgEnd;
  delete[] myMinAttribute;
  delete[] myData;
  delete[] myName;
  visMsgDbg << "a6" << visMsgEnd;
  delete[] myOffset;
  delete[] myCursor;
  delete[] myMaxIndex;
  visMsgDbg << "a5" << visMsgEnd;

  visAdaptiveGrid::deallocate();
}

void visUniformGrid::setCursor(const int *p) 
{
  myIndex = 0;
  int i;
  for (i = 0; i < this->getDim(); i++) {
    visAssert(p[i] < mySize[i]);
    visAssert(p[i] >= 0);
    myCursor[i] = p[i];
    //myIndex += myCursor[i] * myOffset[i];
  }
}

int visUniformGrid::getCursor(unsigned int i)
{
  return myCursor[i];
}

void visUniformGrid::getCellAttribute(int a, visAttribute* attr)
{
  int i, index = 0;
  for (i = 0; i < myDim; i++) {
    index += myCursor[i] * myOffset[i];
  }
  index *= myAttrFactory[a]->getDim();
  double *buffer = attr->lendBuffer(myAttrFactory[a]->getDim());
  for (i = 0; i < myAttrFactory[a]->getDim(); i++) {
    buffer[i] = myData[a][index + i];
  }
  attr->takebackBuffer(myAttrFactory[a]->getDim(), buffer);
}

void visUniformGrid::getNodeAttribute(int a, visAttribute* attr, int n)
{
  int i, index = 0;
  for (i = 0; i < myDim; i++) {
    index += myCursor[i] * myOffset[i];
    if ((n & (1 << i)) != 0) index += myOffset[i];
  }
  index *= myAttrFactory[a]->getDim();
  double *buffer = attr->lendBuffer(myAttrFactory[a]->getDim());
  for (i = 0; i < myAttrFactory[a]->getDim(); i++) {
    buffer[i] = myData[a][i + index];
  }
  attr->takebackBuffer(myAttrFactory[a]->getDim(), buffer);
}

const char *visUniformGrid::getAttrName(int comp) 
{
  if (!this->isAllocated()) return "<unknown>";
  return myName[comp];
};

const visAttribute* visUniformGrid::getMinAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  visAttribute* attr = myAttrFactory[comp]->getNewAttribute();
  attr->setValue(myAttrFactory[comp]->getDim(), myMinAttribute[comp]);
  return attr;
};

const visAttribute* visUniformGrid::getMaxAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  visAttribute* attr = myAttrFactory[comp]->getNewAttribute();
  attr->setValue(myAttrFactory[comp]->getDim(), myMaxAttribute[comp]);
  return attr;
};

int visUniformGrid::getCellPos(unsigned int i)
{
  if (!this->isAllocated()) return 0;
  return myCursor[i];
}

int visUniformGrid::getCellSize(unsigned int)
{
  return 1;
}
 
int visUniformGrid::getCells() 
{
  return myCells;
};

int visUniformGrid::getNodes() 
{
  return myNodes;
};


/* -----------------------------------------------------------------------------
   visOctree Methods
   ---------------------------------------------------------------------------*/

visOctree::visOctree() : myOctreeRoot(NULL),
  myName(NULL),
  myCursorPath(NULL), myVirtualSize(0),
  myIsValidCursor(false), myMaxLevel(0)
{
};

visOctree::~visOctree() 
{
  this->deallocate();
};

bool visOctree::allocate(visAdaptiveGrid *source,
			 visAttrComb *attrComb, 
			 int maxlevel) {
  visMsgCall << "visOctree::allocate: called." << visMsgEnd;
  
  this->deallocate();
  
  // check if source is allocated
  
  if (!source->isAllocated()) {
    visMsgError << "visOctree::allocate: "
		<< "source field must be allocated."
		<< visMsgErrorEnd;
    return false;
  }

  if (!(source->getAttrAssoc() & visHasCellData)) {
    visMsgError << "visOctree::allocate: "
		<< "only cell associated source fields supported."
		<< visMsgErrorEnd;
    return false;
  }
  
  int dim = source->getDim();
  int *size = new int[dim];
  int i;
  for (i = 0; i < dim; i++) {
    size[i] = source->getSize(i);
  }
  visAttributeFactory** factory =
    new visAttributeFactory*[source->getAttributes()];
  for (i = 0; i < source->getAttributes(); i++) {
    factory[i] = source->getAttrFactory(i);
  }

  // get attribute combiner

  myAttrComb = attrComb;
  if (myAttrComb == NULL) {
    myAttrComb = new visAttrCombDefault();
  } else {
    myAttrComb->incRef();
  }

  // Not a very nice interpolation as yet - but things can improve

  if (!visAdaptiveGrid::allocate(dim, size,
				 source->getAttributes(), factory)) {
    for (i = 0; i < source->getAttributes(); i++) {
      factory[i]->decRef();
    }
    delete[] factory;
    delete[] size;
    return false;
  }

  delete[] factory;
  delete[] size;

  // Copy attribute names

  this->myName = new char*[source->getAttributes()];
  for (i = 0; i < source->getAttributes(); i++) {
    this->myName[i] = new char[strlen(source->getAttrName(i)) + 1];
    strcpy(this->myName[i], source->getAttrName(i));
  }

  myCursor = new int[source->getDim()];

  // Generate Tree
  
  // Initialize adaptive parameters

  if (maxlevel <= this->myMaxLevel) {
    this->myMaxVirtualLevel = maxlevel;
  } else {
    this->myMaxVirtualLevel = this->myMaxLevel;
  }

  // Get maximum size of all components
  
  int maxSizeComp = 0;
  for (i = 0; i < source->getDim(); i++) {
    if (maxSizeComp < source->getSize(i)) {
      maxSizeComp = source->getSize(i);
    }
  }
  
  // Use this to calculate virtual size and max cursor level
  
  myMaxLevel = 0; myVirtualSize = 1;
  int s = maxSizeComp * 2 - 1;
  while ((s /= 2) > 0) { 
    myMaxLevel++;
    myVirtualSize *= 2; 
  }
  
  visMsgDbg << "myVirtualSize = " << this->myVirtualSize
	    << visMsgEnd;
  visMsgDbg << "myMaxLevel = " << this->myMaxLevel
	    << visMsgEnd;
  

  visOctreeNode* n = new visOctreeNode(source->getDim(),
				       source->getAttributes());
  this->myOctreeRoot = n;
  int *pos = new int[source->getDim()];
  for (i = 0; i < source->getDim(); i++) {
    pos[i] = 0;
  }
  
  // initialize cursor information

  this->myCursorPath = 
    new visCursorPathElement[this->myMaxLevel + 1];
  for (i = 0; i < this->myMaxLevel + 1; i++) {
    this->myCursorPath[i].init(source->getDim());
  }
  this->myCursorPath[0].node = this->myOctreeRoot;
  this->myCursorLevel = 0;

  // build tree

  myFieldAccess = source;
  this->buildTree();

  visAttribute *attr;

  // initialize Min Max

  myMinAttribute = new visAttribute*[source->getAttributes()];
  myMaxAttribute = new visAttribute*[source->getAttributes()];

  myAdjustMinMax = false;

  if (myAdjustMinMax) {
    for (i = 0; i < source->getAttributes(); i++) {
      myMinAttribute[i] = myOctreeRoot->attribute[i];
      myMaxAttribute[i] = myOctreeRoot->attribute[i];
      myMinAttribute[i]->incRef();
      myMaxAttribute[i]->incRef();
    }
  } else {
    for (i = 0; i < source->getAttributes(); i++) {
      attr = (visAttribute*)myFieldAccess->getMinAttribute(i);
      myMinAttribute[i] = myAttrFactory[i]->getNewAttribute();
      myMinAttribute[i]->setValue(myAttrFactory[i]->getDim(), attr);
      attr->decRef();
      attr = (visAttribute*)myFieldAccess->getMaxAttribute(i);
      myMaxAttribute[i] = myAttrFactory[i]->getNewAttribute();
      myMaxAttribute[i]->setValue(myAttrFactory[i]->getDim(), attr);
      attr->decRef();
    }
  }

  myCells = new int[myMaxLevel + 1];
  for (i = 0; i < myMaxLevel + 1; i++) {
    myCells[i] = 0;
  }
  initNumberOfCells(this->myOctreeRoot, 0);

  visMsgDbg << "Tree built" << visMsgEnd;

  for (i = 0; i < source->getDim(); i++) {
    pos[i] = 0;
  }
  //  this->dumpTree(this->myOctreeRoot, 0, pos);

  this->myCursorPath[0].node = this->myOctreeRoot;
  this->myCursorLevel = 0;
  this->myIsValidCursor = true;
  
  visSETSTATUSIDLE;

  // Set Cursor to origin
  {
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->setCursor(pos);
    delete[] pos;
  }

  myFieldAccess = NULL;

  return true;
}

bool visOctree::allocate(istream &in) {
  visMsgCall << "visOctree::allocate: called." << visMsgEnd;

  this->deallocate();

  // test if stream ok

  if (in.bad()) {
    visMsgError << "bad istream!" << visMsgErrorEnd;
    return false;
  }
  
  // skip comments
  
  char s[256], line[256];
#ifdef _MSC_VER
  in.eatwhite();
#else
  eatwhite(in);
#endif
  do {
    in >> s;
    if (s[0] == '#') {
      in.getline(line, 256);
    }
  } while (!in.eof() && s[0] == '#');
  
  int i;
  for (i = 0; i < strlen(s); i++) {
    s[i] = tolower(s[i]);
  }
  int attrSize;
  int dim;
  int attributeCount;

  istrstream *header;

#define visOctFormatUnknown 0
#define visOctFormatOctree  1
#define visOctFormatTree    2

  int format = visOctFormatUnknown;
  
  if (!strcmp(s, "octree")) {
    format = visOctFormatOctree;
    // hardcoded hack
    attrSize = 1;
    dim = 3;
    attributeCount = 1;
  } else if (!strcmp(s, "tree")) {
    format = visOctFormatTree;
    char c;
    attrSize = 1;
    in >> dim;
    dim = (int)((log((float)dim) / log(2.0f)) + 0.5f);
    visAssert(dim >= 1);

#ifdef _MSC_VER
  in.eatwhite();
#else
  eatwhite(in);
#endif

    // read header into stack and then goto last line of header
    
    {
      ostrstream headerbuf;
      do {
	in.read(&c, 1);
	headerbuf.write(&c, 1);
      } while (!in.eof() && (c == '[' || c == ']' || c == '(' || c == ')' ||
			     isdigit(c)));
      header = new istrstream(headerbuf.str(), strlen(headerbuf.str()));
      visMsgDbg << "structure: " << headerbuf.str() << visMsgEnd;
    }
    
    // Get attributecount by reading first data line

    const int dataLineSize = 512;
    char dataLine[dataLineSize];
    int dataOffset = in.tellg();
#ifdef _MSC_VER
    // ugly workaround for MS Visual C++: tellg moves filepointer back!
    char buffer[dataLineSize];
    in.eatwhite();
    in.getline(buffer, dataLineSize); 
    visMsgDbg << "read " << buffer << visMsgEnd;
#endif
#ifdef _MSC_VER
    in.eatwhite();
#else
    eatwhite(in);
#endif
    in.getline(dataLine, dataLineSize); 
    if (in.gcount() <= 0) {
      visMsgError << "no data" << visMsgErrorEnd;
      delete header;
      return false;
    }
    in.seekg(dataOffset, ios::beg);
#ifdef _MSC_VER
    // ugly workaround for MS Visual C++: tellg moves filepointer back!
    in.eatwhite();
    in.getline(buffer, dataLineSize); 
    visMsgDbg << "read " << dataLine << visMsgEnd;
#endif
#ifdef _MSC_VER
    in.eatwhite();
#else
    eatwhite(in);
#endif
    istrstream str(dataLine, strlen(dataLine));
    int n = -1;
    do {
      str >> s;
      n++;
    } while (!str.eof());
    attributeCount = n;
    if (attributeCount <= 0) {
      visMsgError << "no data" << visMsgErrorEnd;
      delete header;
      return false;
    }
  } else {
    visMsgError << "Unsupported octree file format\n" << visMsgErrorEnd;
    return false;
  }

  visMsgDbg << "attributeCount = " << attributeCount << visMsgEnd;
  visMsgDbg << "dim = " << dim << visMsgEnd;
  visMsgDbg << "attrSize = " << attrSize << visMsgEnd;

  // hardcoded hack

  visAttributeFactory** factory = new visAttributeFactory*[attributeCount];
  for (i = 0; i < attributeCount; i++) {
    if (attrSize == 1) {
      factory[i] = new visScalarAttributeFactory();
    } else {
      factory[i] = new visVectorAttributeFactory(attrSize);
    }
  }

  // get attribute combiner

  myAttrComb = new visAttrCombDefault();

  int *size = new int[dim];
  for (i = 0; i < dim; i++) {
    size[i] = 1;
  }

  // Allocate structure

  if (!visAdaptiveGrid::allocate(dim, size,
				 attributeCount, factory)) {
    for (i = 0; i < attributeCount; i++) {
      factory[i]->decRef();
    }
    delete[] factory;
    myAttrComb->decRef();
    return false;
  }

  // Generate Tree
  
  this->myOctreeRoot = new visOctreeNode(dim, attributeCount);

  switch (format) {
  case visOctFormatUnknown:
    visAssert(0);
    break;
  case visOctFormatOctree:
    this->myMaxLevel = 
      this->buildTree(this->myOctreeRoot, in, 0, dim, attributeCount, factory);
    break;
  case visOctFormatTree:
    this->myMaxLevel = 
      this->buildTree(this->myOctreeRoot, *header, in, 0,
		      dim, attributeCount, factory);
    break;
  }
  if (this->myMaxLevel < 0) {
    visMsgError << "Octree read error" << visMsgErrorEnd;
    visAdaptiveGrid::deallocate();
    for (i = 0; i < attributeCount; i++) {
      factory[i]->decRef();
    }
    delete[] factory;
    myAttrComb->decRef();
    return false;
  }
  visMsgDbg << "max level = " << this->myMaxLevel << visMsgEnd;

  myVirtualSize = 1 << this->myMaxLevel;
  for (i = 0; i < dim; i++) {
    mySize[i] = myVirtualSize;
  }

  delete[] factory;

  // Copy attribute names

  this->myName = new char*[attributeCount];
  for (i = 0; i < attributeCount; i++) {
    this->myName[i] = new char[strlen("<unknown>") + 1];
    strcpy(this->myName[i], "<unknown>");
  }

  myCursor = new int[dim];

  visMsgDbg << "Tree built" << visMsgEnd;

  // initialize Min Max

  myMinAttribute = new visAttribute*[attributeCount];
  myMaxAttribute = new visAttribute*[attributeCount];

  myAdjustMinMax = true;
  visAttribute* attr;
  if (myAdjustMinMax) {
    for (i = 0; i < attributeCount; i++) {
      myMinAttribute[i] = myOctreeRoot->attribute[i];
      myMaxAttribute[i] = myOctreeRoot->attribute[i];
      myMinAttribute[i]->incRef();
      myMaxAttribute[i]->incRef();
    }
  }

  // allocate and init cursor path

  this->myCursorPath = 
    new visCursorPathElement[this->myMaxLevel + 1];
  for (i = 0; i < this->myMaxLevel + 1; i++) {
    this->myCursorPath[i].init(dim);
  }
  this->myCursorPath[0].node = this->myOctreeRoot;
  this->myCursorLevel = 0;
  this->myIsValidCursor = true;
  
  // allocate and init virtual level

  this->myMaxVirtualLevel = this->myMaxLevel;
  myCells = new int[myMaxLevel + 1];
  for (i = 0; i < myMaxLevel + 1; i++) {
    myCells[i] = 0;
  }
  initNumberOfCells(this->myOctreeRoot, 0);

  visSETSTATUSIDLE;

  // Set Cursor to origin
  {
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->setCursor(pos);
    delete[] pos;
  }

  myFieldAccess = NULL;

  /*{
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->dumpTree(this->myOctreeRoot, 0, pos);
    delete[] pos;
    }*/

  return true;
}

void visOctree::deallocate() {
  if (!this->isAllocated()) return;

  visMsgCall << "visOctree::deallocate: called." << visMsgEnd;

  int i;
  delete[] myCursorPath;
  this->deleteTree(myOctreeRoot, 0);
  for (i = 0; i < this->getAttributes(); i++) {
    delete[] myName[i];
  }
  delete[] myName;
  delete[] myCursor;;
  for (i = 0; i < this->getAttributes(); i++) {
    if (myMinAttribute[i]) { myMinAttribute[i]->decRef(); }
    if (myMaxAttribute[i]) { myMaxAttribute[i]->decRef(); }
  }
  delete[] myMinAttribute;
  delete[] myMaxAttribute;

  delete[] myCells;

  myAttrComb->decRef();

  visAdaptiveGrid::deallocate();
}

void visOctree::setCursor(const int *p)
{
  if (!this->isAllocated()) return;

  
  int i, level, offset = 0;

  for (i = 0; i < this->getDim(); i++) {
    visAssert(p[i] < mySize[i]);
    visAssert(p[i] >= 0);
    myCursor[i] = p[i];
  }

  for (level = 0; level < myMaxVirtualLevel; level++) {
    /*  visMsgDbg << "myCursorPos iterate = (" 
	    << myCursorPath[level].pos[0] << ","
	    << myCursorPath[level].pos[1] << ","
	    << myCursorPath[level].pos[2]
	    << ")"
	    << visMsgEnd;
	    */
  
    if (!myCursorPath[level].node->child) {
      myCursorLevel = level;
      return;
    }
    offset = 0;
    int cursorLevelSize = myVirtualSize / (1 << level);
    for (i = 0; i < this->getDim(); i++) {
      if (p[i] >= myCursorPath[level].pos[i] + cursorLevelSize / 2) {
	offset |= (1 << i);
	myCursorPath[level + 1].pos[i] =
	  myCursorPath[level].pos[i] + cursorLevelSize / 2;
      } else {
	myCursorPath[level + 1].pos[i] =
	  myCursorPath[level].pos[i];
      }
    }
    if (myCursorPath[level].node->child[offset]) {
      myCursorPath[level + 1].node = myCursorPath[level].node->child[offset];
    } else {
      myCursorLevel = level;
      return;
    }
    //    visMsgDbg << offset << visMsgEnd;
  }
  myCursorLevel = myMaxVirtualLevel;
};

int visOctree::getCursor(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  return myCursor[i];
}
visAttribute* visOctree::getCellAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  
  visAttribute *p = myCursorPath[myCursorLevel].node->attribute[comp];
  p->incRef();
  return p;
};

void visOctree::getCellAttribute(int comp, visAttribute *attr)
{
  if (!this->isAllocated()) return;
  
  int i, attrDim = myAttrFactory[comp]->getDim();
  double *buffer = attr->lendBuffer(attrDim);
  for (i = 0; i < attrDim; i++) {
    buffer[i] = myCursorPath[myCursorLevel].node->attribute[comp]->getValue(i);
  }
  attr->takebackBuffer(attrDim, buffer);
}

const char *visOctree::getAttrName(int comp) 
{
  if (!this->isAllocated()) return "<unknown>";

  return myName[comp];
};

const visAttribute* visOctree::getMinAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  visAttribute *attr = myAttrFactory[comp]->getNewAttribute();
  attr->setValue(myAttrFactory[comp]->getDim(), myMinAttribute[comp]);
  return attr;
};

const visAttribute* visOctree::getMaxAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  myMaxAttribute[comp]->incRef();
  return myMaxAttribute[comp];
};

int visOctree::getCellPos(unsigned int i) 
{
  if (!this->isAllocated()) return 0;

  return myCursorPath[myCursorLevel].pos[i];
};

int visOctree::getCellSize(unsigned int i) 
{
  if (!this->isAllocated()) return 0;

  int s = myVirtualSize / (1 << myCursorLevel);
  int p = this->getCellPos(i);
  if (p + s > this->getSize(i)) {
    return this->getSize(i) - p;
  } else {
    return s;
  }
};

int visOctree::getCells() 
{
  if (!this->isAllocated()) return 0;
  return myCells[myMaxVirtualLevel];
};

void visOctree::dumpTree(visOctreeNode *node, int level, int *pos) {
  int i, j;
  strstream s;

  for (i = 0; i < level; i++) {
    s << " ";
  }
  s << ends;
  if (node) {
    if (node->attribute) {
      visMsgDbg << s.str() << level << ":" << endl
		<< " pos = ("
		<< pos[0] << "," << pos[1] << ", " << pos[2] << ")" 
		<< " value = " 
		<< node->attribute[0]->
	getScalarValue(myAttrFactory[0]->getDim())
		<< visMsgEnd;
    }
    if (node->child) {
      int *newPos = new int[this->getDim()];
      int cursorLevelSize = myVirtualSize / (1 << level);
      for (i = 0; i < (1 << this->getDim()); i++) {
	for (j = 0; j < this->getDim(); j++) {
	  newPos[j] = pos[j] + ((i & (1 << j)) != 0 ? cursorLevelSize / 2 : 0);
	}
	if (node->child[i]) {
	  dumpTree(node->child[i], level + 1, newPos);
	}
      }
      delete[] newPos;
    }

  } else {
    visMsgDbg << s.str() << "x" << visMsgEnd;
  }
}


int visOctree::buildTree(visOctreeNode *node, istream &in, int level,
			 const int dim, const int attributeCount,
			 visAttributeFactory** factory) 
{
  char c;
  visOctreeAttrAccess *attrAccess;
  int i, l, newLevel = level;
  in >> c;
  visSYSTEM.setStatus("HiVis Building Octree");

  switch (c) {
  case '(':
    for (i = 0; i < (1 << dim); i++) {
      node->child[i] = new visOctreeNode(dim, attributeCount);
      l = this->buildTree(node->child[i], in, level + 1, 
			  dim, attributeCount, factory);
      if (l < 0) {
	return l;
      }
      if (l > newLevel) newLevel = l;
    }
    in >> c;
    if (c != ')') return -1;

    // Take average attribute of subcomponents via combiner

    attrAccess = new visOctreeAttrAccess(node, this);
    myAttrComb->allocate(attrAccess);
    for (i = 0; i < attributeCount; i++) {
      node->attribute[i] = myAttrComb->getAverageAttribute(i);
    }
    /*
    if (mayCombine && myAttrComb->isEqual()) {

      // Delete subtree with same values as defined by comb

      for (i = 0; i < (1 << dim); i++) {
	deleteTree(node->child[i], level + 1);
	myCursorPath[myCursorLevel].node->child[i] = NULL;
      }
      delete[] node->child;
      node->child = NULL;
    } else {
      mayCombine = false;
    }
    */

    attrAccess->decRef();

    break;
  case '0':
    delete[] node->child; node->child = NULL;
    for (i = 0; i < attributeCount; i++) {
      node->attribute[i] = factory[i]->getNewAttribute();
      node->attribute[i]->setValue(factory[i]->getDim(), visAttrAddNeutral);
    }
    break;
  case '1':
    delete[] node->child; node->child = NULL;
    for (i = 0; i < attributeCount; i++) {
      node->attribute[i] = factory[i]->getNewAttribute();
      node->attribute[i]->setValue(factory[i]->getDim(), visAttrMultNeutral);
    }
    break;
  default:
    return -1;
  }
  //  visOctreeNode* = new visOctreeNode(dim, attributeCount);
  
  return newLevel;
}

int visOctree::buildTree(visOctreeNode *node, istream &header, 
			       istream &data, int level,
			       const int dim, const int attributeCount,
			       visAttributeFactory** factory) 
{
  char c;
  char s[256];
  double x;
  visOctreeAttrAccess *attrAccess;
  int i, j, l, newLevel = level;
  header >> c;
  visSYSTEM.setStatus("HiVis Building Octree");
  switch (c) {
  case '(':
    for (i = 0; i < (1 << dim); i++) {
      node->child[i] = new visOctreeNode(dim, attributeCount);
      l = this->buildTree(node->child[i], header, data, level + 1, 
			  dim, attributeCount, factory);
      if (l < 0) {
	return l;
      }
      if (l > newLevel) newLevel = l;
    }
    header >> c;
    if (c != ')') return -1;

    // Take average attribute of subcomponents via combiner

    attrAccess = new visOctreeAttrAccess(node, this);
    myAttrComb->allocate(attrAccess);
    for (i = 0; i < attributeCount; i++) {
      node->attribute[i] = myAttrComb->getAverageAttribute(i);
    }
    attrAccess->decRef();
    break;
  case '[':
    for (i = 0; i < (1 << dim); i++) {
      node->child[i] = new visOctreeNode(dim, attributeCount);
      l = this->buildTree(node->child[i], header, data, level + 1, 
			  dim, attributeCount, factory);
      if (l < 0) {
	return l;
      }
      if (l > newLevel) newLevel = l;
    }
    header >> c;
    if (c != ']') return -1;

    data >> s;
    for (i = 0; i < attributeCount; i++) {
      node->attribute[i] = factory[i]->getNewAttribute();
      for (j = 0; j < factory[i]->getDim(); j++) {
	data >> x;
	node->attribute[i]->setValue(j, x);
      }
    }
    break;
  case '0':
    delete[] node->child; node->child = NULL;
    for (i = 0; i < attributeCount; i++) {
      node->attribute[i] = factory[i]->getNewAttribute();
      node->attribute[i]->setValue(factory[i]->getDim(), visAttrAddNeutral);
    }
    break;
  case '1':
    delete[] node->child; node->child = NULL;
    data >> s;
    for (i = 0; i < attributeCount; i++) {
      node->attribute[i] = factory[i]->getNewAttribute();
      for (j = 0; j < factory[i]->getDim(); j++) {
	data >> x;
	node->attribute[i]->setValue(j, x);
      }
    }
    break;
  default:
    return -1;
  }
  //  visOctreeNode* = new visOctreeNode(dim, attributeCount);
  
  return newLevel;
}

bool visOctree::buildTree() {
  bool mayCombine = true;

  if (!myCursorPath[myCursorLevel].node)
    return mayCombine;

  int i;

  visSYSTEM.setStatus("HiVis Building Octree");
  //  visMsgDbg << "called: myCursorLevel = " << myCursorLevel << " pos = (" << pos[0] << ","
  //	    << pos[1] << ", " << pos[2] << ")" << visMsgEnd;

  int j;
  int cursorLevelSize = myVirtualSize / (1 << myCursorLevel);
  if (cursorLevelSize <= 1) {

    // init attribute

    myFieldAccess->setCursor(myCursorPath[myCursorLevel].pos);
    for (j = 0; j < this->getAttributes(); j++) {
      myCursorPath[myCursorLevel].node->attribute[j] = 
	(visAttribute *) myFieldAccess->getCellAttribute(j);
      myCursorPath[myCursorLevel].node->attribute[j]->incRef();
      myCursorPath[myCursorLevel].node->attribute[j]->decRef();
    }
    if (myCursorPath[myCursorLevel].node->child) {
      delete[] myCursorPath[myCursorLevel].node->child;
      myCursorPath[myCursorLevel].node->child = NULL;
    }
  } else {

    for (i = 0; i < (1 << this->getDim()); i++) {

      // calculate child pos

      for (j = 0; j < this->getDim(); j++) {
	myCursorPath[myCursorLevel + 1].pos[j] =
	  myCursorPath[myCursorLevel].pos[j] +
	  ((i & (1 << j)) != 0 ? cursorLevelSize / 2 : 0);
      }

      // if not valid pos then declare as NULL

      if (!isValidPos(myCursorPath[myCursorLevel + 1].pos)) {
	myCursorPath[myCursorLevel].node->child[i] = NULL;
      } else {
	myCursorPath[myCursorLevel].node->child[i] =
	  new visOctreeNode(this->getDim(), 
			    this->getAttributes());
	myCursorPath[myCursorLevel + 1].node =
	  myCursorPath[myCursorLevel].node->child[i];
	myCursorLevel++;
	bool b = buildTree();
	mayCombine = mayCombine && b;
	myCursorLevel--;
      }
    }

    // Take average attribute of subcomponents via combiner

#ifdef NEWCOMB
    visAttrCombiner* attrCombiner = myAttrCombiner->getNewAttrCombiner();

    for (j = 0; j < this->getAttributes(); j++) {
      attrCombiner->
	addAttrCombiner(myCursorPath[myCursorLevel].attrCombiner[j]);
    }
    if (mayCombine && attrCombiner->mayCombineAverage()) {
      
    }
    
#else

    visOctreeAttrAccess *attrAccess =
      new visOctreeAttrAccess(myCursorPath[myCursorLevel].node, this);
    myAttrComb->allocate(attrAccess);
    for (j = 0; j < this->getAttributes(); j++) {
      myCursorPath[myCursorLevel].node->attribute[j] =
	myAttrComb->getAverageAttribute(j);
    }
    if (mayCombine && (myAttrComb->isEqual() ||
		       myCursorLevel >= myMaxVirtualLevel)) {

      // Delete subtree with same values as defined by comb

      for (i = 0; i < (1 << this->getDim()); i++) {
	deleteTree(myCursorPath[myCursorLevel].node->child[i], 
		   myCursorLevel + 1);
	myCursorPath[myCursorLevel].node->child[i] = NULL;
      }
      delete[] myCursorPath[myCursorLevel].node->child;
      myCursorPath[myCursorLevel].node->child = NULL;
    } else {
      mayCombine = false;
    }
    attrAccess->decRef();
  }
  return mayCombine;
#endif
}
  
void visOctree::deleteTree(visOctreeNode *node, int level) {
  //visSYSTEM.setStatus("HiVis Deleting Octree");
  if (node) {

    int i;

    if (node->attribute) {
      for (i = 0; i < this->getAttributes(); i++) {
	if (node->attribute[i]) {
	  node->attribute[i]->decRef();
	  node->attribute[i] = NULL;
	}
      }
      delete[] node->attribute;
      node->attribute = NULL;
    }
    if (node->child) {
      for (i = 0; i < (1 << this->getDim()); i++) {
	deleteTree(node->child[i], level + 1);
	node->child[i] = NULL;
      }
      delete[] node->child;
      node->child = NULL;
    }
    delete node;
  }
}

void visOctree::initNumberOfCells(visOctreeNode *node, int level) {
  int i;
  if (!node) return;

  myCells[level]++;
  if (node->child) {
    for (i = 0; i < (1 << this->getDim()); i++) {
      initNumberOfCells(node->child[i], level + 1);
    }
  } else {
    for (i = level + 1; i < myMaxLevel + 1; i++) {
      myCells[i]++;
    }
    if (myAdjustMinMax) {
      for (i = 0; i < this->getAttributes(); i++) {
	if (myMaxAttribute[i]->isLess(myAttrFactory[i]->getDim(), 
				      node->attribute[i])) {
	  myMaxAttribute[i]->decRef();
	  myMaxAttribute[i] = node->attribute[i];
	  myMaxAttribute[i]->incRef();
	}
	if (node->attribute[i]->isLess(myAttrFactory[i]->getDim(),
				       myMinAttribute[i])) {
	  myMinAttribute[i]->decRef();
	  myMinAttribute[i] = node->attribute[i];
	  myMinAttribute[i]->incRef();
	}
      }
    }
  }
}

bool visOctree::isValidPos(int *pos) {
  int i;
  bool valid = true;
  for (i = 0; i < this->getDim(); i++) {
    if (pos[i] >= this->getSize(i)) {
      valid = false;
    }
  }
  return valid;
};

/* -----------------------------------------------------------------------------
   visSparseGrid Methods
   ---------------------------------------------------------------------------*/

visSparseGrid::visSparseGrid() :
  myName(NULL)
{
};

visSparseGrid::~visSparseGrid() 
{
  this->deallocate();
};
  
bool visSparseGrid::allocate(visAdaptiveGrid *source,
			     visAttrComb *attrComb, 
			     int maxlevel, int maxLdScopeDiff = 0)
{
  visMsgCall << "visSparseGrid::allocate: called." << visMsgEnd;

  this->deallocate();

  myMaxLdScopeDiff = maxLdScopeDiff;

  // init node data options

  myIsScopeClipped = false;

  int i;

  // check if source is allocated

  if (!source->isAllocated()) {
    visMsgError << "visSparseGrid::allocate: "
		<< "source field must be allocated."
		<< visMsgErrorEnd;
    return false;
  }

  // Check if field is node associated

  if (!(source->getAttrAssoc() & visHasNodeData)) {
    visMsgError << "visSparseGrid::allocate"
		<< ": source field must contain node associated data."
		<< visMsgErrorEnd;
    return false;
  }
  
  // copy dimension

  int dim = source->getDim();

  int *size = new int[dim];
  for (i = 0; i < dim; i++) {
    size[i] = source->getSize(i);
    if (size[i] < 2) {
      visMsgError << "visSparseGrid::allocate: "
		  << "source field size yet must be at least at least 2 "
		  << "in each dimension."
		  << visMsgErrorEnd;
      return false;
    }
  }

  // copy factories

  visAttributeFactory** factory =
    new visAttributeFactory*[source->getAttributes()];
  for (i = 0; i < source->getAttributes(); i++) {
    factory[i] = source->getAttrFactory(i);
  }

  // copy attribute names

  myName = new char*[source->getAttributes()];
  for (i = 0; i < source->getAttributes(); i++) {
    this->myName[i] = new char[strlen(source->getAttrName(i)) + 1];
    strcpy(this->myName[i], source->getAttrName(i));
  }

  // copy min and max attribute

  {
    int a;
    myMinAttribute = new visAttribute*[source->getAttributes()];
    myMaxAttribute = new visAttribute*[source->getAttributes()];
    for (a = 0; a < source->getAttributes(); a++) {
      myMinAttribute[a] = (visAttribute*)source->getMinAttribute(a);
      myMaxAttribute[a] = (visAttribute*)source->getMaxAttribute(a);
    }
  }

  // initialize superclass

  if (!visAdaptiveGrid::allocate(dim, size, source->getAttributes(), 
			      factory)) {
    for (i = 0; i < source->getAttributes(); i++) {
      factory[i]->decRef();
    }
    delete[] factory;
    delete[] size;
    return false;
  }

  for (i = 0; i < source->getAttributes(); i++) {
    factory[i]->decRef();
  }
  delete[] factory;

  // get attribute combiner

  myAttrComb = attrComb;
  if (myAttrComb == NULL) {
    myAttrComb = new visAttrCombDefault();
  } else {
    myAttrComb->incRef();
  }
  myAttrCompare = new visCompareAttrAccess(this, visAttrAddNeutral);

  // init variables dependend on dim

  myLdSize = new int[dim];
  myCursorPos = new int[dim];
  myCursorBuf = new int[dim];
  for (i = 0; i < dim; i++) {
    myCursorPos[i] = -1;
  }
  myCursorScope = new int[dim];
  myCellPos1 = new int[dim];
  myCellPos2 = new int[dim];

  // calculate virtual size and maximal nodes
  
  myNodesMax = 1;
  for (i = 0; i < dim; i++) {
    myNodesMax *= size[i] + 1;
    myLdSize[i] = 0;
    while ((1 << myLdSize[i]) < (size[i] - 1)) {
      myLdSize[i]++;
    }
    visMsgDbg << "ldsize[" << i << "] = " << myLdSize[i]
	      << visMsgEnd;
  }

  // initialize factor info
  
  {
    int j;
    myFactors = 3;
    for (i = 0; i < this->getDim() - 1; i++) {
      myFactors *= 3;
    }
    myFactorInfo = new visSparseGridFactorInfo[myFactors];
    myFactorInfo[0].factor = 1 << this->getDim();
    if (this->getDim() & 1 != 0) {
      myFactorInfo[0].factor = - myFactorInfo[0].factor;
    }
    myFactorInfo[0].pos = new int[this->getDim()];
    for (i = 0; i < this->getDim(); i++) {
      myFactorInfo[0].pos[i] = -1;
    }
    
    for (i = 1; i < myFactors; i++) {
      myFactorInfo[i].pos = new int[this->getDim()];
      myFactorInfo[i].pos[0] = myFactorInfo[i - 1].pos[0] + 1;
      int remainder;
      for (j = 1; j < this->getDim(); j++) {
	if (myFactorInfo[i].pos[j - 1] > 1) {
	  myFactorInfo[i].pos[j - 1] = -1;
	  remainder = 1;
	} else {
	  remainder = 0;
	}
	myFactorInfo[i].pos[j] = myFactorInfo[i - 1].pos[j] + remainder;
      }
      myFactorInfo[i].factor = 1;
      for (j = 0; j <  this->getDim(); j++) {
	if (myFactorInfo[i].pos[j]) {
	  myFactorInfo[i].factor *= -2;
	}
      }
    }

    /*visMsgDbg << "factor info: entries = " << myFactors << visMsgEnd;
    for (i = 0; i < myFactors; i++) {
      visMsgDbg << "  " << i << ": f = " << myFactorInfo[i].factor
		<< visMsgEnd;
      visMsgDbg << "    p = " << visMsgEnd;
      for (j = 0; j < this->getDim(); j++) {
	visMsgDbg << myFactorInfo[i].pos[j] << visMsgEnd;
      }
    }
    */
  }

  // Alloc attribute buffer and result attribut

  {
    int j;
    myAttributeBuffer = new visAttribute*[this->getAttributes()];
    myResAttribute = new visAttribute*[this->getAttributes()];
    for (i = 0; i < this->getAttributes(); i++) {
      myAttributeBuffer[i] = myAttrFactory[i]->getNewAttribute();
      myResAttribute[i] = myAttrFactory[i]->getNewAttribute();
    }
    myEdgeAttribute = new visAttribute**[this->getAttributes()];
    for (i = 0; i < this->getAttributes(); i++) {
      myEdgeAttribute[i] = new visAttribute*[1 << dim];
      for (j = 0; j < (1 << dim); j++) {
	myEdgeAttribute[i][j] = myAttrFactory[i]->getNewAttribute();
      }
    }
    myFloatPosBuf = new float[dim];
  }

  // calculate maxlevel

  myMaxLevel = 0;
  for (i = 0; i < dim; i++) {
    myMaxLevel += (myLdSize[i] + 2);
  }
  visMsgDbg << "myMaxLevel = " << myMaxLevel << visMsgEnd;

  // init variables dependend on maxlevel

  myLdScope = new int[dim * (myMaxLevel + 1)];
  myPos = new int[dim * (myMaxLevel + 1)];
  myEvalLdScope = new int[dim * (myMaxLevel + 1)];
  myEvalPos = new int[dim * (myMaxLevel + 1)];
  for (i = 0; i < dim; i++) {
    myLdScope[i] = myLdSize[i] + 1;
    myPos[i] = 0;
  }
  myEvalLdScope[0] = myLdSize[0] + 1;
  myEvalPos[0] = 0;

  // build tree

  {
    // Init Statistics variables
    myNodesMax = 1;
    for (i = 0; i < dim; i++) {
      myNodesMax *= mySize[i] + 1;
    }
    myNodes = 0;
    myFieldAccess = source;
    this->buildTree(myRootNode, 0, 0);
    myFieldAccess = NULL;
    visMsgDbg << "Tree built" << visMsgEnd;
    visSETSTATUSIDLE;
  }

  delete[] size;

  // count cells

  {
    int a;
    myCells = 0;
    visAttributeEnumeration *e = new visAdaptiveGridCellEnumeration(this);
    visMsgDbg << "Counting cells" << visMsgEnd;
    char msg[256] = "SparseGrid counting cells ", msg2[256];
    strcpy(msg2, msg);
    e->setCursorFirstElement();
    while (e->isValidElement()) {
      myCells++;
      if (!(myCells % 1000)) {
	strcpy(msg2, msg);
	sprintf(&msg2[strlen(msg2)], "(%d)", myCells);
      }
      visSYSTEM.setStatus(msg2);
      if (visSYSTEM.getStatus() & visStatInterrupt) {
	e->decRef();
	this->deallocate();
	return false;
      }
      e->setCursorNextElement();
    }
    e->decRef();
  }

  // Set Cursor to origin

  {
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->setCursor(pos);
    delete[] pos;
  }

  // Dump statistics

  {
    visMsgDbg << "Nodes = " << myNodes << " of max = " << myNodesMax
	      << ", ratio = " << ((float) myNodes) / myNodesMax << visMsgEnd;
    int a;
    for (a = 0; a < myAttrCount; a++) {
      visMsgDbg << "Min[" << a << "] = " 
		<< myMinAttribute[a]->getScalarValue(myAttrFactory[a]->
						     getDim()) 
		<< " Max[" << a << "] = " 
		<< myMaxAttribute[a]->getScalarValue(myAttrFactory[a]->
						     getDim()) 
		<< visMsgEnd;
    }
  }
  return true;
}

bool visSparseGrid::allocate(istream &in, visAttrComb *attrComb, 
			     int maxlevel, int maxLdScopeDiff = 0)
{
  visMsgCall << "visSparseGrid::allocate: called." << visMsgEnd;

  this->deallocate();

  myMaxLdScopeDiff = maxLdScopeDiff;

  // init node data options

  myIsScopeClipped = true;

  // test if stream ok

  // skip comments
  /*
  char s[256], line[256];
  do {
    in >> s;
    if (s[0] == '#') {
      in.getline(line, 256);
    }
  } while (!in.eof() && s[0] == '#');
  
  int i;
  for (i = 0; i < strlen(s); i++) {
    s[i] = tolower(s[i]);
  }
  if (strcmp(s, "octree")) {
    visMsgError << "Unsupported octree file format\n" << visMsgErrorEnd;
    return false;
  }
  

  // skip blanks

  char c;
  do {
    in >> c;
  } while (!in.eof() && (c == ' ' || c == '\t' || c == '\n'));
  in.seekg(-1, ios::cur);
  */

  int i;
  // Init Statistics variables

  myNodes = 0;

  // alloc factories (neded to build tree)

  int attributeCount = 1;  // hardcoded hack
  visAttributeFactory** factory = new visAttributeFactory*[attributeCount];
  for (i = 0; i < attributeCount; i++) {
    // hardcoded hack
    int attrSize = 1;
    if (attrSize == 1) {
      factory[i] = new visScalarAttributeFactory();
    } else {
      factory[i] = new visVectorAttributeFactory(attrSize);
    }
  }

  // build tree

  {
    myMaxLevelStack.empty();
    double n; in >> n;
    myAttrCount = attributeCount;
    myAttrFactory = factory;
    if (!this->getSparseGridSizeAndDim(myRootNode, in, 0, 0)) {
      visMsgError << "SparseGrid read error" << visMsgErrorEnd;
      return 0;
    }
    
    visMsgDbg << "tree built"  << visMsgEnd;
    // truncate zero sized dimensions
    
    visSparseGridNode *&node = myRootNode;
    int level;
    for (level = 0; level < myMaxLevelStack.getSize() - 1; level++) {
      if (((visSparseGridStructure*&)node)->next != NULL &&
	  node->left == NULL && node->right == NULL) {
	visMsgDbg << "truncating sparsegrid at level " << level 
		  << " overwriting node " << (void*)node << " with " 
		  << (void*)((visSparseGridStructure*&)node)->next << visMsgEnd;
	visSparseGridNode *t = node;
	node = ((visSparseGridStructure*&)node)->next;
	delete t;
      } else {
	break;
      }
    }
  }

  // calculate dim, size from myMaxLevelStack

  int dim = 0;
  int *size = new int[myMaxLevelStack.getSize()];
  for (i = 0; i < myMaxLevelStack.getSize(); i++) {
    if (myMaxLevelStack[i] > 0) {
      size[dim++] = (1 << (myMaxLevelStack[i] - 1));
    }
  }
  myMaxLevelStack.empty();
  visMsgDbg << "SparseGrid dim = " << dim << " maxlevel = " << myMaxLevel
	    << ", size = " << visMsgEnd;
  for (i = 0; i < dim; i++) {
    visMsgDbg << "size [" << i << "] = " << size[i] << visMsgEnd;
  }

  // alloc attribute names

  this->myName = new char*[attributeCount];
  for (i = 0; i < attributeCount; i++) {
    this->myName[i] = new char[strlen("<unknown>") + 1];
    strcpy(this->myName[i], "<unknown>");
  }

  // alloc min and max attribute

  {
    int a;
    myMinAttribute = new visAttribute*[attributeCount];
    myMaxAttribute = new visAttribute*[attributeCount];
    for (a = 0; a < attributeCount; a++) {
      myMinAttribute[a] = myAttrFactory[a]->getNewAttribute();
      myMinAttribute[a]->setValue(myAttrFactory[a]->getDim(), visAttrMax);
      myMaxAttribute[a] = myAttrFactory[a]->getNewAttribute();
      myMaxAttribute[a]->setValue(myAttrFactory[a]->getDim(), visAttrMin);
    }
  }

  // initialize superclass

  if (!visAdaptiveGrid::allocate(dim, size,
				 attributeCount, factory)) {
    for (i = 0; i < attributeCount; i++) {
      factory[i]->decRef();
    }
    delete[] factory;
    delete[] size;
    return false;
  }

  for (i = 0; i < attributeCount; i++) {
    factory[i]->decRef();
  }
  delete[] factory;

  // get attribute combiner

  myAttrComb = attrComb;
  if (myAttrComb == NULL) {
    myAttrComb = new visAttrCombDefault();
  } else {
    myAttrComb->incRef();
  }
  myAttrCompare = new visCompareAttrAccess(this, visAttrAddNeutral);

  // init variables dependend on dim

  myLdSize = new int[dim];
  myCursorPos = new int[dim];
  myCursorBuf = new int[dim];
  for (i = 0; i < dim; i++) {
    myCursorPos[i] = -1;
  }
  myCursorScope = new int[dim];
  myCellPos1 = new int[dim];
  myCellPos2 = new int[dim];

  // calculate virtual size and maximal nodes
  
  myNodesMax = 1;
  for (i = 0; i < dim; i++) {
    myNodesMax *= size[i] + 1;
    myLdSize[i] = 0;
    while ((1 << myLdSize[i]) < size[i]) {
      myLdSize[i]++;
    }
    visMsgDbg << "ldsize[" << i << "] = " << myLdSize[i]
	      << visMsgEnd;
  }

  // initialize factor info

  {
    int j;
    myFactors = 3;
    for (i = 0; i < this->getDim() - 1; i++) {
      myFactors *= 3;
    }
    myFactorInfo = new visSparseGridFactorInfo[myFactors];
    myFactorInfo[0].factor = 1 << this->getDim();
    if (this->getDim() & 1 != 0) {
      myFactorInfo[0].factor = - myFactorInfo[0].factor;
    }
    myFactorInfo[0].pos = new int[this->getDim()];
    for (i = 0; i < this->getDim(); i++) {
      myFactorInfo[0].pos[i] = -1;
    }
    
    for (i = 1; i < myFactors; i++) {
      myFactorInfo[i].pos = new int[this->getDim()];
      myFactorInfo[i].pos[0] = myFactorInfo[i - 1].pos[0] + 1;
      int remainder;
      for (j = 1; j < this->getDim(); j++) {
	if (myFactorInfo[i].pos[j - 1] > 1) {
	  myFactorInfo[i].pos[j - 1] = -1;
	  remainder = 1;
	} else {
	  remainder = 0;
	}
	myFactorInfo[i].pos[j] = myFactorInfo[i - 1].pos[j] + remainder;
      }
      myFactorInfo[i].factor = 1;
      for (j = 0; j <  this->getDim(); j++) {
	if (myFactorInfo[i].pos[j]) {
	  myFactorInfo[i].factor *= -2;
	}
      }
    }

    /*visMsgDbg << "factor info: entries = " << myFactors << visMsgEnd;
    for (i = 0; i < myFactors; i++) {
      visMsgDbg << "  " << i << ": f = " << myFactorInfo[i].factor
		<< visMsgEnd;
      visMsgDbg << "    p = " << visMsgEnd;
      for (j = 0; j < this->getDim(); j++) {
	visMsgDbg << myFactorInfo[i].pos[j] << visMsgEnd;
      }
      }*/
  }

  // Alloc attribute buffer and result attribut

  {
    int j;
    myAttributeBuffer = new visAttribute*[this->getAttributes()];
    myResAttribute = new visAttribute*[this->getAttributes()];
    for (i = 0; i < this->getAttributes(); i++) {
      myAttributeBuffer[i] = myAttrFactory[i]->getNewAttribute();
      myResAttribute[i] = myAttrFactory[i]->getNewAttribute();
    }
    myEdgeAttribute = new visAttribute**[this->getAttributes()];
    for (i = 0; i < this->getAttributes(); i++) {
      myEdgeAttribute[i] = new visAttribute*[1 << dim];
      for (j = 0; j < (1 << dim); j++) {
	myEdgeAttribute[i][j] = myAttrFactory[i]->getNewAttribute();
      }
    }
    myFloatPosBuf = new float[dim];
  }

  // calculate maxlevel

  myMaxLevel = 0;
  for (i = 0; i < dim; i++) {
    myMaxLevel += (myLdSize[i] + 2);
  }
  visMsgDbg << "myMaxLevel = " << myMaxLevel << visMsgEnd;

  // init variables dependend on maxlevel

  myLdScope = new int[dim * (myMaxLevel + 1)];
  myPos = new int[dim * (myMaxLevel + 1)];
  myEvalLdScope = new int[dim * (myMaxLevel + 1)];
  myEvalPos = new int[dim * (myMaxLevel + 1)];
  for (i = 0; i < dim; i++) {
    myLdScope[i] = myLdSize[i] + 1;
    myPos[i] = 0;
  }
  myEvalLdScope[0] = myLdSize[0] + 1;
  myEvalPos[0] = 0;

  delete[] size;

  // count cells

  {
    int a;
    myCells = 0;
    visAttributeEnumeration *e = new visAdaptiveGridCellEnumeration(this);
    visMsgDbg << "Counting cells" << visMsgEnd;
    char msg[256] = "SparseGrid counting cells ", msg2[256];
    strcpy(msg2, msg);
    e->setCursorFirstElement();
    while (e->isValidElement()) {
      myCells++;
      if (!(myCells % 1000)) {
	strcpy(msg2, msg);
	sprintf(&msg2[strlen(msg2)], "(%d)", myCells);
      }
      visSYSTEM.setStatus(msg2);
      if (visSYSTEM.getStatus() & visStatInterrupt) {
	e->decRef();
	this->deallocate();
	return false;
      }
      e->setCursorNextElement();
    }
    e->decRef();
  }

  // adjust min max

  myNode = 0.0f;
  this->adjustMinMax(myRootNode, 0, 0);
  if (visSYSTEM.getStatus() & visStatInterrupt) {
    visMsgDbg << "interrupted" << visMsgEnd;
    return false;
  }

  // Set Cursor to origin

  {
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->setCursor(pos);
    delete[] pos;
  }

  // Dump statistics

  {
    visMsgDbg << "Nodes = " << myNodes << " of max = " << myNodesMax
	      << ", ratio = " << ((float) myNodes) / myNodesMax << visMsgEnd;
    int a;
    for (a = 0; a < myAttrCount; a++) {
      visMsgDbg << "Min[" << a << "] = " 
		<< myMinAttribute[a]->getScalarValue(myAttrFactory[a]->
						     getDim()) 
		<< " Max[" << a << "] = " 
		<< myMaxAttribute[a]->getScalarValue(myAttrFactory[a]->
						     getDim()) 
		<< visMsgEnd;
    }
  }
  return true;
}

void visSparseGrid::deallocate() {
  if (!this->isAllocated()) return;

  visMsgCall << "visSparseGrid::deallocate: called." << visMsgEnd;

  int i, j, k;

  // delete tree
  visMsgDbg << "d-1" << visMsgEnd;
  this->deleteTree(myRootNode, 0);

  visMsgDbg << "d-2" << visMsgEnd;
  //delete myAttrCompare;
  myAttrCompare->decRef();

  visMsgDbg << "d-3" << visMsgEnd;
  // delete min and max attributes

  for (i = 0; i < this->getAttributes(); i++) {
    if (myMinAttribute[i]) { myMinAttribute[i]->decRef(); }
    if (myMaxAttribute[i]) { myMaxAttribute[i]->decRef(); }
  }
  delete[] myMinAttribute;
  delete[] myMaxAttribute;
  visMsgDbg << "d-4" << visMsgEnd;

  //  Delete attribute buffer and result attribut

  delete[] myFloatPosBuf;
  if (myEdgeAttribute) {
    for (i = 0; i < this->getAttributes(); i++) {
      for (j = 0; j < (1 << this->getDim()); j++) {
	myEdgeAttribute[i][j]->decRef();
      }
      delete[] myEdgeAttribute[i];
    }
  }
  visMsgDbg << "d-5" << visMsgEnd;

  for (i = 0; i < this->getAttributes(); i++) {
    myAttributeBuffer[i]->decRef();
    myResAttribute[i]->decRef();
  }
  visMsgDbg << "d-6" << visMsgEnd;

  delete[] myAttributeBuffer;
  delete[] myResAttribute;
  visMsgDbg << "d-7" << visMsgEnd;

  // delete factor info

  for (i = 0; i < myFactors; i++) {
    delete[] myFactorInfo[i].pos;
  }
  delete[] myFactorInfo;
  visMsgDbg << "d-8" << visMsgEnd;

  // delete attribute names

  for (i = 0; i < this->getAttributes(); i++) {
    delete[] myName[i];
  }
  delete[] myName;
  visMsgDbg << "d-9" << visMsgEnd;

  // delete cursor pos buffer and ldSize

  delete[] myCellPos2;
  delete[] myCellPos1;
  delete[] myCursorBuf;
  delete[] myCursorPos;
  delete[] myCursorScope;
  delete[] myLdSize;
  delete[] myLdScope;
  delete[] myPos;
  delete[] myEvalLdScope;
  delete[] myEvalPos;
  visMsgDbg << "d-10" << visMsgEnd;

  myAttrComb->decRef();
  visMsgDbg << "d-11" << visMsgEnd;

  visAdaptiveGrid::deallocate();
  visMsgDbg << "d-12" << visMsgEnd;

}

void visSparseGrid::getCellBounds(visSparseGridNode *node,
				  int index, int dim)
{

  if (!node) return;

  visAssert(index + 1 < myMaxLevel);

  int i;
  for (i = 0; i < myDim; i++) { 
    visAssert(myCursorPos[i] >= 0);
  }

  if (dim + 1 == myDim) {
    /*
    visMsgDbg << "myPos = " 
	      << myPos[index * myDim + 0] << ","
	      << myPos[index * myDim + 1] << ","
	      << myPos[index * myDim + 2]
	      << visMsgEnd;
    */
    
    // check if maxLdScope - minLdScope < maxLdSizeDiff
    
    int minLdScope = visMAXINT, maxLdScope = visMININT;
    if (myMaxLdScopeDiff > 0) {
      for (i = 0; i < myDim; i++) { 
	if (minLdScope > myLdScope[index * myDim + i]) {
	  minLdScope = myLdScope[index * myDim + i];
	}
	if (maxLdScope < myLdScope[index * myDim + i]) {
	  maxLdScope = myLdScope[index * myDim + i];
	}
      }
    }
      /*visMsgDbg << "maxLdScope = " << maxLdScope
	<< "minLdScope = " << minLdScope << visMsgEnd;
	visMsgDbg << "maxLdScopeDiff = " << maxLdScopeDiff <<
	visMsgEnd;*/
    if (myMaxLdScopeDiff <= 0 || maxLdScope - minLdScope <= myMaxLdScopeDiff) {
      for (i = 0; i < myDim; i++) {
	if (myPos[index * myDim + i] <= myCursorPos[i]) {
	  if (myPos[index * myDim + i] > myCellPos1[i]) {
	    // Check if influence less than eps
	    /*
	      int a;
	      for (a = 0; a < myAttrCount; a++) {
	      myAttributeBuffer[a]->setValue(myAttrFactory[a]->getDim(), 
	      ((visSparseGridVal*)node)->
	      attr[a]);
	      myAttributeBuffer[a]->mult(myAttrFactory[a]->getDim(), (1 - x));
	      }
	      myAttrCompare->setCompareAttribute(myAttributeBuffer);
	      myAttrComb->allocate(myAttrCompare);
	    */
	    myAttrCompare->setCompareAttribute(((visSparseGridVal*)node)->attr);
	    myAttrComb->allocate(myAttrCompare);
	    if (!myAttrComb->isEqual()) {
	      myCellPos1[i] = myPos[index * myDim + i];
	    }
	  }
	} else {
	  if (myPos[index * myDim + i] < myCellPos2[i]) {
	    // Check if influence less than eps
	    myAttrCompare->setCompareAttribute(((visSparseGridVal*)node)->attr);
	    myAttrComb->allocate(myAttrCompare);
	    if (!myAttrComb->isEqual()) {
	      myCellPos2[i] = myPos[index * myDim + i];
	    }
	  }
	}
	visAssert(myCellPos1[i] <= myCellPos2[i]);
      }
    }
  } else {
    myPos[index * myDim + dim + 1] = 0;
    this->getCellBounds(((visSparseGridStructure*)node)->next, index, dim + 1);
  }

  if (myCursorPos[dim] < myPos[index * myDim + dim]) {
    for (i = 0; i < myDim; i++) {
      if (i == dim) {
	myPos[(index + 1) * myDim + i] = myPos[index * myDim + i] - 
	  (1 << (myLdScope[index * myDim + i] - 1));
	myLdScope[(index + 1) * myDim + i] = myLdScope[index * myDim + i] - 1;
      } else {
	myPos[(index + 1) * myDim + i] = myPos[index * myDim + i];
	myLdScope[(index + 1) * myDim + i] = myLdScope[index * myDim + i];
      }
    }
    this->getCellBounds(node->left, index + 1, dim);
  } else {
    for (i = 0; i < myDim; i++) {
      if (i == dim) {
	myPos[(index + 1) * myDim + i] = myPos[index * myDim + i] + 
	  (1 << (myLdScope[index * myDim + i] - 1));
	myLdScope[(index + 1) * myDim + i] = myLdScope[index * myDim + i] - 1;
      } else {
	myPos[(index + 1) * myDim + i] = myPos[index * myDim + i];
	myLdScope[(index + 1) * myDim + i] = myLdScope[index * myDim + i];
      }
    }
    this->getCellBounds(node->right, index + 1, dim);  
  }
}

void visSparseGrid::evaluate(visSparseGridNode *node, int index, int dim)
{

  if (!node) return;

  visAssert(index + 1 < myMaxLevel);

  int i;
  for (i = 0; i < myDim; i++) {
    if (myCursorScope[i] > myLdScope[index * myDim + i]) {
      return;
    }
  }
  if (dim + 1 == myDim) {
    float fac = 1;
    for (i = 0; i < myDim; i++) {
      float x;
      if (myPos[index * myDim + i] <= myCursorPos[i]) {
	x = myCursorPos[i] - myPos[index * myDim + i];
      } else {
	  x = myPos[index * myDim + i] - myCursorPos[i];
      } 
      if (myIsScopeClipped && myLdScope[index * myDim + i] > myLdSize[i]) {
	x /= (1 << myLdSize[i]);
      } else {
	x /= (1 << myLdScope[index * myDim + i]);
      }
      fac *= (1 - x);
    }
    visSparseGridVal *vnode = (visSparseGridVal*) node;
    
    /*visMsgDbg << "fac = " << fac << visMsgEnd;*/
    for (i = 0; i < myAttrCount; i++) {
      myAttributeBuffer[i]->setValue
	(myAttrFactory[i]->getDim(), vnode->attr[i]);
      /*visMsgDbg << "attribute[" << i << "] = " 
	<< myAttributeBuffer[i]->getScalarValue(myAttrFactory[i]->
	getDim())
	<< visMsgEnd;*/
      myAttributeBuffer[i]->mult(myAttrFactory[i]->getDim(), fac);
      myResAttribute[i]->add(myAttrFactory[i]->getDim(), 
			     myAttributeBuffer[i]);
    }
    /*visMsgDbg << "evaluate pos/ldSize: " << visMsgEnd;
      for (i = 0; i < myDim; i++) {
      visMsgDbg << " " << myPos[index * myDim + i] 
      << "/" << myLdScope[index * myDim + i] << visMsgEnd;
      }*/
  } else {
    myPos[index * myDim + dim + 1] = 0;
    this->evaluate(((visSparseGridStructure*)node)->next, index, dim + 1);
  }

  if (myCursorPos[dim] < myPos[index * myDim + dim]) {
    for (i = 0; i < myDim; i++) {
      if (i == dim) {
	myPos[(index + 1) * myDim + i] = myPos[index * myDim + i] - 
	  (1 << (myLdScope[index * myDim + i] - 1));
	myLdScope[(index + 1) * myDim + i] = 
	  myLdScope[index * myDim + i] - 1;
      } else {
	myPos[(index + 1) * myDim + i] = myPos[index * myDim + i];
	myLdScope[(index + 1) * myDim + i] =
	  myLdScope[index * myDim + i];
      }
    }
    this->evaluate(node->left, index + 1, dim);
  } else {
    for (i = 0; i < myDim; i++) {
      if (i == dim) {
	myPos[(index + 1) * myDim + i] = myPos[index * myDim + i] + 
	  (1 << (myLdScope[index * myDim + i] - 1));
	myLdScope[(index + 1) * myDim + i] = 
	  myLdScope[index * myDim + i] - 1;
      } else {
	myPos[(index + 1) * myDim + i] = myPos[index * myDim + i];
	myLdScope[(index + 1) * myDim + i] =
	  myLdScope[index * myDim + i];
      }
    }
    this->evaluate(node->right, index + 1, dim);  
  }
}

void visSparseGrid::buildTree(visSparseGridNode *&node, int index, int dim)
{
  int i, k;
  
  visAssert(index + 1 < myMaxLevel);

  visSYSTEM.setStatus("SparseGrid building");
  if (dim + 1 == myDim) {
    /*visMsgDbg << "pos = " <<  visMsgEnd;
    for (i = 0; i < myDim; i++) {
      visMsgDbg << myPos[index * myDim + i] << "/"
		<< myLdScope[index * myDim + i] << visMsgEnd;
		}*/
    myNodes++;
    node = new visSparseGridVal;
    ((visSparseGridVal*)node)->attr = new visAttribute*[myAttrCount];
    for (i = 0; i < myAttrCount; i++) {
      ((visSparseGridVal*)node)->attr[i] = myAttrFactory[i]->getNewAttribute();
      ((visSparseGridVal*)node)->attr[i]->setValue(myAttrFactory[i]->getDim(), 
			       visAttrAddNeutral);
    }
    for (k = 0; k < myFactors; k++) {
      for (i = 0; i < myDim; i++) {
	myPos[(index + 1) * myDim + i] = myPos[index * myDim + i] +
	  (myFactorInfo[k].pos[i] * (1 << myLdScope[index * myDim + i]));
      }
      if (isValidPos(&myPos[(index + 1) * myDim]) &&
	  myFieldAccess->setCursorN(&myPos[(index + 1) * myDim])) {
	for (i = 0; i < myAttrCount; i++) {
	  myFieldAccess->getAttributeN(i, myAttributeBuffer[i]);
	  /*visMsgDbg << myAttributeBuffer[i]->getScalarValue
	    (myAttrFactory[i]->getDim()) << "/" 
		    << myFactorInfo[k].factor
		    << visMsgEnd;*/
	  myAttributeBuffer[i]->div
	    (myAttrFactory[i]->getDim(), myFactorInfo[k].factor);
	  ((visSparseGridVal*)node)->attr[i]->add(myAttrFactory[i]->getDim(), 
						  myAttributeBuffer[i]);
	}
      }
    }
  } else {
    node = new visSparseGridStructure;
    myPos[index * myDim + dim + 1] = 0;
    this->buildTree(((visSparseGridStructure*)node)->next, index, dim + 1);
  }

  for (i = 0; i < myDim; i++) {
    if (i == dim) {
      myPos[(index + 1) * myDim + i] = myPos[index * myDim + i] - 
	(1 << (myLdScope[index * myDim + i] - 1));
      myLdScope[(index + 1) * myDim + i] = myLdScope[index * myDim + i] - 1;
    } else {
      myPos[(index + 1) * myDim + i] = myPos[index * myDim + i];
      myLdScope[(index + 1) * myDim + i] = myLdScope[index * myDim + i];
   }
  }
  if (myLdScope[(index + 1) * myDim + dim] >= 0 &&
      myPos[(index + 1) * myDim + dim] >= 0) {
    this->buildTree(node->left, index + 1, dim);
  } else {
    node->left = NULL;
  }
  for (i = 0; i < myDim; i++) {
    if (i == dim) {
      myPos[(index + 1) * myDim + i] = myPos[index * myDim + i] + 
	(1 << (myLdScope[index * myDim + i] - 1));
      myLdScope[(index + 1) * myDim + i] = myLdScope[index * myDim + i] - 1;
    } else {
      myPos[(index + 1) * myDim + i] = myPos[index * myDim + i];
      myLdScope[(index + 1) * myDim + i] = myLdScope[index * myDim + i];
    }
  }
  if (myLdScope[(index + 1) * myDim + dim] >= 0 &&
      //---(myPos[(index) * myDim + dim] <= mySize[dim] ||
      //myPos[(index + 1) * myDim + dim] <= mySize[dim])) {
      (myPos[(index) * myDim + dim] < mySize[dim] ||
       myPos[(index + 1) * myDim + dim] < mySize[dim])) {
    this->buildTree(node->right, index + 1, dim);
  } else {
    node->right = NULL;
  }

  // delete entry if equal according to associated attribute combiner

  if (dim + 1 == myDim && node->left == NULL && node->right == NULL) {
    myAttrCompare->setCompareAttribute(((visSparseGridVal*)node)->attr);
    myAttrComb->allocate(myAttrCompare);
    if (myAttrComb->isEqual()) {
      for (i = 0; i < myAttrCount; i++) {
	((visSparseGridVal*)node)->attr[i]->decRef();
      }
      delete[] ((visSparseGridVal*)node)->attr;
      node = NULL;
      myNodes--;
    }
  }
}

void visSparseGrid::deleteTree(visSparseGridNode *&node, int dim) 
{
  if (!node) {
    return;
  }
  if (dim + 1 == myDim) {
    int i;
    for (i = 0; i < myAttrCount; i++) {
      ((visSparseGridVal*)node)->attr[i]->decRef();
    }
    delete[] ((visSparseGridVal*)node)->attr;
  } else {
    this->deleteTree(((visSparseGridStructure*)node)->next, dim + 1);
  }
  this->deleteTree(node->left, dim);
  this->deleteTree(node->right, dim);
  delete node;
  node = NULL;
}

void visSparseGrid::adjustMinMax(visSparseGridNode *node, int level, int dim)
{
  if (!node) return;

  visSYSTEM.setStatus("SparseGrid calculating min/max", myNode++/myNodes);
  if (visSYSTEM.getStatus() & visStatInterrupt) {
    return;
  }

  visAssert(level + 1 < myMaxLevel);

  int i, a;
  if (dim + 1 == myDim) {
    this->setCursorN(&myEvalPos[level * myDim]);
    for (a = 0; a < myAttrCount; a++) {
      this->getAttributeN(a, myAttributeBuffer[a]);
      if (myMaxAttribute[a]->isLess(myAttrFactory[a]->getDim() ,
				    myAttributeBuffer[a])) {
	myMaxAttribute[a]->setValue(myAttrFactory[a]->getDim(),
				    myAttributeBuffer[a]);
      }
      if (!myMinAttribute[a]->isLess(myAttrFactory[a]->getDim(),
				     myAttributeBuffer[a])) {
	myMinAttribute[a]->setValue(myAttrFactory[a]->getDim(),
				    myAttributeBuffer[a]);
      }
    }
  } else {
    myEvalPos[level * myDim + dim + 1] = 0;
    myEvalLdScope[level * myDim + dim + 1] = myLdSize[dim + 1] + 1;
    /*visMsgDbg << "calling next level = " << level << visMsgEnd;
    for (i = 0; i < myDim; i++) {
      visMsgDbg << "myEvalPos[" << level * myDim + i 
		<< "] = " << myEvalPos[level * myDim + i] << visMsgEnd;
      visMsgDbg << "myEvalLdScope[" << level * myDim + i 
		<< "] = " << myEvalLdScope[level * myDim + i] << visMsgEnd;
		}*/
    this->adjustMinMax(((visSparseGridStructure*)node)->next, level, dim + 1);
  }
  for (i = 0; i < dim + 1; i++) {
    if (i == dim) {
      myEvalPos[(level + 1) * myDim + i] = myEvalPos[level * myDim + i] - 
	(1 << (myEvalLdScope[level * myDim + i] - 1));
      myEvalLdScope[(level + 1) * myDim + i] =
	myEvalLdScope[level * myDim + i] - 1;
    } else {
      myEvalPos[(level + 1) * myDim + i] = myEvalPos[level * myDim + i];
      myEvalLdScope[(level + 1) * myDim + i] = myEvalLdScope[level * myDim + i];
    }
  }
  /*visMsgDbg << "calling left level = " << level+1 << visMsgEnd;
  for (i = 0; i < myDim; i++) {
    visMsgDbg << "myEvalPos[" << (level+1) * myDim + i 
	      << "] = " << myEvalPos[(level+1) * myDim + i] << visMsgEnd;
    visMsgDbg << "myEvalLdScope[" << (level+1) * myDim + i 
	      << "] = " << myEvalLdScope[(level+1) * myDim + i] << visMsgEnd;
	      }*/
  this->adjustMinMax(node->left, level + 1, dim);
  for (i = 0; i < dim + 1; i++) {
    if (i == dim) {
      myEvalPos[(level + 1) * myDim + i] = myEvalPos[level * myDim + i] + 
	(1 << (myEvalLdScope[level * myDim + i] - 1));
      myEvalLdScope[(level + 1) * myDim + i] = 
	myEvalLdScope[level * myDim + i] - 1;
    } else {
      myEvalPos[(level + 1) * myDim + i] = myEvalPos[level * myDim + i];
      myEvalLdScope[(level + 1) * myDim + i] = myEvalLdScope[level * myDim + i];
    }
  }
  /*visMsgDbg << "calling right level = " << level+1<< visMsgEnd;
  for (i = 0; i < myDim; i++) {
    visMsgDbg << "myEvalPos[" << (level+1) * myDim + i 
	      << "] = " << myEvalPos[(level+1) * myDim + i] << visMsgEnd;
    visMsgDbg << "myEvalLdScope[" << (level+1) * myDim + i 
	      << "] = " << myEvalLdScope[(level+1) * myDim + i] << visMsgEnd;
	      }*/
  this->adjustMinMax(node->right, level + 1, dim);
}

int visSparseGrid::getSparseGridSizeAndDim(visSparseGridNode *&node,
					   istream &in,
					   int dim, int level)
{

  visSYSTEM.setStatus("SparseGrid building");

  // update dim and size information

  if (myMaxLevelStack.getSize() < dim + 1) {
    myMaxLevelStack.push(level);
  }
  if (level > myMaxLevelStack[dim]) {
    myMaxLevelStack[dim] = level;
  }
  int i;
  double length, value, left, right;
  in >> length;
  //visMsgDbg << "reading length " << length << visMsgEnd;
  if (in.eof()) {
    visMsgError << "Read SparseGrid: Unexpected end of file!" << visMsgErrorEnd;
    return 0;
  }
  if (length == 1.0f) {
    if (myMaxLevelStack.getSize() != dim + 1) {
      return 0;
    }
    node = new visSparseGridVal;
    //visMsgDbg << "p = " << node << " visSparseGridVal" << visMsgEnd;
    visAttribute **&attr = ((visSparseGridVal*)node)->attr;
    attr = new visAttribute*[myAttrCount];
    for (i = 0; i < myAttrCount; i++) {
      attr[i] = myAttrFactory[i]->getNewAttribute();
      // hardcoded hack
      in >> value;
      //visMsgDbg << "reading value " << value << visMsgEnd;

      attr[i]->setValue(myAttrFactory[i]->getDim(), value);
    }
    myNodes++;
  } else {
    node = new visSparseGridStructure;
    //visMsgDbg << "p = " << node << " visSparseGridStructure" << visMsgEnd;
    if (length > 0.0f) {
      if (!this->getSparseGridSizeAndDim(((visSparseGridStructure*&)node)->next,
					 in, dim + 1, 0)) {
	return 0;
      }
    } else {
      ((visSparseGridStructure*&)node)->next = NULL;
    }
    //visMsgDbg << "p = " << node << " p->next = " << node->next << visMsgEnd;
  }
  in >> left;
  //visMsgDbg << "reading left " << left << visMsgEnd;

  if (left) {
    if (!this->getSparseGridSizeAndDim(node->left, in, dim, level + 1)) {
      return 0;
    }
  } else {
    node->left = NULL;
  }
  //visMsgDbg << "p = " << node << " p->left = " << node->left << visMsgEnd;
  in >> right;
  //visMsgDbg << "reading right " << right << visMsgEnd;
  if (right) {
    if (!this->getSparseGridSizeAndDim(node->right, in, dim, level + 1)) {
      return 0;
    }
  } else {
    node->right = NULL;
  }
  //visMsgDbg << "p = " << node << " p->right = " << node->right << visMsgEnd;

  return 1;
}

void visSparseGrid::setCursor(const int *p)
{
  if (!this->isAllocated()) return;

  int i, j;

  //visMsgDbg << "cpos = " << p[0] << "," << p[1] << "," << p[2] << visMsgEnd;

  int isEqualPos = true;
  for (i = 0; i < myDim; i++) {
    visAssert(p[i] < mySize[i]);
    visAssert(p[i] >= 0);
    if (p[i] != myCursorPos[i]) {
      isEqualPos = false;
      break;
    }
  }
  if (isEqualPos) return;

  for (i = 0; i < myDim; i++) {
    myCursorPos[i] = p[i];
    myCellPos1[i] = 0;
    myCellPos2[i] = mySize[i];
    myLdScope[i] = myLdSize[i] + 1;
    myPos[i] = 0;
  }
  this->getCellBounds(myRootNode, 0, 0);
  /*visMsgDbg << "pos1 = " 
	    << myCellPos1[0] << ","  
	    << myCellPos1[1] << "," 
	    << myCellPos1[2] << visMsgEnd;
  visMsgDbg << "pos2 = " 
	    << myCellPos2[0] << ","  
	    << myCellPos2[1] << "," 
	    << myCellPos2[2] << visMsgEnd;*/
  myAttrValid = 0;

};

int visSparseGrid::getCursor(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  return myCursorPos[i];
}
visAttribute* visSparseGrid::getCellAttribute(int comp)
{
  if (!this->isAllocated()) return NULL;
  
  visAttribute *attr = myAttrFactory[comp]->getNewAttribute();
  this->getCellAttribute(comp, attr);
  return attr;
};

void visSparseGrid::getCellAttribute(int comp, visAttribute *attr)
{
  if (!this->isAllocated()) return;

  if (!myAttrValid) {
    this->getNodeAttributes();
    myAttrValid = 1;
  }
  attr->setValue(myAttrFactory[comp]->getDim(), myEdgeAttribute[comp][0]);
}

const char *visSparseGrid::getAttrName(int comp)
{
  if (!this->isAllocated()) return "<unknown>";

  return myName[comp];
};

const visAttribute* visSparseGrid::getMinAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  visAttribute *attr = myAttrFactory[comp]->getNewAttribute();
  attr->setValue(myAttrFactory[comp]->getDim(), myMinAttribute[comp]);
  return attr;
};

const visAttribute* visSparseGrid::getMaxAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  myMaxAttribute[comp]->incRef();
  return myMaxAttribute[comp];
};

int visSparseGrid::getCellPos(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  return myCellPos1[i];
};

int visSparseGrid::getCellSize(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  return myCellPos2[i] - myCellPos1[i];
};

int visSparseGrid::getCells() 
{
  if (!this->isAllocated()) return 0;
  //  return myCells[myMaxVirtualLevel];
  return myCells;
};

int visSparseGrid::getNodes() 
{
  if (!this->isAllocated()) return 0;
  //  return myCells[myMaxVirtualLevel];
  return myNodes;
};

int visSparseGrid::visGetScopeByPos(int pos, int dim) 
{
  int scope = myLdScope[dim];
  while (pos % (1 << scope) != 0) {
    scope--;
  }
  return scope;
}

void visSparseGrid::getNodeAttributes()
{
  int i, j;
  for (j = 0; j < (1 << myDim); j++) {
    for (i = 0; i < this->getDim(); i++) {
      myCursorPos[i] = (j & (1 << i)) == 0 ? myCellPos1[i] : myCellPos2[i];
      myCursorScope[i] = visGetScopeByPos(myCursorPos[i], i);
    }
    for (i = 0; i < myAttrCount; i++) {
      myResAttribute[i]->
	setValue(myAttrFactory[i]->getDim(), visAttrAddNeutral);
    }
    for (i = 0; i < myDim; i++) {
      myPos[i] = 0;
    }
    this->evaluate(myRootNode, 0, 0);
    
    for (i = 0; i < myAttrCount; i++) {

      // clip to max and min
      
      /*      if (!myResAttribute[i]->isLess(myAttrFactory[i]->getDim(),
	      myMaxAttribute[i])) {
	      myResAttribute[i]->setValue(myAttrFactory[i]->getDim(),
	      myMaxAttribute[i]);
	      } else if (myResAttribute[i]->isLess(myAttrFactory[i]->getDim(),
	      myMinAttribute[i])) {
	      myResAttribute[i]->setValue(myAttrFactory[i]->getDim(),
	      myMinAttribute[i]);
	      }*/
      myEdgeAttribute[i][j]->setValue(myAttrFactory[i]->getDim(),
				      myResAttribute[i]);
      
    }
  }
}

void visSparseGrid::getNodeAttribute(int comp, 
				     visAttribute *attr,
				     int edge)
{
  if (!this->isAllocated()) return;

  if (!myAttrValid) {
    this->getNodeAttributes();
    myAttrValid = 1;
  }
  attr->setValue(myAttrFactory[comp]->getDim(), myEdgeAttribute[comp][edge]);
}

void visSparseGrid::getInterpAttribute(int comp,
					     visAttribute *attr,
					     float *pos)
{
  if (!this->isAllocated()) return;
  //  visMsgDbg << "getInterpAttribute called" << visMsgEnd;

  if (!myAttrValid) {
    this->getNodeAttributes();
    myAttrValid = 1;
  }
  /*  visMsgDbg << "pos = "
	    << pos[0] << "," << pos[1] << "," << pos[2] << visMsgEnd;
  visMsgDbg << "cellpos1 = "
	    << myCellPos1[0] << "," 
	    << myCellPos1[1] << "," << myCellPos1[2] << visMsgEnd;
  visMsgDbg << "cellpos2 = "
	    << myCellPos2[0] << "," 
	    << myCellPos2[1] << "," << myCellPos2[2] << visMsgEnd;*/
  double div = 1;
  int i, j, dim = myDim, attrDim = myAttrFactory[comp]->getDim();
  //visMsgDbg << "attrDim = " << attrDim << visMsgEnd;
  float *&negPos = myFloatPosBuf;
  for (i = 0; i < dim; i++) {
    div *= myCellPos2[i] - myCellPos1[i];
    negPos[i] = - pos[i] + (myCellPos2[i] - myCellPos1[i]);
  }
  //visMsgDbg << "div = " << div << visMsgEnd;
  //visMsgDbg << "comp = " << comp << visMsgEnd;
  double mult;
  visAttribute *multAttr = myAttrFactory[comp]->getNewAttribute();
  attr->setValue(attrDim, visAttrAddNeutral);
  for (i = 0; i < (1 << dim); i++) {
    mult = 1;
    for (j = 0; j < dim; j++) {
      mult *= ((i & (1 << j)) != 0 ? pos[j] : negPos[j]);
    }
    /*visMsgDbg << "mult = " << mult << visMsgEnd;
    visMsgDbg << "attr = "
	      << myEdgeAttribute[comp][i]->getValue(0) << "," 
	      << myEdgeAttribute[comp][i]->getValue(1) << ","
	      << myEdgeAttribute[comp][i]->getValue(2) << visMsgEnd;*/
    multAttr->setValue(attrDim, myEdgeAttribute[comp][i]);
    multAttr->mult(attrDim, mult);
    /*visMsgDbg << "mult attr = "
	      << multAttr->getValue(0) << "," 
	      << multAttr->getValue(1) << ","
	      << multAttr->getValue(2) << visMsgEnd;*/
    attr->add(attrDim, multAttr);
  }
  attr->div(attrDim, div);
  /*visMsgDbg << "result = "
	    << attr->getValue(0) << "," 
	    << attr->getValue(1) << ","
	    << attr->getValue(2) << visMsgEnd;*/
  multAttr->decRef();
}

bool visSparseGrid::isValidPos(int *pos) {
  int i;
  bool valid = true;
  for (i = 0; i < this->getDim(); i++) {
    if (pos[i] < 0 || pos[i] > this->getSize(i)) {
      valid = false;
    }
  }
  return valid;
};

/* -----------------------------------------------------------------------------
   visStreamLineEuler Methods
   ---------------------------------------------------------------------------*/
#ifdef OBS
visStreamLineEuler::visStreamLineEuler()
{
};

visStreamLineEuler::~visStreamLineEuler()
{
  this->deallocate();
};

bool visStreamLineEuler::allocate(visAdaptiveGrid *fieldAccess, 
				  visCell2Node *cellInterp,
				  int vectorAttribute,
				  float *probePos,
				  int direction,
				  int stepsPerCell,
				  int maxSteps) 
{
  
  visMsgCall << "visStreamLineEuler::allocate: called." << visMsgEnd;

  this->deallocate();

  myFieldAccess = fieldAccess;
  myVectorAttribute = vectorAttribute;
  myMaxSteps = maxSteps;
  int dim = myFieldAccess->getDim();
  int remainingSteps = maxSteps;

  // Sanity checks

  if (maxSteps <= 0) {
    return false;
  }

  if (!fieldAccess || !fieldAccess->isAllocated()) {
    visMsgError << "visStreamLineEuler::"
		<< "allocate: fieldAccess unitialized."
		<< visMsgErrorEnd;
    return false;
  }

  if (vectorAttribute < 0 || 
      vectorAttribute >= fieldAccess->getAttributes()) {
    visMsgError << "visStreamLineEuler::"
		<< "allocate: Vector component out of range."
		<< visMsgErrorEnd;
    return false;
  }

  visAttributeFactory** attrFactory =
    new visAttributeFactory*[myFieldAccess->getAttributes()];
  int i;
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    attrFactory[i] = myFieldAccess->getAttrFactory(i);
  }
  
  if (attrFactory[vectorAttribute]->getDim() != dim) {
    visMsgError << "visStreamLineEuler::allocate: "
		<< "Vector dimension must be equal to field dimension."
		<< visMsgErrorEnd;
    return false;
  }
  
  // Copy attribute names

  this->myName = new char*[myFieldAccess->getAttributes()];
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    this->myName[i] = new char[strlen(myFieldAccess->getAttrName(i)) + 1];
    strcpy(this->myName[i], myFieldAccess->getAttrName(i));
  }

  if (!cellInterp) {
    myCellInterp = fieldAccess->getNodeGrid();
    visAssert(myCellInterp);
  } else {
    myCellInterp = cellInterp;
    cellInterp->incRef();
  }

  float aPos[3];
  float bPos[3];

  for (i = 0; i < dim; i++) {
    aPos[i] = probePos[i];
  }
  
  int goBackward = false;
  if (direction & 2) {
    goBackward = true;
    
  }

  visAttribute *attr = attrFactory[vectorAttribute]->getNewAttribute();
  double aVector[3], bVector[3], cVector[3];

  if (myCellInterp->setCursorF(aPos)) {
    myCellInterp->getCellAttributeF(vectorAttribute, attr, aPos);
    bVector[0] = - (double) attr->getValue(1);
    bVector[1] = (double) attr->getValue(0);
    bVector[2] = (double) attr->getValue(2);

    double l, m;
    
    myStreamAttribute = new visStack<visAttribute*>();
    myStreamCoord = new visStack<float>();
    myStreamNormal = new visStack<float>();
    myStreamDir = new visStack<float>();

    while (--remainingSteps >= 0) {

      // Check if aPos in field
      
      if (!myCellInterp->setCursorF(aPos)) {
	visMsgDbg << "aPos out of field" << visMsgEnd;
	break;
      }

      // Push attributes on stack
      
      for (i = 0; i < myCellInterp->getAttributes(); i++) {
	visAttribute *a = attrFactory[i]->getNewAttribute();
	myCellInterp->getCellAttributeF(i, a, aPos);
	myStreamAttribute->push(a);
      }
      
      attr->setValue(dim, myStreamAttribute->
		     operator[](myStreamAttribute->getSize() - 
				myCellInterp->getAttributes() +
				vectorAttribute));
      
      // get vector a

      for (i = 0; i < dim; i++) {
	aVector[i] = attr->getValue(i);
      }

      /*      visMsgDbg << "raw: aPos=("
		<< aPos[0] << "," << aPos[1] << "," << aPos[2] << ") "
		<< "bVector=("
		<< bVector[0] << "," << bVector[1] << "," << bVector[2] << ") "
		<< "aVector=("
		<< aVector[0] << "," << aVector[1] << "," << aVector[2] << ") "
		<< visMsgEnd;*/
      // normalize a vector

      l = 0;
      for (i = 0; i < dim; i++) {
	double x = aVector[i];
	l += x * x;
      }
      if (l == 0) {
	visMsgDbg << "Length is zero (A)." << visMsgEnd;

	for (i = 0; i < dim; i++) {
	  visMsgDbg << "Pos[" << i << "] = " << aPos[i] << visMsgEnd;
	}
	myCellInterp->setCursorF(aPos);
	myCellInterp->getCellAttributeF(vectorAttribute, attr, aPos);
	for (i = 0; i < dim; i++) {
	  visMsgDbg << "Val[" << i << "] = " << attr->getValue(i) << visMsgEnd;
	}
	break;
      }

      m = (double) 1 / sqrt(l);
      for (i = 0; i < dim; i++) {
	aVector[i] *= m;
      }
      
      // align b vector to be orthogonal to stream
      
      m = 0;
      for (i = 0; i < dim; i++) {
	m += aVector[i] * bVector[i];
      }
      for (i = 0; i < dim; i++) {
	bVector[i] -= m * aVector[i];
      }
      
      // normalize b vector
      
      l = 0;
      for (i = 0; i < dim; i++) {
	l += bVector[i] * bVector[i];
      }
      if (l == 0) {
	visMsgDbg << "bVector length is zero." << visMsgEnd;
	break;
      }
      m = (double) 1 / sqrt(l);
      for (i = 0; i < dim; i++) {
	bVector[i] *= m;
      }

      /*      visMsgDbg << "proc: aPos=("
		<< aPos[0] << "," << aPos[1] << "," << aPos[2] << ") "
		<< "bVector=("
		<< bVector[0] << "," << bVector[1] << "," << bVector[2] << ") "
		<< "aVector=("
		<< aVector[0] << "," << aVector[1] << "," << aVector[2] << ") "
		<< visMsgEnd;*/
      // push a vector on stack
      
      for (i = 0; i < dim; i++) {
	myStreamDir->push(aVector[i]);
      }
      
      // Push pos a on stack
      
      for (i = 0; i < dim; i++) {
	myStreamCoord->push(aPos[i]);
      }

      // push b vector on stack
      
      for (i = 0; i < dim; i++) {
	myStreamNormal->push(bVector[i]);
      }
      
      // calculate pos b
      
      for (i = 0; i < dim; i++) {
	bPos[i] = aPos[i] + bVector[i] / stepsPerCell;
      }
      if (!myCellInterp->setCursorF(bPos)) {
	visMsgDbg << "bPos out of field." << visMsgEnd;
	break;
      }
      myCellInterp->getCellAttributeF(vectorAttribute, attr, bPos);
      
      // calculate new pos b
      
      for (i = 0; i < dim; i++) {
	cVector[i] = attr->getValue(i);
      }
      l = 0;
      for (i = 0; i < dim; i++) {
	l += cVector[i] * cVector[i];
      }
      if (l == 0) {
	break;
      }
      l = sqrt(l);
      m = (double) 1 / (l * stepsPerCell);
      if (goBackward) {
	m = -m;
      }
      for (i = 0; i < dim; i++) {
	bPos[i] += cVector[i] * m;
      }

      // calculate new pos a
      
      m = (double) 1 / stepsPerCell;
      if (goBackward) {
	m = -m;
      }
      
      for (i = 0; i < dim; i++) {
	aPos[i] += aVector[i] * m;
      }

      // calculate new vector b

      for (i = 0; i < dim; i++) {
	bVector[i] = bPos[i] - aPos[i];
      }
    }
  }
  attr->decRef();
  int *size = new int[1];
  size[0] = maxSteps - remainingSteps;

  if (!visAdaptiveGrid::allocate(1, size,
				myFieldAccess->getAttributes() + 
				visStreamLineAttrTypes,
				attrFactory,
				myCellInterp)) {
    return false;
  }
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    attrFactory[i]->decRef();
  }
  delete[] attrFactory;

  myCells = myStreamAttribute->getSize();

  // Set Cursor to origin
  {
    int *pos = new int[1];
    pos[0] = 0;
    this->setCursor(pos);
    delete[] pos;
  }

  delete[] size;

  myCellInterp->decRef();
  myFieldAccess->decRef();

  return true;
}

void visStreamLineEuler::deallocate() 
{
  if (!this->isAllocated()) return;

  visMsgCall << "visStreamLineEuler::deallocate: called." << visMsgEnd;
  
  delete myStreamAttribute;
  delete myStreamCoord;
  delete myStreamNormal;
  delete myStreamDir;

  int i;
  for (i = 0; i < this->getAttributes(); i++) {
    if (myMinAttribute[i]) { myMinAttribute[i]->decRef(); }
    if (myMaxAttribute[i]) { myMaxAttribute[i]->decRef(); }
  }
  delete[] myMinAttribute;
  delete[] myMaxAttribute;
  
  // delete attribute names

  for (i = 0; i < this->getAttributes() - visStreamLineAttrTypes; i++) {
   delete[] myName[i];
  }
  delete[] myName;

  // delete temporaries

  delete[] myPosA;
  delete[] myPosB;

  visAdaptiveGrid::deallocate();
}

void visStreamLineEuler::setCursor(const int *p) 
{
  myCursor = p[0];
}

int visStreamLineEuler::getCursor(unsigned int)
{
  return myCursor;
}

void visStreamLineEuler::getCellAttribute(int comp, visAttribute* attr)
{
  int i;
  if (comp < visStreamLineAttrTypes) {
    switch (comp) {
    case visStreamLineAttrPosition:
      for (i = 0; i < myVectorDim; i++) {
	attr->setValue(i, 
		       myStreamCoord->operator[](myCursor * myVectorDim + i));
      }
      break;
    case visStreamLineAttrDirection:
      for (i = 0; i < myVectorDim; i++) {
	attr->setValue(i, 
		       myStreamDir->operator[](myCursor * myVectorDim + i));
      }
      break;
    case visStreamLineAttrNormal:
      for (i = 0; i < myVectorDim; i++) {
	attr->setValue(i, 
		       myStreamNormal->operator[](myCursor * myVectorDim + i));
      }
      break;
    }
  } else {

    attr->setValue(myAttrFactory[comp]->getDim(),
		   myStreamAttribute->
		   operator[]((this->getAttributes() - 
			       visStreamLineAttrTypes) * myCursor
			      + comp - visStreamLineAttrTypes));
  }
}

const char *visStreamLineEuler::getAttrName(int comp) 
{
  if (!this->isAllocated()) ;

  if (comp < 0 || comp >= this->getAttributes()) {
    return "<unknown>";
  }

  switch (comp) {
  case visStreamLineAttrPosition:
    return "Position";
  case visStreamLineAttrDirection:
    return "Direction";
  case visStreamLineAttrNormal:
    return "Normal";
  default:
    return myName[comp - visStreamLineAttrTypes];
  }
};

const visAttribute* visStreamLineEuler::getMinAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  visAttribute *attr = myAttrFactory[comp]->getNewAttribute();
  attr->setValue(myAttrFactory[comp]->getDim(), myMinAttribute[comp]);
  return attr;
};
  
const visAttribute* visStreamLineEuler::getMaxAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  myMaxAttribute[comp]->incRef();
  return myMaxAttribute[comp];
};

int visStreamLineEuler::getCellPos(unsigned int)
{
  if (!this->isAllocated()) return 0;
  return myCursor;
}
int visStreamLineEuler::getCellSize(unsigned int)
{
  return 1;
}
 
int visStreamLineEuler::getCells() 
{
  return myCells;
};
#endif

/* -----------------------------------------------------------------------------
   visFieldSlice Methods
   ---------------------------------------------------------------------------*/

visAdaptiveGridSlice::visAdaptiveGridSlice() 
{
};

visAdaptiveGridSlice::~visAdaptiveGridSlice()
{
  this->deallocate();
};

bool visAdaptiveGridSlice::allocate(visAdaptiveGrid *fieldAccess, 
				   int axis, float pos, bool adjustMinMax) 
{
  visMsgCall << "visAdaptiveGridSlice::allocate: called." << visMsgEnd;
  if (!fieldAccess || !fieldAccess->isAllocated()) {
    visMsgError << "visAdaptiveGridSlice::allocate"
		<< ": called with unallocated fieldAccess."
		<< visMsgErrorEnd;
    return false;
  }
  
  this->deallocate();

  myAxis = axis;
  myPos = pos;

  myFieldAccess = fieldAccess;
  myFieldAccess->incRef();
  int dim = myFieldAccess->getDim() - 1;
  int *size = new int[dim];
  int i;
  for (i = 0; i < dim; i++) {
    if (i >= myAxis) {
      size[i] = myFieldAccess->getSize(i + 1);
    } else {
      size[i] = myFieldAccess->getSize(i);
    }
  }
  visAttributeFactory** factory =
    new visAttributeFactory*[myFieldAccess->getAttributes()];
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    factory[i] = myFieldAccess->getAttrFactory(i);
  }
  if (!visAdaptiveGrid::allocate(dim, size,
				 myFieldAccess->getAttributes(), factory)) {
    for (i = 0; i < myFieldAccess->getAttributes(); i++) {
      factory[i]->decRef();
    }
    delete[] factory;
    delete[] size;
    return false;
  }
  for (i = 0; i < myFieldAccess->getAttributes(); i++) {
    factory[i]->decRef();
  }
  delete[] factory;
  delete[] size;

  // Initialize Min and Max attributes and count cells

  myFieldAttributeCount = myFieldAccess->getAttributes();
  myMinAttribute = new visAttribute*[myFieldAttributeCount];
  myMaxAttribute = new visAttribute*[myFieldAttributeCount];

  visAttributeEnumeration *e = new visAdaptiveGridCellEnumeration(this);
  e->setCursorFirstElement();
  if (adjustMinMax) {
    for (i = 0; i < myFieldAttributeCount; i++) {
      myMinAttribute[i] = myAttrFactory[i]->getNewAttribute();
      //    myMinAttribute[i]->setValue(myAttrFactory[i]->getDim(), visAttrMax);
      myFieldAccess->getCellAttribute(i, myMinAttribute[i]);
      myMaxAttribute[i] = myAttrFactory[i]->getNewAttribute();
      //    myMaxAttribute[i]->setValue(myAttrFactory[i]->getDim(), visAttrMin);
      myFieldAccess->getCellAttribute(i, myMaxAttribute[i]);
    }
  } else {
    for (i = 0; i < myFieldAttributeCount; i++) {
      myMinAttribute[i] = (visAttribute*) myFieldAccess->getMinAttribute(i);
      myMaxAttribute[i] = (visAttribute*) myFieldAccess->getMaxAttribute(i);
    }
  }
  myCells = 0;

  while (e->isValidElement()) {
    myCells++;
    if (adjustMinMax) {
      for (i = 0; i < myFieldAttributeCount; i++) {
	visAttribute* attr = (visAttribute*) myFieldAccess->getCellAttribute(i);
	if (myMaxAttribute[i]->isLess(myAttrFactory[i]->getDim(), attr)) {
	  myMaxAttribute[i]->setValue(myAttrFactory[i]->getDim(), attr);
	}
	if (attr->isLess(myAttrFactory[i]->getDim(), myMinAttribute[i])) {
	  myMinAttribute[i]->setValue(myAttrFactory[i]->getDim(), attr);
	}
	attr->decRef();
      }
    }
    e->setCursorNextElement();
  }
  delete e;

  /*  int *startCursor = new int[myFieldAccess->getDim()];
  for (i = 0; i < myFieldAccess->getDim(); i++) {
    if (i != myAxis) {
      startCursor[i] = 0;
    } else {
      startCursor[i] = myPos;
    }
  }
  delete[] startCursor;
  */

  // Set Cursor to origin
  {
    int *pos = new int[dim];
    for (i = 0; i < dim; i++) {
      pos[i] = 0;
    }
    this->setCursor(pos);
    delete[] pos;
  }

  return true;
};

void visAdaptiveGridSlice::deallocate() {
  if (!this->isAllocated()) return;

  visMsgCall << "visAdaptiveGridSlice::deallocate: called." << visMsgEnd;

  int i;
  for (i = 0; i < myFieldAttributeCount; i++) {
    if (myMinAttribute[i]) { myMinAttribute[i]->decRef(); }
    if (myMaxAttribute[i]) { myMaxAttribute[i]->decRef(); }
  }
  delete[] myMinAttribute;
  delete[] myMaxAttribute;
  myFieldAccess->decRef();

  visObject::deallocate();
}

void visAdaptiveGridSlice::setCursor(const int *p) 
{
  if (!this->isAllocated()) return;
  int i, *pos = new int[myFieldAccess->getDim()];
  for (i = 0; i < myAxis; i++) {
    visAssert(p[i] < mySize[i]);
    visAssert(p[i] >= 0);
    pos[i] = p[i];
  }
  pos[myAxis] = myPos;
  for (i = myAxis; i < myDim; i++) {
    visAssert(p[i] < mySize[i]);
    visAssert(p[i] >= 0);
    pos[i + 1] = p[i];
  }
  myFieldAccess->setCursor(pos);
  delete[] pos;
}

int visAdaptiveGridSlice::getCursor(unsigned int i)
{
  if (!this->isAllocated()) return 0;
  if (i >= myAxis) {
    return myFieldAccess->getCursor(i + 1);
  } else {
    return myFieldAccess->getCursor(i);
  }
}

visAttribute* visAdaptiveGridSlice::getCellAttribute(int i) 
{
  if (!this->isAllocated()) return NULL;
  return myFieldAccess->getCellAttribute(i);
}

void visAdaptiveGridSlice::getCellAttribute(int comp, visAttribute *attr)
{
  if (!this->isAllocated()) return;
  myFieldAccess->getCellAttribute(comp, attr);
}

visAttribute* visAdaptiveGridSlice::getNodeAttribute(int t, int n) 
{
  if (!this->isAllocated()) return NULL;
  int mask = (1 << myAxis) - 1;
  int nc = n & mask; mask |= mask + 1;
  nc |= (n & (0xffffffff ^ mask)) >> 1;
  return myFieldAccess->getNodeAttribute(t, nc);
}

void visAdaptiveGridSlice::getNodeAttribute(int t, visAttribute *attr, int n)
{
  if (!this->isAllocated()) return;
  int mask = (1 << myAxis) - 1;
  int nc = n & mask; mask |= mask + 1;
  nc |= (n & (0xffffffff ^ mask)) >> 1;
  myFieldAccess->getNodeAttribute(t, attr, nc);
}

const char *visAdaptiveGridSlice::getAttrName(int i) 
{
  if (!this->isAllocated()) return 0;
  return myFieldAccess->getAttrName(i);
}

const visAttribute* visAdaptiveGridSlice::getMinAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  visAttribute *attr = myAttrFactory[comp]->getNewAttribute();
  attr->setValue(myAttrFactory[comp]->getDim(), myMinAttribute[comp]);
  return attr;
};
  
const visAttribute* visAdaptiveGridSlice::getMaxAttribute(int comp) 
{
  if (!this->isAllocated()) return NULL;
  myMaxAttribute[comp]->incRef();
  return myMaxAttribute[comp];
};

int visAdaptiveGridSlice::getCellPos(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  if (i >= myAxis) {
    return myFieldAccess->getCellPos(i + 1);
  } else {
    return myFieldAccess->getCellPos(i);
  }
  }

int visAdaptiveGridSlice::getCellSize(unsigned int i) 
{
  if (!this->isAllocated()) return 0;
  if (i >= myAxis) {
    return myFieldAccess->getCellSize(i + 1);
  } else {
    return myFieldAccess->getCellSize(i);
  }
}

int visAdaptiveGridSlice::getCells() 
{
  if (!this->isAllocated()) return 0;

  return myCells;
};

/* -----------------------------------------------------------------------------
   visAttributeEnumeration Methods
   ---------------------------------------------------------------------------*/

visAttributeEnumeration::visAttributeEnumeration(visAdaptiveGrid *fieldAccess)
{
  myFieldAccess = fieldAccess;
  myFieldAccess->incRef();
  myDim = myFieldAccess->getDim();
}

visAttributeEnumeration::~visAttributeEnumeration()
{
  myFieldAccess->decRef();
  myDim = 0;
}

/* -----------------------------------------------------------------------------
   visAdaptiveGridCellEnumeration Methods
   ---------------------------------------------------------------------------*/

visAdaptiveGridCellEnumeration::visAdaptiveGridCellEnumeration
(visAdaptiveGrid *fieldAccess) : visAttributeEnumeration(fieldAccess)
{
  myMask = new int[myDim];
  myMaskEnd = new int[myDim];
}
  
visAdaptiveGridCellEnumeration::~visAdaptiveGridCellEnumeration()
{
  if (getNumberOfRefs() <= 0) return;
  delete[] myMask;
  delete[] myMaskEnd;
}
  
visAdaptiveGrid *visAdaptiveGridCellEnumeration::getFieldAccess()
{
  myFieldAccess->incRef();
  return myFieldAccess;
}

void visAdaptiveGridCellEnumeration::setCursorFirstElement() 
{
  if (myDim <= 0) return;

  myCellIndex = -1;

  int *myMask = new int[myDim];
  int *myMaskEnd = new int[myDim];
  
  int i;
  for (i = 0; i < myDim; i++) {
    myIntStack.push(0);
  }
  for (i = 0; i < myFieldAccess->getDim(); i++) {
    myIntStack.push(myFieldAccess->getSize(i));
  }
  //myStackSize = myDim * 2;
  myMaxStackSize = 0;
  myNumberOfSetCursorCalls = 0;
  myIsValidElement = true;
  setCursorNextElement();
}

void visAdaptiveGridCellEnumeration::setCursorNextElement() 
{
  if (myDim <= 0) return;

  if (myIntStack.getSize() / 2 > myMaxStackSize) {
    myMaxStackSize = myIntStack.getSize() / 2;
  }
  
  myCellIndex++;

  int i;

  if (myIntStack.getSize() == 0) {
    visMsgProfile << "AttributeEnumeration: Stack entries = " 
		  << myMaxStackSize
		  << ", setCursor() calls = "
		  << myNumberOfSetCursorCalls << visMsgEnd;
    myIsValidElement = false;
    return;
  }
  
  for (i = myDim - 1; i >= 0; i--) {
    myMaskEnd[i] = myIntStack.pop();
  }
  for (i = myDim - 1; i >= 0; i--) {
    myMask[i] = myIntStack.pop();
  }
  
  myNumberOfSetCursorCalls++;

  for (i = 0; i < myDim; i++) {
    if (myMask[i] >= myMaskEnd[i]) {
      this->setCursorNextElement();
      return;
    }
  }
  myFieldAccess->setCursor(myMask);
  
  int axis;
  for (axis = 0; axis < myDim; axis++) {
    for (i = 0; i < myFieldAccess->getDim(); i++) {
      if (i == axis) {
	myIntStack.push(myFieldAccess->getCellPos(axis) 
			+ myFieldAccess->getCellSize(axis));
      } else {
	myIntStack.push(myMask[i]);
      }
    }
    for (i = 0; i < axis + 1; i++) {
      myIntStack.push(myMaskEnd[i]);
    }
    for (i = axis + 1; i < myDim; i++) {
      myIntStack.push(myFieldAccess->getCellPos(i)
		      + myFieldAccess->getCellSize(i)
		      < myMaskEnd[i] 
		      ?
		      myFieldAccess->getCellPos(i)
		      + myFieldAccess->getCellSize(i)
		      : myMaskEnd[i]);
    }
    
  }
  for (i = 0; i < myFieldAccess->getDim(); i++) {
    if (myFieldAccess->getCellPos(i) < myMask[i]) {
      this->setCursorNextElement();
      break;
    }
  }
}

bool visAdaptiveGridCellEnumeration::isValidElement() 
{
  return myIsValidElement;
}

int visAdaptiveGridCellEnumeration::getCellIndex() 
{
  return myCellIndex;
}

int visAdaptiveGridCellEnumeration::getNodeIndex(int n) 
{
  return myCellIndex * myDim;
}

