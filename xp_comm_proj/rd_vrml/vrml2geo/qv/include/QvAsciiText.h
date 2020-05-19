#ifndef  _QV_ASCIITEXT_
#define  _QV_ASCIITEXT_

#include <QvSFString.h>
#include <QvMFFloat.h>
#include <QvSFFloat.h>
#include <QvSFEnum.h>
#include <QvSubNode.h>

class QvAsciiText : public QvNode {

    QV_NODE_HEADER(QvAsciiText);

  public:
    enum Justification {
	LEFT,
	CENTER,
	RIGHT
    };

    // Fields
    QvSFString		string;		// In a real implementation, this would be QvMFString
    QvSFFloat		spacing;
    QvSFEnum		justification;
    QvSFFloat		width;		// In a real implementation, this would be QvMFFloat

};

#endif /* _QV_ASCIITEXT_ */
