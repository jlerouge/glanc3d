#include "Rebouchage.h"

/**
 * Constructeur de Rebouchage
 *
 * @param mesh
 */
Rebouchage::Rebouchage(Mesh *mesh): mesh_(mesh), aretes_(), trous_()
{
    srand(time(NULL)); // Initialise la matrice de l'univers
}

/**
 * Destructeur de Rebouchage
 *
 */
Rebouchage::~Rebouchage()
{}

/**
 * Fixe la valeur de mesh_
 *
 * @param mesh
 */
void Rebouchage::setMesh(Mesh *mesh)
{
    mesh_=mesh;
}

/**
 * Renvoie la valeur de mesh_
 *
 * @return Mesh
 */
Mesh* Rebouchage::getMesh()
{
    return mesh_;
}

/**
 * Renvoie la valeur de aretes_
 *
 * @return std::vector<Arete>
 */
std::vector<Arete>& Rebouchage::getAretes()
{
    return aretes_;
}

/**
 * Renvoie la valeur de trous_
 *
 * @return std::vector<Bord>
 */
std::vector<Bord>& Rebouchage::getTrous()
{
    return trous_;
}

/**
 * Initialise les arêtes (à ne faire que si nécessaire)
 *
 */
void Rebouchage::initAretes()
{
    std::vector<Vertex> &vertices=mesh_->getVertices();
    std::vector<Triangle> &triangles=mesh_->getTriangles();
    int nbTriangles=mesh_->getTriangles().size();
    std::set<Arete> aretes;
    aretes_.clear();

    for (int i=0;i<nbTriangles;i++)
    {

        Vertex *v1=&vertices[triangles[i].getIndiceVertex(1)];
        Vertex *v2=&vertices[triangles[i].getIndiceVertex(2)];
        Vertex *v3=&vertices[triangles[i].getIndiceVertex(3)];

        Arete a1=Arete(v1, v2);

        Arete a2=Arete(v1, v3);


        Arete a3=Arete(v2, v3);


        a1.setId(aretes.size());
        rajouterAreteSansDoublons(a1, aretes);
        a2.setId(aretes.size());
        rajouterAreteSansDoublons(a2, aretes);
        a3.setId(aretes.size());
        rajouterAreteSansDoublons(a3, aretes);
    }
}

/**
 * Rajoute l'arête (sans doublons)
 *
 * @param a
 * @param aretes
 */
void Rebouchage::rajouterAreteSansDoublons(Arete &a, std::set<Arete> &aretes)
{

    std::pair<std::set<Arete>::iterator, bool> pair=aretes.insert(a);


    int iArete=(*pair.first).getId();
    if (pair.second)
    {
        aretes_.push_back(a);


    }

    aretes_[iArete].incrementerNbTriangles();
}

/**
 * Renvoie les arêtes qui n'ont qu'un seul triangle
 *
 * @return std::set<std::pair<int, int> >
 */
std::set<std::pair<int, int> > Rebouchage::obtenirAreteBord()
{
    std::set< std::pair<int, int> > aretes;
    int nbAretes=aretes_.size();
    for (int i=0;i<nbAretes;i++)
    {
        if (aretes_[i].estAreteSimple())
        {
            std::pair<int, int> pair;
            pair.first=aretes_[i].getVertex1()->getId();
            pair.second=aretes_[i].getVertex2()->getId();
            aretes.insert(pair);
        }
    }
    return aretes;
}

/**
 * Initialise les trous d'un mesh
 *
 */
void Rebouchage::initTrous()
{

    initAretes();
    trous_.clear();
    std::set< std::pair<int, int> > aretesBord=obtenirAreteBord();
    std::set<std::pair<int, int> >::iterator it=aretesBord.begin();
    while (!(aretesBord.empty()))
    {
        Bord trou;
        int iDebut=it->first;
        int iFin=it->second;
        aretesBord.erase(it);
        if (trouverSensTrou(iDebut, iFin))
        {
            creerTrou(aretesBord, iFin, iDebut, iFin, trou);
            trou.ajouterPoint(iFin);
        }
        else
        {
            creerTrou(aretesBord, iDebut, iFin, iDebut, trou);
            trou.ajouterPoint(iDebut);
        }
        trous_.push_back(trou);
        it=aretesBord.begin();
    }
}

