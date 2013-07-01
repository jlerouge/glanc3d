#include "Octree.h"
#include "Mesh.h"
/**
 * Constructeur de Octree
 *
 * @param parent
 * @param profondeur
 * @param profondeurMax
 * @param maxVertices
 */
Octree::Octree(Octree* parent, int profondeur, int profondeurMax, int maxVertices):
    parent_(parent), fils_(),
    vertices_(),
    nbVerticesMax_(maxVertices),
    profondeur_(profondeur),
    profondeurMax_(profondeurMax)
{
    for (int i=0;i<8;i++)
        fils_[i]=0;
}

/**
 * Destructeur de Octree
 *
 */
Octree::~Octree()
{
    for (int i=0;i<8;i++)
        delete fils_[i];
}

/**
 * Renvoie la valeur de vertices_
 *
 * @return std::set<Vertex>
 */
std::set<Vertex*>& Octree::getVertices()
{
    return vertices_;
}

/**
  * Renvoie le nombre de vertices contenus dans cet octree ou tous les sous-octrees
  *
  * @return int
  */

int Octree::getNbVerticesTotal()
{
    int nbTotal = 0;
    if (fils_[0]!=0)
    {
        for (int i=0;i<8;i++)
            nbTotal += fils_[i]->getNbVerticesTotal();
    }
    else
        nbTotal = vertices_.size();
    return nbTotal;
}

/**
 * Creation de l'arbre à  partir d'un vector de vertices
 *
 * @param vertices
 * @param aabb
 */
void Octree::creer(std::vector<Vertex> &vertices, const Aabb &aabb)
{
    setMax(aabb.getMax());
    setMin(aabb.getMin());
    vertices_.clear();
    int nbVertices=vertices.size();
    for (int i=0;i<nbVertices;i++)
    {
        if (this->vertexDansAabb(vertices[i]))
            vertices_.insert(&vertices[i]);
    }
    int nbVerticesNoeud=vertices_.size();
    //on decide si c'est un noeud ou une feuille
    if (nbVerticesNoeud>nbVerticesMax_ && profondeur_<profondeurMax_)
        constructionNoeuds();
    // this->changerTailleOctree(0.1);
}

/**
 * Creation de l'arbre à  partir d'un set de pointeurs de vertices
 *
 * @param vertices
 * @param aabb
 */
void Octree::creer(const std::set<Vertex*> &vertices, const Aabb &aabb)
{
    setMax(aabb.getMax());
    setMin(aabb.getMin());
    vertices_.clear();
    std::set<Vertex*>::iterator it;
    for (it=vertices.begin();it!=vertices.end();++it)
    {
        if (this->vertexDansAabb(**it))
            vertices_.insert(*it);
    }
    int nbVerticesNoeud=vertices_.size();
    //on decide si c'est un noeud ou une feuille
    if (nbVerticesNoeud>nbVerticesMax_ && profondeur_<profondeurMax_)
        constructionNoeuds();
    //   this->changerTailleOctree(0.1);
}

/**
 * Construction des noeuds de l'arbre
 *
 */
void Octree::constructionNoeuds()
{
    QVector3D deltaX((max_.x()-min_.x())/2, 0.f, 0.f);
    QVector3D deltaY(0.f, (max_.y()-min_.y())/2, 0.f);
    QVector3D deltaZ(0.f, 0.f, (max_.z()-min_.z())/2);
    QVector3D milieu=(min_+max_)/2;
    int profondeurNew=profondeur_+1;
    fils_[0]=new Octree(this, profondeurNew, profondeurMax_);
    fils_[0]->creer(vertices_, Aabb(min_, milieu));
    fils_[1]=new Octree(this, profondeurNew, profondeurMax_);
    fils_[1]->creer(vertices_, Aabb(min_+deltaX, milieu+deltaX));
    fils_[2]=new Octree(this, profondeurNew, profondeurMax_);
    fils_[2]->creer(vertices_, Aabb(milieu-deltaY, max_-deltaY));
    fils_[3]=new Octree(this, profondeurNew, profondeurMax_);
    fils_[3]->creer(vertices_, Aabb(min_+deltaZ, milieu+deltaZ));
    fils_[4]=new Octree(this, profondeurNew, profondeurMax_);
    fils_[4]->creer(vertices_, Aabb(min_+deltaY, milieu+deltaY));
    fils_[5]=new Octree(this, profondeurNew, profondeurMax_);
    fils_[5]->creer(vertices_, Aabb(milieu-deltaZ, max_-deltaZ));
    fils_[6]=new Octree(this, profondeurNew, profondeurMax_);
    fils_[6]->creer(vertices_, Aabb(milieu, max_));
    fils_[7]=new Octree(this, profondeurNew, profondeurMax_);
    fils_[7]->creer(vertices_, Aabb(milieu-deltaX, max_-deltaX));
    vertices_.clear();
}

