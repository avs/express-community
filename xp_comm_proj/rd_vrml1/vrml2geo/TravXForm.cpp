//
// travXForm.C
//
#include <QvElement.h>
#include <QvNodes.h>
#include <QvUnknownNode.h>
#include <QvState.h>
#include <avs/geom.h>

#include <travUtil.h>
#include <travXForm.h>

//
//	Generate current transformation matrix
//
void
GeometricTraverse(QvElement *elem)
{

    Matrix	*mat = getLast(gmatrix);

    switch ( elem->type ) {
      case QvElement::Transform:
	{
	    // Note: Order of Transformations:
	    //	center'(translate -1*center)
	    //	scaleOrientation'(rotate -1*scaleOrientation)
	    //	scaleFactor
	    //	scaleOrientation(rotate scaleOrientation)
	    //	rotation
	    //	center(translate center)
	    //	translation

	    Matrix	rm;
	    QvTransform	*t = (QvTransform *)elem->data;

	    // center' //
	    mat->trans(-t->center.value[0],
		       -t->center.value[1],
		       -t->center.value[2]);

	    // scaleOrientation' //
	    rm.reset();
	    rm.glRotMatrix(-t->scaleOrientation.angle, t->scaleOrientation.axis);
	    *mat *= rm;

	    // scale //
	    mat->scale(t->scaleFactor.value[0],
		       t->scaleFactor.value[1],
		       t->scaleFactor.value[2]);

	    // scaleOrientation //
	    rm.reset();
	    rm.glRotMatrix(t->scaleOrientation.angle, t->scaleOrientation.axis);
	    *mat *= rm;

	    // rotation //
	    rm.reset();
	    rm.glRotMatrix(t->rotation.angle, t->rotation.axis);
	    *mat *= rm;

	    // center //
	    mat->trans(t->center.value[0],
		       t->center.value[1],
		       t->center.value[2]);

	    // translation //
	    mat->trans(t->translation.value[0],
		       t->translation.value[1],
		       t->translation.value[2]);

	    break;
	}
      case QvElement::MatrixTransform:
	{
	    Matrix	mm;
	    QvMatrixTransform *t = (QvMatrixTransform *)elem->data;
	    for ( int i = 0; i < 4; i++ )
		for ( int j = 0; j < 4; j++ )
		    mm.value[i][j] = t->matrix.value[i][j];
	    *mat *= mm;
	    break;
	}
      case QvElement::Translation:
	{
	    QvTranslation *t = (QvTranslation *)elem->data;
	    mat->trans(t->translation.value[0],
		       t->translation.value[1],
		       t->translation.value[2]);
	    break;
	}
      case QvElement::Scale:
	{
	    QvScale *t = (QvScale *)elem->data;
	    mat->scale(t->scaleFactor.value[0],
		       t->scaleFactor.value[1],
		       t->scaleFactor.value[2]);
	    break;
	}
      case QvElement::Rotation:
	{
	    Matrix	rm;
	    QvRotation	*t = (QvRotation *)elem->data;
	    rm.glRotMatrix(t->rotation.angle, t->rotation.axis);
	    *mat *= rm;
	    break;
	}
    }

    // recurrent call //
    if ( elem->next != NULL)
	GeometricTraverse(elem->next);

}

//
//	Traverse Transformation
//
void
goTrans(QvState *state)
{
    QvElement	*elt;

    // Traverse geometric matrix //
    elt = state->getTopElement(QvState::TransformationIndex);
    if ( elt ) {
	GeometricTraverse(elt);
    }

}

// EOF