/**
 * Pour trouver le sens dans lequel on doit tourner autour du trou
 *
 * @param iVer1
 * @param iVer2
 * @return bool
 */
bool Rebouchage::trouverSensTrou(int iVer1, int iVer2)
{
    std::vector<int> iTris1=mesh_->getVertex(iVer1).getIndicesTriangles();
    std::vector<int> iTris2=mesh_->getVertex(iVer2).getIndicesTriangles();
    Outil::ranger(iTris1);
    Outil::ranger(iTris2);
    std::vector<int> iTrisInter(iTris1.size()+iTris2.size());
    std::set_intersection(iTris1.begin(), iTris1.end(), iTris2.begin(),
                          iTris2.end(), iTrisInter.begin());
    int iTri=iTrisInter[0];
    Triangle &tri=mesh_->getTriangle(iTri);
    int i1=tri.getIndiceVertex(1);
    int i2=tri.getIndiceVertex(2);
    int i3=tri.getIndiceVertex(3);
    if (iVer1==i1)
        return iVer2==i2;
    if (iVer1==i2)
        return iVer2==i3;
    return iVer2==i1;
}

/**
 * Crée un trou
 *
 * @param aretes
 * @param iPrev
 * @param iNext
 * @param iFin
 * @param trou
 * @return bool
 */
bool Rebouchage::creerTrou(std::set< std::pair<int, int> > &aretes,
                           int iPrev, int iNext, int iFin, Bord &trou)
{
    std::set< std::pair<int, int> >::iterator it;
    std::vector< std::pair<int, int> > aretesCandidates;
    trou.ajouterPoint(iNext);
    for (it=aretes.begin();it!=aretes.end();++it)
    {
        int i1=it->first;
        int i2=it->second;
        if (i1==iNext)
        {
            if (i2==iFin)
            {
                aretes.erase(it);
                return true;
            }
            else
                aretesCandidates.push_back(*it);
        }
        else if (i2==iNext)
        {
            if (i1==iFin)
            {
                aretes.erase(it);
                return true;
            }
            else
                aretesCandidates.push_back(*it);
        }
    }
    if (aretesCandidates.empty())
        return true;
    float max=-1.f;
    float dotTest=0.0;
    QVector3D vNext=mesh_->getVertex(iNext);
    QVector3D vecAvant=(mesh_->getVertex(iPrev)-vNext).normalized();
    int iTest=0;
    int iSuivant=0;
    int j=0;
    int nbAretesCandidates=aretesCandidates.size();
    for (int i=0;i<nbAretesCandidates;i++)
    {
        iTest=aretesCandidates[i].first;
        if (iTest==iNext)
            iTest=aretesCandidates[i].second;
        dotTest=QVector3D::dotProduct(vecAvant, (mesh_->getVertex(iTest)-vNext).normalized());
        if (dotTest>max)
        {
            max=dotTest;
            j=i;
            iSuivant=iTest;
        }
    }
    aretes.erase(aretesCandidates[j]);
    creerTrou(aretes, iNext, iSuivant, iFin, trou);
    return true;
}

/**
 * Fusion de points lorsque distance est petite
 *
 * @param vertices
 * @param indices
 * @param vertex
 * @param seuil
 * @return int
 */
int Rebouchage::fusionnerPoint(std::vector<Vertex> vertices, std::vector<int> indices,
                               QVector3D vertex, float seuil)
{
    float dmin = seuil;
    int indice = -1;
    for (uint i=0; i<indices.size(); ++i)
    {
        if (Geometrie::distance(vertex, vertices[indices[i]]) < dmin)
        {
            dmin = Geometrie::distance(vertex, vertices[indices[i]]);
            indice = i;
        }
    }
    return indice;
}

/**
 * Calcule le degré de lissage d'un vertex par rapport aux normales
 * des autres vertices des triangles auquel le vertex appartient
 *
 * @param indiceMesh
 * @return float
 */
