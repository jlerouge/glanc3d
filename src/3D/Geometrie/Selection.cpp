#include "Selection.h"

QVector3D Selection::vertexIntersection_ = QVector3D();

/**
 * Constructeur de Selection
 *
 */
Selection::Selection() : mesh_(0), triangleSelectionne_(-1), verticesSelectionnes_(),
    tailleZone_(0.5), modeCylindre_(false), modePoint_(false), modeFusion_(false),
    mode_(MODE_DEFORMATION), selectionEnCours_(false), ajoutFini_(false), meshSelection_(0),
    formeLibre_(0), distanceMax_(0), indiceCentre_(-1)
{}

/**
 * Destructeur de Selection
 *
 */
Selection::~Selection()
{
    delete meshSelection_;
    delete formeLibre_;
}

/**
 * Fixe la valeur de mesh_
 *
 * @param mesh
 */
void Selection::setMesh(Mesh *mesh)
{
    mesh_=mesh;
}

/**
 * Renvoie la valeur de mesh_
 *
 * @return Mesh
 */
Mesh* Selection::getMesh()
{
    return mesh_;
}

/**
 * Renvoie la valeur de triangleSelectionne_
 *
 * @return int
 */
int Selection::getTriangleSelectionne()
{
    return triangleSelectionne_;
}

/**
 * Renvoie la valeur de verticesSelectionnes_
 *
 * @return std::vector<int>
 */
std::vector<int>& Selection::getVerticesSelectionnees()
{
    return verticesSelectionnes_;
}

/**
 * Fixe la valeur de tailleZone_
 *
 * @param  tailleZone
 */
void Selection::setTailleZone(float tailleZone)
{
    tailleZone_=tailleZone;
}

/**

 * Retourne le mesh sélectionné
 * @retun Mesh*
 */
Mesh* Selection::getMeshSelection()
{
    return meshSelection_;
}

/**
 * Retourne si un ajout est fini ou non
 * @return bool

 */
bool Selection::getAjoutFini()
{
    return ajoutFini_;
}

/**
 * Modifie la valeur de la variable ajoutFini_ avec celle passée en paramètre
 * @param estFini:bool
 */
void Selection::setAjoutFini(bool estFini)
{
    ajoutFini_ = estFini;
}

/**
 * Retourne si une sélection est en cours ou non
 * @return bool
 */
bool Selection::getSelectionEnCours()
{
    return selectionEnCours_;
}

/**

 * Retourne la forme libre
 * @return Forme2D*
 */
Forme2D* Selection::getFormeLibre()
{
    return formeLibre_;
}

/**
 * Fixe la valeur de mode_
 *
 * @param mode
 */
void Selection::setMode(ModeSelection mode)
{
    mode_ = mode;
}

/**
 * Renvoie la valeur de mode_
 *
 * @return ModeSelection
 */
Selection::ModeSelection Selection::getMode()
{
    return mode_;
}

/**
 * A partir d'un ensemble de mesh, trouve le mesh ainsi que le triangle qui est
 * traversé par le rayon écran-souris calculé à  partir de la position de la souris,
 * détermine aussi les vertices sélectionnés
 *
 * @param meshes
 * @param positionSouris
 * @param tailleFenetre
 */
void Selection::setVerticesSelectionnees(std::vector<Mesh*> &meshes,
                                         const QPointF &positionSouris,
                                         const QSizeF &tailleFenetre)
{
    QVector3D vertexPres, vertexLoin;
    Geometrie::calculerRayonEcranSouris(positionSouris, tailleFenetre, vertexPres, vertexLoin);
    intersectionRayonMeshes(meshes, vertexPres, vertexLoin);
    if (mesh_)
    {
        if (mode_ != MODE_POINT && mode_ != MODE_FUSION)
            setVerticesDansZoneSelection();
        else
            setVertexSelectionne();
    }


}

/**
 * A partir d'un ensemble de mesh, trouve le mesh ainsi que le triangle
 * qui est traversé par un rayon
 *
 * @param meshes
 * @param vertSouris1
 * @param vertSouris2
 */
