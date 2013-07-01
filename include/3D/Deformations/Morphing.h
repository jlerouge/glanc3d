#ifndef __MORPHING_H__
#define __MORPHING_H__

#include "Mesh.h"

/**
 * La classe Morphing gère les outils de sculpture 3D
 * @author ToothPIC
 */
class Morphing
{
    public:
        enum SculptMode{AJOUT,RETRAIT,LISSAGE,SCULPTAGE};
        Morphing();
        ~Morphing();
        void setMesh(Mesh *mesh);
        void setIntensite(float intensite);
        void setMode(SculptMode mode);

        void sculpterMesh(const std::vector<int> &iVerts,QPointF p, QSize size);
        void lisserMeshXFois(const std::vector<int> &iVerts,int nbFois);

        bool getMorphing();
        void setMorphing(bool m );
        void deplacerVertex(int iVert, QVector3D v);
        SculptMode getMode();
        void sculpter(const std::vector<int> &iVerts, const std::vector<float> &distances);
        void initParamSculptage(Mesh *mesh, const std::vector<int> &iVerts);
        void initCentre(int icentre);

    private:
        void sculpterVertex(int iVert,float intensite);
        void lisserVertex(int iVert);
        /** Le mesh sélectionné */
        Mesh* mesh_;
        /** Intensite de déformation */
        float intensite_;
        /** Le mode de déformation */
        SculptMode mode_;
        bool morphingEnCours_;
        int centre_;
        std::vector<float> distances_;
        std::vector<float> coefs_;
};

#endif /*__MORPHING_H__*/
