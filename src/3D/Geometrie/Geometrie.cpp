#include "Geometrie.h"
#include "Mesh.h"

/**
 * Renvoie la distance entre 2 vertices
 *
 * @param v1
 * @param v2
 * @return float
 */
float Geometrie::distance(const QVector3D &v1, const QVector3D &v2)
{
    return fabsf((float)(v2-v1).length());
}

/**
 * Renvoie l'angle (non signé) entre deux vecteurs
 *
 * @param v1
 * @param v2
 * @return float
 */
float Geometrie::angle(const QVector3D &v1, const QVector3D & v2)
{
    QVector3D v1norm, v2norm;
    v1norm = v1.normalized();
    v2norm = v2.normalized();
    return acos(QVector3D::dotProduct(v1norm, v2norm));
}

/**
 * Renvoie true si un segment traverse un triangle défini par trois vertices.
 * Calcule également le vertex d'intersection.
 *
 * @param s1
 * @param s2
 * @param v1
 * @param v2
 * @param v3
 * @param normale
 * @param vertInter
 * @return bool
 */
bool Geometrie::intersectionTriangle(const QVector3D &s1, const QVector3D &s2,
                                     const QVector3D &v1, const QVector3D &v2,
                                     const QVector3D &v3, const QVector3D &normale,
                                     QVector3D &vertInter)
{
    float dist1 = QVector3D::dotProduct(s1-v1, normale);
    float dist2 = QVector3D::dotProduct(s2-v1, normale);
    if ((dist1*dist2)>= 0.0f)
        return false;
    if (dist1==dist2)        //rayon parallele au plan du triangle
        return false;
    //point d'intersection entre le plan du triangle et le rayon
    vertInter = s1+(s2-s1)*(-dist1/(dist2-dist1));
    //verification si intersection est a l'interieur du triangle avec les bords
    if (QVector3D::dotProduct(QVector3D::crossProduct(normale, v2-v1), vertInter-v1)<0.0f)
        return false;
    if (QVector3D::dotProduct(QVector3D::crossProduct(normale, v3-v2), vertInter-v2)<0.0f)
        return false;
    if (QVector3D::dotProduct(QVector3D::crossProduct(normale, v1-v3), vertInter-v1)<0.0f)
        return false;
    return true;
}

/**
 * Projette les coordonnées souris sur une sphère de centre (0, 0, 0)
 *
 * @param positionSouris
 * @return QVector3D
 */
QVector3D Geometrie::sourisSurSphereUnite(const QPointF &positionSouris)
{
    float tempZ = 1-positionSouris.x()*positionSouris.x()-positionSouris.y()*positionSouris.y();
    float positionSourisZ= tempZ>0 ? sqrt(tempZ) : 0;
    QVector3D sourisSphere(positionSouris.x(), positionSouris.y(), positionSourisZ);
    return sourisSphere.normalized();
}

/**
 * Normalise les coordonnées souris entre -1 et 1
 *
 * @param tailleFenetre
 * @param positionSouris
 * @return QPointF
 */
QPointF Geometrie::sourisNormalisee(const QPointF &positionSouris, const QSizeF tailleFenetre)
{
    return QPointF(2.0*positionSouris.x()/tailleFenetre.width()-1.0,
                   1.0-(2.0*positionSouris.y()/tailleFenetre.height()));
}


/**
 * Calcule les coordonnees des vertices du rayon écran-souris (un vertex situe sur la camera
 * et l'autre a l'horizon en passant par la souris)
 *
 * @param positionSouris
 * @param tailleFenetre
 * @param vertexPres
 * @param vertexLoin
 */
void Geometrie::calculerRayonEcranSouris(const QPointF &positionSouris,
                                         const QSizeF &tailleFenetre,
                                         QVector3D &vertexPres, QVector3D &vertexLoin)
{
    double modele[16];
    double projection[16];
    int vue[4];
    double dX, dY, dZ;
    glGetIntegerv(GL_VIEWPORT, vue);
    glGetDoublev (GL_MODELVIEW_MATRIX, modele);
    glGetDoublev (GL_PROJECTION_MATRIX, projection);
    gluUnProject (positionSouris.x(), tailleFenetre.height()-positionSouris.y(),
                  0.0, modele, projection, vue, &dX, &dY, &dZ);
    vertexPres.setX((float)dX);
    vertexPres.setY((float)dY);
    vertexPres.setZ((float)dZ);
    gluUnProject (positionSouris.x(), tailleFenetre.height()-positionSouris.y(),
                  1.0, modele, projection, vue, &dX, &dY, &dZ);
    vertexLoin.setX((float)dX);
    vertexLoin.setY((float)dY);
    vertexLoin.setZ((float)dZ);
}


/**
 * Calcule le vecteur du rayon écran-plan caméra entre deux coordonnées de souris à   l'écran
 *
 * @param positionSourisOld
 * @param positionSourisNew
 * @param tailleFenetre
 * @return QVector3D
 */