/**
 * Renvoie l'ensemble des vertices des octree associés à  un vertex
 *
 * @param vert
 * @return std::vector<int>
 */
std::vector<int> Octree::getVerticesVoisins(const Vertex &vert) const
{
    std::vector<int> verticesDansNoeud;
    std::vector<int> vertices;
    if (vertexDansAabb(vert))
    {
        if (fils_[0]==0)
        {
            std::set<Vertex*>::iterator it;
            for (it=vertices_.begin();it!=vertices_.end();++it)
                vertices.push_back((*it)->getId());
            return vertices;
        }
        for (int i=0;i<8;i++)
        {
            if (fils_[i]->vertexDansAabb(vert))
            {
                vertices=fils_[i]->getVerticesVoisins(vert);
                verticesDansNoeud.insert(verticesDansNoeud.end(),
                                         vertices.begin(),
                                         vertices.end());
            }
        }
    }
    Outil::ranger(verticesDansNoeud);
    return verticesDansNoeud;
}

/**
 * Dessine l'aabb de toutes les feuilles
 *
 */
void Octree::dessiner() const
{

    Aabb::dessiner();
    if (fils_[0]!=0)
        for (int i=0;i<8;i++)
            fils_[i]->dessiner();

}

/**
 * On recupère l'ensemble des indices des vertices contenus dans les sphère circonscrites
 * aux sommets des aabb feuille traversées par le lancer de rayon
 *
 * @param vertSouris1
 * @param vectDirection
 * @return std::vector<int>
 */
std::vector<int> Octree::getVerticesIntersectionRayon(const QVector3D &vertSouris1,
                                                      const QVector3D &vectDirection) const
{
    return this->getVerticesIntersection(&vertSouris1, 0, NULL, &vectDirection);
}

/**
 * On recupère l'ensemble des indices des vertices contenus dans les sphères circonscrites
 * aux sommets des aabb feuille traversées par le lancer de cylindre
 *
 * @param vertSouris1
 * @param vectDirection
 * @param rayon
 * @return std::vector<int>
 */
std::vector<int> Octree::getVerticesIntersectionCylindre(const QVector3D &vertSouris1,
                                                         const QVector3D &vectDirection,
                                                         float rayon) const
{
    return this->getVerticesIntersection(&vertSouris1, rayon, NULL, &vectDirection);
}

/**
 * On recupère l'ensemble des indices des vertices contenus dans les  circonscrites
 * aux sommets des aabb feuille traversées par le lancer de sphère
 *
 * @param vertCentre
 * @param rayon
 * @param prox_norm
 * @return std::vector<int>
 */
std::vector<int> Octree::getVerticesIntersectionSphere(const QVector3D &vertCentre,
                                                       float rayon, const QVector3D &normale) const
{
    return this->getVerticesIntersection(&vertCentre, rayon, &normale);
}

/**
 * On récupère l'ensemble des indices des vertices contenus dans le
 * volume d'intersection ou dont un des triangles
 * est traversé par un rayon (vertex+direction).
 *
 * @param vert1
 * @param rayon
 * @param prox_norm
 * @param vect2
 * @return std::vector<int>
 */
std::vector<int> Octree::getVerticesIntersection(const QVector3D *vert1, float rayon,
                                                 const QVector3D *normale,
                                                 const QVector3D *vert2) const
{
    std::vector<int> iVertices;
    QVector3D centreAABB=(min_+max_)/2.f;
    float rayonCirconscritAABB=Geometrie::distance(min_, max_)/2.f;
    if ((vert2 && centreAABB.distanceToLine(*vert1, *vert2)<(rayonCirconscritAABB+rayon))||
            (!vert2 && Geometrie::distance(centreAABB, *vert1)<(rayonCirconscritAABB+rayon)))
    {
        if (fils_[0]!=0)
        {
            for (int i=0;i<8;i++)
            {
                std::vector<int> iVerts=fils_[i]->getVerticesIntersection(vert1, rayon, normale,
                                                                          vert2);
                iVertices.insert(iVertices.end(), iVerts.begin(), iVerts.end());
            }
        }
        else
        {
            std::set<Vertex*>::iterator it;
            for (it=vertices_.begin();it!=vertices_.end();++it)
            {
                float angle = 0.f;
                float distance = 0.f;
                if (!vert2 && normale)
                {
                    angle = Geometrie::angle((*it)->getNormale(), *normale);
                    distance = Geometrie::distance(**it, *vert1);
                }
                if ((vert2 && (*it)->distanceToLine(*vert1, *vert2) <=
                     ((*it)->getDistanceMax()+rayon)) ||
                        (!vert2 && (!normale || (angle >= M_PI/4)) &&
                         ((normale && distance<=rayon) ||
                          (!normale && distance<=sin(angle/2)*rayon))))
                    iVertices.push_back((*it)->getId());
            }
        }
        Outil::ranger(iVertices);
    }
    return iVertices;
}


