#include "Arete.h"

/**
 * Constructeur d'Arete
 *
 * @param v1
 * @param v2
 */
Arete::Arete(Vertex *v1, Vertex *v2):id_(0), nbTriangles_(0), vertex1_(v1), vertex2_(v2)
{}

/**
 * Destructeur d'Arete
 *
 */
Arete::~Arete()
{}

/**
 * Renvoie la valeur de id_
 *
 * @return int
 */
int Arete::getId() const
{
    return id_;
}

/**
 * Fixe la valeur de id_
 *
 * @param id
 */
void Arete::setId(int id)
{
    id_=id;
}

/**
 * Renvoie si l'arête est une arete simple ou non
 *
 * @return bool
 */
bool Arete::estAreteSimple() const
{
    return nbTriangles_==1;
}

/**
 * Incrémente nbTriangles_
 *
 * @return bool
 */
void Arete::incrementerNbTriangles()
{
    nbTriangles_++;
}

/**
 * Renvoie la valeur de vertex1_
 *
 *  @return Vertex
 */
Vertex* Arete::getVertex1() const
{
    return vertex1_;
}

/**
 * Fixe la valeur de vertex1_
 *
 * @param vertex1
 */
void Arete::setVertex1(Vertex *vertex1)
{
    vertex1_=vertex1;
}

/**
 * Renvoie la valeur de vertex2_
 *
 *  @return Vertex
 */
Vertex* Arete::getVertex2() const
{
    return vertex2_;
}

/**
 * Fixe la valeur de vertex2_
 *
 * @param vertex2
 */
void Arete::setVertex2(Vertex *vertex2)
{
    vertex2_=vertex2;
}

/**
 * Opérateur de comparaison d'infériorité entre deux arêtes
 *
 * @param a
 * @param b
 * @return bool
 */
bool operator<( const Arete  &a, const  Arete &b)
{
    QVector3D milieuA=(*a.getVertex1() + *a.getVertex2())/2.f;
    QVector3D milieuB=(*b.getVertex1() + *b.getVertex2())/2.f;
    if (milieuA.x()<milieuB.x())
        return true;
    else if (milieuA.x()>milieuB.x())
        return false;
    else if(a==b)
        return false;
    else
    {
        if (milieuA.y()<milieuB.y())
            return true;
        else if (milieuA.y()>milieuB.y())
            return false;
        else
        {
            if (milieuA.z()<milieuB.z())
                return true;
            else if (milieuA.z()>milieuB.z())
                return false;
        }
    }
    return false;
}

/**
 * Opérateur de comparaison d'égalité entre deux arêtes
 *
 * @param a
 * @param b
 * @return bool
 */
bool operator==(const Arete &a, const Arete &b)
{
    int indA1 = a.getVertex1()->getId();
    int indA2 = a.getVertex2()->getId();
    int indB1 = b.getVertex1()->getId();
    int indB2 = b.getVertex2()->getId();
    return (((indA1 == indB1) || (indA1 == indB2)) &&
            ((indA2 == indB1) || (indA2 == indB2)));
}
