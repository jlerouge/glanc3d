#include "Vertex.h"
#include "Mesh.h"

/**
 * Constructeur de Vertex
 *
 * @param x
 * @param y
 * @param z
 * @param id
 */
Vertex::Vertex(float x, float y, float z, int id):QVector3D(x, y, z), id_(id), indicesTriangles_(),
    distanceMax_(0), normale_()
{}

/**
 * Constructeur de Vertex
 *
 * @param vec
 * @param id
 */
Vertex::Vertex(const QVector3D &vec, int id):QVector3D(vec), id_(id), indicesTriangles_(),
    distanceMax_(0), normale_()
{}

/**
 * Constructeur de Vertex
 *
 * @param vec
 */
Vertex::Vertex(const Vertex &vec):QVector3D(vec), id_(vec.id_),
    indicesTriangles_(vec.indicesTriangles_), distanceMax_(vec.distanceMax_),
    normale_()
{}

/**
 * Destructeur de Vertex
 *
 */
Vertex::~Vertex()
{
    indicesTriangles_.clear();
}

/**
 * Fixe la valeur de la variable distanceMax_
 *
 * @param distMax
 */
void Vertex::setDistanceMax(float distMax)
{
    distanceMax_=distMax;
}

/**
 * Renvoie la valeur de la variable distanceMax_
 *
 * @return float
 */
float Vertex::getDistanceMax() const
{
    return distanceMax_;
}

/**
 * Renvoie la valeur de indicesTriangles_
 *
 * @return std::vector<int>
 */
std::vector<int> Vertex::getIndicesTriangles() const
{
    return indicesTriangles_;
}

/**
 *  Fixe la valeur de indicesTriangles_
 *
 * @param iTriangles
 */
void Vertex::setIndicesTriangles(const std::vector<int> &iTriangles)
{
    indicesTriangles_=iTriangles;
}

/**
 * Ajoute un indice de triangle à    indicesTriangles_
 *
 * @param iTri
 */
void Vertex::ajouterTriangle(int iTri)
{
    indicesTriangles_.push_back(iTri);
}

/**
 * Supprime un indice de triangle à    indicesTriangles_
 *
 * @param iTri
 */
void Vertex::supprimerTriangle(int iTri)
{
    std::vector<int>::iterator it;
    for (it = indicesTriangles_.begin(); it != indicesTriangles_.end(); ++it)
    {
        if (*it == iTri)
        {
            indicesTriangles_.erase(it);
            break;
        }
    }
}


/**
 * Renvoie la valeur de id_
 *
 * @return int
 */
int Vertex::getId() const
{
    return id_;
}

/**
 * Fixe la valeur de id_
 *
 * @param id
 */
void Vertex::setId(int id)
{
    id_=id;
}

/**
 * Renvoie la valeur de normale_
 *
 * @return QVector3D
 */
QVector3D Vertex::getNormale() const
{
    return normale_;
}

QVector3D Vertex::getNormaleMoyenne(Mesh* mesh)
{
    std::vector<int> iVers=mesh->getVerticesAPartirDeTriangles(getIndicesTriangles());
    std::vector<int> indTri = this->getIndicesTriangles();

    for (uint i=0; i<iVers.size(); ++i)
    {
        Vertex v = mesh->getVertex(iVers[i]);

        std::vector<int> list = v.getIndicesTriangles();

        std::vector<int> iVers2 = mesh->getVerticesAPartirDeTriangles(list);

        for (uint k=0; k<iVers2.size(); k++)
        {
            Vertex v2 = mesh->getVertex((iVers2[k]));
            std::vector<int> list2 = v2.getIndicesTriangles();
            indTri.insert(indTri.end(), list2.begin(), list2.end() );
        }
        indTri.insert(indTri.end(), list.begin(), list.end() );
    }
    Outil::ranger(indTri);
    QVector3D normMoyenne;

    for (uint j=0; j<indTri.size(); ++j)
        normMoyenne += (mesh->getTriangle(indTri[j])).getNormale();
    normMoyenne.normalize();
    return normMoyenne;
}

/**
 * Fixe la valeur de normale_
 *
 * @param normale
 */
void Vertex::setNormale(const QVector3D &normale)
{
    normale_ = normale;
}

/** Dessiner un bord suivant le mode
  *
  * @param size
  * @param decalage
  * @param couleur
  */
void Vertex::dessiner(float size, float decalage, QColor couleur)
{
    glPointSize(size);
    glBegin(GL_POINTS);
    QVector3D normale = this->getNormale();
    glColor3ub(couleur.red(), couleur.green(), couleur.blue());
    glVertex3f(this->x()+normale.x()*decalage,
               this->y()+normale.y()*decalage,
               this->z()+normale.z()*decalage);
    glEnd();
    glPointSize(1.f);
}

/**
 * @brief Opérateur de comparaison entre deux vertices
 *
 * @param a
 * @param b
 * @return bool operator
 */
bool operator<( const Vertex  &a, const  Vertex & b)
{
    if (a.x()<b.x())
        return true;
    else if (a.x()>b.x())
        return false;
    else
    {
        if (a.y()<b.y())
            return true;
        else if (a.y()>b.y())
            return false;
        else
        {
            if (a.z()<b.z())
                return true;
            else if (a.z()>b.z())
                return false;
        }
    }
    return false;
}
