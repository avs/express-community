#include <QvPerspectiveCamera.h>

#if 000 /* typo!! */
#define M_PI_4 3.144159265/4
#else
#ifndef M_PI_4
#define M_PI_4 3.14159265/4.0
#endif /*M_PI_4*/
#endif

QV_NODE_SOURCE(QvPerspectiveCamera);

QvPerspectiveCamera::QvPerspectiveCamera()
{
    QV_NODE_CONSTRUCTOR(QvPerspectiveCamera);
    isBuiltIn = TRUE;

    QV_NODE_ADD_FIELD(position);
    QV_NODE_ADD_FIELD(orientation);
    QV_NODE_ADD_FIELD(focalDistance);
    QV_NODE_ADD_FIELD(heightAngle);
    //  Added by Yoshikawa
    QV_NODE_ADD_FIELD(aspectRatio);
    QV_NODE_ADD_FIELD(nearDistance);
    QV_NODE_ADD_FIELD(farDistance);

    position.value[0] = 0.0;
    position.value[1] = 0.0;
    position.value[2] = 1.0;
    orientation.axis[0] = 0.0;
    orientation.axis[1] = 0.0;
    orientation.axis[2] = 1.0;
    orientation.angle = 0.0;
    focalDistance.value = 5.0;
    heightAngle.value = M_PI_4; // 45 degrees

    // Added by Yoshikawa
    aspectRatio.value = 1.0;
    nearDistance.value = 1.0;
    farDistance.value = 10.0;
}

QvPerspectiveCamera::~QvPerspectiveCamera()
{
}
