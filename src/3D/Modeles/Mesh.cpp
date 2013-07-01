#include "Mesh.h"
#include <iostream>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

/**
 * Constructeur de Mesh.
 *
 */
Mesh::Mesh():nbTriangles_(0), nbVertices_(0),
    verticesBuffer_(QGLBuffer::VertexBuffer),
    normalesBuffer_(QGLBuffer::VertexBuffer),
    /*couleursBuffer_(QGLBuffer::VertexBuffer), */
    indicesBuffer_(QGLBuffer::IndexBuffer), centre_(),
    shaderType_(PHONG), programmeShader_(), couleur_(200, 150, 120),
    octree_(0), affichageOctree_(false),
    matTransform_(), matTransformArray_(), estSelectionne_(false), estEditable_(true)
{
    majTransformation();  // Transformation identité pour le début.
    octree_=new Octree();
}

/**
 * Constructeur de Mesh.
 *
 * @param verticesSelectionnes
 * @param mesh
 */
Mesh::Mesh(std::vector<int> verticesSelectionnes, Mesh *mesh):nbTriangles_(0),
    nbVertices_(0), verticesBuffer_(QGLBuffer::VertexBuffer),
    normalesBuffer_(QGLBuffer::VertexBuffer),
    /*couleursBuffer_(QGLBuffer::VertexBuffer), */
    indicesBuffer_(QGLBuffer::IndexBuffer), centre_(),
    shaderType_(PHONG), programmeShader_(), couleur_(200, 150, 120),
    octree_(0), affichageOctree_(false),
    matTransform_(), matTransformArray_(), estSelectionne_(false), estEditable_(true)
{
    majTransformation();
    octree_=new Octree();
    std::vector<Vertex> &vertices=mesh->getVertices();
    std::vector<Triangle> &triangles=mesh->getTriangles();
    std::vector<int> trianglesSelectionnes=mesh->getTrianglesAPartirDeVertices(
                verticesSelectionnes);
    nbTriangles_=trianglesSelectionnes.size();
    Vertex v1, v2, v3;
    int iVer1, iVer2, iVer3;
    std::set<Vertex> setVertices;
    for (int i=0; i<nbTriangles_; i++)
    {
        v1=Vertex(vertices[triangles[trianglesSelectionnes[i]].getIndiceVertex(1)],
                  setVertices.size());
        iVer1=rajouterVertexSansDoublons(v1, i, setVertices);
        v2=Vertex(vertices[triangles[trianglesSelectionnes[i]].getIndiceVertex(2)],
                  setVertices.size());
        iVer2=rajouterVertexSansDoublons(v2, i, setVertices);
        v3=Vertex(vertices[triangles[trianglesSelectionnes[i]].getIndiceVertex(3)],
                  setVertices.size());
        iVer3=rajouterVertexSansDoublons(v3, i, setVertices);
        triangles_.push_back(Triangle(triangles[trianglesSelectionnes[i]].getNormale(),
                                      iVer1, iVer2, iVer3, i));
    }
    nbVertices_=vertices_.size();
    deplacerEn(QVector3D(0, 0, 0));
    initDistanceVertexMaxEtCentreMeshEtOctree();
    initVBO();
}

/**
 * Destructeur de Mesh.
 *
 */
Mesh::~Mesh()
{
    delete octree_;
    vertices_.clear();
    triangles_.clear();
}

/**
 * Renvoie la valeur de vertices_.
 *
 * @return std::vector<Vertex>
 */
std::vector<Vertex>& Mesh::getVertices()
{
    return vertices_;
}

/**
 * Renvoie la valeur de triangles_.
 *
 * @return std::vector<Triangle>
 */
std::vector<Triangle>& Mesh::getTriangles()
{
    return triangles_;
}

/**
 * Renvoie la valeur du ième vertex de vertices_.
 *
 * @param i
 * @return Vertex
 */
Vertex& Mesh::getVertex(int i)
{
    return vertices_[i];
}