float Rebouchage::calculerDegre(int indiceMesh)
{
    Vertex vertice=mesh_->getVertex(indiceMesh);
    float alpha=0;
    std::vector<int> indiceTri = vertice.getIndicesTriangles();
    int nbTri=indiceTri.size();
    for (int i=0;i<nbTri-1;i++)
    {
        QVector3D ni, nj;
        ni=mesh_->getTriangle(indiceTri[i]).getNormale();
        nj=mesh_->getTriangle(indiceTri[i+1]).getNormale();
        alpha=alpha+acos(QVector3D::dotProduct(ni, nj)/(ni.length()*nj.length()));
    }
    return alpha=alpha/(nbTri-1);
}

/**
 * Retourne l'indice sur le trou du vertex au degré de lissage le plus petit
 *
 * @param degre
 * @return int
 */
int Rebouchage::getMinDegre(std::vector<float> degre)
{
    int nbVerticesTrou = degre.size();
    float minDegre = F_MIN_INIT;
    int indiceTrou = 0;
    for (int i=0; i<nbVerticesTrou; i++)
    {
        if (degre[i] < minDegre)
        {
            minDegre = degre[i];
            indiceTrou = i;
        }
    }
    return indiceTrou;
}

/**
 * Rebouche les trous
 *
 * @param longueurTrouMax
 * @param surfaceTriangleMax
 * @param verticesConcernes
 */
void Rebouchage::reboucherTrous(float longueurTrouMax, float surfaceTriangleMax,
                                std::vector<int> &verticesConcernes, bool rebouchagePlan,
                                bool plusGrandTrou)
{
    bool aReboucher;
    int nbTrous=trous_.size();
    int bordPuits = getTrouLePlusGrand();
    for (int i=0;i<nbTrous;i++)
    {
        aReboucher = false;
        for (uint j=0; j<trous_[i].getIndices().size(); ++j)
        {
            if (std::find(verticesConcernes.begin(), verticesConcernes.end(),
                          trous_[i].getIndices()[j]) != verticesConcernes.end())
            {
                aReboucher = true;
                break;
            }
        }
        if (aReboucher && (getLongueurTrou(trous_[i])<longueurTrouMax &&
                           (plusGrandTrou || (i != bordPuits))))
        {
            int nbVertices=trous_[i].getIndices().size();
            if (nbVertices >= 3)
            {
                if (rebouchagePlan)
                    reboucherTrouPlan(trous_[i], surfaceTriangleMax);
                else
                    reboucherTrou(trous_[i], surfaceTriangleMax);
            }
        }
    }
}

/**
 * Rebouche un trou
 *
 * @param trou
 * @param surfaceTriangleMax
 */