/**
 * On récuà¨àÅ¡re l'ensemble des indices des vertices contenus dans le
 * volume d'intersection ou dont un des triangles
 * est traversé par un rayon (vertex+direction).
 *
 * @param vert1
 * @param rayon
 * @param prox_norm
 * @param vect2
 * @return std::vector<int>
 */
std::vector<int> Octree::getVerticesIntersectionErosion(const QVector3D vert1,
                                                        float rayon, int indiceVertex) const
{
    std::vector<int> iVertices;
    QVector3D centreAABB=(min_+max_)/2.f;
    float rayonCirconscritAABB=Geometrie::distance(min_, max_)/2.f;
    if (Geometrie::distance(centreAABB, vert1)<=(rayonCirconscritAABB+rayon))
    {
        if (fils_[0]!=0)
        {
            for (int i=0;i<8;i++)
            {
                std::vector<int> iVerts=fils_[i]->getVerticesIntersectionErosion(
                            vert1, rayon, indiceVertex);
                iVertices.insert(iVertices.end(), iVerts.begin(), iVerts.end());
            }
        }
        else
        {
            std::set<Vertex*>::iterator it;
            for (it=vertices_.begin();it!=vertices_.end();++it)
            {
                float distance = 0.f;
                distance = Geometrie::distance(**it, vert1);
                if (distance <= (rayon) && (**it).getId() != indiceVertex)
                    iVertices.push_back((*it)->getId());
            }
        }
        Outil::ranger(iVertices);
    }
    return iVertices;
}
/**
 * Met a jour l'octree en supprimant un vertex de l'arbre
 *
 * @param vert
 */
void Octree::supprimerVertex(Vertex &vert)
{
    if (vertexDansAabb(vert))
    {
        if (fils_[0]!=0)
        {
            for (int i=0;i<8;i++)
                fils_[i]->supprimerVertex(vert);
        }
        else
            vertices_.erase(&vert);
    }
}

/**
 * Met a jour l'octree en rajoutant un vertex dans l'arbre
 *
 * @param vert
 */
bool Octree::rajouterVertex(Vertex &vert)
{
    if (vertexDansAabb(vert))
    {
        if (fils_[0]!=0)
        {
            for (int i=0;i<8;i++)
            {
                if (fils_[i]->rajouterVertex(vert))
                    return true;
            }
        }
        else
        {
            vertices_.insert(&vert);
            return true;
        }
    }
    return false;

}

/**
 * Modifie la taille de tous les octree de l'arbre
 *
 * @param facteur
 */
void Octree::changerTailleOctree(float facteur)
{
    min_ = min_ -(max_-min_)*facteur/2;
    max_ = max_ +(max_-min_)*facteur/2;
    majNoeuds();
}

/**
 * Renvoie le vertex le plus proche d'un point 3D. Attention ne pas oublier de faire
 * auparavant la transformation inverse sur le vertex (transformationInverse()*vert)
 *
 * @param vert
 * @param premierAppel
 * @return Vertex
 */
Vertex* Octree::getVertexLePlusProche(const QVector3D &vert, bool premierAppel)
{
    if (fils_[0]==0)
        return getVertexProche(vert);
    Vertex *vertexLePlusProche=0;
    bool estDansCube=pointDansAabb(vert);
    if (!estDansCube && premierAppel)
    {
        float min = F_MIN_INIT;
        int numeroFils = 0;
        for (int i=0; i<8; i++)
        {
            float distance=Geometrie::distance(vert, fils_[i]->getCentre());
            if (min>distance)
            {
                min=distance;
                numeroFils=i;
            }
        }
        vertexLePlusProche = fils_[numeroFils]->getVertexLePlusProche(vert, true);
    }
    else if (estDansCube)
    {
        for (int i=0;i<8;i++)
            if (fils_[i]->pointDansAabb(vert))
                vertexLePlusProche = fils_[i]->getVertexLePlusProche(vert, false);
    }
    if (!vertexLePlusProche)
    {
        for (int i=0;i<8;i++)
            if ((vertexLePlusProche = fils_[i]->getVertexLePlusProche(vert, false)))
                return vertexLePlusProche;
    }
    return vertexLePlusProche;
}

