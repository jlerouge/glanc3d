#ifndef __SELECTION_H__
#define __SELECTION_H__

#include "Mesh.h"
#include "Geometrie.h"
#include "Dessin.h"
#include "Forme2D.h"

/**
 * La classe Selection aide à la selection de triangle ou bien de zone sur des meshes
 * @author ToothPIC
 */
class Selection
{
    public:
        enum ModeSelection{MODE_DEFORMATION, MODE_POINT, MODE_FUSION , MODE_LIBRE};
        Selection();
        ~Selection();
        void setMesh(Mesh *mesh);
        Mesh* getMesh();
        int getTriangleSelectionne();
        std::vector<int>& getVerticesSelectionnees();
        void setTailleZone(float tailleZone);
        void setVerticesSelectionnees(std::vector<Mesh*> &meshes,const QPointF &positionSouris,const QSizeF &tailleFenetre);
        void intersectionRayonMeshes(std::vector<Mesh*> &meshes,const QVector3D &vertSouris1, const QVector3D &vertSouris2);
        static int intersectionRayonMesh(Mesh *mesh,const QVector3D &vertexPres, const QVector3D &vertexLoin, float &distance);
        void setVerticesSelectionneesDansCylindre(Mesh *mesh,const QPointF &positionSouris,const QSizeF &tailleFenetre,float rayon);
        static std::vector<int> intersectionCylindreMesh(Mesh *mesh,const QVector3D &vertexPres, const QVector3D &vertexLoin,float rayon);

        void setMode(ModeSelection mode);
        ModeSelection getMode();

        void selectionLibre(const QPointF positionSouris, const QSizeF &tailleFenetre);
        void dessiner();
        void dessinerPolygone();
        void razPolygone();
        void debuterSelection();
        void finirSelection(const QSizeF &tailleFenetre);
        Mesh* getMeshSelection();
        bool getAjoutFini();
        void setAjoutFini(bool estFini);
        bool getSelectionEnCours();
        Forme2D* getFormeLibre();
        float getDistanceMax();
        int getIndiceCentre();

    private:
        void setVerticesDansZoneSelection();
        void setVertexSelectionne();
        void propagationZoneSelection(const QVector3D &centreSelect,const Vertex &vert, std::set<int> &vertsSelect);

        /** Le mesh sélectionné */
        Mesh *mesh_;
        /** L'indice du triangle traversé par le rayon */
        int triangleSelectionne_;
        /** Les indices de triangles sélectionnées par la zone de sélection */
        std::vector<int> verticesSelectionnes_;
        /** La taille de la zone de sélection */
        float tailleZone_;
        /** L'activation du mode de sélection cylindrique **/
        bool modeCylindre_;
        /** L'activation du mode de sélection point à point **/
        bool modePoint_;
        /** L'activation du mode de sélection point à point de la fusion **/
        bool modeFusion_;
        /** Le vertex d'intersection le plus proche de nous **/
        static QVector3D vertexIntersection_;
        /** Le mode de sélection **/
        ModeSelection mode_;
        /** La succesion de points 2D utilisé pour le polygone de sélection libre (à la souris) **/
        std::vector<QPointF> polygoneLibre_;
        /** La liste des vertices contenus à l'intérieur de la forme libre **/
        std::vector<int> verticesFormeLibre_;
        /** Si une sélection est en cours ou non **/
        bool selectionEnCours_;
        /** Si la l'utilisateur a terminé d'ajouter des points
          * au polygone de sélection libre **/
        bool ajoutFini_;
        /** Le mesh obtenu par sélection **/
        Mesh *meshSelection_;
        /** Le polygone de sélection libre (à la souris) **/
        Forme2D *formeLibre_;
        /** La distance maximale d'un point par rapport au centre de sélection **/
        float distanceMax_;
        /** Indice du centre de la sélection */
        int indiceCentre_;
};

#endif /*__SELECTION_H__*/