void Rebouchage::reboucherTrou(const Bord &trou, float surfaceTriangleMax)
{
    std::vector<Vertex> &vertices=mesh_->getVertices();
    std::vector<int> indices=trou.getIndices();
    int nbVertices=indices.size();

    int i, im1, ip1, inew;
    int iMin = 0;
    int iFus = 0;
    Vertex v, vm1, vp1, new_ver;
    float angle, m, n, q;
    std::vector<float> degre;
    QVector3D centreTrou = getCentreTrou(trou), centreTri;
    bool fusion = false;

    for (int k=0;k<nbVertices;k++)
        degre.push_back(calculerDegre(indices[k]));

    for (int k=0;k<nbVertices;k++)
    {
        iMin = getMinDegre(degre);
        i = indices[iMin];
        im1 = indices[(iMin-1+nbVertices)%nbVertices];
        ip1 = indices[(iMin+1)%nbVertices];

        v = vertices[i];
        vm1 = vertices[im1];
        vp1 = vertices[ip1];

        angle = acos(QVector3D::dotProduct(vm1-v, v-vp1)/
                     (Geometrie::distance(v, vm1)*Geometrie::distance(v, vp1)));
        centreTri = (v+vm1+vp1)/3.f;

        // angle > 180 , passe au point suivant
        if (Geometrie::distance(v, centreTrou) < Geometrie::distance(centreTri, centreTrou))
        {
            angle = 2*M_PI-angle;
            degre[iMin] = F_MIN_INIT;
        }
        else if (0 < angle && angle <= M_PI/2.f) // 0 < angle <= 90
        {
            creerTriangles(i, ip1, im1, surfaceTriangleMax);
            indices.erase(indices.begin()+iMin);
            degre.erase(degre.begin()+iMin);
            nbVertices--;
        }
        else if (M_PI/2.f < angle && angle <= M_PI) // 90 < angle <= 180
        {
            iMin = rand()%nbVertices;
            i = indices[iMin];
            im1 = indices[(iMin-1+nbVertices)%nbVertices];
            ip1 = indices[(iMin+1)%nbVertices];
            v = vertices[i];
            vm1 = vertices[im1];
            vp1 = vertices[ip1];

            m=(vp1-vm1).lengthSquared()*QVector3D::dotProduct(v-vp1, v-vm1)/
                    (2.f*QVector3D::crossProduct(v-vp1, v-vm1).lengthSquared());
            n=(v-vm1).lengthSquared()*QVector3D::dotProduct(vp1-v, vp1-vm1)/
                    (2.f*QVector3D::crossProduct(v-vp1, vp1-vm1).lengthSquared());
            q=(v-vp1).lengthSquared()*QVector3D::dotProduct(vm1-v, vm1-vp1)/
                    (2.f*QVector3D::crossProduct(v-vp1, vp1-vm1).lengthSquared());

            if (isnan(m) || isnan(n) || isnan(q) || ((fabsf(m)+fabsf(n)+fabsf(q)) > F_MIN_INIT))
                break;

            new_ver = Vertex(m*v+n*vp1+q*vm1, mesh_->getNbVertices());

            // Fusion de points lorsque distance est petite
            iFus = fusionnerPoint(vertices, indices, new_ver, 0.05f);
            fusion = (iFus == -1);
            inew = indices[iFus];

            if (!fusion)
            {
                mesh_->setNbVertices(mesh_->getNbVertices()+1);
                vertices.push_back(new_ver);
                inew = new_ver.getId();
                indices[iMin] = inew;
                degre[iMin]=calculerDegre(inew);
            }
            creerTriangles(im1, i, inew, surfaceTriangleMax);
            creerTriangles(i, ip1, inew, surfaceTriangleMax);
            if (fusion)
                break;
        }
    }
    if (indices.size()>3)
    {
        if (fusion)
        {
            Bord newBord1 = Bord(), newBord2 = Bord();
            int nb1 = (iFus - (iMin+1) + nbVertices)%nbVertices;
            int nb2 = ((iMin-1) - iFus + nbVertices)%nbVertices;
            for (int k = 1; k <= nb1; k++)
                newBord1.ajouterPoint(indices[((iMin+k) + nbVertices)%nbVertices]);
            for (int k = 0; k <= (nb2-1); k++)
                newBord2.ajouterPoint(indices[(iFus+k + nbVertices)%nbVertices]);
            reboucherTrou(newBord1, surfaceTriangleMax);
            reboucherTrou(newBord2, surfaceTriangleMax);
        }
        else
        {
            Bord newBord = Bord();
            for (uint i=0; i<indices.size(); ++i)
                newBord.ajouterPoint(indices[i]);
            reboucherTrou(newBord, surfaceTriangleMax);
        }
    }
    else if (indices.size() == 3)
        creerTriangles(indices[0], indices[1], indices[2], surfaceTriangleMax);
}

/**
 * Rebouche un trou de manière plane
 *
 * @param trou
 * @param surfaceTriangleMax
 */
void Rebouchage::reboucherTrouPlan(const Bord &trou, float surfaceTriangleMax)
{
    std::vector<int> indices=trou.getIndices();
    int nbVertices=indices.size();
    if (nbVertices == 3)
        creerTriangles(indices[0], indices[1], indices[2], surfaceTriangleMax);
    else
    {
        std::vector<Vertex> &vertices=mesh_->getVertices();
        Vertex centre;
        for (int i=0;i<nbVertices;i++)
            centre+=vertices[indices[i]];
        centre/=nbVertices;
        std::vector<float> angles;
        calculerAngles(vertices, indices, centre, angles);
        int iMin = std::min_element(angles.begin(), angles.end()) - angles.begin();
        if (angles[iMin] < M_PI)
        {
            creerTriangles(indices[iMin], indices[(iMin+1)%nbVertices],
                           indices[(iMin-1+nbVertices)%nbVertices], surfaceTriangleMax);
            Bord newTrou = Bord();
            for (int i=0;i<nbVertices;i++)
                if (i!=iMin)
                    newTrou.ajouterPoint(indices[i]);
            reboucherTrouPlan(newTrou, surfaceTriangleMax);
        }
    }
}


