INCLUDEPATH += \
    include \
    include/3D/Modeles \
    include/3D/Deformations \
    include/3D/Geometrie \
    include/3D/Rendu \
    include/Flux/ \
    include/IHM \
    
HEADERS += \
    include/Constantes.h \
    include/3D/Modeles/Aabb.h \
    include/3D/Modeles/Bord.h \
    include/3D/Modeles/Mesh.h \
    include/3D/Modeles/Octree.h \
    include/3D/Modeles/Outil.h \
    include/3D/Modeles/Triangle.h \
    include/3D/Modeles/Vertex.h \
    include/3D/Deformations/Transformation.h \
    include/3D/Rendu/Camera.h \
    include/3D/Rendu/Dessin.h \
    include/3D/Deformations/Morphing.h \
    include/3D/Geometrie/Selection.h \
    include/3D/Geometrie/Geometrie.h \
    include/3D/Geometrie/Micropather.h \
    include/Flux/ControleurFlux.h \
    include/IHM/FenetrePrincipale.h \
    include/IHM/Widget3D.h \
    include/3D/Modeles/Arete.h \
    include/3D/Deformations/Rebouchage.h \
    include/3D/Deformations/Reduction.h \
    include/3D/Modeles/Forme2D.h \
    include/IHM/PictureLabel.h \
    
SOURCES += \
    src/main.cpp \
    src/3D/Modeles/Aabb.cpp \
    src/3D/Modeles/Mesh.cpp \
    src/3D/Modeles/Octree.cpp \
    src/3D/Modeles/Outil.cpp \
    src/3D/Modeles/Triangle.cpp \
    src/3D/Modeles/Vertex.cpp \
    src/3D/Deformations/Morphing.cpp \
    src/3D/Geometrie/Selection.cpp \
    src/3D/Geometrie/Geometrie.cpp \
    src/3D/Geometrie/Micropather.cpp \
    src/3D/Rendu/Camera.cpp \
    src/3D/Deformations/Transformation.cpp \
    src/3D/Modeles/Bord.cpp \
    src/3D/Modeles/Axe.cpp \
    src/3D/Rendu/Dessin.cpp \
    src/Flux/ControleurFlux.cpp \
    src/IHM/FenetrePrincipale.cpp \
    src/IHM/Widget3D.cpp \
    src/3D/Modeles/Arete.cpp \
    src/3D/Deformations/Rebouchage.cpp \
    src/3D/Deformations/Reduction.cpp \
    src/3D/Modeles/Forme2D.cpp \
    src/IHM/PictureLabel.cpp \
    
QT += opengl xml network

OTHER_FILES += \
     src/3D/Rendu/transparenceVertexShader.glsl \
     src/3D/Rendu/transparenceFragmentShader.glsl \
     src/3D/Rendu/phongVertexShader.glsl \
     src/3D/Rendu/phongFragmentShader.glsl
