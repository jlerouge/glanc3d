#ifndef __FORME2D_H__
#define __FORME2D_H__

#include "Geometrie.h"

/**
 * La classe Forme2D représente un forme libre sélectionné à  la souris
 * @author ToothPIC
 */
class Forme2D
{

public:
    Forme2D();
    Forme2D(std::vector<QPointF> &points, const QSizeF &tailleFenetre);
    ~Forme2D();
    std::vector<QPointF> getVertices();

    void getModele(GLdouble* params);
    void getProjection(GLdouble* params);
    void getVue(GLint* params);
    QSizeF getTailleFenetre();
    void setMatTransf(QMatrix4x4 transfo);
    QMatrix4x4 getMatTransf();
    void reduireForme();

private:
    /** Liste des points du polygone de la forme2D */
    std::vector<QPointF> formeLibre2D_;
    /** Matrice contenant les informations sur le modèle */
    GLdouble *modele_;
    /** Matrice contenant les informations sur la projection */
    GLdouble *projection_;
    /** Matrice contenant les informations sur la vue */
    GLint *vue_;
    /** Taille de la fenetre du widget*/
    QSizeF tailleFenetre_;
    /** Matrice de transformation modele/empreinte ajustée */
    QMatrix4x4 matTransform_;
};
#endif /*__Forme2D_H__*/
