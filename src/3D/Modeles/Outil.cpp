#include "Outil.h"

/**
 * Pour le débuggage de matrice
 *
 * @param matrice
 */
void Outil::afficherMatrice(const QMatrix4x4 &matrice)
{
    std::cout<<"DEBUT MATRICE"<<std::endl;
    std::cout<<matrice(0, 0)<<" et "<<matrice(0, 1)
            <<" et "<<matrice(0, 2)<<" et "
            << matrice(0, 3)<<std::endl;
    std::cout<<matrice(1, 0)<<" et "<<matrice(1, 1)
            <<" et "<<matrice(1, 2)<<" et "
            << matrice(1, 3)<<std::endl;
    std::cout<<matrice(2, 0)<<" et "<<matrice(2, 1)
            <<" et "<<matrice(2, 2)<<" et "
            << matrice(2, 3)<<std::endl;
    std::cout<<matrice(3, 0)<<" et "<<matrice(3, 1)
            <<" et "<<matrice(3, 2)<<" et "
            << matrice(3, 3)<<std::endl;
    std::cout<<"FIN MATRICE"<<std::endl;
}

/**
 * Pour le débuggage de vertex
 *
 * @param vertex
 */
void Outil::afficherVertex(const QVector3D &vertex)
{
    std::cout<<"DEBUT VERTEX "<<vertex.x()<<" et "
            <<vertex.y()<<" et "<<vertex.z()
           <<" FIN VERTEX"<<std::endl;
}
