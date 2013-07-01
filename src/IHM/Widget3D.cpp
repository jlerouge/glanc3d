#include "Widget3D.h"

/**
 * Constructeur de Widget3D
 *
 * @param type
 * @param fenetrePrincipale
 * @param widgetPartage
 */
Widget3D::Widget3D(FenetrePrincipale::TypeWidget type,
                   FenetrePrincipale *fenetrePrincipale,
                   QGLWidget *widgetPartage):
    QGLWidget(QGLFormat(QGL::SampleBuffers), fenetrePrincipale,
              widgetPartage), mesh_(0), meshes_(), camera_(),
    selection_(), morphing_(), transformation_(),
    positionSouris_(), fenetrePrincipale_(fenetrePrincipale),
    modeTransformation_(false), focus_(false),
    estADessiner_(false)
{
    type_ = type;
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    switch (type_)
    {
        case FenetrePrincipale::DEFORMATION:
            selection_.setMode(Selection::MODE_DEFORMATION);
            break;
        case FenetrePrincipale::INDEFINI:
            selection_.setMode(Selection::MODE_DEFORMATION);
            break;
    }
    chrono_ = QTime();
}

/**
 * Destructeur de Widget3D
 *
 */
Widget3D::~Widget3D()
{
    int nbMeshes=meshes_.size();
    for (int i=0;i<nbMeshes;i++)
        delete meshes_[i];
}

/**
 * Taille minimale de la fenêtre
 *
 * @return QSize
 */
QSize Widget3D::minimumSizeHint() const
{
    return QSize(200, 200);
}

/**
 * Taille conseillée de la fenêtre
 *
 * @return QSize
 */
QSize Widget3D::sizeHint() const
{
    return QSize(QApplication::desktop()->width(), QApplication::desktop()->height());
}

/**
 * Redimensionnement openGL
 *
 * @param largeur
 * @param longueur
 */
void Widget3D::resizeGL(int largeur, int longueur)
{
    camera_.setTailleFenetre(QSizeF(largeur, longueur));
}

/**
 * Initialisation openGL (couleur de fond, lumière, etc...)
 *
 */
void Widget3D::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glClearColor(0.2, 0.2, 0.2, 0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    int posLumiere[4] = {0, 0, 0, 1};           //Point de vue de l'observateur
    GLfloat lightKa[] = {.2f, .3f, .2f, 1.0f};  //lumiere ambiente
    GLfloat lightKd[] = {.7f, .8f, .7f, 1.0f};  //lumiere diffuse
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightiv(GL_LIGHT0, GL_POSITION, posLumiere);
    glEnable(GL_LIGHT0);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //pour la transparence
}

/**
 * On dessine ce qu'il y a sur l'écran
 *
 */
void Widget3D::paintGL()
{
    if (estADessiner_)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera_.majProjection();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        camera_.regarder();
        int nbMeshes=meshes_.size();
        for (int i=0;i<nbMeshes;i++)
            meshes_[i]->dessiner((meshes_[i] == mesh_) && fenetrePrincipale_->getContour());
        selection_.dessiner();

        switch (camera_.getMode())
        {
            case Camera::XY:
                Dessin::dessinerGrille(3.0f, 1.0f);
                break;
            default:
                Dessin::dessinerRepereSpherique(10.f);
                break;
        }

        if (focus_)
        {
            vue2D();
            dessinerFocus();
            vue3D();
        }
    }
}

/**
 * Action à effectuer quand une touche est pressée
 *
 * @param event
 */
