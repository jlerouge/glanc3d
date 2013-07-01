#ifndef __AXE_H__
#define __AXE_H__

#include <vector>
#include <algorithm>
#include <QtOpenGL>
#include <GL/glu.h>

/**
 * La classe Axe représente un axe
 * @author ToothPIC
 */
class Axe
{
public:
    Axe();
    ~Axe();
    void dessiner();
    void setVertex1(QVector3D vertex1);
    void setVertex2(QVector3D vertex2);
    QVector3D getVertex1();
    QVector3D getVertex2();

protected:
    /** Le premier vertex (au fond du puits) */
    QVector3D vertex1_;
    /** Le second vertex (en l'air, au niveau du bord) */
    QVector3D vertex2_;
};

#endif /*__AXE_H__*/
