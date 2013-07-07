#include "FenetrePrincipale.h"
#include "Widget3D.h"
#include "Rebouchage.h"
#include "Constantes.h"

/**
 * Constructeur de FenetrePrincipale
 *
 */
FenetrePrincipale::FenetrePrincipale(): paletteDeformation_(0), barreProgression_(0),
    paletteCouleur_(0), projection_(0), choixShader_(0), octree_(0), nomMesh_(0),
    panneauOptions_(0), activationContour_(0), contourTemp_(false), contour_(true)
{
    setWindowTitle(tr("glanc3d"));
    setWindowIcon(QIcon("ressources/images/glanc3d_icon.png"));

    widgetDeformation_ = new Widget3D(DEFORMATION, this);
    widgetDeformation_->setEstADessiner(true);

    setCentralWidget(widgetDeformation_);

    initialiserPaletteDeformation();
    initialiserFenetreDeplacement();
    initialiserMenu();
    initialiserStatusBar();
}

/**
 * Destructeur de FenetrePrincipale
 *
 */
FenetrePrincipale::~FenetrePrincipale()
{
    delete widgetDeformation_;
    delete paletteDeformation_;
}

/**
 * Initialise la palette d'outils
 *
 */
void FenetrePrincipale::initialiserPaletteDeformation()
{
    paletteDeformation_=new QDockWidget("Déformation", this);
    addDockWidget(Qt::LeftDockWidgetArea, paletteDeformation_);

    QPushButton *ajouterMatiere = new QPushButton("Ajouter de la matière");
    ajouterMatiere->setCheckable(true);
    ajouterMatiere->setChecked(true);
    ajouterMatiere->setToolTip(QString("Ajouter de la matière"));

    QPushButton *enleverMatiere = new QPushButton("Enlever de la matière");
    enleverMatiere->setCheckable(true);
    enleverMatiere->setToolTip(QString("Enlever de la matière"));

    QPushButton *lisserMatiere = new QPushButton("Polir");
    lisserMatiere->setCheckable(true);
    lisserMatiere->setToolTip(QString("Lisser une zone de matière"));

    QPushButton *sculpter = new QPushButton("Sculpter");
    sculpter->setCheckable(true);
    sculpter->setToolTip(QString("Sculpter de la matière"));


    QSlider *sliderPuissance=new QSlider(Qt::Horizontal);
    sliderPuissance->setGeometry(10, 60, 100, 20);
    sliderPuissance->setRange(0, 200);
    sliderPuissance->setToolTip(QString("Puissance d'action"));


    QLCDNumber *lcdPuissance=new QLCDNumber();
    lcdPuissance->setSegmentStyle(QLCDNumber::Flat);

    QSlider *sliderRayon=new QSlider(Qt::Horizontal);
    sliderRayon->setGeometry(10, 60, 100, 20);
    sliderRayon->setRange(0, 200);
    sliderRayon->setValue(100);
    sliderRayon->setToolTip(QString("Rayon d'action"));


    QLCDNumber *lcdRayon=new QLCDNumber();
    lcdRayon->setSegmentStyle(QLCDNumber::Flat);
    lcdRayon->display(100);

    QButtonGroup *groupButtonMorphing = new QButtonGroup;
    groupButtonMorphing->addButton(ajouterMatiere);
    groupButtonMorphing->addButton(enleverMatiere);
    groupButtonMorphing->addButton(lisserMatiere);
    groupButtonMorphing->addButton(sculpter);

    QVBoxLayout *dockLayout = new QVBoxLayout;
    dockLayout->addWidget(ajouterMatiere);
    dockLayout->addWidget(enleverMatiere);
    dockLayout->addWidget(lisserMatiere);
    dockLayout->addWidget(sculpter);
    dockLayout->addWidget(new QLabel("Puissance :"));
    dockLayout->addWidget(sliderPuissance);
    dockLayout->addWidget(lcdPuissance);
    dockLayout->addWidget(new QLabel("Rayon :"));
    dockLayout->addWidget(sliderRayon);
    dockLayout->addWidget(lcdRayon);

    QWidget *contenuPaletteDeformation = new QWidget();
    contenuPaletteDeformation->setLayout(dockLayout);
    contenuPaletteDeformation->setFixedSize(contenuPaletteDeformation->sizeHint());

    paletteDeformation_->setWidget(contenuPaletteDeformation);
    paletteDeformation_->setFloating(false);
    //Sliders
    connect(sliderPuissance, SIGNAL(valueChanged(int)), lcdPuissance, SLOT(display(int)));
    connect(sliderPuissance, SIGNAL(valueChanged(int)), widgetDeformation_,
            SLOT(setPuissanceDeform(int)));
    connect(sliderRayon, SIGNAL(valueChanged(int)), lcdRayon, SLOT(display(int)));
    connect(sliderRayon, SIGNAL(valueChanged(int)), widgetDeformation_,
            SLOT(setRayonSelection(int)));

    //Deformation
    connect(ajouterMatiere, SIGNAL(clicked()), widgetDeformation_, SLOT(setSensDeformPlus()));
    connect(enleverMatiere, SIGNAL(clicked()), widgetDeformation_, SLOT(setSensDeformMoins()));
    connect(lisserMatiere, SIGNAL(clicked()), widgetDeformation_, SLOT(setLissage()));
    connect(sculpter, SIGNAL(clicked()), widgetDeformation_, SLOT(setSculptage()));
}

