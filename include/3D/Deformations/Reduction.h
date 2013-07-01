#ifndef __REDUCTION_H__
#define __REDUCTION_H__

#include "Morphing.h"
#include "Arete.h"
#include "Bord.h"
#include "Vertex.h"
#include "Geometrie.h"
#include <math.h>
#include "Axe.h"

class Reduction
{
    public:
        Reduction(Mesh *mesh);
        ~Reduction();
        void setMesh(Mesh *mesh);
        Mesh* getMesh();
        void setAxeInsertion(Axe *axeInsertion);
        Axe* getAxeInsertion();
        Bord getBordCervical();
        void setBordCervical(std::vector<Arete> bordCervical);
        float fonctionHorizontal(float angle, float distanceHorizontal);
        float fonctionVertical(float angle, float distanceVertical);
        void reduireUnPoint(int indice, float distanceH, float distanceV , float spacerGeneral); //, Widget3D *widget
        void reduireSpacer(Vertex &v, float angle, float distanceH, float distanceV , float spacerGeneral);
        void reduire3D(float distanceHorizontal, float distanceVertical , float spacerGeneral = 0);

    private:
        Mesh *mesh_;
        Bord bordCervical_;
        std::vector<int> verticesConcerns_;
        Axe *axeInsertion_;
};

#endif /*__REDUCTION_H__*/
