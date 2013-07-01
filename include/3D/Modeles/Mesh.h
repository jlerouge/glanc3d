#ifndef __MESH_H__
#define __MESH_H__

#include <string>
#include <QtOpenGL>
#include <algorithm>
#include <stdint.h>
#include "Outil.h"
#include "Octree.h"
#include "Triangle.h"
#include "Vertex.h"
#include "FenetrePrincipale.h"
#include "Micropather.h"

/**
 * La classe Mesh représente un mesh
 * @author ToothPIC
 */
class Mesh
{

public:
    enum ShaderMode{FLAT, NORMAL, PHONG, TRANSPARENCE, FIL_DE_FER, FIL_DE_FER_TRANSPARENT, VERTEX};
    Mesh();
    Mesh(std::vector<int> verticesSelectionnes, Mesh *mesh);
    ~Mesh();
    std::vector<Vertex>& getVertices();
    std::vector<Triangle>& getTriangles();
    Vertex& getVertex(int i);
    Triangle& getTriangle(int i);
    int getNbTriangles() const;
    int getNbVertices() const;
    QVector3D getCentre() const;
    QColor getCouleur();
    QVector3D getCentreTriangle(int iTri);
    Octree* getOctree();
    Mesh* supprimerVertices(std::vector<int>);

    void setShader(ShaderMode);
    ShaderMode getShader();
    void setCouleur(const QColor &couleur);
    void setNbTriangles(int nbTriangles);
    void setNbVertices(int nbVertices);
    void changerAffichageOctree();
    bool getAffichageOctree();

    void dessiner(bool contour=false);
    void dessinerContour();
    int rajouterVertexSansDoublons(Vertex &v, int iTri, std::set<Vertex> &vertices);
    void initVBO(FenetrePrincipale *fenetre=0);
    void initDistanceVertexMaxEtCentreMeshEtOctree(FenetrePrincipale *fenetre=0);

    void homothetie(float x);
    void deplacerEn(const QVector3D &destination, bool identity = true);
    void setTransformation(QMatrix4x4 matTransform);
    void autoriserEdition(bool estAutorise);
    bool estEditable();
    QMatrix4x4 getTransformation() const;
    QMatrix4x4 getTransformationInverse() const ;

    std::vector<int> getVerticesLiesAVertex(int iVert, int degre = 0) const;
    std::vector<int> getTrianglesLiesATriangle(int iTri) const;
    std::vector<int> getTrianglesAPartirDeVertices(const std::vector<int> &iVerts) const;
    std::vector<int> getVerticesAPartirDeTriangles(const std::vector<int> &iTris) const;

    void setXYZ(int i, float x, float y, float z);
    void setXYZ(int i, const QVector3D &nouveau);
    void majNormalesEtDistanceMaxEtOctree(const std::vector<int> &iTris);
    void setNomMesh(QString nom);
    std::vector<int> indicesPointsAberrants();

    QString getNomMesh();
    void inverserNormales();
    void setSelectionne(bool);

    void concatener(Mesh *mesh);
    void propagationMesh(
            int iVert, std::set<int> &indicesTenon, std::set<int> &indicesBord);


private:
    void majTransformation();
    void majArrayNormale(int iVer);
    void majNormale(int iTri);
    void majDistanceVertexMax(int iVer);
    void dessinerBuffer();
    void dessinerBufferShader();
    /** Nombre de triangles dans le mesh */
    int nbTriangles_;
    /** Nombre de vertices dans le mesh */
    int nbVertices_;
    /** Les vertices du mesh */
    std::vector<Vertex> vertices_;
    /** Les triangles du mesh */
    std::vector<Triangle> triangles_;
    /** Le buffer VBO pour les vertices */
    QGLBuffer verticesBuffer_;
    /** Le buffer VBO pour les vertices */
    QGLBuffer normalesBuffer_;
    /** Le buffer VBO pour les indices */
    QGLBuffer indicesBuffer_;
    /** Le centre du mesh */
    QVector3D centre_;
    /** Le shader utilisé */
    ShaderMode shaderType_;
    /** Le programme shader */
    QGLShaderProgram programmeShader_;
    /** La couleur du mesh */
    QColor couleur_;
    /** La racine de l'octree du mesh */
    Octree *octree_;
    /** Si l'octree doit être affiché ou non */
    bool affichageOctree_;
    /** Lieu de stockage sur la carte graphique de l'attribut vertex pour les shader */
    QMatrix4x4 matTransform_;
    /** La matrice de transformation du mesh pour OpenGL */
    GLfloat matTransformArray_[16];
    /** Le nom du mesh */
    QString nomMesh_;
    /** Si le mesh est sélectionné ou non */
    bool estSelectionne_;
    /** Si le mesh est éditable (morphing, transformations) ou non */
    bool estEditable_;

};

#endif /*__MESH_H__*/