/**
 * Initialise le menu
 *
 */
void FenetrePrincipale::initialiserMenu()
{
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");

    QAction *actionNouveau = menuFichier->addAction("&Nouvelle commande");
    actionNouveau->setShortcut(QKeySequence("Ctrl+N"));
    actionNouveau->setIcon(QIcon(QDir::currentPath()+"/ressources/images/nouveau.png"));
    actionNouveau->setToolTip(QString("Ajouter une nouvelle commande"));

    QAction *actionOuvrir = menuFichier->addAction("&Ouvrir");
    actionOuvrir->setShortcut(QKeySequence("Ctrl+O"));
    actionOuvrir->setIcon(QIcon(QDir::currentPath()+"/ressources/images/ouvrir.png"));
    actionOuvrir->setToolTip(QString("Ouvrir un fichier"));

    QAction *actionSauvegarder = menuFichier->addAction("&Sauvegarder");
    actionSauvegarder->setShortcut(QKeySequence("Ctrl+S"));
    actionSauvegarder->setIcon(QIcon(QDir::currentPath()+"/ressources/images/sauvegarder.png"));
    actionSauvegarder->setToolTip(QString("Enregistrer le fichier"));

    QAction *actionQuitter = menuFichier->addAction("&Quitter");
    actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
    actionQuitter->setIcon(QIcon(QDir::currentPath()+"/ressources/images/quitter.png"));
    actionQuitter->setToolTip(QString("Quitter le logiciel"));

    QMenu *menuEdition = menuBar()->addMenu("&Edition");
    QAction *actionPreferences = menuEdition->addAction("&Préférences");
    actionPreferences->setShortcut(QKeySequence("Ctrl+P"));
    actionPreferences->setIcon(QIcon(QDir::currentPath()+"/ressources/images/preferences.png"));
    actionPreferences->setToolTip(QString("Préférences d'affichage"));

    panneauOptions_ = new QDialog;
    QVBoxLayout *optionsLayout = new QVBoxLayout;
    activationContour_ = new QCheckBox("Contour mesh");
    activationContour_->setChecked(true);
    activationContour_->setToolTip(QString("Visualiser le contour du mesh"));

    projection_=new QPushButton("Vision orthographique");
    projection_->setToolTip(QString("Changer le mode de vision"));

    octree_ = new QPushButton("Afficher l'octree");
    octree_->setToolTip(QString("Afficher les octrees"));

    choixCamera_ = new QComboBox();
    choixCamera_->setFocusPolicy(Qt::NoFocus);
    choixCamera_->addItem("Trackball (sphère)");
    choixCamera_->addItem("Trackball (plan)");
    choixCamera_->addItem("Caméra XY");
    choixCamera_->setToolTip(QString("Type de caméra"));
    optionsLayout->addWidget(activationContour_);
    optionsLayout->addWidget(projection_);
    optionsLayout->addWidget(octree_);
    optionsLayout->addWidget(choixCamera_);
    QDialogButtonBox *validerOptions = new QDialogButtonBox(QDialogButtonBox::Ok
                                                            | QDialogButtonBox::Cancel);
    optionsLayout->addWidget(validerOptions);
    panneauOptions_->setLayout(optionsLayout);

    QToolBar *toolBarFichier = addToolBar("Barre de raccourcis");
    toolBarFichier->addSeparator();
    toolBarFichier->addAction(actionNouveau);
    toolBarFichier->addSeparator();
    toolBarFichier->addAction(actionOuvrir);
    toolBarFichier->addSeparator();
    toolBarFichier->addAction(actionSauvegarder);
    toolBarFichier->addSeparator();
    toolBarFichier->setMovable(false);
    toolBarFichier->setStyleSheet(
                "QToolButton:pressed{color: lightGray; background-color: darkBlue;}");

    QMenu *menuAffichage = menuBar()->addMenu("&Affichage");
    if (paletteDeformation_)
    {
        paletteDeformation_->toggleViewAction()->setShortcut(QKeySequence("Alt+D"));
        menuAffichage->addAction(paletteDeformation_->toggleViewAction());
    }
    if (fenetreDeplacement_)
    {
        fenetreDeplacement_->toggleViewAction()->setShortcut(QKeySequence("Alt+S"));
        menuAffichage->addAction(fenetreDeplacement_->toggleViewAction());
    }

    QMenu *menuAide = menuBar()->addMenu("&Aide");
    QDialog *aPropos = new QDialog;
    QGridLayout *aProposLayout = new QGridLayout;
    QLabel *texteAPropos = new QLabel;
    texteAPropos->setText(QString("glanc3d - Révision %1\n"
                                  "Développé par :\n"
                                  " * Julien Lerouge (aka jujurackham) \n"
                                  " * Mehdi Alaoui (aka blitzfis) \n").arg(BUILD_REV));
    QLabel *imageGlanc3d = new QLabel;
    imageGlanc3d->setPixmap(QPixmap("ressources/images/glanc3d_small.png"));
    QLabel *imageTwitter = new QLabel;
    imageTwitter->setPixmap(QPixmap("ressources/images/tw_social.png"));
    QLabel *imageFacebook = new QLabel;
    imageFacebook->setPixmap(QPixmap("ressources/images/fb_social.png"));
    QLabel *imageGplus = new QLabel;
    imageGplus->setPixmap(QPixmap("ressources/images/g+_social.png"));

    aProposLayout->addWidget(imageGlanc3d, 0, 0);
    aProposLayout->addWidget(texteAPropos, 0, 1);
    aProposLayout->addWidget(imageTwitter, 1, 2);
    aProposLayout->addWidget(imageFacebook, 1, 3);
    aProposLayout->addWidget(imageGplus, 1, 4);
    aPropos->setLayout(aProposLayout);
    aPropos->setWindowIcon(QIcon(QDir::currentPath()+"/ressources/images/info.png"));

    QAction *actionAPropos = new QAction(tr("&A propos"), this);
    actionAPropos->setIcon(QIcon(QDir::currentPath()+"/ressources/images/info.png"));
    actionAPropos->setShortcut(QKeySequence("Ctrl+?"));
    actionAPropos->setToolTip(QString("Informations sur le logiciel"));
    menuAide->addAction(actionAPropos);

    connect(actionAPropos, SIGNAL(triggered()), aPropos, SLOT(exec()));
    connect(actionNouveau, SIGNAL(triggered()), this, SLOT(NULL));
    connect(actionOuvrir, SIGNAL(triggered()), widgetDeformation_, SLOT(chargerFichier()));
    connect(actionSauvegarder, SIGNAL(triggered()), widgetDeformation_,
            SLOT(sauvegarderFichier()));
    connect(actionQuitter, SIGNAL(triggered()), this, SLOT(quitter()));
    connect(actionPreferences, SIGNAL(triggered()), this, SLOT(ouvrirOptions()));
    connect(validerOptions, SIGNAL(accepted()), this, SLOT(validerOptions()));
    connect(validerOptions, SIGNAL(accepted()), panneauOptions_, SLOT(accept()));
    connect(validerOptions, SIGNAL(rejected()), panneauOptions_, SLOT(reject()));
    connect(activationContour_, SIGNAL(toggled(bool)), SLOT(changerContour(bool)));

    // Projection
    connect(projection_, SIGNAL(clicked()), widgetDeformation_, SLOT(setProjection()));

    //Octree
    connect(octree_, SIGNAL(clicked()), widgetDeformation_, SLOT(setOctree()));

    // Camera
    connect(choixCamera_, SIGNAL(activated(int)), widgetDeformation_, SLOT(setCamera(int)));
}

