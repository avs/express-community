#include <QvAsciiText.h>

QV_NODE_SOURCE(QvAsciiText);

QvAsciiText::QvAsciiText()
{
    QV_NODE_CONSTRUCTOR(QvAsciiText);
    isBuiltIn = TRUE;

    QV_NODE_ADD_FIELD(string);
    QV_NODE_ADD_FIELD(spacing);
    QV_NODE_ADD_FIELD(justification);
    QV_NODE_ADD_FIELD(width);

    string.value = "";
    spacing.value = 1;

    justification.value = LEFT;

    QV_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
    QV_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
    QV_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
    QV_NODE_SET_SF_ENUM_TYPE(justification, Justification);

    width.value = 0;
    
}

QvAsciiText::~QvAsciiText()
{
}
