#include "Transformation.h"

/**
 * Constructeur de Transformation
 *
 */
Transformation::Transformation(): mesh_(0), mode_(TRANSLATION), positionSouris_(),
    tailleFenetre_(), zoom_(), modeBouton_(TRANSLATION), modeBoutonActif_(false)
{}

/**
 * Destructeur de Transformation
 *
 */
Transformation::~Transformation()
{}

/**
 * Fixe la valeur de mesh_
 *
 * @param mesh
 */
void Transformation::setMesh(Mesh *mesh)
{
    mesh_=mesh;
}

/**
 * Renvoie la valeur de mesh_
 *
 * @return Mesh
 */
Mesh* Transformation::getMesh()
{
    return mesh_;
}

/**
 * Fixe la valeur de mode_
 *
 * @param mode
 */
void Transformation::setMode(TransformationMode mode)
{
    mode_=mode;
}

/**
 * Fixe la valeur de modeBouton_
 *
 * @param mode
 */
void Transformation::setModeBouton(TransformationMode mode)
{
    modeBouton_=mode;
}

/**
 * Fixe la valeur de modeBoutonActif_
 *
 * @param mode
 */
void Transformation::activerModeBouton(bool estActif)
{
    modeBoutonActif_=estActif;
}


/**
 * Débute la transformation
 *
 * @param positionSouris
 * @param tailleFenetre
 * @param zoom
 */
void Transformation::debuter(const QPointF &positionSouris,
                             const QSizeF &tailleFenetre,
                             float zoom)
{
    tailleFenetre_=tailleFenetre;
    positionSouris_=positionSouris;
    zoom_=zoom/150.f;
}


/**
 * Transforme le mesh
 *
 * @param positionSouris
 */
void Transformation::transformer(const QPointF& positionSouris)
{
    if (!mesh_->estEditable())
        return;
    TransformationMode mode;
    if (modeBoutonActif_)
        mode = modeBouton_;
    else
        mode = mode_;

    if (mode == ROTATION)
    {
        QPointF positionSourisOldInv(positionSouris_.y(), -positionSouris_.x());
        QPointF positionSourisNewInv(positionSouris.y(), -positionSouris.x());
        QVector3D vecteurEcran=Geometrie::calculerVecteurEcranPlanCamera(
                    positionSourisOldInv, positionSourisNewInv, tailleFenetre_);
        QVector3D centreMesh=mesh_->getTransformation()*mesh_->getCentre();
        QMatrix4x4 matTransformation;
        matTransformation.translate(centreMesh);
        matTransformation.rotate((positionSouris-positionSouris_).manhattanLength()/5.f,
                                 vecteurEcran);
        matTransformation.translate(-centreMesh);
        mesh_->setTransformation(matTransformation*mesh_->getTransformation());
    }
    else if (mode == TRANSLATION)
    {
        QVector3D vecteurEcran=Geometrie::calculerVecteurEcranPlanCamera(
                    positionSouris_, positionSouris, tailleFenetre_);
        QMatrix4x4 matTransformation;
        matTransformation.translate(
                    vecteurEcran*zoom_*(positionSouris-positionSouris_).manhattanLength()/4.2f);
        mesh_->setTransformation(matTransformation*mesh_->getTransformation());
    }
    else
    {
        float coef = 1.f+(zoom_*(positionSouris-positionSouris_).manhattanLength()/50);
        if ((positionSouris-positionSouris_).x() < 0)
            coef = 1/coef;
        mesh_->homothetie(coef);
    }
    positionSouris_ = positionSouris;
}


/**
 * Renvoie la valeur de modeBoutonActif_
 *
 * @return bool
 */
bool Transformation::getModeBoutonActif ()
{
    return modeBoutonActif_;
}