/**
 * Initialise la barre de statut
 *
 */
void FenetrePrincipale::initialiserStatusBar()
{
    barreProgression_=new QProgressBar(this);
    barreProgression_->setStyleSheet(
                QString("QProgressBar {"
                        "   border: 2px solid grey;"
                        "   border-radius: 5px;"
                        "   text-align: center;"
                        "}"
                        "QProgressBar::chunk {"
                        "   background-color: #C3D3E3;"
                        "}"));
    barreProgression_->setVisible(false);
    statusBar()->showMessage("CTD Inlay-core");
    statusBar()->addPermanentWidget(barreProgression_);
}

/**
 * Action à effectuer quand une touche est pressée
 *
 * @param event
 */
void FenetrePrincipale::keyPressEvent(QKeyEvent *event)
{

}

/**
 * Renvoie la valeur de barreProgression_
 *
 * @return QProgressBar
 */
QProgressBar* FenetrePrincipale::getBarreProgression()
{
    return barreProgression_;
}

/**
 * Renvoie la valeur de paletteCouleur_
 *
 * @return QPushButton
 */
QPushButton* FenetrePrincipale::getPaletteCouleur()
{
    return paletteCouleur_;
}

/**
 * Renvoie la valeur de choixCamera_
 *
 * @return QComboBox*
 */