void Selection::intersectionRayonMeshes(std::vector<Mesh*> &meshes,
                                        const QVector3D &vertSouris1,
                                        const QVector3D &vertSouris2)
{
    if (mesh_)
        mesh_->setSelectionne(false);
    mesh_=0;
    triangleSelectionne_=-1;
    std::vector<Mesh*>::iterator it;
    float min = F_MIN_INIT;
    for (it=meshes.begin(); it!=meshes.end(); ++it)
    {
        float distance;
        int iTri=intersectionRayonMesh(*it, (*it)->getTransformationInverse()*vertSouris1,
                                       (*it)->getTransformationInverse()*vertSouris2, distance);
        if (iTri!=-1)
        {
            if (distance<min)
            {
                min=distance;
                mesh_=*it;
                triangleSelectionne_=iTri;
            }
        }
    }

    if (mesh_)
        mesh_->setSelectionne(true);
}

/**
 * Renvoie l'indice triangle d'un mesh traversé par un lancer de rayon
 * (si le rayon ne touche pas le mesh, alors renvoie -1).
 * Calcule également la distance entre le triangle vé et le vertex 1 du rayon
 *
 * @param mesh
 * @param vertexPres
 * @param vertexLoin
 * @param distance
 * @return int
 */
int Selection::intersectionRayonMesh(Mesh *mesh, const QVector3D &vertexPres,
                                     const QVector3D &vertexLoin, float &distance)
{
    std::vector<int> iVerts=mesh->getOctree()->getVerticesIntersectionRayon(
                vertexPres, (vertexLoin-vertexPres).normalized());
    std::vector<int> iTrisCandidats = mesh->getTrianglesAPartirDeVertices(iVerts);
    distance = F_MIN_INIT;
    int nbTrisCandidats = iTrisCandidats.size();
    int triSelect = -1;
    for (int i=0;i<nbTrisCandidats;i++)
    {
        QVector3D vertInter;
        Triangle &t=mesh->getTriangle(iTrisCandidats[i]);
        if (Geometrie::intersectionTriangle(vertexPres, vertexLoin,
                                            mesh->getVertex(t.getIndiceVertex(1)),
                                            mesh->getVertex(t.getIndiceVertex(2)),
                                            mesh->getVertex(t.getIndiceVertex(3)),
                                            t.getNormale(), vertInter))
        {
            float testDistance=Geometrie::distance(vertexPres, vertInter);
            if (testDistance<distance) //on prend le triangle le plus proche de nous
            {
                distance=testDistance;
                triSelect=iTrisCandidats[i];
                vertexIntersection_=vertInter;
            }
        }
    }
    return triSelect;
}

/**
 *  Renvoie la valeur de Octree de verticesIntersectionCylindre d'un mesh traversé par un
 *  lancer de rayon (si le rayon ne touche pas le mesh, alors renvoie -1).
 *
 * @param mesh
 * @param vertexPres
 * @param vertexLoin
 * @param rayon
 * @return std::vector<int>
 */
std::vector<int> Selection::intersectionCylindreMesh(Mesh *mesh, const QVector3D &vertexPres,
                                                     const QVector3D &vertexLoin, float rayon)
{
    return mesh->getOctree()->getVerticesIntersectionCylindre(
                vertexPres, (vertexLoin-vertexPres).normalized(), rayon);
}

/**
 * Détermine le pointélecténn en mode pà  t   point
 *
 */
void Selection::setVertexSelectionne()
{
    verticesSelectionnes_.clear();
    Vertex &v1 = mesh_->getVertex(mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(1));
    Vertex &v2 = mesh_->getVertex(mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(2));
    Vertex &v3 = mesh_->getVertex(mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(3));
    if (Geometrie::distance(v1, vertexIntersection_)<Geometrie::distance(v2, vertexIntersection_))
    {
        if (Geometrie::distance(v1, vertexIntersection_)<Geometrie::distance(v3,
                                                                             vertexIntersection_))
            verticesSelectionnes_.push_back(
                        mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(1));
        else
            verticesSelectionnes_.push_back(
                        mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(3));
    }else if (Geometrie::distance(v2, vertexIntersection_)<
              Geometrie::distance(v3, vertexIntersection_))
    {
        verticesSelectionnes_.push_back(
                    mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(2));
    }else
    {
        verticesSelectionnes_.push_back(
                    mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(3));
    }

    indiceCentre_ =  verticesSelectionnes_[0];
}

/**
 * Détermine les triangles selectionnés par la zone de sélection
 *
 */
