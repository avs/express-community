#include <QvUnknownNode.h>

QV_NODE_SOURCE(QvUnknownNode);

QvUnknownNode::QvUnknownNode()
{
    QV_NODE_CONSTRUCTOR(QvUnknownNode);

    className = NULL;

    // Set global field data to this instance's
    instanceFieldData = new QvFieldData;
    fieldData = instanceFieldData;
}

void
QvUnknownNode::setClassName(const char *name)
{
#ifdef WIN32
    className = _strdup(name);
#else
    className = strdup((char *)name);
#endif
}

#ifdef	sun
#define	FREETYPE	(char *)
#else
#define	FREETYPE	(void *)
#endif

QvUnknownNode::~QvUnknownNode()
{
    if (className != NULL)
	free(FREETYPE className);
}
#undef FREETYPE
