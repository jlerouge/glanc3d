#define NOMINMAX
#include "Camera.h"

/**
 * Constructeur de Camera
 *
 * @param tailleFenetre
 */
Camera::Camera():mode_(SPHERE), rot_(), positionSourisNormalisee_(), tailleFenetre_(),
    angleX_(0), angleY_(0), transX_(0), transY_(0), zoom_(20), ortho_(true)
{}

/**
 * Destructeur de Camera
 *
 */
Camera::~Camera()
{}

/**
 * Fixe la valeur de tailleFenetre_
 *
 * @param tailleFenetre
 */
void Camera::setTailleFenetre(const QSizeF &tailleFenetre)
{
    tailleFenetre_=tailleFenetre;
}

/**
 * Charge la matrice de projection
 *
 */
void Camera::majProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio=(GLdouble)tailleFenetre_.width()/tailleFenetre_.height();
    if (ortho_)
        glOrtho(-ratio*zoom_, ratio*zoom_, -zoom_, zoom_, 0, 1000);
    else
        gluPerspective(70, ratio, 0.1, 1000);
    glViewport(0, 0, tailleFenetre_.width(), tailleFenetre_.height());
    glMatrixMode(GL_MODELVIEW);
}

/**
 * Effectue un demi-tour avec la caméra
 *
 */
void Camera::demiTour()
{
    if (mode_==SPHERE)
        rot_ =QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 180)*rot_;
    else if (mode_==PLAN)
        rot_ =QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 180)*rot_;
    else
        angleX_ = fmodf(angleX_+180.f, 360.f);
}


/**
 * Fixe la valeur de ortho_
 *
 * @param ortho
 */
void Camera::setOrtho(bool ortho)
{
    ortho_=ortho;
}

/**
 * Renvoie la valeur de ortho_
 *
 * @return ortho
 */
bool Camera::getOrtho()
{
    return ortho_;
}

/**
 * Renvoie la valeur de mode_
 *
 * @return CameraMode
 */
Camera::CameraMode Camera::getMode()
{
    return mode_;
}

/**
 * Renvoie la valeur de zoom_
 *
 * @return float
 */
float Camera::getZoom()
{
    return zoom_;
}

/**
 * Fixe le type de caméra utilisée
 *
 * @param mode
 */
void Camera::setMode(CameraMode mode)
{
    angleX_=0;
    angleY_=0;
    rot_=QQuaternion();
    mode_=mode;
}

/**
 * Fixe le type de caméra utilisée
 *
 * @param positionSouris
 * @param tailleFenetre
 */
void Camera::debuter(const QPointF& positionSouris)
{
    positionSourisNormalisee_=Geometrie::sourisNormalisee(positionSouris, tailleFenetre_);
}

/**
 * Met à  jour le quaternion représentant la rotation grâce aux nouvelles coordonnées de la souris
 *
 * @param positionSouris
 */
void Camera::tourner(const QPointF& positionSouris)
{
    QPointF positionSourisNormalisee=Geometrie::sourisNormalisee(positionSouris, tailleFenetre_);
    if (mode_==SPHERE)
    {
        QVector3D pointSphereAvant = Geometrie::sourisSurSphereUnite(positionSourisNormalisee_);
        QVector3D pointSphereApres = Geometrie::sourisSurSphereUnite(positionSourisNormalisee);
        float angle=acosf(QVector3D::dotProduct(pointSphereAvant, pointSphereApres));
        QVector3D axe = QVector3D::crossProduct(pointSphereAvant, pointSphereApres);
        rot_ =QQuaternion::fromAxisAndAngle(axe, angle*180/M_PI)*rot_;
    }
    else if (mode_==PLAN)
    {
        QLineF diff(positionSourisNormalisee_, positionSourisNormalisee);
        QVector3D axe = QVector3D(-diff.dy(), diff.dx(), 0.0f).normalized();
        rot_ = QQuaternion::fromAxisAndAngle(axe, diff.length()*180/M_PI)*rot_;
    }
    else
    {
        angleX_-=(positionSourisNormalisee.y()-positionSourisNormalisee_.y())*50.f;
        angleX_ = fmodf(angleX_, 360.f);
        if ((angleX_<-90.f && angleX_>-270)||(angleX_>90.f && angleX_<270))
            angleY_-=(positionSourisNormalisee.x()-positionSourisNormalisee_.x())*50.f;
        else
            angleY_+=(positionSourisNormalisee.x()-positionSourisNormalisee_.x())*50.f;
        angleY_ = fmodf(angleY_, 360.f);
    }
    positionSourisNormalisee_ = positionSourisNormalisee;
}

/**
 * Translate la caméra
 *
 * @param translationSouris
 */
void Camera::translater(const QPointF& deltaSouris)
{
    transX_-=(deltaSouris.x())*zoom_/800.f;
    transY_+=(deltaSouris.y())*zoom_/800.f;
}

/**
 * Zoom la caméra avec la molette
 *
 * @param deltaMolette
 */
void Camera::zoomer(float deltaMolette)
{
    zoom_ = std::max(1.f, zoom_-deltaMolette/100.f);
    if (ortho_)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float ratio = (GLdouble)tailleFenetre_.width()/tailleFenetre_.height();
        glOrtho(-ratio*zoom_, ratio*zoom_, -zoom_, zoom_, 0, 1000);
        glMatrixMode(GL_MODELVIEW);
    }
}

/**
 * Fait tourner la caméra
 *
 */
void Camera::regarder()
{
    if (ortho_)
        gluLookAt(transX_, transY_, 500, transX_, transY_, 0, 0, 1, 0);
    else
        gluLookAt(transX_, transY_, zoom_, transX_, transY_, 0, 0, 1, 0);
    if (mode_!=XY)
    {
        QMatrix4x4 matRot;
        matRot.rotate(rot_);
        float *dataMat = matRot.data();
        GLfloat matriceArray[16];
        for (int i= 0; i < 16; i++)
            matriceArray[i] = dataMat[i];
        glMultMatrixf(matriceArray);
    }
    else
    {
        glRotatef(angleX_, 1, 0, 0);
        glRotatef(angleY_, 0, 1, 0);
    }
}

/**
 * Renvoie la valeur de transX_
 *
 * @return float
 */
float Camera::getTransX()
{
    return transX_;
}

/**
 * Renvoie la valeur de transY_
 *
 * @return float
 */
float Camera::getTransY()
{
    return transY_;
}
