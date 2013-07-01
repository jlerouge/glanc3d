#ifndef __REBOUCHAGE_H__
#define __REBOUCHAGE_H__

#include <math.h>
#include <time.h>
#include "Morphing.h"
#include "Arete.h"
#include "Bord.h"
#include "Vertex.h"
#include "Geometrie.h"
#include "Constantes.h"

/**
 * La classe Rebouchage gère la reconstruction de surface
 * @author ToothPIC
 */
class Rebouchage
{

public:
    Rebouchage(Mesh *mesh = 0);
    ~Rebouchage();
    void setMesh(Mesh *mesh);
    Mesh* getMesh();
    std::vector<Arete>& getAretes();
    std::vector<Bord>& getTrous();
    QVector3D getCentreTrou(const Bord &bord);
    int getTrouLePlusGrand();
    void initTrous();
    void reboucherTrous(float longueurTrouMax, float surfaceTriangleMax,
                        std::vector<int> &verticesConcernes, bool rebouchagePlan = true,
                        bool plusGrandTrou = true);
    void recollerPuits(Mesh *meshPuits, Octree *modelePerce);
    void recollerInlayCore();
    void recollerAntagoniste(QVector3D centre);
    void recollerModeleFusionne(std::vector<int> indices1, std::vector<int> indices2);

private:
    void creerTriangles(int i1,int i2,int i3, float surfaceTriangleMax);
    void initAretes();
    bool trouverSensTrou(int iVer1,int iVer2);
    bool creerTrou(std::set< std::pair<int,int> > &aretes,int iPrev,int iNext,int iFin, Bord &trou);
    void calculerAngles(std::vector<Vertex> vertices, std::vector<int> indices, QVector3D centreTrou, std::vector<float> &angles);
    int fusionnerPoint(std::vector<Vertex> vertices, std::vector<int> indices, QVector3D vertex, float seuil);
    float calculerDegre(int indice);
    int getMinDegre(std::vector<float> degre);
    void reboucherTrou(const Bord &trou,float surfaceTriangleMax);
    void reboucherTrouPlan(const Bord &trou, float surfaceTriangleMax);

    void rajouterAreteSansDoublons(Arete &v, std::set<Arete> &aretes);
    std::set<std::pair<int,int> > obtenirAreteBord();
    QVector3D getCentreDuTrouLePlusGrand(Mesh *mesh);
    float getLongueurTrou(const Bord &bord);
    int retrouverBordPuits(Mesh *meshPuits);
    /** Le mesh sur lequel on doit détecter les trous */
    Mesh *mesh_;
    /** Les arêtes du mesh */
    std::vector<Arete> aretes_;
    /** Les trous du mesh */
    std::vector<Bord> trous_;
};

#endif /*__REBOUCHAGE_H__*/