void Selection::setVerticesDansZoneSelection()
{
    QVector3D centreSelect=mesh_->getCentreTriangle(triangleSelectionne_);
    verticesSelectionnes_.clear();
    Vertex &vert=mesh_->getVertex(mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(1));
    std::set<int> vertsSelectionne;
    vertsSelectionne.insert(mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(2));
    propagationZoneSelection(centreSelect, vert, vertsSelectionne);
    verticesSelectionnes_=std::vector<int>(vertsSelectionne.begin(), vertsSelectionne.end());

    QVector3D centre = mesh_->getVertex(verticesSelectionnes_[0]);
    QVector3D vec(0, 0, 0);
    for (uint i =0; i < verticesSelectionnes_.size() ; i++)
    {
        vec = mesh_->getVertex(verticesSelectionnes_[i]);
        if (Geometrie::distance(centre, vec) > distanceMax_)
            distanceMax_ = Geometrie::distance(centre, vec);
    }

    Vertex &v1 = mesh_->getVertex(mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(1));
    Vertex &v2 = mesh_->getVertex(mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(2));
    Vertex &v3 = mesh_->getVertex(mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(3));
    if (Geometrie::distance(v1, vertexIntersection_) <
            Geometrie::distance(v2, vertexIntersection_))
    {
        if (Geometrie::distance(v1, vertexIntersection_) <
                Geometrie::distance(v3, vertexIntersection_))
            indiceCentre_ =
                    mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(1);
        else
            indiceCentre_ = mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(3);
    }
    else if (Geometrie::distance(v2, vertexIntersection_) <
             Geometrie::distance(v3, vertexIntersection_))
        indiceCentre_ = mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(2);
    else
        indiceCentre_ = mesh_->getTriangle(triangleSelectionne_).getIndiceVertex(3);
}

/**
 * Détermine les triangles selectionnés par la zone de sélection récursivement
 *
 * @param centreSelect
 * @param vertex
 * @param vertsSelect
 */
void Selection::propagationZoneSelection(const QVector3D &centreSelect, const Vertex &vert,
                                         std::set<int> &vertsSelect)
{
    if (Geometrie::distance(centreSelect, vert) < tailleZone_)
    {
        if (vertsSelect.insert(vert.getId()).second)
        {
            std::vector<int> verticesLies = mesh_->
                    getVerticesAPartirDeTriangles(vert.getIndicesTriangles());
            int nbVerticesLies = verticesLies.size();
            for (int i=0; i<nbVerticesLies; i++)
                propagationZoneSelection(
                            centreSelect, mesh_->getVertex(verticesLies[i]), vertsSelect);
        }
    }
}


/**
 * Détermine Les vertices selectionnés par la projection d'un cylindre sur un mesh
 *
 * @param mesh
 * @param positionSouris
 * @param tailleFenetre
 * @param rayon
 */
void Selection::setVerticesSelectionneesDansCylindre(Mesh *mesh, const QPointF &positionSouris,
                                                     const QSizeF &tailleFenetre, float rayon)
{
    QVector3D vertexPres, vertexLoin;
    Geometrie::calculerRayonEcranSouris(positionSouris, tailleFenetre, vertexPres, vertexLoin);
    mesh_ = mesh;
    verticesSelectionnes_ = intersectionCylindreMesh(
                mesh_, mesh_->getTransformationInverse()*vertexPres,
                mesh_->getTransformationInverse()*vertexLoin, rayon);
}


/**
 * Ajoute des points à la selection libre
 *
 * @param positionSouris:QPointF, tailleFenetre:QSizeF
 */
void Selection::selectionLibre(const QPointF positionSouris, const QSizeF &tailleFenetre)
{
    if (selectionEnCours_)
    {
        if (!polygoneLibre_.empty())
        {
            if (Geometrie::distance2D(polygoneLibre_.back(), positionSouris) > 8)
                polygoneLibre_.push_back(positionSouris);
        }
        else
            polygoneLibre_.push_back(positionSouris);
    }
}


/**
 * Initialise le début d'une sélection libre
 */
void Selection::debuterSelection()
{
    selectionEnCours_ = true;
    if (!polygoneLibre_.empty())
        polygoneLibre_.clear();

    if (!verticesFormeLibre_.empty())
        verticesFormeLibre_.clear();
}

/**
 * Termine une sélection libre
 * @param tailleFenetre:QSizeF
 */
