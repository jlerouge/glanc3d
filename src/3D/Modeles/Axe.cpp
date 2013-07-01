#include "Axe.h"

/**
 * Constructeur de Axe
 *
 */
Axe::Axe():vertex1_(), vertex2_()
{}

/**
 * Destructeur de Axe
 *
 */
Axe::~Axe()
{}

/**
 * Fixe la valeur vertex1_
 *
 * @param vertex1
 */
void Axe::setVertex1(QVector3D vertex1)
{
    vertex1_ = vertex1;
}

/**
 * Fixe la valeur de vertex2_
 *
 * @param vertex2
 */
void Axe::setVertex2(QVector3D vertex2)
{
    vertex2_ = vertex2;
}

/**
 * Retourne la valeur de vertex1_
 *
 * @return QVector3D
 */
QVector3D Axe::getVertex1()
{

    return vertex1_;
}

/**
 * Retourne la valeur de vertex2_
 *
 * @return QVector3D
 */
QVector3D Axe::getVertex2()
{

    return vertex2_;

}

/**
 * Dessine l'axe defini entre les vertices vertex1_ et vertex2_
 *
 */
void Axe::dessiner()
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.2f, 0.2f);
    glVertex3f(vertex1_.x(), vertex1_.y(), vertex1_.z());
    glVertex3f(vertex2_.x(), vertex2_.y(), vertex2_.z());
    glEnd();
    glEnable(GL_LIGHTING);
}