/**
 * Calcule l'angle entre le vertex courant sur le trou et les deux vertices voisins
 *
 * @param vertices
 * @param indices
 * @param centreTrou
 * @param angles
 */
void Rebouchage::calculerAngles(std::vector<Vertex> vertices, std::vector<int> indices,
                                QVector3D centreTrou, std::vector<float> &angles)
{
    angles.clear();
    int nbIndices = indices.size();
    Vertex v, vm1, vp1;
    float angle;
    for (int k=0; k<nbIndices; ++k)
    {
        v = vertices[indices[k]];
        vm1 = vertices[indices[(k-1+nbIndices)%nbIndices]];
        vp1 = vertices[indices[(k+1)%nbIndices]];
        angle = Geometrie::angle(vm1-v, vp1-v);
        if (Geometrie::distance(v, centreTrou) < Geometrie::distance((v+vm1+vp1)/3.f, centreTrou))
            angle= 2*M_PI-angle;
        angles.push_back(angle);
    }
}

/**
 * Crée un triangle
 *
 * @param i1
 * @param i2
 * @param i3
 * @param surfaceTriangleMax
 */
void Rebouchage::creerTriangles(int i1, int i2, int i3, float surfaceTriangleMax)
{
    Vertex &v1=mesh_->getVertex(i1);
    Vertex &v2=mesh_->getVertex(i2);
    Vertex &v3=mesh_->getVertex(i3);
    float surface=(Geometrie::distance(v1, v2)*v3.distanceToLine(v1, (v2-v1).normalized()))/2.f;
    if (surface>surfaceTriangleMax)
    {
        std::vector<Vertex> &vertices=mesh_->getVertices();
        Vertex centre;
        int nbVertices=mesh_->getNbVertices();
        centre=(v1+v2+v3)/3.f;
        centre.setId(nbVertices++);
        int iCentre=centre.getId();
        vertices.push_back(centre);
        mesh_->setNbVertices(nbVertices);
        creerTriangles(i1, i2, iCentre, surfaceTriangleMax);
        creerTriangles(i2, i3, iCentre, surfaceTriangleMax);
        creerTriangles(i3, i1, iCentre, surfaceTriangleMax);
    }
    else
    {
        std::vector<Triangle> &triangles=mesh_->getTriangles();
        int nbTriangles=mesh_->getNbTriangles();
        triangles.push_back(Triangle(QVector3D::normal(v2-v1, v3-v1), i1, i2, i3, nbTriangles));
        v1.ajouterTriangle(nbTriangles);
        v2.ajouterTriangle(nbTriangles);
        v3.ajouterTriangle(nbTriangles);
        mesh_->setNbTriangles(nbTriangles+1);
    }
}

/**
 * Recolle le bord du puits le plus long avec le mesh
 * (on prend l'octree pour rechercher les vertices
 * sur le modèle avant fusion)
 *
 * @param meshPuits
 * @param modelePerce
 */
