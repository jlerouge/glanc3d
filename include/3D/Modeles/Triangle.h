#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <QVector3D>
#include <vector>
#include <iostream>

/**
 * La classe Triangle représente un triangle d'un Mesh
 * @author ToothPIC
 */
class Triangle
{

public:
    Triangle();
    Triangle(QVector3D n,int iVer1=-1,int iVer2=-1,int iVer3=-1,int id=-1);
    ~Triangle();
    int getIndiceVertex(int i) const;
    QVector3D getNormale() const;
    void setNormale(const QVector3D &normale);
    int getId() const;
    void setId(int id);
    void inverserNormale();

private:
    /** Identifiant du triangle */
    int id_;
    /** Indices des vertices */
    int indicesVertices_[3];
    /** Normale associée au triangle */
    QVector3D normale_;
};

#endif /*__TRIANGLE_H__*/
