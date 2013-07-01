#ifndef __AABB_H__
#define __AABB_H__

#include <QVector3D>
#include <QtOpenGL>
#include "Vertex.h"

/**
 * La classe Aabb (Axis-aligned bounding box) représente un pavé
 * droit dont les arêtes sont alignées avec les axes du repère.
 *
 * @author ToothPIC
 */
class Aabb
{
public:
    Aabb(const QVector3D &min=QVector3D(0.f,0.f,0.f),const QVector3D &max=QVector3D(0.f,0.f,0.f));
    ~Aabb();
    QVector3D getMin() const;
    QVector3D getMax() const;
    void setMin(const QVector3D &min);
    void setMax(const QVector3D &max);
    bool vertexDansAabb(const Vertex &vert) const;
    bool pointDansAabb(const QVector3D &vert) const;
    void majTaille(const QVector3D &ver);
    void dessiner() const;
    QVector3D getCentre() const;

protected :
    /** Sommet de l'Aabb avec les valeurs x, y et z minimales  */
    QVector3D min_;
    /** Sommet de l'Aabb avec les valeurs x, y et z maximales  */
    QVector3D max_;
};

#endif /*__AABB_H__*/
