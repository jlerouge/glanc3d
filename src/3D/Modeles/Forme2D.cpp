#include "Forme2D.h"
#include <iostream>

/**
 * Constructeur de Forme2D
 *
 */

Forme2D::Forme2D():modele_(0), projection_(0), vue_(0)
{}


/**
 * Constructeur de Forme2D
 *
 * @param points
 * @param tailleFenetre
 */
Forme2D::Forme2D(std::vector<QPointF> &points,  const QSizeF &tailleFenetre):formeLibre2D_(points),
    tailleFenetre_(tailleFenetre)
{
    vue_  = (GLint*)malloc(sizeof(GLint)*4);
    modele_ = (GLdouble*)malloc(sizeof(GLdouble)*16);
    projection_ = (GLdouble*)malloc(sizeof(GLdouble)*16);
    glGetIntegerv(GL_VIEWPORT, vue_);
    glGetDoublev (GL_MODELVIEW_MATRIX, modele_);
    glGetDoublev (GL_PROJECTION_MATRIX, projection_);
}


/**
 * Destructeur de Forme2D
 *
 */
Forme2D::~Forme2D()
{}


/**
 * Retourne la valeur de formeLibre2D_
 *
 * @return std::vector<QPointF>
 */

std::vector<QPointF> Forme2D::getVertices()
{
    return formeLibre2D_;
}


/**
 * Retourne les valeurs de modele_
 *
 * @param params
 */

void Forme2D::getModele(GLdouble* params)
{
    for (int i=0;i<16;++i)
        params[i] = modele_[i];
}


/**
 * Retourne les valeurs de projection_
 *
 * @param params
 */

void Forme2D::getProjection(GLdouble* params )
{
    for (int i=0;i<16;++i)
        params[i] = projection_[i];
}

/**
 * Retourne les valeurs de vue_
 *
 * @param params
 */

void Forme2D::getVue(GLint* params)
{
    for (int i=0;i<4;++i)
        params[i] = vue_[i];
}


/**
 * Retourne la valeur de tailleFenetre_
 *
 * @return QSizeF
 */

QSizeF Forme2D::getTailleFenetre()
{
    return tailleFenetre_;
}


/**
 * Fixe la valeur de matTransform_
 *
 * @param transfo
 */

void Forme2D::setMatTransf(QMatrix4x4 transfo)
{
    matTransform_ = transfo;
}


/**
 *  Retourne la valeur de matTransform_
 *
 * @return QMatrix4x4
 */

QMatrix4x4 Forme2D::getMatTransf()
{
    return matTransform_;
}


/**
 * Reduit/Augmente la distance entre les points de la Forme2D et de leur centre de gravité
 *
 */

void Forme2D::reduireForme()
{
    QPointF centre = QPointF();
    std::vector<QPointF>::iterator it;
    for (it=formeLibre2D_.begin();it!=formeLibre2D_.end();++it)
        centre +=*it;
    centre /= formeLibre2D_.size();
    for (uint i=0; i<formeLibre2D_.size(); ++i)
        formeLibre2D_[i] +=0.05*(formeLibre2D_[i]-centre);
}