void Rebouchage::recollerPuits(Mesh *meshPuits, Octree *modelePerce)
{
    initTrous();
    std::vector<int> indices = trous_[retrouverBordPuits(meshPuits)].getIndices();
    int nbIndices=indices.size();
    std::vector<int> indicesAretes;
    int nbTrous=trous_.size();
    for (int i=0;i<nbTrous;i++)
    {
        std::vector<int> inds=trous_[i].getIndices();
        indicesAretes.insert(indicesAretes.end(), inds.begin(), inds.end());
    }
    Outil::ranger(indicesAretes);
    int i1, i2, i3, i4;
    Vertex *v;
    for (int i=0;i<nbIndices;i++)
    {
        i1=indices[i];
        if (i==(nbIndices-1))
            i2=indices[0];
        else
            i2=indices[i+1];
        v=modelePerce->getVertexLePlusProche(mesh_->getVertex(i1));
        i3=v->getId();
        if (std::find(indicesAretes.begin(), indicesAretes.end(), i3)!=indicesAretes.end())
        {
            indices.push_back(i3);
            creerTriangles(i1, i2, i3, 2.f);
        }
        v=modelePerce->getVertexLePlusProche(mesh_->getVertex(i2));
        i4=v->getId();
        if (i3!=i4 && std::find(indicesAretes.begin(),
                                indicesAretes.end(),
                                i4)!=indicesAretes.end())
        {
            indices.push_back(i4);
            creerTriangles(i4, i3, i2, 2.f);
        }
    }
    initTrous();
    reboucherTrous(15.f, 3.f, indices);
    indices=mesh_->getVerticesAPartirDeTriangles(mesh_->getTrianglesAPartirDeVertices(indices));
    initTrous();
    reboucherTrous(15.f, 3.f, indices);
    Outil::ranger(indices);
    indices=mesh_->getVerticesAPartirDeTriangles(mesh_->getTrianglesAPartirDeVertices(indices));
    Morphing morphing;
    morphing.setMesh(mesh_);
    morphing.setIntensite(1.f);
    morphing.lisserMeshXFois(indices, 10);
}

/**
 * Recolle le bord du tenon avec la partie sup.
 *
 */
void Rebouchage::recollerInlayCore()
{
    initTrous();
    std::vector<float> taillesTrou;
    for (uint i=0; i<trous_.size(); ++i)
        taillesTrou.push_back(getLongueurTrou(trous_[i]));
    int iTrou1 = std::max_element(taillesTrou.begin(), taillesTrou.end())-taillesTrou.begin();
    taillesTrou[iTrou1] = 0.f;
    int iTrou2 = std::max_element(taillesTrou.begin(), taillesTrou.end())-taillesTrou.begin();
    std::vector<int> indices1 = trous_[iTrou1].getIndices();
    std::vector<int> indices2 = trous_[iTrou2].getIndices();
    int nbIndices1 = indices1.size();
    int nbIndices2 = indices2.size();

    std::vector<int> indices;
    indices.insert(indices.end(), indices1.begin(), indices1.end());
    indices.insert(indices.end(), indices2.begin(), indices2.end());

    int i1, i2, i3, i4;
    float dmin;
    for (int i=0; i<nbIndices2; i++)
    {
        i1=indices2[i];
        if (i==(nbIndices2-1))
            i2=indices2[0];
        else
            i2=indices2[i+1];
        dmin = F_MIN_INIT;
        i3 = -1;
        for (int j=0; j<nbIndices1; ++j)
        {
            if (Geometrie::distance(mesh_->getVertex(i1), mesh_->getVertex(indices1[j])) < dmin)
            {
                dmin = Geometrie::distance(mesh_->getVertex(i1), mesh_->getVertex(indices1[j]));
                i3 = j;
            }
        }
        if (std::find(indices.begin(), indices.end(), indices1[i3])!=indices.end())
        {
            creerTriangles(i1, i2, indices1[i3], 2.f);
            indices.push_back(indices1[i3]);
        }

        dmin = F_MIN_INIT;
        i4 = -1;
        for (int j=0; j<nbIndices1; ++j)
        {
            if (Geometrie::distance(mesh_->getVertex(i2), mesh_->getVertex(indices1[j])) < dmin)
            {
                dmin = Geometrie::distance(mesh_->getVertex(i2), mesh_->getVertex(indices1[j]));
                i4 = j;
            }
        }

        if (i3!=i4 && std::find(indices.begin(), indices.end(), indices1[i4])!=indices.end())
        {
            creerTriangles(indices1[i4], indices1[i3], i2, 2.f);
            indices.push_back(indices1[i4]);
        }
        Outil::ranger(indices);
    }
}

