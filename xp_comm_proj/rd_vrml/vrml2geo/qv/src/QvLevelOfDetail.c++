#include <QvLevelOfDetail.h>

QV_NODE_SOURCE(QvLevelOfDetail);

QvLevelOfDetail::QvLevelOfDetail()
{
    QV_NODE_CONSTRUCTOR(QvLevelOfDetail);
    isBuiltIn = TRUE;

    QV_NODE_ADD_FIELD(range);
    QV_NODE_ADD_FIELD(center);

    range.values[0] = 0;
    center.value[0] = center.value[1] = center.value[2] = 0.0;
}

QvLevelOfDetail::~QvLevelOfDetail()
{
}
