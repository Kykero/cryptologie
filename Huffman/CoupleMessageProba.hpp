#ifndef COUPLEMESSAGEPROBA_HPP
#define COUPLEMESSAGEPROBA_HPP

#include <iostream>
#include <stdlib.h>
using namespace std;

/**
 *  \author Marc Chaumont
 *  \date   21 mars 2006
 *
 *  La classe CoupleMessageProba permet de g�rer des paires (message, proba). 
 *
 *  Remarque : Cette classe va �tre utilis�e lors de la construction de l'histogramme 
 *  (message <-> proba) ainsi que lors de l'initialisation de la table contenant
 *  des noeuds.
 */

class CoupleMessageProba {

private :
    
    char message; /**< Dans ce tp, un message (symbole) est un caract�re.*/
    double proba; /**< Probabilit� associ�e au caract�re.*/

public:

    /**
     * CoupleMessageProba
     * Constructeur par d�faut.
     */    
    CoupleMessageProba(); 
    
    /**
     * CoupleMessageProba
     * Constructeur par valeur.
     * \param message : le symbole (caract�re). 
     * \param proba   : la probabilit� du message.
     */    
    CoupleMessageProba(char message, double proba); 

    /**
     * ~CoupleMessageProba
     * Destructeur.
     */    
    ~CoupleMessageProba(); 
    
    /**
     * Acces au message.
     * \return le message (un caract�re).
     */    
     char getMessage() const;

    /**
     * Acces � la proba.
     * \return la probabilit� associ�e au message.
     */    
     double getProba() const;
    
    /**
     * Positionne le message.
     */    
     void setMessage(char message);

    /**
     * Positionne la proba.
     */    
     void setProba(double proba);
};    
#endif // COUPLEMESSAGEPROBA_HPP

