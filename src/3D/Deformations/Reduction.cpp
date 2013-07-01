#include "Reduction.h"

/**
 * Constructeur de Reduction, avec paramètres.
 *
 * @param mesh
 */
Reduction::Reduction(Mesh *mesh ): bordCervical_(), axeInsertion_(0)
{
    mesh_=mesh;
    for (int i = 0; i < mesh_->getNbVertices(); i++)
        verticesConcerns_.push_back(i);
}

/**
 * Destructeur de Reduction.
 */
Reduction::~Reduction()
{}

/**
 * Fixe la valeur de mesh_
 *
 * @param mesh
 */
void Reduction::setMesh(Mesh *mesh)
{
    mesh_=mesh;
}

/**
 * Retourne la valeur de mesh_
 *
 * @return Mesh*
 */
Mesh* Reduction::getMesh()
{
    return mesh_;
}

/**
 * Fixe la valeur de axeInsertion_
 *
 * @param axeInsertion
 */
void Reduction::setAxeInsertion(Axe *axeInsertion)
{
    axeInsertion_ = axeInsertion;
}

/**
 * Retourne la valeur de axeInsertion_
 *
 * @return Axe*
 */
Axe* Reduction::getAxeInsertion()
{
    return axeInsertion_;
}

/**
 * Convertit le paramètre en Bord, et le stocke dans bordCervical_
 *
 * @param bordCervical
 */
void Reduction::setBordCervical(std::vector<Arete> bordCervical)
{
    for (int i = 0; i < bordCervical.size(); i++)
    {
        bordCervical_.ajouterPoint(bordCervical[i].getVertex1()->getId());
        for (int j = 0; j < verticesConcerns_.size(); j++)
            if (bordCervical_.getIndices().back() ==verticesConcerns_[j])
                verticesConcerns_.erase(verticesConcerns_.begin()+j);

    }
}

/**
 * Retourne la valeur de bordCervical_
 * @return Bord
 */
Bord Reduction::getBordCervical()
{
    return bordCervical_;
}

/**
 * Calcule un spacer horizontal pondéré par
 * un facteur dépendant d'un angle (sin²)
 *
 * @param angle
 * @param distanceHorizontal
 *
 * @return float
 */
float Reduction::fonctionHorizontal(float angle, float distanceHorizontal)
{
    return distanceHorizontal*sin(angle)*sin(angle);
}

/**
 * Calcule un spacer vertical pondéré par
 * un facteur dépendant d'un angle (cos²)
 *
 * @param angle
 * @param distanceVertical
 *
 * @return float
 */
float Reduction::fonctionVertical(float angle, float distanceVertical)
{
    return distanceVertical*cos(angle)*cos(angle);
}

/**
 * Déplace un Vertex suivant sa normale, l'angle de sa normale
 * et différents spacers.
 *
 * @param v
 * @param angle
 * @param distanceHorizontal
 * @param distanceVertical
 * @param spacerGeneral
 */
void Reduction::reduireSpacer(Vertex &v, float angle, float distanceHorizontal,
                              float distanceVertical, float spacerGeneral)
{
    QVector3D a = (fonctionVertical(angle, distanceVertical) +
                   fonctionHorizontal(angle, distanceHorizontal))*(v.getNormale().normalized());
    mesh_->setXYZ(v.getId(), v-a-spacerGeneral*v.getNormale().normalized());
}

/**
 * Déplace un Vertex afin de réduire un mesh.
 *
 * @param indice
 * @param distanceHorizontal
 * @param distanceVertical
 * @param spacerGeneral
 */
void Reduction::reduireUnPoint(int indice, float distanceHorizontal, float distanceVertical,
                               float spacerGeneral)
{
    QVector3D nor = mesh_->getVertex(indice).getNormale();
    QVector3D axe = (axeInsertion_->getVertex2()-axeInsertion_->getVertex1()).normalized();

    float angle = Geometrie::angle(nor, axe);
    reduireSpacer(mesh_->getVertex(indice), angle, distanceHorizontal,
                  distanceVertical, spacerGeneral);
}

/**
 * Réduit un mesh avec 3 paramètres de réduction :
 * un spacer horizontal, un spacer vertical, et un spacer général.
 *
 * @param distanceHorizontal
 * @param distanceVertical
 * @param spacerGeneral
 */
void Reduction::reduire3D(float distanceHorizontal, float distanceVertical, float spacerGeneral)
{
    int nbVertices= verticesConcerns_.size();
    for (int i=0; i<(nbVertices);i++)
    {
        reduireUnPoint(verticesConcerns_[i], distanceHorizontal, distanceVertical, spacerGeneral );
    }
}
