#include "Aabb.h"

/**
 * Constructeur de Aabb
 *
 * @param min
 * @param max
 */
Aabb::Aabb(const QVector3D &min, const QVector3D &max):min_(min), max_(max)
{}

/**
 * Destructeur de Aabb
 *
 */
Aabb::~Aabb()
{}

/**
 * Renvoie la valeur de min_
 *
 * @return QVector3D
 */
QVector3D Aabb::getMin() const
{
    return min_;
}

/**
 * Renvoie la valeur de max_
 *
 * @return QVector3D
 */
QVector3D Aabb::getMax() const
{
    return max_;
}

/**
 * Fixe la valeur de min_
 *
 * @param min
 */
void Aabb::setMin(const QVector3D &min)
{
    min_.setX(min.x());
    min_.setY(min.y());
    min_.setZ(min.z());
}

/**
 * Fixe la valeur de max_
 *
 * @param max
 */
void Aabb::setMax(const QVector3D &max)
{
    max_.setX(max.x());
    max_.setY(max.y());
    max_.setZ(max.z());
}

/**
 * Renvoie true dans le cas où au moins un point de la boule dont le centre est le
 * vertex passé en argument et le rayon égale à sa distanceMax_ est à l'intérieur de l'aabb.
 *
 * @param vert
 * @return bool
 */
bool Aabb::vertexDansAabb(const Vertex &vert) const
{
    float rayonVert=vert.getDistanceMax();
    if ((vert.x()+rayonVert)<min_.x())
        return false;
    if ((vert.x()-rayonVert)>max_.x())
        return false;
    if ((vert.y()+rayonVert)<min_.y())
        return false;
    if ((vert.y()-rayonVert)>max_.y())
        return false;
    if ((vert.z()+rayonVert)<min_.z())
        return false;
    if ((vert.z()-rayonVert)>max_.z())
        return false;
    return true;
}

/**
 * La même chose que vertexDansAabb sans prendre en compte le rayon des vertices
 *
 * @param vert
 * @return bool
 */
bool Aabb::pointDansAabb(const QVector3D &vert) const
{
    if (vert.x()<min_.x())
        return false;
    if (vert.x()>max_.x())
        return false;
    if (vert.y()<min_.y())
        return false;
    if (vert.y()>max_.y())
        return false;
    if (vert.z()<min_.z())
        return false;
    if (vert.z()>max_.z())
        return false;
    return true;
}

/**
 * Met à jour les valeurs min_ et max_ de façon à agrandir la taille de l'aabb
 * afin que le vertex passé en argument soit à l'intérieur de celui-ci
 *
 * @param ver
 */
void Aabb::majTaille(const QVector3D &ver)
{
    if (ver.x()>max_.x())
        max_.setX(ver.x());
    if (ver.x()<min_.x())
        min_.setX(ver.x());
    if (ver.y()>max_.y())
        max_.setY(ver.y());
    if (ver.y()<min_.y())
        min_.setY(ver.y());
    if (ver.z()>max_.z())
        max_.setZ(ver.z());
    if (ver.z()<min_.z())
        min_.setZ(ver.z());
}

/**
 * Dessine les arêtes de l'aabb en rouge
 *
 */
void Aabb::dessiner() const
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.f, 0.f);
    glVertex3f(min_.x(), min_.y(), min_.z());
    glVertex3f(max_.x(), min_.y(), min_.z());

    glVertex3f(max_.x(), min_.y(), min_.z());
    glVertex3f(max_.x(), max_.y(), min_.z());

    glVertex3f(max_.x(), max_.y(), min_.z());
    glVertex3f(min_.x(), max_.y(), min_.z());

    glVertex3f(min_.x(), max_.y(), min_.z());
    glVertex3f(min_.x(), min_.y(), min_.z());

    glVertex3f(min_.x(), min_.y(), min_.z());
    glVertex3f(min_.x(), min_.y(), max_.z());

    glVertex3f(min_.x(), min_.y(), max_.z());
    glVertex3f(max_.x(), min_.y(), max_.z());

    glVertex3f(max_.x(), min_.y(), max_.z());
    glVertex3f(max_.x(), min_.y(), min_.z());

    glVertex3f(max_.x(), min_.y(), max_.z());
    glVertex3f(max_.x(), max_.y(), max_.z());

    glVertex3f(max_.x(), max_.y(), max_.z());
    glVertex3f(max_.x(), max_.y(), min_.z());

    glVertex3f(max_.x(), max_.y(), max_.z());
    glVertex3f(min_.x(), max_.y(), max_.z());

    glVertex3f(min_.x(), max_.y(), max_.z());
    glVertex3f(min_.x(), max_.y(), min_.z());

    glVertex3f(min_.x(), max_.y(), max_.z());
    glVertex3f(min_.x(), min_.y(), max_.z());
    glEnd();
    glEnable(GL_LIGHTING);
}

/**
 * Renvoie le centre de l'aabb
 *
 * @return QVector3D
 */
QVector3D Aabb::getCentre() const
{
    return (min_+max_)/2.f;
}