/**
 * Renvoie le vertex le plus proche d'un point 3D
 *
 * @param vert
 * @return Vertex
 */
Vertex* Octree::getVertexProche(const QVector3D &vert)
{
    Vertex *vertexLePlusProche = 0;
    float min = F_MIN_INIT;
    std::set<Vertex*>::iterator it;
    for (it=vertices_.begin();it!=vertices_.end();++it)
    {
        float distance=Geometrie::distance(vert, QVector3D((*it)->x(), (*it)->y(), (*it)->z()));
        if (min > distance)
        {
            min = distance;
            vertexLePlusProche = (*it);
        }
    }
    return vertexLePlusProche;
}

/**
 * Test la collision entre un octree et un vecteur
 *
 * @param v
 * @return bool
 */
bool Octree::collisionVertexDansAabb(const QVector3D &v, double coef)
{
    if (!pointDansAabb(v))
        return false;

    if (fils_[0]!=0)
    {
        for (int i = 0; i<8; ++i)
            if (fils_[i]->collisionVertexDansAabb(v, coef))
                return true;
    }
    else
    {
        std::set<Vertex*>::iterator it;
        for (it=vertices_.begin();it!=vertices_.end();++it)
            if (Geometrie::distance(v, *(*it)) < coef)
                return true;
    }
    return false;
}

/**
 * Test la collision entre deux octree
 *
 * @param octree2
 * @param mesh1
 * @param mesh2
 * @return std::vector<int>
 */
std::vector<int> Octree::collision(Octree *octree2, Mesh *mesh1, Mesh *mesh2)
{
    float rayon1 = Geometrie::distance(min_, max_)/2.f;
    QVector3D centre1 = mesh1->getTransformation()*getCentre();
    float rayon2 =  Geometrie::distance(octree2->getMin(), octree2->getMax())/2.f;
    QVector3D centre2 = mesh2->getTransformation()*octree2->getCentre();
    std::vector<int> verticesDansNoeud;
    std::vector<int> vertices;

    if (rayon1+rayon2 >= Geometrie::distance(centre1, centre2))
    {
        if (fils_[0]!=0)
        {
            for (int i=0;i<8;i++)
            {
                std::vector<int> iVerts = fils_[i]->collision(octree2, mesh1, mesh2);
                verticesDansNoeud.insert(verticesDansNoeud.end(), iVerts.begin(), iVerts.end());
            }
            Outil::ranger(verticesDansNoeud);
            return verticesDansNoeud;
        }
        else
        {
            std::set<Vertex*>::iterator it;
            for (it=vertices_.begin();it!=vertices_.end();++it)
            {
                vertices.push_back((*it)->getId());
            }
        }
    }
    return vertices;
}

/**
 * Met à jour récursivement
 * les noeuds fils de l'octree
 *
 */
void Octree::majNoeuds()
{
    if (fils_[0] != 0)
    {
        QVector3D deltaX((max_.x()-min_.x())/2, 0.f, 0.f);
        QVector3D deltaY(0.f, (max_.y()-min_.y())/2, 0.f);
        QVector3D deltaZ(0.f, 0.f, (max_.z()-min_.z())/2);
        QVector3D milieu=(min_+max_)/2;

        fils_[0]->setMin(min_);
        fils_[0]->setMax(milieu);
        fils_[1]->setMin(min_+deltaX);
        fils_[1]->setMax(milieu+deltaX);
        fils_[2]->setMin(milieu-deltaY);
        fils_[2]->setMax(max_-deltaY);
        fils_[3]->setMin(min_+deltaZ);
        fils_[3]->setMax(milieu+deltaZ);
        fils_[4]->setMin(min_+deltaY);
        fils_[4]->setMax(milieu+deltaY);
        fils_[5]->setMin(milieu-deltaZ);
        fils_[5]->setMax(max_-deltaZ);
        fils_[6]->setMin(milieu);
        fils_[6]->setMax(max_);
        fils_[7]->setMin(milieu-deltaX);
        fils_[7]->setMax(max_-deltaX);

        for (int i =0; i<8 ;++i)
            fils_[i]->majNoeuds();
    }
}
