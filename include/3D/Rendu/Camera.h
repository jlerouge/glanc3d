#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <GL/glu.h>
#include <QtOpenGL>
#include <Geometrie.h>


/**
 * La classe Camera sert à gérer la caméra
 * @author ToothPIC
 */
class Camera
{

public:
    enum CameraMode{SPHERE,PLAN,XY};
    Camera();
    ~Camera();
    void setTailleFenetre(const QSizeF &tailleFenetre);
    void setOrtho(bool ortho);
    bool getOrtho();
    CameraMode getMode();
    void setMode(CameraMode mode);
    float getZoom();
    float getTransX();
    float getTransY();
    void debuter(const QPointF& positionSouris);
    void tourner(const QPointF& positionSouris);
    void translater(const QPointF& deltaSouris);
    void zoomer(float delta);
    void regarder();
    void majProjection();
    void demiTour();

private:
    /** Le mode de la caméra */
    CameraMode mode_;
    /** Le quaternion représentant la rotation */
    QQuaternion rot_;
    /** La position de la souris sur l'écran entre -1 et 1 */
    QPointF positionSourisNormalisee_;
    /** Taille de la fenetre de la vue OpenGL */
    QSizeF tailleFenetre_;
    /** Angle de rotation en X pour la caméra en XY */
    float angleX_;
    /** Angle de rotation en Y pour la caméra en XY */
    float angleY_;
    /** Translation de la caméra en X */
    float transX_;
    /** Translation de la caméra en Y */
    float transY_;
    /** Le zoom */
    float zoom_;
    /** Si l'on a affaire à   une projection orthographique (sinon perspective) */
    bool ortho_;
};

#endif /*__CAMERA_H__*/