void Rebouchage::recollerAntagoniste(QVector3D centre)
{
    initTrous();
    std::vector<float> distanceAuPoint;
    for (uint i=0; i<trous_.size(); ++i)
        distanceAuPoint.push_back( Geometrie::distance(trous_[i].getPointCentre(mesh_), centre ));

    int iTrou1 = std::min_element(distanceAuPoint.begin(),
                                  distanceAuPoint.end())-distanceAuPoint.begin();
    distanceAuPoint[iTrou1] = F_MIN_INIT;
    int iTrou2 = std::min_element(distanceAuPoint.begin(),
                                  distanceAuPoint.end())-distanceAuPoint.begin();

    std::vector<int> indices1 = trous_[iTrou1].getIndices();
    std::vector<int> indices2 = trous_[iTrou2].getIndices();
    int nbIndices1 = indices1.size();
    int nbIndices2 = indices2.size();

    std::vector<int> indices;
    indices.insert(indices.end(), indices1.begin(), indices1.end());
    indices.insert(indices.end(), indices2.begin(), indices2.end());

    int i1, i2, i3, i4;
    float dmin;
    for (int i=0; i<nbIndices2; i++)
    {
        i1=indices2[i];
        if (i == (nbIndices2-1))
            i2 = indices2[0];
        else
            i2 = indices2[i+1];
        dmin = F_MIN_INIT;
        i3 = -1;
        for (int j=0; j<nbIndices1; ++j)
        {
            if (Geometrie::distance(mesh_->getVertex(i1), mesh_->getVertex(indices1[j])) < dmin)
            {
                dmin = Geometrie::distance(mesh_->getVertex(i1), mesh_->getVertex(indices1[j]));
                i3 = j;
            }
        }
        if (std::find(indices.begin(), indices.end(), indices1[i3])!=indices.end())
        {
            creerTriangles(i1, i2, indices1[i3], 2.f);
            indices.push_back(indices1[i3]);
        }

        dmin = F_MIN_INIT;
        i4 = -1;
        for (int j=0; j < nbIndices1; ++j)
        {
            if (Geometrie::distance(mesh_->getVertex(i2), mesh_->getVertex(indices1[j])) < dmin)
            {
                dmin = Geometrie::distance(mesh_->getVertex(i2), mesh_->getVertex(indices1[j]));
                i4 = j;
            }
        }
        if (i3!=i4 && std::find(indices.begin(), indices.end(), indices1[i4])!=indices.end())
        {
            creerTriangles(indices1[i4], indices1[i3], i2, 2.f);
            indices.push_back(indices1[i4]);
        }
        Outil::ranger(indices);
    }

    initTrous();

    reboucherTrous(15.f, 0.5f, indices, true, false);
    Morphing morphing;
    morphing.setMesh(mesh_);
    morphing.setIntensite(1);

    morphing.lisserMeshXFois(indices, 10);
}

/**
 * Retrouve le bord du puits sur le modèle fusionné
 *
 * @param meshPuits
 * @return int
 */
int Rebouchage::retrouverBordPuits(Mesh *meshPuits)
{
    QVector3D centreBordPuits = meshPuits->getTransformation()*
            getCentreDuTrouLePlusGrand(meshPuits);
    int iBord = 0;
    float min = F_MIN_INIT;
    float distance;
    int nbTrous = trous_.size();
    for (int i=0; i<nbTrous; i++)
    {
        distance = (getCentreTrou(trous_[i])-centreBordPuits).lengthSquared();
        if (distance < min)
        {
            min = distance;
            iBord = i;
        }
    }
    return iBord;
}

/**
 * Renvoie le centre du trou
 *
 * @param bord
 * @return QVector3D
 */
QVector3D Rebouchage::getCentreTrou(const Bord &bord)
{
    QVector3D centre;
    if (!mesh_)
        return centre;
    std::vector<int> indices = bord.getIndices();
    int nbIndices = indices.size();
    for (int i=0; i<nbIndices; i++)
        centre+=mesh_->getVertex(indices[i]);
    centre /= nbIndices;
    return centre;
}

/**
 * Renvoie la longueur du trou
 *
 * @param bord
 * @return float
 */
float Rebouchage::getLongueurTrou(const Bord &bord)
{
    float longueur = 0.f;
    if (!mesh_)
        return longueur;
    std::vector<int> indices=bord.getIndices();
    int nbIndices = indices.size();
    int i1, i2;
    for (int i=0; i<nbIndices; i++)
    {
        i1 = indices[i];
        if (i == (nbIndices-1))
            i2 = indices[0];
        else
            i2 = indices[i+1];
        longueur += Geometrie::distance(mesh_->getVertex(i1), mesh_->getVertex(i2));
    }
    return longueur;
}

