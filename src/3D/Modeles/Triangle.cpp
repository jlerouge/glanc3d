#include "Triangle.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Constructeur de Triangle par défaut
 *
 */
Triangle::Triangle():id_(0), indicesVertices_()
{}

/**
 * Constructeur de Triangle avec arguments
 *
 * @param n
 * @param iVer1
 * @param iVer2
 * @param iVer3
 * @param id
 */
Triangle::Triangle(QVector3D n, int iVer1, int iVer2, int iVer3, int id):
    id_(id), indicesVertices_(), normale_(n)
{
//    indicesVertices_ = (int*)malloc(sizeof(int)*3);
    indicesVertices_[0]=iVer1;
    indicesVertices_[1]=iVer2;
    indicesVertices_[2]=iVer3;
}

/**
 * Destructeur de Triangle
 *
 */
Triangle::~Triangle()
{
    //free(indicesVertices_);
}

/**
 * Renvoie l'indice correspondant au ième vertex du triangle
 *
 * @param i
 * @return int
 */
int Triangle::getIndiceVertex(int i) const
{
    switch (i)
    {
        case 1 : return indicesVertices_[0];
        case 2 : return indicesVertices_[1];
        case 3 : return indicesVertices_[2];
        default: return -1;
    }
}

/**
 * Renvoie la valeur de normale_
 *
 * @return QVector3D
 */
QVector3D Triangle::getNormale() const
{
    return normale_;
}

/**
 * Fixe la valeur de normale_
 *
 * @param normale
 */
void Triangle::setNormale(const QVector3D &normale)
{
    normale_=normale;
}

/**
 * Renvoie l'identifiant du triangle
 *
 * @return int
 */
int Triangle::getId() const
{
    return id_;
}

/**
 * Fixe la valeur de l'identifiant de id_
 *
 * @param id
 */
void Triangle::setId(int id)
{
    id_=id;

}

/**
 * Inverse le sens du triangle
 *
 */
void Triangle::inverserNormale()
{
    int temp=indicesVertices_[0];
    indicesVertices_[0]=indicesVertices_[1];
    indicesVertices_[1]=temp;
    normale_=-normale_;
}