void Widget3D::keyPressEvent(QKeyEvent *event)
{
    if (mesh_)
    {
        if (event->key()==Qt::Key_Delete)
        {
            switch (type_)
            {
                case FenetrePrincipale::DEFORMATION:
                    this->supprimerMesh(mesh_);
                    mesh_=0;
                    selection_.setMesh(mesh_);
                    morphing_.setMesh(mesh_);
                    break;
                default:
                    break;
            }
        }
    }


    if (event->key()==Qt::Key_Control)
    {

        modeCamera_ = !modeTransformation_;
        modeTransformation_ = true;
        transformation_.activerModeBouton(true);
        transformation_.setModeBouton(Transformation::ROTATION);

    }

    if (event->key()==Qt::Key_Shift)
    {
        modeCamera_ = !modeTransformation_;
        modeTransformation_ = true;
        transformation_.activerModeBouton(true);
        transformation_.setModeBouton(Transformation::TRANSLATION);

    }
    if (event->key()==Qt::Key_Alt)
    {
        modeCamera_ = !modeTransformation_;
        modeTransformation_ = true;
        transformation_.activerModeBouton(true);
        transformation_.setModeBouton(Transformation::REDUCTION);
    }

    updateGL();
}

/**
 * Action à  effectuer quand une touche est relachée
 *
 * @param event
 */
void Widget3D::keyReleaseEvent(QKeyEvent *event)
{

    if (event->key() == Qt::Key_Control || event->key() == Qt::Key_Shift ||
            event->key()==Qt::Key_Alt)
    {
        transformation_.activerModeBouton(false);
        if (modeCamera_)
            modeTransformation_ = false;
    }
    updateGL();
}

/**
 * Action à  effectuer quand un bouton de la souris est pressée
 *
 * @param event
 */
void Widget3D::mousePressEvent(QMouseEvent *event)
{
    emit clique();
    mesh_ = selection_.getMesh();
    transformation_.setMesh(mesh_);
    if (mesh_)
    {
        std::vector<Mesh*>::iterator it; //place le mesh au bon endroit
        it = std::find(meshes_.begin(), meshes_.end(), mesh_);
        meshes_.erase(it);
        it = meshes_.begin();
        while (it != meshes_.end())
        {
            if ((*it)->getShader()!=Mesh::TRANSPARENCE)
                ++it;
            else
                break;
        }
        meshes_.insert(it, mesh_);
        if (QPushButton *paletteCouleur = fenetrePrincipale_->getPaletteCouleur())
            paletteCouleur->setPalette(QPalette(QColor(mesh_->getCouleur())));
        if (QComboBox *choixShader = fenetrePrincipale_->getChoixShader())
            choixShader->setCurrentIndex((int)mesh_->getShader());
        if (QPushButton *octree = fenetrePrincipale_->getOctree())
            octree->setDown(mesh_->getAffichageOctree());
        if (QLabel *nomMesh = fenetrePrincipale_->getNomMesh())
            nomMesh->setText(mesh_->getNomMesh());
    }
    else
    {
        if (QPushButton *paletteCouleur = fenetrePrincipale_->getPaletteCouleur())
            paletteCouleur->setPalette(QPalette());
        if (QPushButton *octree = fenetrePrincipale_->getOctree())
            octree->setDown(false);
        if (QLabel *nomMesh = fenetrePrincipale_->getNomMesh())
            nomMesh->setText("");
    }
    if (!(chrono_.isValid()) || (chrono_.elapsed() <= 50))
    {
        if (!(chrono_.isValid()))
            chrono_.start();
        else
            chrono_.restart();
        if (event->buttons() == Qt::RightButton)
        {
            if (!transformation_.getModeBoutonActif ())
                camera_.debuter(event->pos());
            else if (modeTransformation_ && transformation_.getMesh() &&
                     transformation_.getModeBoutonActif())
                transformation_.debuter(event->pos(), size(), camera_.getZoom());
        }
        else if ((event->buttons() == Qt::MidButton) ||
                 ((event->buttons() & Qt::LeftButton) && (event->buttons() & Qt::RightButton)))
        {
            if (modeTransformation_ && transformation_.getMesh() &&
                    !transformation_.getModeBoutonActif())
                transformation_.debuter(event->pos(), size(), camera_.getZoom());
        }
        else if (event->buttons() == Qt::LeftButton)
        {
            switch (selection_.getMode())
            {
                case Selection::MODE_LIBRE:
                    selection_.debuterSelection();
                    break;
                case Selection::MODE_POINT:
                    break;
                case Selection::MODE_FUSION:
                    break;
                default:
                    break;
            }
        }
    }

    if (morphing_.getMode() == Morphing::SCULPTAGE)
        if (event->buttons() & Qt::LeftButton)
        {
            if (!morphing_.getMorphing())
            {
                morphing_.setMorphing(true);
                selection_.setVerticesSelectionnees(meshes_,  event->pos(), size());
                morphing_.initCentre(selection_.getIndiceCentre());
                morphing_.initParamSculptage(selection_.getMesh(),
                                             selection_.getVerticesSelectionnees());
            }
        }
    updateGL();
}