/**
 * Renvoie l'indice du trou le plus grand d'un mesh
 *
 * @param mesh
 * @return int
 */
int Rebouchage::getTrouLePlusGrand()
{
    int max = 0, iMax = 0;
    for (uint i=0; i<trous_.size(); i++)
    {
        if ((int)trous_[i].getIndices().size()>max)
        {
            max = trous_[i].getIndices().size();
            iMax = i;
        }
    }
    return iMax;
}

/**
 * Renvoie le centre du trou le plus grand d'un mesh
 *
 * @param mesh
 * @return QVector3D
 */
QVector3D Rebouchage::getCentreDuTrouLePlusGrand(Mesh *mesh)
{
    Rebouchage rebouchage(mesh);
    rebouchage.initTrous();
    std::vector<Bord> &bordPuits = rebouchage.getTrous();
    QVector3D centre;
    int max = 0;
    int nbBordPuits = bordPuits.size();
    for (int i=0; i<nbBordPuits; i++)
    {
        if ((int)bordPuits[i].getIndices().size() > max)
        {
            max = bordPuits[i].getIndices().size();
            centre = rebouchage.getCentreTrou(bordPuits[i]);
        }
    }
    return centre;
}

/**
 * Recolle la partie supérieure
 * de l'inlay-core avec le modèle
 * fusionné précédemment.
 *
 * @param indices1
 * @param indices2
 */
void Rebouchage::recollerModeleFusionne(std::vector<int> indices1, std::vector<int> indices2)
{
    int nbIndices1 = indices1.size();
    int nbIndices2 = indices2.size();
    std::vector<int> indices;
    indices.insert(indices.end(), indices1.begin(), indices1.end());
    indices.insert(indices.end(), indices2.begin(), indices2.end());
    Outil::ranger(indices);

    int i1, i2, i3, i4;
    float dmin;
    for (int i=0; i<nbIndices2; i++)
    {
        i1=indices2[i];
        if (i==(nbIndices2-1))
            i2=indices2[0];
        else
            i2=indices2[i+1];
        dmin = F_MIN_INIT;
        i3 = -1;
        for (int j=0; j<nbIndices1; ++j)
        {
            if (Geometrie::distance(mesh_->getVertex(i1), mesh_->getVertex(indices1[j])) < dmin)
            {
                dmin = Geometrie::distance(mesh_->getVertex(i1), mesh_->getVertex(indices1[j]));
                i3 = j;
            }
        }
        if (std::find(indices.begin(), indices.end(), indices1[i3])!=indices.end())
        {
            creerTriangles(i2, i1, indices1[i3], 2.f);
            indices.push_back(indices1[i3]);
        }
        dmin = F_MIN_INIT;
        i4 = -1;
        for (int j=0; j<nbIndices1; ++j)
        {
            if (Geometrie::distance(mesh_->getVertex(i2), mesh_->getVertex(indices1[j])) < dmin)
            {
                dmin = Geometrie::distance(mesh_->getVertex(i2), mesh_->getVertex(indices1[j]));
                i4 = j;
            }
        }
        if (i3!=i4 && std::find(indices.begin(), indices.end(), indices1[i4])!=indices.end())
        {
            creerTriangles(indices1[i3], indices1[i4], i2, 2.f);
            indices.push_back(indices1[i4]);
        }
        Outil::ranger(indices);
    }
    std::vector<int> indices_bis;
    for (int j=0; j<mesh_->getNbVertices(); j++)
        indices_bis.push_back(j);

    initTrous();
    reboucherTrous(15.f, 3.f, indices_bis);
    indices = mesh_->getVerticesAPartirDeTriangles(mesh_->getTrianglesAPartirDeVertices(indices));
    initTrous();
    reboucherTrous(15.f, 3.f, indices_bis);

    initTrous();
    Outil::ranger(indices);
    indices = mesh_->getVerticesAPartirDeTriangles(mesh_->getTrianglesAPartirDeVertices(indices));
    Morphing morphing;
    morphing.setMesh(mesh_);
    morphing.setIntensite(1.f);
    morphing.lisserMeshXFois(indices, 1);
}
