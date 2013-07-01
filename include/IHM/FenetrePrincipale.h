#ifndef __FENETRE_PRINCIPALE_H__
#define __FENETRE_PRINCIPALE_H__

#include <QtGui>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QCheckBox>

class Widget3D;

/**
 * La classe FenetrePrincipale gère l'interface graphique dans son ensemble
 * @author ToothPIC
 */
class FenetrePrincipale : public QMainWindow
{
    Q_OBJECT

    public:
        enum TypeWidget{DEFORMATION, INDEFINI};
        FenetrePrincipale();
        ~FenetrePrincipale();
        QProgressBar* getBarreProgression();
        QPushButton* getPaletteCouleur();
        QComboBox* getChoixCamera();
        QPushButton* getProjection();
        QComboBox* getChoixShader();
        QPushButton* getOctree();
        QLabel* getNomMesh();
        void setWidgetFocus(TypeWidget type);
        bool getContour();

    protected:
        void keyPressEvent(QKeyEvent *event);

    private slots:
        void ouvrirOptions();
        void changerContour(bool estActive);
        void validerOptions();
        void quitter();

    private:
        void majOnglets();
        void initialiserInfoGenerale();
        void majInfoGenerale(QLabel *label);
        void initialiserPaletteDeformation();
        void initialiserFenetreDeplacement();
        void initialiserMenu();
        void initialiserStatusBar();
        void cacherPalettes();

        /** La première zone d'affichage 3D du mode deformation */
        Widget3D *widgetDeformation_;
        /** La palette contenant les outils de morphing */
        QDockWidget *paletteDeformation_;
        /** La palette contenant les fonctionnalités de la scène */
        QDockWidget *fenetreDeplacement_;
        /** La palette pour l'information générale */
        QDockWidget *infoGenerale_;
        /** Le slider pour changer la puissance du morphing */
        QProgressBar *barreProgression_;
        /** Le bouton pour changer la couleur */
        QPushButton *paletteCouleur_;
        /** Pour connaitre le type de camèra */
        QComboBox *choixCamera_;
        /** Pour changer de mode de projection */
        QPushButton *projection_;
        /** Pour effectuer un demi-tour avec la camèra */
        QPushButton *demiTour_;
        /** Le boutton pour changer de shader */
        QComboBox *choixShader_;
        /** Le boutton pour afficher l'octree */
        QPushButton *octree_;
        /** Le nom du mesh */
        QLabel *nomMesh_;
        /** Le panneau pour choisir les options générales */
        QDialog *panneauOptions_;
        /** Pour l'activation des contours de mesh */
        QCheckBox *activationContour_;
        /** Valeur du contour avant validation ou non de la fenêtre de dialogue */
        bool contourTemp_;
        /** Si le contour est activé ou non */
        bool contour_;
        /** Bouton pour changer le type de caméra */
        QPushButton *modeCamera_;
        /** Bouton pour lancer la rotation */
        QPushButton *pivoter_;
        /** Bouton pour lancer la translation */
        QPushButton *translater_;
        /** Bouton pour lancer la réduction */
        QPushButton *reduire_;
};
#endif /*__FENETRE_PRINCIPALE_H__*/