/**
 * Action à   effectuer quand la souris est bougée
 *
 * @param event
 */
void Widget3D::mouseMoveEvent(QMouseEvent *event)
{
    if (morphing_.getMode() != Morphing::SCULPTAGE ||
            !(this->getType() == FenetrePrincipale::DEFORMATION &&
              morphing_.getMorphing()))
        if (!selection_.getSelectionEnCours() || !selection_.getMesh())
            selection_.setVerticesSelectionnees(meshes_, event->pos(), size());
    if (chrono_.isValid() && (chrono_.elapsed() > 50))
    {
        switch (selection_.getMode())
        {
            case Selection::MODE_LIBRE:
            {
                selection_.selectionLibre(event->pos(), size());
                break;
            }
            case Selection::MODE_POINT:
            {
                break;
            }
            default:
                break;
        }
        if (event->buttons() == Qt::RightButton)
        {
            if (!transformation_.getModeBoutonActif ())
                camera_.tourner(event->pos());
            else if (transformation_.getModeBoutonActif () && transformation_.getMesh() )
                transformation_.transformer(event->pos());


        }
        else if ((event->buttons() == Qt::MidButton) || ((event->buttons() & Qt::LeftButton) &&
                                                         (event->buttons() & Qt::RightButton)))
        {
            if (!modeTransformation_)
                camera_.translater(event->pos()-positionSouris_);
            else if (transformation_.getMesh() && !transformation_.getModeBoutonActif ())
                transformation_.transformer(event->pos());
        }
        else if (event->buttons() == Qt::LeftButton && selection_.getMesh() &&
                 (selection_.getMode() == Selection::MODE_DEFORMATION))
        {
            morphing_.setMesh(selection_.getMesh());
            morphing_.sculpterMesh(selection_.getVerticesSelectionnees(), event->pos(),
                                   size());
        }
    }
    positionSouris_ = event->pos();
    updateGL();
}

/**
 * Action à effectuer quand la souris est relachée
 *
 * @param event
 */
void Widget3D::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton)
        morphing_.setMorphing(false);

    if (selection_.getMode() == Selection::MODE_LIBRE)
    {
        if (selection_.getSelectionEnCours())
            selection_.finirSelection(size());
        if (selection_.getAjoutFini())
        {
            selection_.setAjoutFini(false);
        }
    }
    chrono_.setHMS(0, 0, 0, -1);
    updateGL();
}

/**
 * Action à effectuer quand on touche à   la molette
 *
 * @param event
 */
void Widget3D::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
        camera_.zoomer(event->delta());
    updateGL();
}

/**
 * Action à effectuer quand l'utilisateur charge un fichier
 *
 */
void Widget3D::chargerFichier()
{
    ControleurFlux ctrlFlux;
    QString nomFichier = QFileDialog::getOpenFileName(
                this, tr("Chargement d'un fichier STL"),
                tr("C:\\"), tr("Fichier STL (*.stl )"));
    if (!nomFichier.isNull())
    {
        fenetrePrincipale_->getBarreProgression()->setVisible(true);
        Mesh *mesh;
        mesh = ctrlFlux.charger(nomFichier, fenetrePrincipale_);
        fenetrePrincipale_->getBarreProgression()->setVisible(false);
        meshes_.insert(meshes_.begin(), mesh);
    }
    updateGL();

}

