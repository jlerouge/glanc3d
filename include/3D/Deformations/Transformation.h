#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include <QVector3D>
#include <QtOpenGL>
#include <GL/glu.h>
#include "Mesh.h"

/**
 * La classe Transformation gère transformations comme la rotation ou la translation
 * @author ToothPIC
 */
class Transformation
{

public:
    enum TransformationMode{TRANSLATION,ROTATION,REDUCTION};
    Transformation();
    ~Transformation();
    void setMesh(Mesh *mesh);
    Mesh* getMesh();
    void setMode(TransformationMode mode);
    void debuter(const QPointF& positionSouris,const QSizeF &tailleFenetre,float zoom);
    void transformer(const QPointF& positionSouris);
    void setModeBouton(TransformationMode mode);
    void activerModeBouton(bool estActif);
    bool getModeBoutonActif ();

private:
    /** Le mesh sélectionné */
    Mesh* mesh_;
    /** Le mode de la caméra */
    TransformationMode mode_;
    /** La position de la souris sur l'écran */
    QPointF positionSouris_;
    /** Taille de la fenetre de la vue OpenGL */
    QSizeF tailleFenetre_;
    /** Le zoom (pour être dépendant de la vitesse de translation) */
    float zoom_;
    /** Le mode de la caméra quand un raccourci est activé */
    TransformationMode modeBouton_;
    /** Renseigne si un raccourci de transformation est en cours d'utilisation */
    bool modeBoutonActif_;
};

#endif /*__TRANSFORMATION_H__*/
