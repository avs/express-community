#include <QvFontStyle.h>

QV_NODE_SOURCE(QvFontStyle);

QvFontStyle::QvFontStyle()
{
    QV_NODE_CONSTRUCTOR(QvFontStyle);
    isBuiltIn = TRUE;

    QV_NODE_ADD_FIELD(size);
    QV_NODE_ADD_FIELD(family);
    QV_NODE_ADD_FIELD(style);

    size.value = 10;

    family.value = SERIF;
    QV_NODE_DEFINE_ENUM_VALUE(Fontfamily, SERIF);
    QV_NODE_DEFINE_ENUM_VALUE(Fontfamily, SANS);
    QV_NODE_DEFINE_ENUM_VALUE(Fontfamily, TYPEWRITER);
    QV_NODE_SET_SF_ENUM_TYPE(family, Fontfamily);

    style.value = NONE;
    QV_NODE_DEFINE_ENUM_VALUE(Fontstyle, NONE);
    QV_NODE_DEFINE_ENUM_VALUE(Fontstyle, BOLD);
    QV_NODE_DEFINE_ENUM_VALUE(Fontstyle, ITALIC);
    QV_NODE_SET_SF_ENUM_TYPE(style, Fontstyle);
    
}

QvFontStyle::~QvFontStyle()
{
}
