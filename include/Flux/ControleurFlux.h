#ifndef __CONTROLEUR_FLUX_H__
#define __CONTROLEUR_FLUX_H__

#include <fstream>
#include <iostream>
#include "Mesh.h"
#include "FenetrePrincipale.h"

/**
 * La classe ControleurFlux permet de gérer les flux, comme par exemple le chargement ou la sauvegarde
 * @author ToothPIC
 */
class ControleurFlux
{

public:
    ControleurFlux();
    ~ControleurFlux();
    Mesh* charger(const QString &cheminFichier,FenetrePrincipale *fenetre);
    void sauvegarder(Mesh *mesh,const QString &nomFichier,FenetrePrincipale *fenetre);
};


#endif /*__CONTROLEUR_FLUX_H__*/