void Selection::finirSelection(const QSizeF &tailleFenetre)
{
    if (polygoneLibre_.size()%2 == 0)
        polygoneLibre_.pop_back();

    if (polygoneLibre_.size() > 1)
    {
        polygoneLibre_.push_back(polygoneLibre_.front());

        if (mesh_)
        {
            std::vector<Vertex> &vertices=mesh_->getVertices();
            int nbVertices=vertices.size();
            for (int i=0;i<nbVertices;i++)
            {
                if (Geometrie::insidePolygon(
                            polygoneLibre_, polygoneLibre_.size(), Geometrie::point3Dto2D(
                                mesh_->getTransformation()*vertices[i], tailleFenetre)))
                    verticesFormeLibre_.push_back(vertices[i].getId());
            }
            meshSelection_ = new Mesh(verticesFormeLibre_, mesh_);
            formeLibre_ = new Forme2D(polygoneLibre_, tailleFenetre);
            formeLibre_->reduireForme();
        }
    }
    selectionEnCours_ = false;
    ajoutFini_ = true;
}

/**
 * Dessine la sélection libre
 */
void Selection::dessinerPolygone()
{
    glLineWidth(3.f);
    glColor3ub(0, 50, 220);
    glBegin(GL_LINES);
    std::vector<QPointF>::iterator it;
    for (it = polygoneLibre_.begin(); it != polygoneLibre_.end(); ++it)
        glVertex3f((*it).x(), (*it).y(), 1);
    glEnd();
    glLineWidth(1.f);
}

/** Efface le polygone de sélection */
void Selection::razPolygone()
{
    polygoneLibre_.clear();
}

/**
 * Dessine la zone de sélection
 *
 */
void Selection::dessiner()
{
    if (mesh_!=0 && !(verticesSelectionnes_.empty()))
    {
        glPushMatrix();
        QMatrix4x4 mat=mesh_->getTransformation();
        float *dataMat = mat.data();
        GLfloat matriceArray[16];
        for (int i= 0; i < 16; i++)
            matriceArray[i] = dataMat[i];
        glMultMatrixf(matriceArray);
        glDisable(GL_LIGHTING);

        float decalage=0.001; //petit decalage par rapport
        //a la normale pour faire la différence avec le mesh
        if (mode_ == MODE_DEFORMATION)
        {
            std::vector<int> trianglesSelect=mesh_->getTrianglesAPartirDeVertices(
                        verticesSelectionnes_);
            int nbTriangles=trianglesSelect.size();
            QVector3D normale=mesh_->getTriangle(triangleSelectionne_).getNormale();
            for (int i=0;i<nbTriangles;i++)
            {
                Triangle &tri=mesh_->getTriangle(trianglesSelect[i]);
                Vertex &v1=mesh_->getVertex(tri.getIndiceVertex(1));
                Vertex &v2=mesh_->getVertex(tri.getIndiceVertex(2));
                Vertex &v3=mesh_->getVertex(tri.getIndiceVertex(3));
                if (trianglesSelect[i]==triangleSelectionne_)
                    glBegin(GL_TRIANGLES);
                else
                    glBegin(GL_LINE_LOOP);
                glColor3ub(255, 0, 0);
                glVertex3f(v1.x()+normale.x()*decalage,
                           v1.y()+normale.y()*decalage,
                           v1.z()+normale.z()*decalage);
                glVertex3f(v2.x()+normale.x()*decalage,
                           v2.y()+normale.y()*decalage,
                           v2.z()+normale.z()*decalage);
                glVertex3f(v3.x()+normale.x()*decalage,
                           v3.y()+normale.y()*decalage,
                           v3.z()+normale.z()*decalage);
                glEnd();
            }
        }
        else if (mode_ == MODE_POINT || mode_ == MODE_FUSION)
        {
            //petit decalage par rapport a la normale pour faire la différence avec le mesh
            Vertex &v=mesh_->getVertex(verticesSelectionnes_[0]);
            QVector3D normale=v.getNormale();
            glPointSize(10.f);
            glBegin(GL_POINTS);
            glColor3ub(0, 0, 255);
            glVertex3f(v.x()+normale.x()*decalage,
                       v.y()+normale.y()*decalage,
                       v.z()+normale.z()*decalage);
            glEnd();
        }
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }
}

/**
 * Renvoie la distance maximale
 * @return  float
 */
float Selection::getDistanceMax()
{
    return distanceMax_;
}

/**
 * Renvoie l'indice du centre
 * @return int
 */
int Selection::getIndiceCentre()
{
    return indiceCentre_;
}