QVector3D Geometrie::calculerVecteurEcranPlanCamera(const QPointF &positionSourisOld,
                                                    const QPointF &positionSourisNew,
                                                    const QSizeF &tailleFenetre)
{
    QVector3D vertSouris1, vertSouris2;
    double modele[16];
    double projection[16];
    int vue[4];
    double dX, dY, dZ;
    glGetIntegerv(GL_VIEWPORT, vue);
    glGetDoublev (GL_MODELVIEW_MATRIX, modele);
    glGetDoublev (GL_PROJECTION_MATRIX, projection);
    gluUnProject (positionSourisOld.x(), tailleFenetre.height()-positionSourisOld.y(),
                  0.0, modele, projection, vue, &dX, &dY, &dZ);
    vertSouris1.setX((float)dX);
    vertSouris1.setY((float)dY);
    vertSouris1.setZ((float)dZ);
    gluUnProject (positionSourisNew.x(), tailleFenetre.height()-positionSourisNew.y(),
                  0.0, modele, projection, vue, &dX, &dY, &dZ);
    vertSouris2.setX((float)dX);
    vertSouris2.setY((float)dY);
    vertSouris2.setZ((float)dZ);
    return (vertSouris2-vertSouris1).normalized();
}

/**
 * Renvoie les coordonnées 3D de la projection d'un point 2D au niveau de écran pour un z donné
 *
 * @param positionSouris
 * @param tailleFenetre
 * @param z
 * @return QVector3D
 */
QVector3D Geometrie::point2Dto3D(const QPointF &positionSouris,
                                 const QSizeF &tailleFenetre,
                                 float z)
{
    double modele[16];
    double projection[16];
    int vue[4];
    double dX, dY, dZ;
    QVector3D vertex;
    glGetIntegerv(GL_VIEWPORT, vue);
    glGetDoublev (GL_MODELVIEW_MATRIX, modele);
    glGetDoublev (GL_PROJECTION_MATRIX, projection);
    gluUnProject (positionSouris.x(), tailleFenetre.height()-positionSouris.y(), z,
                  modele, projection, vue, &dX, &dY, &dZ);
    vertex.setX((float)dX);
    vertex.setY((float)dY);
    vertex.setZ((float)dZ);
    return vertex;
}

/**
 * Calcule le vertex placé sur un rayon le plus proche d'un autre vertex
 *
 * @param vertex
 * @param vertexPres
 * @param vertexLoin
 * @return QVector3D
 */
QVector3D Geometrie::vertexSurRayonLePlusProche(const QVector3D &vertex,
                                                const QVector3D &vertexPres,
                                                const QVector3D &vertexLoin)
{
    QVector3D ab = vertexLoin-vertexPres;
    float abCarre = ab.lengthSquared();
    float dot = QVector3D::dotProduct(ab, (vertex - vertexPres));
    float t = dot / abCarre;
    return (vertexPres + ab*t);
}


/**
 * Renvoie la distance entre un ensemble de vertices et un mesh
 *
 * @param mesh
 * @param vertices
 * @param matTrans
 * @return float
 */
float Geometrie::distanceMeshVertices(Mesh *mesh, const std::vector<QVector3D> &vertices,
                                      const QMatrix4x4 &matTrans)
{
    float distance=0;
    int nbVertices=vertices.size();
    for (int i=0;i<nbVertices;i++)
    {
        QVector3D vert=mesh->getTransformationInverse()*matTrans*vertices[i];
        QVector3D vProche=*(mesh->getOctree()->getVertexLePlusProche(vert));
        distance+=Geometrie::distance(vProche, vert);
    }
    return distance;
}

/**
 * Renvoie la distance entre un ensemble de vertices
 * et un mesh en prennant compte des transformations
 *
 * @param mesh
 * @param vertices
 * @param matTrans
 * @return float
 */
float Geometrie::distanceMeshVerticesTrans(Mesh *mesh, const std::vector<QVector3D> &vertices,
                                           const QMatrix4x4 &matTrans)
{
    float distance=0;
    int nbVertices=vertices.size();
    for (int i=0;i<nbVertices;i++)
    {
        QVector3D vert = matTrans*vertices[i];
        QVector3D vProche=*(mesh->getOctree()->
                            getVertexLePlusProche(mesh->getTransformationInverse()*vert));
        distance += Geometrie::distance(mesh->getTransformation()*vProche, vert);
    }
    return distance;
}
/**
 * Calcule un vecteur perpendiculaire à   un autre vecteur
 *
 * @param vec
 * @return QVector3D
 */
QVector3D Geometrie::getVecteurPlan(const QVector3D &vec)
{
    QVector3D resultat;
    if (vec.x() == 0.0f || vec.y() == 0.0f || vec.z() == 0.0f)
    {
        if (vec.x() == 0.0f)
            resultat.setX(1.0f);
        else if (vec.y() == 0.0f)
            resultat.setY(1.0f);
        else
            resultat.setZ(1.0f);
    }
    else
    {
        resultat.setX(vec.z());
        resultat.setY(vec.z());
        resultat.setZ(-(vec.x()+vec.y()));
        resultat.normalize();
    }
    return resultat;
}

/**
 * Transforme un point 3D en point 2D
 *
 * @param event:QVector3D, tailleFenetre:QZiseF

 * @return QPointF
 */
