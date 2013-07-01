#include "Dessin.h"

/**
 * Dessine le repère sphérique
 *
 * @param rayon
 */
void Dessin::dessinerRepereSpherique(float rayon)
{
    float degInRad;
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.4f, 0.4f);
    glBegin(GL_LINES);
    for (int i=0;i<360;i++)
    {
        degInRad = i * M_PI/180;
        glVertex3f(0.0f, cos(degInRad)*rayon, sin(degInRad)*rayon);
    }
    glEnd();
    glColor3f(0.4f, 1.0f, 0.4f);
    glBegin(GL_LINES);
    for (int i=0;i<360;i++)
    {
        degInRad = i * M_PI/180;
        glVertex3f(cos(degInRad)*rayon, sin(degInRad)*rayon, 0.0f);
    }
    glEnd();
    glColor3f(0.4f, 0.4f, 1.0f);
    glBegin(GL_LINES);
    for (int i=0;i<360;i++)
    {
        degInRad = i * M_PI/180;
        glVertex3f(sin(degInRad)*rayon, 0.0f, cos(degInRad)*rayon);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

/**
 * Dessine le cercle de sélection
 *
 * @param centrePosition
 * @param dernierePosition
 */
void Dessin::dessinerCercleSelection(const QPointF &centrePosition,
                                     const QPointF &dernierePosition)
{
    double x1 = centrePosition.x();
    double y1 = centrePosition.y();
    double x2 = dernierePosition.x();
    double y2 = dernierePosition.y();
    double radius = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    float degInRad;
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    for (int i=0;i<360;i++)
    {
        degInRad = i * M_PI/180;
        glVertex2f(x1+cos(degInRad)*radius, y1+sin(degInRad)*radius);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

/**
 * Dessine une grille avec le repere x, y, z
 *
 * @param taille
 * @param pas
 */
void Dessin::dessinerGrille(float taille, float pas)
{
    int deltaY=-7;
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(0.5f, 0.5f, 0.5f);
    for (float i=pas; i <= taille; i+= pas)
    {
        glVertex3f(-taille, deltaY,  i);
        glVertex3f( taille, deltaY,  i);
        glVertex3f(-taille, deltaY, -i);
        glVertex3f( taille, deltaY, -i);
        glVertex3f( i, deltaY, -taille);
        glVertex3f( i, deltaY,  taille);
        glVertex3f(-i, deltaY, -taille);
        glVertex3f(-i, deltaY,  taille);
    }
    glColor3f(1.f, 0, 0);
    glVertex3f(0, deltaY, 0);
    glVertex3f(taille/5, deltaY, 0);
    glColor3f(0, 1.f, 0);
    glVertex3f(0, deltaY, 0);
    glVertex3f(0, deltaY+taille/5, 0);
    glColor3f(0, 0, 1.f);
    glVertex3f(0, deltaY, 0);
    glVertex3f(0, deltaY, taille/5);
    glEnd();
    glEnable(GL_LIGHTING);
}

/**
 * Dessine un ensemble de points dans la scène
 *
 * @param mesh
 * @param indices
 * @param couleur
 */
void Dessin::dessinerPoints(Mesh *mesh, std::vector<int> indices, QColor couleur)
{
    int nbPoints= indices.size();
    if (nbPoints>0)
    {
        glPushMatrix();
        QMatrix4x4 mat = mesh->getTransformation();
        float *dataMat = mat.data();
        GLfloat matriceArray[16];
        for (int i= 0; i < 16; i++)
            matriceArray[i] = dataMat[i];
        glMultMatrixf(matriceArray);
        glDisable(GL_LIGHTING);
        float decalage=0.001f; // pour faire la différence avec le mesh
        glPointSize(10.f);
        glBegin(GL_POINTS);
        for (int i=0; i<nbPoints; i++)
        {
            Vertex &v = mesh->getVertex(indices[i]);
            QVector3D normale = v.getNormale();
            glColor3ub(couleur.red(), couleur.green(), couleur.blue());
            glVertex3f(v.x()+normale.x()*decalage, v.y()+normale.y()*decalage,
                       v.z()+normale.z()*decalage);
        }
        glEnd();
        glPointSize(1.f);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }
}
