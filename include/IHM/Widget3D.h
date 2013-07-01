#ifndef __WIDGET3D_H__
#define __WIDGET3D_H__

#include "Camera.h"
#include "ControleurFlux.h"
#include "FenetrePrincipale.h"
#include "Selection.h"
#include "Morphing.h"
#include "Transformation.h"
#include "Axe.h"
#include "Dessin.h"
#include "Rebouchage.h"
#include "Reduction.h"
#include <QTime>

/**
 * La classe Widget3D gère une zone d'affichage 3D
 * @author ToothPIC
 */
class Widget3D : public QGLWidget
{
    Q_OBJECT

public:
    Widget3D(FenetrePrincipale::TypeWidget type, FenetrePrincipale *fenetrePrincipale, QGLWidget *widgetPartage=0);
    ~Widget3D();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void resizeGL(int largeur, int longueur);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void setEstADessiner(bool estADessiner);
    void ajouterMesh(Mesh* mesh);
    void supprimerMesh(Mesh* mesh);
    int getNbMeshes();
    void resetMeshes();
    void dessinerFocus();
    FenetrePrincipale::TypeWidget getType();
    void setFocus(bool booleen);
    bool getFocus();
    void setMode(Selection::ModeSelection mode);

signals:
    void clique();

public slots:
    void chargerFichier();
    void sauvegarderFichier();
    void setRayonSelection(int rayon);
    void setPuissanceDeform(int puissance);
    void setSensDeformPlus();
    void setSensDeformMoins();
    void setLissage();
    void setSculptage();
    void setModeCamera();
    void setRotation();
    void setTranslation();
    void setReduction();
    void setMeshShader(int type);
    void setCamera(int type);
    void setOctree();
    void setCouleurMesh();
    void setProjection();
    void demiTour();

protected:
    void initializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void initialiserLumieres();
    void vue2D();
    void vue3D();

    /** Le mesh sélectionné */
    Mesh *mesh_;
    /** Les meshes à  l'écran*/
    std::vector<Mesh*> meshes_;
    /** La camera */
    Camera camera_;
    /** La zone de sélection */
    Selection selection_;
    /** Pour le morphing */
    Morphing morphing_;
    /** Pour les transformations */
    Transformation transformation_;
    /** La position de la souris sur l'écran */
    QPointF positionSouris_;
    /** La fenêtre principale */
    FenetrePrincipale *fenetrePrincipale_;
    /** Si l'on est en mode transformation (rotation/translation mesh) ou caméra */
    bool modeTransformation_;
    /** Si l'on doit réactiver la caméra */
    bool modeCamera_;
    /** Si le widget3D doit recevoir ou non les signaux */
    bool focus_;
    /** Si le widget3D doit se dessiner */
    bool estADessiner_;
    /** Le type de widget */
    FenetrePrincipale::TypeWidget type_;
    /** Un chronométre pour déclencher les événements de la souris */
    QTime chrono_;
};
#endif /*__WIDGET3D_H__*/