QPointF Geometrie::point3Dto2D(QVector3D event, const QSizeF &tailleFenetre)
{


    GLdouble winx, winy, winz;
    double modele[16];
    double projection[16];
    int vue[4];
    glGetIntegerv(GL_VIEWPORT, vue);
    glGetDoublev (GL_MODELVIEW_MATRIX, modele);
    glGetDoublev (GL_PROJECTION_MATRIX, projection);

    gluProject(event.x(), event.y(), event.z(), modele, projection, vue, &winx, &winy, &winz);
    return QPointF(winx, tailleFenetre.height()-winy);
}

/**
 * Transforme un point 3D en forme 2D
 *
 * @param event:QVector3D, forme:Forme2D
 * @return QPointF
 */
QPointF Geometrie::point3Dto2DForme(QVector3D event, Forme2D *forme)
{
    GLdouble winx, winy, winz;
    GLdouble modele[16];
    GLdouble projection[16];
    GLint vue[4];

    forme->getModele(modele);
    forme->getProjection(projection);
    forme->getVue(vue);

    gluProject(event.x(), event.y(), event.z(), modele, projection, vue, &winx, &winy, &winz);
    return QPointF(winx, forme->getTailleFenetre().height()-winy);
}

/**
 * Test si un point est à   l'inérieur d'un polygone
 *
 * @param polygon, N:int, p:QPointF
 * @return int
 */
int Geometrie::insidePolygon(std::vector<QPointF> polygon, int N, QPointF p)
{
    int counter = 0;
    int i;
    double xinters;
    QPointF p1, p2;

    p1 = polygon[0];
    for (i=1;i<=N;i++)
    {
        p2 = polygon[i % N];
        if (p.y() > std::min(p1.y(), p2.y()))
            if (p.y() <= std::max(p1.y(), p2.y()))
                if (p.x() <= std::max(p1.x(), p2.x()))
                    if (p1.y() != p2.y())
                    {
                        xinters = (p.y()-p1.y())*(p2.x()-p1.x())/(p2.y()-p1.y())+p1.x();
                        if (p1.x() == p2.x() || p.x() <= xinters)
                            counter++;
                    }
        p1 = p2;
    }

    if (counter % 2 == 0)
        return 0;
    else
        return 1;
}

/**
 * Retourne la distance entre deux points
 *
 * @param a:QPointF, b:QPointF
 * @return float
 */
float Geometrie::distance2D(const QPointF a, QPointF b)
{
    return sqrt((b.x()-a.x())*(b.x()-a.x())+(b.y()-a.y())*(b.y()-a.y()));
}

/**
 * Test la collision entre deux mesh
 *
 * @param Mesh*:mesh1, Mesh*:mesh2, std::vector<int> vertices1
 * @return bool
 */
bool Geometrie::collisionMesh(Mesh *mesh1, Mesh *mesh2)
{
    std::vector<int> vertices1 = mesh1->getOctree()->collision(mesh2->getOctree(), mesh1, mesh2);
    QVector3D vec;
    for (uint i=0; i<vertices1.size(); ++i)
    {
        vec = mesh2->getTransformationInverse()*mesh1->getTransformation()*
                mesh1->getVertex(vertices1[i]);
        if (mesh2->getOctree()->collisionVertexDansAabb(vec))
        {
            return true;
        }
    }
    return false;
}

/**
 * Calcule la distance courbe d'un mesh entre deux vertices
 * dont les indices sont passés en paramètres
 *
 * @param Mesh*:mesh, int:indice1, int:indice2
 * @return float
 */
float Geometrie::distanceCourbe(Mesh *mesh, int indice1, int indice2)
{
    std::vector<int> vertices;
    vertices.push_back(indice1);
    vertices.push_back(indice2);
    float longuOld = 1.0f;
    float longu = 0.0f;
    float seuil = 0.5f;
    int nbIter = 0;

    while (fabs(longuOld - longu) > seuil && nbIter < 5)
    {
        longuOld = longu;
        longu = 0;
        nbIter++;
        for (uint i=0; i<vertices.size()-1; i+=2)
        {
            subdiviser(mesh, vertices, i);
            longu += (mesh->getVertex(vertices[i]) - mesh->getVertex(vertices[i+1])).length();
            longu += (mesh->getVertex(vertices[i+1]) - mesh->getVertex(vertices[i+2])).length();
        }
    }
    return longu;
}

/**
 * A partir des indices de deux vertices, on détermine le vertex situé
 * à la moitié du vecteur formé par les deux premiers et on trouve le vertex
 * du mesh le plus près
 *
 * @param mesh
 * @param vertices
 * @param i
 */
void Geometrie::subdiviser(Mesh *mesh, std::vector<int> &vertices, int i)
{
    QVector3D milieu = (mesh->getVertex(vertices[i]) + mesh->getVertex(vertices[i+1]))/2;
    Vertex *vertexProche = mesh->getOctree()->getVertexLePlusProche(milieu);
    vertices.insert(vertices.begin()+i, vertexProche->getId());
}
