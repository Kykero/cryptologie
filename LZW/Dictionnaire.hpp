#ifndef DICTIONNAIRE_HPP
#define DICTIONNAIRE_HPP

#include <iostream>
#include <stdlib.h>
using namespace std;

#include "SequenceMessages.hpp"
#include <vector>

/**
 *  \author Marc Chaumont
 *  \date   24 mars 2006
 *
 *  La classe dictionnaire permet la gestion des sequences de messages.
 *
 */
 
class Dictionnaire {
    
private :

    vector<SequenceMessages> dico; /**< Le dictionnaire.*/
    
public:
    
    /**
     * Constructeur par défaut.
     */
    Dictionnaire();

    /**
     * addEntry()
     * Ajoute une entrée au dictionnaire c'est à dire une sequence de messages. 
     * \param seq : une séquence de messages.
     */
    void addEntry(const SequenceMessages& seq);

    /**
     * isPresent()
     * \param seq : une séquence de messages.
     * \return vrai si une sequence de messages est présente dans le dictionnaire.
     */
    bool isPresent(const SequenceMessages& seq) const;
    
    /**
     * getIndice()
     * \return l'indice de la séquence si elle est présente dans le dictionnaire 
     * retourne -1 sinon.
     */
    int getIndice(const SequenceMessages& seq) const;


    /**
     * indiceValide()
     * \params incide : indice d'entrée dans le dictionnaire.
     * \return vrai si l'indice est valide et faux sinon.
     */
     bool indiceValide(int indice) const;

    /**
     * getSequence()
     * \return la séquence de messages à l'indice indice.
     */
    const SequenceMessages& getSequence(int indice) const;

    /**
     * operator<<
     * L'opérateur permet de faire l'affichage du dictionnaire.
     * 
     * Remarque : l'opérateur est une fonction amie et ne fait donc pas partie de 
     * la classe SequenceMessages.
     */
     friend ostream& operator<< (ostream& os, const Dictionnaire& d);
         
     /**
      * test()
      * Fonction de test de la classe dictionnaire.
      */
      static void test();
};    
#endif //DICTIONNAIRE_HPP
