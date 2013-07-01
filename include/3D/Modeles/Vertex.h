#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <QVector3D>
#include <QtOpenGL>
#include <vector>
#include "Outil.h"

class Mesh;

/**
 * La classe Vertex représente un point d'un Mesh
 * @author ToothPIC
 */
class Vertex : public QVector3D
{

public:
    Vertex(float x=0,float y=0,float z=0,int id=-1);
    Vertex(const QVector3D &vec,int id=-1);
    Vertex(const Vertex &vec);
    ~Vertex();
    std::vector<int> getIndicesTriangles() const;
    void setIndicesTriangles(const std::vector<int> &triangles);
    void ajouterTriangle(int iTri);
    void supprimerTriangle(int iTri);
    void setDistanceMax(float distMax);
    float getDistanceMax() const;
    QVector3D getNormale() const;
    void setNormale(const QVector3D &normale);
    int getId() const;
    void setId(int id);
    void dessiner(float, float, QColor);
    QVector3D getNormaleMoyenne(Mesh *mesh);
private:
    /** Identifiant du vertex */
    int id_;
    /** Indices des triangles liés à ce vertex */
    std::vector<int> indicesTriangles_;
    /** Distance du vertex du cercle de rayon minimum circonscrit à tous
    * les vertices des triangles voisins  */
    float distanceMax_;
    /** Normale associée au vertex */
    QVector3D normale_;
};

bool operator<(const Vertex &a,const Vertex &b);

#endif /*__VERTEX_H__*/
