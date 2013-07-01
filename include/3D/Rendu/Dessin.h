#ifndef __DESSIN_H__
#define __DESSIN_H__

#include <QVector3D>
#include <QtOpenGL>
#include <GL/glu.h>
#include "Mesh.h"

/**
 * Le namespace Dessin permet de faire des figures avec OpenGL
 * @author ToothPIC
 */
namespace Dessin
{
    void dessinerRepereSpherique(float rayon);
    void dessinerCercleSelection(const QPointF &centrePosition,const QPointF &dernierePosition);
    void dessinerGrille(float size,float step);
    void dessinerPoints(Mesh *mesh, std::vector<int> indices, QColor couleur);
}

#endif /*__DESSIN_H__*/
