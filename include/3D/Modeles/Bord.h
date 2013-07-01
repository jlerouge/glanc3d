#ifndef __BORD_H__
#define __BORD_H__

#include <string>
#include <vector>
#include <set>
#include <QtOpenGL>
#include <algorithm>
#include "Vertex.h"
#include "Mesh.h"
#include "Geometrie.h"

/**
 * La classe Bord représente un bord de sélection sur un Mesh, composé d'une succession de vertices
 * @author ToothPIC
 */
class Bord
{
    public:
        Bord();
        ~Bord();
        std::vector<int> getIndices() const;
        void ajouterPoint(int);
        void ajouterPoint(int, Mesh *);
        void raz();
        void dessiner(GLenum, Mesh *, float, float, QColor);
        QVector3D getPointCentre(Mesh *mesh);
    private:
        void trier(std::vector<int>*, std::vector<int>::iterator, Mesh*);

        /** Indices des vertices du bord de sélection */
        std::vector<int> indices_;

};
#endif /*__BORD_H__*/
