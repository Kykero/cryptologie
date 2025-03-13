#include "CoupleMessageProba.hpp"

/**
 *  \author Marc Chaumont
 *  \date   21 mars 2006
 *
 *  La classe CoupleMessageProba permet de gérer des paires (message, proba). 
 *
 *  Remarque : Cette classe va être utilisée lors de la construction de l'histogramme 
 *  (message <-> proba) ainsi que lors de l'initialisation de la table contenant
 *  des noeuds.
 */

    /**
     * CoupleMessageProba
     * Constructeur par défaut.
     */    
    CoupleMessageProba::CoupleMessageProba() {
    }     
    
    /**
     * CoupleMessageProba
     * Constructeur par valeur.
     * \param message : le symbole (caractère). 
     * \param proba   : la probabilité du message.
     */    
    CoupleMessageProba::CoupleMessageProba(char message, double proba) {
      (*this).message = message;
      (*this).proba   = proba;
    }     

    /**
     * ~CoupleMessageProba
     * Destructeur.
     */    
    CoupleMessageProba::~CoupleMessageProba() {
    }    
    
    /**
     * Acces au message.
     * \return le message (un caractère).
     */    
     char CoupleMessageProba::getMessage() const {
         return message;
     }    

    /**
     * Acces à la proba.
     * \return la probabilité associée au message.
     */    
     double CoupleMessageProba::getProba() const {
         return proba;
     }    
    
    /**
     * Positionne le message.
     */    
     void CoupleMessageProba::setMessage(char message) {
         (*this).message = message;
     }    

    /**
     * Positionne la proba.
     */    
     void CoupleMessageProba::setProba(double proba) {
         (*this).proba = proba;
     }    

