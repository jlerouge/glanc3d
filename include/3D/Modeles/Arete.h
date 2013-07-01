#ifndef __ARETE_H__
#define __ARETE_H__

#include <Vertex.h>
#include <Triangle.h>
#include "Geometrie.h"

/**
 * La classe Arete représente une arête
 * @author ToothPIC
 */
class Arete
{

public:
    Arete(Vertex *v1=0,Vertex *v2=0);
    ~Arete();
    int getId() const;
    void setId(int id);
    bool estAreteSimple() const;
    void incrementerNbTriangles();
    Vertex* getVertex1() const;
    void setVertex1(Vertex *vertex1);
    Vertex* getVertex2() const;
    void setVertex2(Vertex *vertex2);

private:
    /** Identifiant de l'arête */
    int id_;
    /** Nombre de triangles sur l'arêtes (on peut stocker les pointeurs sur ces triangles si c'est important) */
    int nbTriangles_;
    /** Le premier vertex de l'arête */
    Vertex *vertex1_;
    /** Le deuxième vertex de l'arête */
    Vertex *vertex2_;

};

bool operator<(const Arete &a,const Arete &b);
bool operator==(const Arete &a,const Arete &b);

#endif /*__ARETE_H__*/