/**
 * Action à  effectuer quand l'utilisateur sauvegarde un fichier
 *
 */
void Widget3D::sauvegarderFichier()
{
    if (focus_)
    {
        ControleurFlux ctrlFlux;
        QString nomFichier = QFileDialog::getSaveFileName(
                    this, tr("Sauvegarde d'un fichier STL"),
                    tr("C:\\"), tr("Fichier STL (*.stl )"));
        if (!nomFichier.isNull() && mesh_)
        {
            fenetrePrincipale_->getBarreProgression()->setVisible(true);
            ctrlFlux.sauvegarder(mesh_, nomFichier, fenetrePrincipale_);
            fenetrePrincipale_->getBarreProgression()->setVisible(false);
        }
        updateGL();
    }
}

/**
 * Fixe le rayon de sélection
 *
 * @param rayon
 */
void Widget3D::setRayonSelection(int rayon)
{
    selection_.setTailleZone(rayon/200.f);
}

/**
 * Fixe la puissance de déformation
 *
 * @param puissance
 */
void Widget3D::setPuissanceDeform(int puissance)
{
    morphing_.setIntensite(puissance/400.f);
}

/**
 * Passe en mode rajouter de la matière
 *
 */
void Widget3D::setSensDeformPlus()
{
    morphing_.setMode(Morphing::AJOUT);
}

/**
 * Passe en mode enlever de la matière
 *
 */
void Widget3D::setSensDeformMoins()
{
    morphing_.setMode(Morphing::RETRAIT);
}

/**
 * Passe en mode polir de la matière
 *
 */
void Widget3D::setLissage()
{
    morphing_.setMode(Morphing::LISSAGE);
}

void Widget3D::setSculptage()
{
    morphing_.setMode(Morphing::SCULPTAGE);
}


/**
 * Passe en mode caméra
 *
 */
void Widget3D::setModeCamera()
{
    modeTransformation_=false;
}

/**
 * Passe en mode rotation
 *
 */
void Widget3D::setRotation()
{
    modeTransformation_=true;
    transformation_.setMode(Transformation::ROTATION);
}

/**
 * Passe en mode translation
 *
 */
void Widget3D::setTranslation()
{
    modeTransformation_=true;
    transformation_.setMode(Transformation::TRANSLATION);
}

/**
 * Passe en mode réduction
 *
 */
void Widget3D::setReduction()
{
    modeTransformation_=true;
    transformation_.setMode(Transformation::REDUCTION);
}

/**
 * Fixe le shader au mesh, trie également les meshes pour mettre les transparents en dernier
 *
 * @param type
 */
void Widget3D::setMeshShader(int type)
{
    if (mesh_)
    {
        Mesh::ShaderMode shader= Mesh::ShaderMode(type);
        mesh_->setShader(shader);
        std::vector<Mesh*>::iterator it;
        it=std::find(meshes_.begin(), meshes_.end(), mesh_);
        meshes_.erase(it);
        if (shader==Mesh::TRANSPARENCE)
            meshes_.push_back(mesh_);
        else
            meshes_.insert(meshes_.begin(), mesh_);
    }
    updateGL();
}

/**
 * Change le type de caméra
 *
 * @param type
 */
void Widget3D::setCamera(int type)
{
    camera_.setMode(Camera::CameraMode(type));
    updateGL();
}

/**
 * Affiche l'octree
 *
 */
void Widget3D::setOctree()
{
    if (mesh_ && focus_)
    {
        mesh_->changerAffichageOctree();
        if (QPushButton *octree = fenetrePrincipale_->getOctree())
            octree->setDown(mesh_->getAffichageOctree());
    }
    updateGL();
}

/**
 * Fixe la couleur du mesh
 *
 */
void Widget3D::setCouleurMesh()
{
    if (mesh_ && focus_)
    {
        QColor couleur=QColorDialog::getColor(mesh_->getCouleur(), this, "couleur du mesh");
        if (couleur.isValid())
        {
            mesh_->setCouleur(couleur);
            if (QPushButton *paletteCouleur = fenetrePrincipale_->getPaletteCouleur())
                paletteCouleur->setPalette(QPalette(QColor(couleur)));
        }
    }
}