QComboBox* FenetrePrincipale::getChoixCamera()
{
    return choixCamera_;
}

/**
 * Renvoie la valeur de projection_
 *
 * @return QPushButton
 */
QPushButton* FenetrePrincipale::getProjection()
{
    return projection_;
}

/**
 * Renvoie la valeur de choixShader_
 *
 * @return QComboBox
 */
QComboBox* FenetrePrincipale::getChoixShader()
{
    return choixShader_;
}

/**
 * Renvoie la valeur de octree_
 *
 * @return QPushButton
 */
QPushButton* FenetrePrincipale::getOctree()
{
    return octree_;
}

/**
 * Renvoie la valeur de nomMesh_
 *
 * @return QLabel
 */
QLabel* FenetrePrincipale::getNomMesh()
{
    return nomMesh_;
}

/**
 * Initialise la fenÃªtre d'outil
 *
 */
void FenetrePrincipale::initialiserFenetreDeplacement()
{
    modeCamera_ = new QPushButton("Caméra");
    modeCamera_->setCheckable(true);
    modeCamera_->setChecked(true);
    modeCamera_->setToolTip(QString("Mode Caméra"));
    pivoter_ = new QPushButton("Pivoter (mesh)");
    pivoter_->setCheckable(true);
    pivoter_->setToolTip(QString("Pivoter le mesh"));
    translater_ = new QPushButton("Translater (mesh)");
    translater_->setCheckable(true);
    translater_->setToolTip(QString("Mode Translation"));
    reduire_ = new QPushButton("Réduire (mesh)");
    reduire_->setCheckable(true);
    reduire_->setToolTip(QString("Mode Réduction"));

    QButtonGroup *groupButtonTransformation = new QButtonGroup;
    groupButtonTransformation->addButton(modeCamera_);
    groupButtonTransformation->addButton(pivoter_);
    groupButtonTransformation->addButton(translater_);
    groupButtonTransformation->addButton(reduire_);

    demiTour_=new QPushButton("Rotation 180°");
    demiTour_->setCheckable(false);
    demiTour_->setToolTip(QString("Pivoter le mesh à 180°"));
    nomMesh_ = new QLabel("Mesh (sans nom)");
    paletteCouleur_ = new QPushButton();
    paletteCouleur_->setToolTip(QString("Changer la couleur du mesh"));
    choixShader_ = new QComboBox();
    choixShader_->setToolTip(QString("Changer la représentation visuelle du mesh"));
    choixShader_->setFocusPolicy(Qt::NoFocus);
    choixShader_->addItem("Plat");
    choixShader_->addItem("Normal");
    choixShader_->addItem("Phong");
    choixShader_->addItem("Transparent");
    choixShader_->addItem("Fil de fer");
    choixShader_->addItem("Fil de fer transparent");
    choixShader_->addItem("Nuage de points");
    choixShader_->setCurrentIndex(2);

    fenetreDeplacement_ = new QDockWidget("Déplacement");
    addDockWidget(Qt::RightDockWidgetArea, fenetreDeplacement_);

    QVBoxLayout *fenetreDeplacementLayout = new QVBoxLayout;
    fenetreDeplacementLayout->addWidget(modeCamera_);
    fenetreDeplacementLayout->addWidget(demiTour_);
    fenetreDeplacementLayout->addWidget(pivoter_);
    fenetreDeplacementLayout->addWidget(translater_);
    fenetreDeplacementLayout->addWidget(reduire_);
    fenetreDeplacementLayout->addWidget(nomMesh_);
    fenetreDeplacementLayout->addWidget(paletteCouleur_);
    fenetreDeplacementLayout->addWidget(choixShader_);

    QWidget *contenuFenetreDeplacement = new QWidget();
    contenuFenetreDeplacement->setLayout(fenetreDeplacementLayout);
    fenetreDeplacement_->setFixedWidth(paletteDeformation_->width());
    fenetreDeplacement_->setFixedHeight(280);
    fenetreDeplacement_->setWidget(contenuFenetreDeplacement);
    fenetreDeplacement_->setFloating(false);

    //Camera
    connect(modeCamera_, SIGNAL(clicked()), widgetDeformation_, SLOT(setModeCamera()));

    //Rotation
    connect(pivoter_, SIGNAL(clicked()), widgetDeformation_, SLOT(setRotation()));

    // Translation
    connect(translater_, SIGNAL(clicked()), widgetDeformation_, SLOT(setTranslation()));

    // Reduction
    connect(reduire_, SIGNAL(clicked()), widgetDeformation_, SLOT(setReduction()));

    // Demi-tour
    connect(demiTour_, SIGNAL(clicked()), widgetDeformation_, SLOT(demiTour()));

    //Shader
    connect(choixShader_, SIGNAL(activated(int)), widgetDeformation_, SLOT(setMeshShader(int)));

    //Couleur
    connect(paletteCouleur_, SIGNAL(clicked()), widgetDeformation_, SLOT(setCouleurMesh()));
}

/**
 * Valide les options générales
 *
 */
void FenetrePrincipale::validerOptions()
{
    contour_ = contourTemp_;
}

/**
 * Modifie la valeur de contour_
 *
 * @param estActive
 */
void FenetrePrincipale::changerContour(bool estActive)
{
    contourTemp_ = estActive;
}

/**
 * Ouvre le panneau d'options
 *
 */
void FenetrePrincipale::ouvrirOptions()
{
    panneauOptions_->exec();
    activationContour_->setChecked(contour_);
}

/**
 * Renvoie la valeur de contour_
 *
 * @param bool
 */
bool FenetrePrincipale::getContour()
{
    return contour_;
}


/**
 * Cache toutes les palettes de conception
 */
void FenetrePrincipale::cacherPalettes()
{
    fenetreDeplacement_->hide();
    paletteDeformation_->hide();
}

/**
 * Fonction qui permet de quitter le logiciel
 */
void FenetrePrincipale::quitter()
{
    QApplication::quit();
}

//#include <../../release/moc_Widget3D.cpp>
