#include "Bord.h"

/**
 * Constructeur de Bord par défaut
 *
 */
Bord::Bord():indices_()
{}

/**
 * Destructeur de Bord
 *
 */
Bord::~Bord()
{}

/**
 * Retourne les indices des vertices constituant le bord de sélection
 *
 * @return std::vector<int>
 */
std::vector<int> Bord::getIndices() const
{
    return indices_;
}

/**
 * Ajoute un indice de vertex au bord de sélection
 * @param i
 */
void Bord::ajouterPoint(int i)
{
    indices_.push_back(i);
}

/**
 * Ajoute un vertex et son indice au bord de sélection, à la place permettant de trier le bord
 *
 * @param i
 * @param v
 */
void Bord::ajouterPoint(int i, Mesh *mesh)
{
    indices_.insert(indices_.begin(), i);
    if (indices_.size() > 3)
        this->trier(&indices_, indices_.begin(), mesh);
}

/**
 * Trie récursivement la liste des points suivant leur proximité
 *
 * @param indicesATrier
 */
void Bord::trier(std::vector<int> *indicesATrier, std::vector<int>::iterator currentIt, Mesh *mesh)
{
    std::vector<int>::iterator nextIt = currentIt + 1;
    if (nextIt != (*indicesATrier).end())
    {
        float coutMin = 0, cout = 0;
        std::vector<int>::iterator it, itMin;
        for (it=nextIt; it != (*indicesATrier).end(); ++it)
        {
            cout = Geometrie::distance(mesh->getVertex(*currentIt), mesh->getVertex(*it));
            if ((cout < coutMin) || (coutMin == 0))
            {
                coutMin = cout;
                itMin = it;
            }
        }
        int aInserer = (*itMin);
        (*indicesATrier).erase(itMin);
        (*indicesATrier).insert(nextIt, aInserer);
        this->trier(indicesATrier, nextIt, mesh);
    }
}

/**
 * Remet à zéro le bord sélection
 *
 */
void Bord::raz()
{
    indices_.clear();
}

/** Dessiner un bord suivant le mode
  *
  * @param mode
  * @param mesh
  * @param size
  * @param decalage
  * @param couleur
  */
void Bord::dessiner(GLenum mode, Mesh *mesh, float size, float decalage, QColor couleur)
{
    if (!indices_.empty())
    {
        glPointSize(size);
        glLineWidth(size);
        glBegin(mode);
        for (uint i=0; i<indices_.size(); i++)
        {
            Vertex &v = mesh->getVertex(this->getIndices()[i]);
            QVector3D normale = v.getNormale();
            glColor3ub(couleur.red(), couleur.green(), couleur.blue());
            glVertex3f(v.x()+normale.x()*decalage,
                       v.y()+normale.y()*decalage,
                       v.z()+normale.z()*decalage);
        }
        glEnd();
        glPointSize(1.f);
        glLineWidth(1.f);
    }
}
/** Retourne le centre du bord
  *
  * @param mesh
  * @return QVector3D
  */
QVector3D Bord::getPointCentre(Mesh *mesh)
{
    int nb=indices_.size();
    QVector3D centre = QVector3D(0, 0, 0);
    for (int i=0; i<nb; i++)
        centre += mesh->getVertex(indices_[i]);
    return (centre/nb);
}