/**
 * Charge la vue orthogonale
 *
 */
void Widget3D::vue2D()
{
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); //charge la matrice de projection
    glPushMatrix();         //push la matrice
    glLoadIdentity();         //charge la matrice identité
    glOrtho(0, width(), height(), 0, -1, 1); //passe en mode projection orthogonale
    glMatrixMode(GL_MODELVIEW);         //charge la matrice de modèle-vue
    glPushMatrix();
    glLoadIdentity();
}

/**
 * Charge la vue perspective
 *
 */
void Widget3D::vue3D()
{
    glMatrixMode( GL_PROJECTION );         //charge la matrice de projection
    glPopMatrix();         //pop la matrice
    glMatrixMode( GL_MODELVIEW );         //charge la matrice de modèle-vue
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

/**
 * Charge la vue perspective
 *
 * @param booleen
 */
void Widget3D::setFocus(bool booleen)
{
    focus_ = booleen;
}

/**
 * Fixe la valeur de estADessiner_
 *
 * @param estADessiner
 */
void Widget3D::setEstADessiner(bool estADessiner)
{
    estADessiner_ = estADessiner;
}

/**
 * Ajoute un mesh au widget3D
 *
 * @param mesh
 */
void Widget3D::ajouterMesh(Mesh *mesh)
{
    meshes_.insert(meshes_.begin(), mesh);
    if (!mesh_)
        mesh_ = mesh;
}

/**
 * Supprimer un mesh du widget3D
 *
 * @param mesh
 */
void Widget3D::supprimerMesh(Mesh *mesh)
{
    std::vector<Mesh*>::iterator it;
    it=std::find(meshes_.begin(), meshes_.end(), mesh);
    meshes_.erase(it);
    delete mesh_;
}

/**
 * Retourne le nombre de meshes affichés dans le Widget
 *
 * @param mesh
 */
int Widget3D::getNbMeshes()
{
    return meshes_.size();
}

/**
 * Remet à    zéro la liste des meshes du Widget
 *
 */
void Widget3D::resetMeshes()
{
    mesh_ = 0;
    meshes_.clear();
}

void Widget3D::setMode(Selection::ModeSelection mode)
{
    selection_.setMode(mode);
}

/**
 * Dessine la zone en mode focus
 *
 */
void Widget3D::dessinerFocus()
{
    glDisable(GL_LIGHTING);
    glLineWidth(150.f);
    glBegin(GL_LINE_LOOP);
    glColor3ub(255, 0, 0);
    glVertex3f(this->width()-3, this->height()-3, 1);
    glVertex3f(this->width()-3, 3, 1);
    glVertex3f(3.f, 3.0, 1);
    glVertex3f(3.f, this->height()-3, 1);
    glEnd();
    glLineWidth(1.f);
    glEnable(GL_LIGHTING);
}

/**
 * Renvoie la valeur de type_
 *
 * @return TypeWidget
 */
FenetrePrincipale::TypeWidget Widget3D::getType()
{
    return type_;
}

/**
 * Renvoie la valeur de focus_
 *
 * @return bool
 */
bool Widget3D::getFocus()
{
    return focus_;
}

/**
 * Alterne entre projection orthographique et perspective
 *
 */
void Widget3D::setProjection()
{
    camera_.setOrtho(!camera_.getOrtho());
    if (camera_.getOrtho())
        fenetrePrincipale_->getProjection()->setText("Orthographique");
    else
        fenetrePrincipale_->getProjection()->setText("Perspective");
    updateGL();
}

/**
 * Effectue un demi-tour à la caméra
 *
 */
void Widget3D::demiTour()
{
    if (focus_)
    {
        camera_.demiTour();
        updateGL();
    }
}

//#include <../../release/moc_FenetrePrincipale.cpp>
