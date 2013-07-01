#ifndef __OCTREE_H__
#define __OCTREE_H__

#include <vector>
#include <set>
#include <algorithm>
#include "Outil.h"
#include "Geometrie.h"
#include "Aabb.h"
#include "Constantes.h"

/**
 * La classe Octree représente un octree (arbre avec 8 fils).
 * Les noeuds de cet arbre sont des aabb qui sont subdivisés en 8 aabb.
 * Les vertices du modèle sont contenus au niveau des feuilles de l'octree.
 * @author ToothPIC
 */

class Mesh;
class Octree : public Aabb
{
public:
    Octree(Octree* parent=0,int profondeur=0,int profondeurMax=4,int maxVertices=25);
    ~Octree();
    std::set<Vertex*>& getVertices();
    int getNbVerticesTotal();
    void creer(std::vector<Vertex> &vertices,const Aabb &aabb);
    std::vector<int> getVerticesVoisins(const Vertex &vert) const;
    std::vector<int> getVerticesIntersectionRayon(const QVector3D &, const QVector3D &) const;
    std::vector<int> getVerticesIntersectionCylindre(const QVector3D &,
                                                     const QVector3D &, float) const;
    std::vector<int> getVerticesIntersectionSphere(const QVector3D &, float,
                                                   const QVector3D &) const;
    void dessiner() const;
    void supprimerVertex(Vertex &vert);
    bool rajouterVertex(Vertex &vert);
    Vertex* getVertexLePlusProche(const QVector3D &vert, bool premierAppel=true);
    Vertex* getVertexProche(const QVector3D &vert);
    std::vector<int> collision(Octree *octree2, Mesh *mesh1, Mesh *mesh2);
    bool collisionVertexDansAabb(const QVector3D &v, double coef = 0.15f);
    void changerTailleOctree(float facteur);
    void majNoeuds();
    std::vector<int> getVerticesIntersectionErosion(const QVector3D, float = 0, int indiceVertex = 0) const;


private:
    void creer(const std::set<Vertex*> &vertices,const Aabb &aabb);

    void constructionNoeuds();
    std::vector<int> getVerticesIntersection(const QVector3D*, float = 0, const QVector3D* = NULL,
                                             const QVector3D* = NULL) const;

    /** Noeud parent ,la valeur est nulle si on est à  la racine */
    Octree* parent_;
    /** Tableau des 8 fils ,la valeur est nulle si on est sur une feuille */
    Octree *fils_[8];
    /** Un ensemble des vertices associé au noeud courant, l'ensemble est vide si le noeud
    * courant n'est pas une feuille*/
    std::set<Vertex*> vertices_;
    /** Nombre maximal de vertices  par noeud */
    int nbVerticesMax_;
    /** Profondeur du noeud */
    int profondeur_;
    /** Profondeur maximale de l'arbre */
    int profondeurMax_;
};

#endif /*__OCTREE_H__*/