/**
 * Renvoie la valeur du ième triangle de triangles_.
 *
 * @param i
 * @return Triangle
 */
Triangle& Mesh::getTriangle(int i)
{
    return triangles_[i];
}

/**
 * Renvoie la valeur de nbTriangles_.
 *
 * @return int
 */
int Mesh::getNbTriangles() const
{
    return nbTriangles_;
}

/**
 * Fixe la valeur de nbVertices_.
 *
 * @param nbVertices
 */
void Mesh::setNbVertices(int nbVertices)
{
    nbVertices_=nbVertices;
}

/**
 * Fixe la valeur de nbTriangles_.
 *
 * @param nbTriangles
 */
void Mesh::setNbTriangles(int nbTriangles)
{
    nbTriangles_=nbTriangles;
}

/**
 * Renvoie la valeur de nbVertices_.
 *
 * @return int
 */
int Mesh::getNbVertices() const
{
    return nbVertices_;
}

/**
 * Renvoie la valeur de centre_.
 *
 * @return QVector3D
 */
QVector3D Mesh::getCentre() const
{
    return centre_;
}

/**
 * Renvoie la valeur de octree_.
 *
 * @return Octree
 */
Octree* Mesh::getOctree()
{
    return octree_;
}

/**
 * Supprime un vertex et toute ses références du mesh.
 *
 * @param verticesASupprimer
 * @return Mesh*
 */
Mesh* Mesh::supprimerVertices(std::vector<int> verticesASupprimer)
{
    Outil::ranger(verticesASupprimer);
    std::vector<int> verticesModele;
    for (int i=0;i<nbVertices_;i++)
        verticesModele.push_back(i);
    std::vector<int> vertices(nbVertices_);
    std::vector<int>::iterator it;
    it=std::set_difference(verticesModele.begin(),
                           verticesModele.end(),
                           verticesASupprimer.begin(),
                           verticesASupprimer.end(),
                           vertices.begin());
    vertices.resize(it-vertices.begin());
    return new Mesh(vertices, this);
}

/**
 * Dessine le mesh en fonction de son type shaderType_ ainsi que son
 * octree.
 *
 * @param contour
 */
