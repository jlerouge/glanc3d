#ifndef __OUTIL_H__
#define __OUTIL_H__

#include <QMatrix4x4>
#include <vector>
#include <algorithm>
#include <iostream>

/**
 * Le namespace Outil offre des fonctions diverses
 * @author ToothPIC
 */
namespace Outil
{
    /**
     * Trie un vector et supprime ses doublons
     *
     * @param vec
     */
    template<typename T> void ranger(std::vector<T> &vec)
    {
        std::sort(vec.begin(), vec.end());
        std::vector<int>::iterator it=std::unique(vec.begin(), vec.end());
        vec.resize(it-vec.begin());
    }

    void afficherMatrice(const QMatrix4x4 &matrice);
    void afficherVertex(const QVector3D &vertex);
}

#endif /*__OUTIL_H__*/
