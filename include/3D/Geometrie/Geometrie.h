#ifndef __GEOMETRIE_H__
#define __GEOMETRIE_H__

#include <QVector3D>
#include <QtOpenGL>
#include <GL/glu.h>
#include "Forme2D.h"
#include "Vertex.h"
class Forme2D;
class Mesh;

/**
 * Le namespace Geometrie permet de faire des opérations géométriques
 * @author ToothPIC
 */
namespace Geometrie
{
    float distance(const QVector3D &v1,const QVector3D &v2);
    float angle(const QVector3D &v1, const QVector3D & v2);
    bool intersectionTriangle(const QVector3D &s1,const QVector3D &s2,const QVector3D &v1,
                              const QVector3D &v2,const QVector3D &v3,const QVector3D &normale,
                              QVector3D &vertInter);
    QVector3D sourisSurSphereUnite(const QPointF &positionSouris);
    QPointF sourisNormalisee(const QPointF &positionSouris, const QSizeF tailleFenetre);
    void calculerRayonEcranSouris(const QPointF &positionSouris,const QSizeF &tailleFenetre, QVector3D &vertexPres,QVector3D &vertexLoin);
    QVector3D calculerVecteurEcranPlanCamera(const QPointF &positionSourisOld,const QPointF &positionSourisNew,const QSizeF &tailleFenetre);
    QVector3D point2Dto3D(const QPointF &positionSouris,const QSizeF &tailleFenetre, float z=0.0f);
    QVector3D vertexSurRayonLePlusProche(const QVector3D &vertex,const QVector3D &vertexPres,const QVector3D &vertexLoin);
    float distanceMeshVertices(Mesh *mesh,const std::vector<QVector3D> &vertices,const QMatrix4x4 &matTrans);
    float distanceMeshVerticesTrans(Mesh *mesh, const std::vector<QVector3D> &vertices, const QMatrix4x4 &matTrans);
    QVector3D getVecteurPlan(const QVector3D &vec);
    QPointF point3Dto2D(QVector3D event,const QSizeF &tailleFenetre);
    QPointF point3Dto2DForme(QVector3D event, Forme2D *forme);
    int insidePolygon(std::vector<QPointF> polygon,int N,QPointF p);
    float distance2D(const QPointF a, QPointF b);
    bool collisionMesh(Mesh *mesh1, Mesh *mesh2);
    float distanceCourbe(Mesh *mesh, int indice1, int indice2);
    void subdiviser(Mesh *mesh, std::vector<int> &vertices, int i);
}

#endif /*__GEOMETRIE_H__*/
