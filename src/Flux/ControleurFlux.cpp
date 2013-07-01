#include "ControleurFlux.h"

/**
 * Constructeur de ControleurFlux
 *
 */
ControleurFlux::ControleurFlux()
{}

/**
 * Destructeur de ControleurFlux
 *
 */
ControleurFlux::~ControleurFlux()
{}

/**
 * Charge un fichier depuis son chemin et initialise les VBO
 *
 * @param cheminFichier
 * @param fenetre
 * @return Mesh
 */
Mesh* ControleurFlux::charger(const QString &cheminFichier, FenetrePrincipale *fenetre)
{
    if (fenetre)
        fenetre->statusBar()->showMessage("Chargement : 1/5 (Suppression doublons)");
    Mesh* mesh = new Mesh();
    std::ifstream fichier(cheminFichier.toStdString().c_str(), std::ios::binary);
    if (fichier)
    {
        mesh->setNomMesh(cheminFichier.split("/").last());
        std::set<Vertex> setVertices; //Pour les doublons ...
        char enTete[80];
        int nbTrianglesFichier;
        fichier.read(enTete, 80); //En tete
        fichier.read((char*)&nbTrianglesFichier, 4); //Nb triangles
        mesh->setNbTriangles(nbTrianglesFichier);
        std::vector<Triangle> &triangles=mesh->getTriangles();
        int nbTriangles=mesh->getNbTriangles();
        int iVer1, iVer2, iVer3;
        float x, y, z;
        Vertex v1, v2, v3;
        Triangle t;
        QVector3D v;
        for (int i=0;i<nbTriangles;++i)
        {
            fichier.read((char*)&x, 12); //normale
            fichier.read((char*)&x, 4); //vertex 1
            fichier.read((char*)&y, 4);
            fichier.read((char*)&z, 4);
            v1=Vertex(x, y, z, setVertices.size());
            iVer1=mesh->rajouterVertexSansDoublons(v1, i, setVertices);
            fichier.read((char*)&x, 4); //vertex 2
            fichier.read((char*)&y, 4);
            fichier.read((char*)&z, 4);
            v2=Vertex(x, y, z, setVertices.size());
            iVer2=mesh->rajouterVertexSansDoublons(v2, i, setVertices);
            fichier.read((char*)&x, 4); //vertex 3
            fichier.read((char*)&y, 4);
            fichier.read((char*)&z, 4);
            v3=Vertex(x, y, z, setVertices.size());
            iVer3=mesh->rajouterVertexSansDoublons(v3, i, setVertices);
            fichier.read((char*)&x, 2); //attribut
            v = QVector3D::normal(v1, v2, v3);
            t = Triangle(v, iVer1, iVer2, iVer3, i);


            triangles.push_back(t);
            if (fenetre)
                fenetre->getBarreProgression()->setValue(100.f*(float)i/nbTriangles);
        }
        fichier.close();
        mesh->setNbVertices(setVertices.size());
        mesh->initDistanceVertexMaxEtCentreMeshEtOctree(fenetre);
        mesh->initVBO(fenetre);
        mesh->deplacerEn(QVector3D(0, 0, 0));
    }
    if (fenetre)
        fenetre->statusBar()->showMessage("CTD InlayCore");
    return mesh;
}

/**
 * Sauvegarde le mesh dans un fichier STL binaire
 *
 * @param mesh
 * @param cheminFichier
 * @param fenetre
 */
void ControleurFlux::sauvegarder(Mesh *mesh,
                                 const QString &cheminFichier,
                                 FenetrePrincipale *fenetre)
{
    if (fenetre)
        fenetre->statusBar()->showMessage("Sauvegarde : 1/1 (en cours)");
    std::ofstream fichier(cheminFichier.toStdString().c_str(), std::ios::binary);
    if (fichier)
    {
        std::vector<Triangle> &triangles=mesh->getTriangles();
        std::vector<Vertex> &vertices=mesh->getVertices();
        char enTete[80];
        fichier.write(enTete, 80);
        int nbTriangles=mesh->getNbTriangles();
        fichier.write((char*)&nbTriangles, 4);
        for (int i=0;i<nbTriangles;i++)
        {
            Triangle &tri=triangles[i];
            QVector3D normale=tri.getNormale();
            normale=mesh->getTransformation()*normale;
            float xTemp=normale.x();;
            float yTemp=normale.y();
            float zTemp=normale.z();
            fichier.write((char*)&xTemp, 4); //normale
            fichier.write((char*)&yTemp, 4);
            fichier.write((char*)&zTemp, 4);
            Vertex v1=vertices[tri.getIndiceVertex(1)];
            v1=mesh->getTransformation()*v1;
            xTemp=v1.x();;
            yTemp=v1.y();
            zTemp=v1.z();
            fichier.write((char*)&xTemp, 4); //vertex 1
            fichier.write((char*)&yTemp, 4);
            fichier.write((char*)&zTemp, 4);
            Vertex v2=vertices[tri.getIndiceVertex(2)];
            v2=mesh->getTransformation()*v2;
            xTemp=v2.x();;
            yTemp=v2.y();
            zTemp=v2.z();
            fichier.write((char*)&xTemp, 4); //vertex 2
            fichier.write((char*)&yTemp, 4);
            fichier.write((char*)&zTemp, 4);
            Vertex v3=vertices[tri.getIndiceVertex(3)];
            v3=mesh->getTransformation()*v3;
            xTemp=v3.x();;
            yTemp=v3.y();
            zTemp=v3.z();
            fichier.write((char*)&xTemp, 4); //vertex 3
            fichier.write((char*)&yTemp, 4);
            fichier.write((char*)&zTemp, 4);
            char attribut[2];
            fichier.write((char*)attribut, 2); //attribut
            fenetre->getBarreProgression()->setValue(100.f*(float)i/nbTriangles);
        }
        fichier.close();
    }
    fenetre->statusBar()->showMessage("CTD InlayCore");
}