void Mesh::dessiner(bool contour)
{
    glPushMatrix();
    glMultMatrixf(matTransformArray_);
    if (contour && shaderType_!=TRANSPARENCE)
        dessinerContour();
    switch (shaderType_)
    {
        case FLAT :
            glShadeModel(GL_FLAT);
            dessinerBuffer();
            glShadeModel(GL_SMOOTH);
            break;
        case NORMAL :
            programmeShader_.bind();
            programmeShader_.setUniformValue("estFocus", estSelectionne_);
            programmeShader_.setUniformValue("speculaireActivee", false);
            programmeShader_.setUniformValue("couleur", couleur_);
            dessinerBufferShader();
            programmeShader_.release();
            break;
        case PHONG :
            programmeShader_.bind();
            programmeShader_.setUniformValue("estFocus", estSelectionne_);
            programmeShader_.setUniformValue("speculaireActivee", true);
            programmeShader_.setUniformValue("couleur", couleur_);
            dessinerBufferShader();
            programmeShader_.release();
            break;
        case TRANSPARENCE :
            glDepthMask(GL_FALSE);
            glEnable(GL_BLEND);
            programmeShader_.bind();
            programmeShader_.setUniformValue("estFocus", estSelectionne_);
            programmeShader_.setUniformValue("positionLumiere", centre_);
            programmeShader_.setUniformValue(
                        "couleur", QColor::fromRgb(couleur_.red(),
                                                   couleur_.green(), couleur_.blue(), 35));
            dessinerBufferShader();
            programmeShader_.release();
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
            break;
        case FIL_DE_FER :
            glPolygonMode(GL_FRONT, GL_LINE);
            glColor3ub(couleur_.red(), couleur_.green(), couleur_.blue());
            dessinerBuffer();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case FIL_DE_FER_TRANSPARENT :
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor3ub(couleur_.red(), couleur_.green(), couleur_.blue());
            dessinerBuffer();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case VERTEX :
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glColor3ub(couleur_.red(), couleur_.green(), couleur_.blue());
            dessinerBuffer();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
    if (affichageOctree_)
        octree_->dessiner();
    glPopMatrix();
}

/**
 * Dessine les contours.
 *
 */
void Mesh::dessinerContour()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    glLineWidth(2.f);
    glColor3ub(150, 250, 250);
    dessinerBuffer();
    glLineWidth(2.f);
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**
 * Dessine les vertices présents dans les VBO verticesBuffer_ en
 * utilisant le VBO normalesBuffer_ et la valeur de couleur_ dans le cas
 * d'un shaderType_ ne requérant pas de Shader.
 *
 */
void Mesh::dessinerBuffer()
{
    verticesBuffer_.bind();
    glVertexPointer(3, GL_FLOAT, 0, 0);
    verticesBuffer_.release();
    normalesBuffer_.bind();
    glNormalPointer(GL_FLOAT, 0, 0);
    normalesBuffer_.release();
    //couleursBuffer_.bind();
    //glColorPointer(3, GL_FLOAT, 0, 0);
    //couleursBuffer_.release();
    indicesBuffer_.bind();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glDrawElements(GL_TRIANGLES,  nbTriangles_*3,  GL_UNSIGNED_INT,  0);
    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    indicesBuffer_.release();
}

/**
 * Dessine les vertices du mesh en se basant sur les attributs passés
 * en arguments aux Shaders, dans le cas d'un shaderType_ requérant un
 * Shader.
 *
 */
void Mesh::dessinerBufferShader()
{
    verticesBuffer_.bind();
    programmeShader_.setAttributeBuffer("vertex", GL_FLOAT,  0,  3);
    verticesBuffer_.release();
    normalesBuffer_.bind();
    programmeShader_.setAttributeBuffer("normale", GL_FLOAT,  0,  3);
    normalesBuffer_.release();
    indicesBuffer_.bind();
    programmeShader_.enableAttributeArray("vertex");
    programmeShader_.enableAttributeArray("normale");
    glDrawElements(GL_TRIANGLES,  nbTriangles_*3,  GL_UNSIGNED_INT,  0);
    programmeShader_.disableAttributeArray("normale");
    programmeShader_.disableAttributeArray("vertex");
    indicesBuffer_.release();
}

/**
 * Associe un fichier glsl à   progamShader_
 * et associe aux attributs de programmeShader_ une location spécifique.
 *
 * @param type
 */
void Mesh::setShader(ShaderMode type)
{
    shaderType_=type;
    programmeShader_.removeAllShaders();
    if (shaderType_==TRANSPARENCE)
    {
        programmeShader_.addShaderFromSourceFile(
                    QGLShader::Vertex, QDir::currentPath()+
                    "/src/3D/Rendu/transparenceVertexShader.glsl");
        programmeShader_.addShaderFromSourceFile(
                    QGLShader::Fragment, QDir::currentPath()+
                    "/src/3D/Rendu/transparenceFragmentShader.glsl");
    }
    else if (shaderType_==PHONG || shaderType_==NORMAL)
    {
        programmeShader_.addShaderFromSourceFile(
                    QGLShader::Vertex, QDir::currentPath()+
                    "/src/3D/Rendu/phongVertexShader.glsl");
        programmeShader_.addShaderFromSourceFile(
                    QGLShader::Fragment, QDir::currentPath()+
                    "/src/3D/Rendu/phongFragmentShader.glsl");
    }
    programmeShader_.link();
}

/**
 * Renvoie la valeur de shaderType_.
 *
 * @return ShaderMode
 */
Mesh::ShaderMode Mesh::getShader()
{
    return shaderType_;
}

/**
 * Renvoie la valeur de couleur_.
 *
 * @return QColor
 */
QColor Mesh::getCouleur()
{
    return couleur_;
}

/**
 * Modifie la valeur de couleur_.
 *
 * @param couleur
 */
void Mesh::setCouleur(const QColor &couleur)
{
    couleur_=couleur;
    /*
    //seulement si l'on veut specifier des couleurs localement pour chaque vertex
    couleursBuffer_.bind();
    for (int i=0;i<nbVertices_;i++)
    {
        float rouge=couleur_.redF();
        float vert=couleur_.greenF();
        float bleu=couleur_.blueF();
        couleursBuffer_.write((i*3)*sizeof(GLfloat), &rouge, sizeof(GLfloat));
        couleursBuffer_.write((i*3+1)*sizeof(GLfloat), &vert, sizeof(GLfloat));
        couleursBuffer_.write((i*3+2)*sizeof(GLfloat), &bleu, sizeof(GLfloat));
    }
    couleursBuffer_.release();
    */
}

/**
 * Rajoute le vertex (sans doublons) et met à   jour les triangles
 * connectés (utilisable pour le chargement du fichier STL).
 *
 * @param v
 * @param iTri
 * @param vertices
 * @return int
 */
int Mesh::rajouterVertexSansDoublons(Vertex &v, int iTri, std::set<Vertex> &vertices)
{
    std::pair<std::set<Vertex>::iterator, bool> pair=vertices.insert(v);
    int iVert=(*pair.first).getId();
    if (pair.second)
        vertices_.push_back(v);
    vertices_[iVert].ajouterTriangle(iTri);
    return iVert;
}

/**
 * Initialise les Vertex Buffer Object(VBO) et calcule les normales et
 * les vertices.
 *
 * @param fenetre
 */
void Mesh::initVBO(FenetrePrincipale *fenetre)
{
    GLfloat* verticesArray=(GLfloat*)malloc(nbVertices_*3*sizeof(GLfloat));
    //GLfloat* couleursArray=(GLfloat*)malloc(nbVertices_*3*sizeof(GLfloat));
    GLfloat* normalesArray=(GLfloat*)malloc(nbVertices_*3*sizeof(GLfloat));
    GLuint* indicesVerticesArray=(GLuint*)malloc(nbTriangles_*3*sizeof(GLuint));
    if (fenetre)
        fenetre->statusBar()->showMessage("Chargement : 4/5 (Init VBO 1)");
    for (int i=0;i<nbVertices_;i++)
    {
        Vertex &ver=vertices_[i];
        verticesArray[i*3]=ver.x();
        verticesArray[i*3+1]=ver.y();
        verticesArray[i*3+2]=ver.z();
        //couleursArray[i*3]=couleur_.redF();
        //couleursArray[i*3+1]=couleur_.greenF();
        //couleursArray[i*3+2]=couleur_.blueF();
        std::vector<int> iTri=ver.getIndicesTriangles();
        int nbTri = iTri.size();
        Vertex normale(0, 0, 0);
        for (int j=0; j<nbTri; j++)
            normale += triangles_[iTri[j]].getNormale();
        normale.normalize();
        vertices_[i].setNormale(normale);
        normalesArray[i*3] = normale.x();
        normalesArray[i*3+1] = normale.y();
        normalesArray[i*3+2] = normale.z();
        if (fenetre)
            fenetre->getBarreProgression()->setValue(100.f*(float)i/nbVertices_);
    }
    if (fenetre)
        fenetre->statusBar()->showMessage("Chargement : 5/5 (Init VBO 2)");
    for (int i=0; i<nbTriangles_; i++)
    {
        Triangle &tri = triangles_[i];
        indicesVerticesArray[i*3] = tri.getIndiceVertex(1);
        indicesVerticesArray[i*3+1] = tri.getIndiceVertex(2);
        indicesVerticesArray[i*3+2] = tri.getIndiceVertex(3);
        if (fenetre)
            fenetre->getBarreProgression()->setValue(100.f*(float)i/nbTriangles_);
    }
    verticesBuffer_.create();
    verticesBuffer_.setUsagePattern(QGLBuffer::DynamicDraw);
    verticesBuffer_.bind();
    verticesBuffer_.allocate(verticesArray, nbVertices_*3*sizeof(GLfloat));
    verticesBuffer_.release();
    normalesBuffer_.create();
    normalesBuffer_.setUsagePattern(QGLBuffer::DynamicDraw);
    normalesBuffer_.bind();
    normalesBuffer_.allocate(normalesArray, nbVertices_*3*sizeof(GLfloat));
    normalesBuffer_.release();
    indicesBuffer_.create();
    indicesBuffer_.setUsagePattern(QGLBuffer::StaticDraw);
    indicesBuffer_.bind();
    indicesBuffer_.allocate(indicesVerticesArray, nbTriangles_*3*sizeof(GLuint));
    indicesBuffer_.release();
    free(verticesArray);
    free(normalesArray);
    free(indicesVerticesArray);
    setShader(shaderType_);
}

/**
 * Calcule, pour chaque vertex, la distance avec son voisin le plus
 * éloigné (pour le picking). Initialise le centre du mesh.
 *
 * @param fenetre
 */
void Mesh::initDistanceVertexMaxEtCentreMeshEtOctree(FenetrePrincipale *fenetre)
{
    Aabb aabb;
    centre_=QVector3D();
    if (fenetre)
        fenetre->statusBar()->showMessage("Chargement : 2/5 (Init topologie)");
    aabb.setMax(vertices_[0]);
    aabb.setMin(vertices_[0]);

    for (int i=0;i<nbVertices_;i++)
    {
        majDistanceVertexMax(i);
        centre_+=vertices_[i];
        if (fenetre)
            fenetre->getBarreProgression()->setValue(100.f*(float)i/nbVertices_);
    }

    centre_/=nbVertices_;
    std::vector<int> indicesAberrants = indicesPointsAberrants();

    for (int i=0;i<nbVertices_;i++)
    {
        if (std::find(indicesAberrants.begin(), indicesAberrants.end(), vertices_[i].getId())
                == indicesAberrants.end())
            aabb.majTaille(vertices_[i]);
    }

    if (fenetre)
        fenetre->statusBar()->showMessage("Chargement : 3/5 (Init octree)");
    octree_->creer(vertices_, aabb);
}

/**
 * Fixe les coordonnées d'un vertex et met à   jour l'array openGL.
 *
 * @param i
 * @param nouveau
 */
void Mesh::setXYZ(int i, const QVector3D &nouveau)
{
    setXYZ(i, nouveau.x(), nouveau.y(), nouveau.z());
}

/**
 * Fixe les coordonnées d'un vertex et met à   jour l'array openGL.
 *
 * @param i
 * @param x
 * @param y
 * @param z
 */
void Mesh::setXYZ(int i, float x, float y, float z)
{
    Vertex &v=vertices_[i];
    //octree_->supprimerVertex(v);
    v.setX(x);
    v.setY(y);
    v.setZ(z);
    verticesBuffer_.bind();
    verticesBuffer_.write((i*3)*sizeof(GLfloat), &x, sizeof(GLfloat));
    verticesBuffer_.write((i*3+1)*sizeof(GLfloat), &y, sizeof(GLfloat));
    verticesBuffer_.write((i*3+2)*sizeof(GLfloat), &z, sizeof(GLfloat));
    verticesBuffer_.release();
}

/**
 * Calcule la normale d'un vertex et met à   jour l'array openGL pour
 * cette normale.
 *
 * @param iVer
 */
void Mesh::majArrayNormale(int iVer)
{
    std::vector<int> iTri = vertices_[iVer].getIndicesTriangles();
    int nbTri = iTri.size();
    QVector3D normale(0, 0, 0);
    for (int i=0;i<nbTri;i++)
        normale+=triangles_[iTri[i]].getNormale();
    normale.normalize();
    vertices_[iVer].setNormale(normale);
    GLfloat nx, ny, nz;
    nx=normale.x();
    ny=normale.y();
    nz=normale.z();
    normalesBuffer_.bind();
    normalesBuffer_.write((iVer*3)*sizeof(GLfloat), &nx, sizeof(GLfloat));
    normalesBuffer_.write((iVer*3+1)*sizeof(GLfloat), &ny, sizeof(GLfloat));
    normalesBuffer_.write((iVer*3+2)*sizeof(GLfloat), &nz, sizeof(GLfloat));
    normalesBuffer_.release();
}

/**
 * Met à   jour les normales d'un groupe de triangles et les distances max
 * des vertices composant ces triangles, ainsi que l'octree.
 *
 * @param iTris
 */
void Mesh::majNormalesEtDistanceMaxEtOctree(const std::vector<int> &iTris)
{
    std::vector<int> iTrisElargi=getTrianglesAPartirDeVertices(
                getVerticesAPartirDeTriangles(iTris));
    std::vector<int> iVers;
    int nbTris=iTrisElargi.size();
    for (int i=0;i<nbTris;i++)
    {
        Triangle &t=triangles_[iTrisElargi[i]];
        majNormale(iTrisElargi[i]);
        iVers.push_back(t.getIndiceVertex(1));
        iVers.push_back(t.getIndiceVertex(2));
        iVers.push_back(t.getIndiceVertex(3));
    }
    Outil::ranger(iVers); //On obtient seulement les vertices concernees par le calcul des normales
    for (int i=0;i<nbTris;i++)
    {
        majArrayNormale(iVers[i]);
        majDistanceVertexMax(iVers[i]);
        //        octree_->rajouterVertex(vertices_[iVers[i]]);
    }
}

/**
 * Met à   jour la normale d'un triangle.
 *
 * @param iTri
 */
void Mesh::majNormale(int iTri)
{
    Triangle &t=triangles_[iTri];
    t.setNormale(QVector3D::normal(vertices_[t.getIndiceVertex(1)],
                                   vertices_[t.getIndiceVertex(2)],
                                   vertices_[t.getIndiceVertex(3)]));
}

/**
 * Calcule la distance qu'il y a entre un vertex et son voisin le plus
 * éloigné dans le mesh.
 *
 * @param iVert
 */
void Mesh::majDistanceVertexMax(int iVert)
{
    Vertex &vert=vertices_[iVert];
    std::vector<int> iVersLies=getVerticesAPartirDeTriangles(vert.getIndicesTriangles());
    int nbVertsLies=iVersLies.size();
    float max=0;
    for (int i=0;i<nbVertsLies;i++)
    {
        float testDist=Geometrie::distance(vert, vertices_[iVersLies[i]]);
        if (testDist>max)
            max=testDist;
    }
    vert.setDistanceMax(max);
}

/**
 * Homothétie sur le mesh, ayant pour centre le centre de gravité du
 * mesh.
 * @param x rapport de l'homothétie.
 */
void Mesh::homothetie(float x)
{
    QMatrix4x4 t;
    t.translate(centre_);
    t.scale(x);
    t.translate(-centre_);

    for (int i=0;i<nbVertices_;i++)
    {
        QVector3D &v=vertices_[i];
        setXYZ(i, (t*v));
    }
   octree_->changerTailleOctree(x-1);
}

/**
 * Déplace le centre du mesh sur un point particulier,
 * la rotation est initialisée
 *
 * @param destination
 */
void Mesh::deplacerEn(const QVector3D &destination, bool identity)
{
    if (identity)
    {
        matTransform_.setToIdentity();
        matTransform_.translate(destination-centre_);
    }
    else
        matTransform_.translate(getTransformationInverse()*destination-centre_);
    majTransformation();
}

/**
 * Applique la transformation de matrice donnée au mesh.
 *
 * @param matTransform
 */
void Mesh::setTransformation(QMatrix4x4 matTransform)
{
    matTransform_=matTransform;
    majTransformation();
}

/**
 * Renvoie le centre d'un triangle d'indice donné.
 *
 * @param iTri
 * @return QVector3D
 */
QVector3D Mesh::getCentreTriangle(int iTri)
{
    Triangle &t=triangles_[iTri];
    return (vertices_[t.getIndiceVertex(1)]+vertices_[t.getIndiceVertex(2)]+
            vertices_[t.getIndiceVertex(3)])/3;
}


/**
 * Renvoie tout les vertices liées à   un vertex,
 * à   plusieurs degrés de séparation.
 *
 * @param iTri
 * @param degre
 * @return std::vector<int>
 */
std::vector<int> Mesh::getVerticesLiesAVertex(int iVert, int degre) const
{
    std::vector<int> verticesLies = getVerticesAPartirDeTriangles(
                vertices_[iVert].getIndicesTriangles());
    if (degre > 0)
    {
        for (int i=1; i<=degre; ++i)
        {
            verticesLies = getTrianglesAPartirDeVertices(verticesLies);
            verticesLies = getVerticesAPartirDeTriangles(verticesLies);
        }
    }
    Outil::ranger(verticesLies);
    return verticesLies;
}

/**
 * Renvoie tout les triangles liés à   un triangle.
 *
 * @param iTri
 * @return std::vector<int>
 */
std::vector<int> Mesh::getTrianglesLiesATriangle(int iTri) const
{
    const Triangle &tri = triangles_[iTri];
    std::vector<int> iVerts;
    iVerts.push_back(tri.getIndiceVertex(1));
    iVerts.push_back(tri.getIndiceVertex(2));
    iVerts.push_back(tri.getIndiceVertex(3));
    return getTrianglesAPartirDeVertices(iVerts);
}

/**
 * Renvoie les vertices composant un ensemble de triangles.
 *
 * @param iVerts
 * @return std::vector<int>
 */
std::vector<int> Mesh::getTrianglesAPartirDeVertices(const std::vector<int> &iVerts) const
{
    std::vector<int> iTris;
    int nbVerts=iVerts.size();
    for (int i=0;i<nbVerts;i++)
    {
        std::vector<int> iTrisi=vertices_[iVerts[i]].getIndicesTriangles();
        iTris.insert(iTris.end(), iTrisi.begin(), iTrisi.end());
    }
    Outil::ranger(iTris);
    return iTris;
}

/**
 * Renvoie les triangles composées par un ensemble de vertices.
 *
 * @param iTris
 * @return std::vector<int>
 */
std::vector<int> Mesh::getVerticesAPartirDeTriangles(const std::vector<int> &iTris) const
{
    std::vector<int> iVerts;
    int nbTris=iTris.size();
    for (int i=0;i<nbTris;i++)
    {
        const Triangle &t=triangles_[iTris[i]];
        iVerts.push_back(t.getIndiceVertex(1));
        iVerts.push_back(t.getIndiceVertex(2));
        iVerts.push_back(t.getIndiceVertex(3));
    }
    Outil::ranger(iVerts);
    return iVerts;
}

/**
 * Désactive/active l'affichage de l'octree.
 *
 */
void Mesh::changerAffichageOctree()
{
    affichageOctree_=!affichageOctree_;
}

/**
 * Renvoie la matrice de transformation du mesh.
 *
 * @return QMatrix4x4
 */
QMatrix4x4 Mesh::getTransformation() const
{
    return matTransform_;
}

/**
 * Met à jour le tableau correspondant à la matrice de transformation.
 *
 */
void Mesh::majTransformation()
{
    float *dataMat = matTransform_.data();
    for (int i= 0; i < 16; i++)
        matTransformArray_[i] = dataMat[i];
}

/**
 * Renvoie la transformation inverse de la matrice de transformation.
 *
 * @return QMatrix4x4
 */
QMatrix4x4 Mesh::getTransformationInverse() const
{
    return matTransform_.inverted();
}

/**
 * Renvoie la valeur de affichageOctree_.
 *
 * @return bool
 */
bool Mesh::getAffichageOctree()
{
    return affichageOctree_;
}

/**
 * Fixe la valeur de nomMesh_.
 *
 * @param nom
 */
void Mesh::setNomMesh(QString nom)
{
    nomMesh_=nom;
}

/**
 * Retourne la valeur de nomMesh_.
 *
 * @return QString
 */
QString Mesh::getNomMesh()
{
    return nomMesh_;
}

/**
 * Inverse toutes les normales des triangles.
 *
 */
void Mesh::inverserNormales()
{
    for (int i=0;i<nbTriangles_;i++)
        triangles_[i].inverserNormale();
    initVBO();
}

/**
 * Fixe la valeur de estSelectionne_.
 *
 * @param booleen
 */
void Mesh::setSelectionne(bool booleen)
{
    estSelectionne_ = booleen;
}

/**
 * Concatène un nouveau mesh à   celui-ci.
 *
 * @param mesh
 */
void Mesh::concatener(Mesh *mesh)
{
    std::vector<Vertex> &vertices=mesh->getVertices();
    std::vector<Triangle> &triangles=mesh->getTriangles();
    int nbTriangles=mesh->getNbTriangles();
    Vertex v1, v2, v3;
    int iVer1, iVer2, iVer3;
    std::set<Vertex> setVertices;
    QMatrix4x4 transformation=getTransformationInverse()*mesh->getTransformation();
    for (int i=0;i<nbTriangles;i++)
    {
        v1=Vertex(transformation*vertices[triangles[i].getIndiceVertex(1)],
                  setVertices.size()+nbVertices_);
        iVer1=rajouterVertexSansDoublons(v1, i+nbTriangles_, setVertices);
        v2=Vertex(transformation*vertices[triangles[i].getIndiceVertex(2)],
                  setVertices.size()+nbVertices_);
        iVer2=rajouterVertexSansDoublons(v2, i+nbTriangles_, setVertices);
        v3=Vertex(transformation*vertices[triangles[i].getIndiceVertex(3)],
                  setVertices.size()+nbVertices_);
        iVer3=rajouterVertexSansDoublons(v3, i+nbTriangles_, setVertices);
        triangles_.push_back(Triangle(QVector3D::normal(v1, v2, v3),
                                      iVer1, iVer2, iVer3, i+nbTriangles_));
    }
    nbVertices_=vertices_.size();
    nbTriangles_=triangles_.size();
    initDistanceVertexMaxEtCentreMeshEtOctree();
    initVBO();
}

/**
 * La liste des points aberrants du Mesh.
 *
 * @return std::vector<int>
 */
std::vector<int> Mesh::indicesPointsAberrants()
{
    std::vector<int> indicesPoints;
    std::vector<float> distances;
    for (uint i=0; i<vertices_.size(); ++i)
        distances.push_back(Geometrie::distance(vertices_[i], centre_));

    float q1, q3;
    std::sort(distances.begin(), distances.end());

    int indQ1 = (int)(distances.size()/4);
    int indQ3 = (int)(3*distances.size()/4);
    q1 = distances[indQ1];
    q3 = distances[indQ3];

    float limite = (q3-q1)*7.5;

    for (uint i=0; i<vertices_.size(); ++i)
        if (Geometrie::distance(vertices_[i], centre_) > limite && limite != 0)
            indicesPoints.push_back(vertices_[i].getId());

    return indicesPoints;
}

/**
 * Retourne si le mesh est editable
 *
 * @return bool

 */
bool Mesh::estEditable()
{
    return estEditable_;
}

/**
 * Autoriser l'edition
 *
 * @param bool

 */
void Mesh::autoriserEdition(bool estAutorise)
{

    estEditable_ = estAutorise;
}

void Mesh::propagationMesh(
        int iVert, std::set<int> &indicesTenon, std::set<int> &indicesBord)
{
    std::vector<int> vertices= getVerticesLiesAVertex(iVert);
    int indiceTest;
    int nbVertices=vertices.size();
    for (int i=0;i<nbVertices;i++)
    {
        indiceTest=vertices[i];
        if (std::find(indicesBord.begin(), indicesBord.end(), indiceTest)==indicesBord.end())
            if (indicesTenon.insert(indiceTest).second)
                propagationMesh(indiceTest, indicesTenon, indicesBord);
    }
}
