#include "Morphing.h"

/**
 * Constructeur de Morphing
 *
 */
Morphing::Morphing(): mesh_(0), intensite_(0.0), mode_(AJOUT), morphingEnCours_(false), centre_(-1)
{}

/**
 * Destructeur de Morphing
 *
 */
Morphing::~Morphing()
{}

/**
 * Fixe la valeur de mesh_
 *
 * @param mesh
 */
void Morphing::setMesh(Mesh *mesh)
{
    mesh_=mesh;
}

/**
 * Fixe la valeur de intensite_
 *
 * @param intensite
 */
void Morphing::setIntensite(float intensite)
{
    intensite_=intensite;
}

/**
 * Fixe la valeur de mode_
 *
 * @param mode
 */
void Morphing::setMode(SculptMode mode)
{
    mode_=mode;
    if (mode_==RETRAIT)
        intensite_=-fabs(intensite_);
    else
        intensite_=fabs(intensite_);
}

/**
 * Sculpte le mesh suivant une intensite et type de morphing
 *
 * @param iVerts
 */
void Morphing::sculpterMesh(const std::vector<int> &iVerts, QPointF p, QSize size)
{
    if (!mesh_->estEditable())
        return;
    int nbVerts=iVerts.size();
    if (mode_ == SCULPTAGE )
    {
        QVector3D vecProjete = Geometrie::point2Dto3D(p, size, 1);
        QVector3D v1 = Geometrie::calculerVecteurEcranPlanCamera(
                    QPointF(0, 0), QPointF(0, 1), size).normalized();
        QVector3D v2 = Geometrie::calculerVecteurEcranPlanCamera(
                    QPointF(0, 0), QPointF(1, 0), size).normalized();
        QVector3D v = mesh_->getTransformation()*mesh_->getVertex(centre_);
        QVector3D vecTransl = vecProjete - v;
        vecTransl = QVector3D::dotProduct(vecTransl, v1)*v1 +
                QVector3D::dotProduct(vecTransl, v2)*v2;
        for (uint i =0; i < iVerts.size();++i)
            mesh_->getOctree()->supprimerVertex(mesh_->getVertex(iVerts[i]));
        sculpter(iVerts, distances_);
        bool test = false;
        if (!test)
        {
            for (uint i=0;i<iVerts.size();i++)
                deplacerVertex(iVerts[i], coefs_[i]*vecTransl);
        }
        for (int i=0;i<nbVerts;i++)
            lisserVertex(iVerts[i]);
    }
    else if (mode_==LISSAGE)
    {
        for (int i=0;i<nbVerts;i++)
            lisserVertex(iVerts[i]); // l'intensite du lissage doit etre entre 0 et 1
    }
    else
    {
        // Formule empirique pour prendre en compte la taille de la zone
        float intensiteDeformation=intensite_*0.1f+intensite_*0.2f/nbVerts;
        for (int i=0;i<nbVerts;i++)
            sculpterVertex(iVerts[i], intensiteDeformation);
        for (int i=0;i<nbVerts;i++)
            lisserVertex(iVerts[i]);
    }
    for (uint i =0; i < iVerts.size();++i)
        mesh_->getOctree()->rajouterVertex(mesh_->getVertex(iVerts[i]));
    std::vector<int> iTris=mesh_->getTrianglesAPartirDeVertices(iVerts);
    mesh_->majNormalesEtDistanceMaxEtOctree(iTris);
}

/**
 * Lisse les vertices du mesh dont les indices sont iVerts, X fois
 *
 * @param iVerts
 * @param nbFois
 */
void Morphing::lisserMeshXFois(const std::vector<int> &iVerts, int nbFois)
{
    int nbVerts=iVerts.size();
    for (int i=0;i<nbFois;i++)
        for (int j=0;j<nbVerts;j++)
            lisserVertex(iVerts[j]);
    std::vector<int> iTris=mesh_->getTrianglesAPartirDeVertices(iVerts);
    mesh_->majNormalesEtDistanceMaxEtOctree(iTris);
}

/**
 * Déplace un vertex suivant sa normale et une intensité
 *
 * @param iVert
 * @param intensite
 */
void Morphing::sculpterVertex(int iVert, float intensite)
{
    Vertex &vert=mesh_->getVertex(iVert);
    mesh_->setXYZ(iVert, vert+vert.getNormale()*intensite);
}

/**
 * Lisse un vertex en calculant le centre de gravité de ses voisins
 *
 * @param iVert
 */
void Morphing::lisserVertex(int iVert)
{
    Vertex &vert=mesh_->getVertex(iVert);
    std::vector<int> iVers=mesh_->getVerticesAPartirDeTriangles(vert.getIndicesTriangles());
    QVector3D centre;
    int nbVers=iVers.size();
    for (int j=0;j<nbVers;j++)
        centre+=mesh_->getVertex(iVers[j]);
    centre/=nbVers;
    mesh_->setXYZ(iVert, vert-(vert-centre)*fabs(intensite_));
}

/**
 * Retourne la valeur de morphingEnCours_
 *
 * @return bool
 */
bool Morphing::getMorphing()
{
    return morphingEnCours_;
}

/**
 * Fixe la valeur de morphingEnCours_
 *
 * @param m
 */
void Morphing::setMorphing(bool m)
{
    morphingEnCours_ = m;
}

/**
 * Translate un vertex suivant v
 *
 * @param iVert
 * @param v
 */
void Morphing::deplacerVertex(int iVert, QVector3D v)
{
    Vertex &vert=mesh_->getVertex(iVert);
    mesh_->setXYZ(iVert, vert+v);
}

/**
 * Retourne la valeur de mode_
 *
 * @return mode_
 */
Morphing::SculptMode Morphing::getMode()
{
    return mode_;
}

/**
 * Sculpte un mesh suivant une translation, des points à sculpter,
 * et leur distance au centre. Cette distance est utilisée par le
 * smoothstep afin d'avoir un rendu lisse.
 *
 * @param iVerts
 * @param iDistances
 * @param vecTransl
 */
void Morphing::sculpter(const std::vector<int> &iVerts, const std::vector<float> &distances)
{
    float dmax= 0;
    float coef = 0;
    float t = 0;
    for (uint i=0; i < iVerts.size(); i++)
    {
        if (distances[i] > dmax)
            dmax = distances[i];
    }
    coefs_.clear();
    for (uint i=0; i < iVerts.size(); i++)
    {
        t = (float)(dmax - distances[i])/dmax;
        // Smoothstep
        coef = (6*pow(t, 5)-15*pow(t, 4)+10*pow(t, 3));
        coefs_.push_back(coef);
    }
}

/**
 * Initialise les distances au centre pour le sculptage d'un mesh
 *
 * @param mesh
 * @param iVerts
 */
void Morphing::initParamSculptage(Mesh *mesh, const std::vector<int> &iVerts)
{
    distances_.clear();
    for (uint i=0; i<iVerts.size(); ++i)
        distances_.push_back(Geometrie::distance(
                                 mesh->getVertex(iVerts[i]), mesh->getVertex(centre_)));
}

/**
 * Initialise le centre du sculptage d'un mesh
 *
 * @param mesh
 * @param iVerts
 */
void Morphing::initCentre(int ivert)
{
    centre_ = ivert;
}
