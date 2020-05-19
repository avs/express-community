#ifndef  _QV_FONTSTYLE_
#define  _QV_FONTSTYLE_

#include <QvSFFloat.h>
#include <QvSFEnum.h>
#include <QvSFBitMask.h>
#include <QvSubNode.h>

class QvFontStyle : public QvNode {

    QV_NODE_HEADER(QvFontStyle);

  public:
    enum Fontfamily {
	SERIF,		// TimesRoman
	SANS,		// Helvetica
	TYPEWRITER	// Courier
    };

    enum Fontstyle {
	NONE	= 0x00,
	BOLD	= 0x01,
	ITALIC	= 0x02
    };

    // Fields
    QvSFFloat		size;
    QvSFEnum		family;
    QvSFBitMask		style;

};

#endif /* _QV_FONTSTYLE_ */
