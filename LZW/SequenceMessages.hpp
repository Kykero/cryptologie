#ifndef SEQUENCEMESSAGES_HPP
#define SEQUENCEMESSAGES_HPP

#include <iostream>
#include <stdlib.h>
using namespace std;

#include <list>

typedef char Message;

/**
 *  \author Marc Chaumont
 *  \date   22 mars 2006
 *
 *  La classe SequenceMessage permet la manipulation de liste de messages.
 *
 */
 
class SequenceMessages {
    
private :
    list<Message> listmessages;  /**< Sequence de messages.*/
public:
    
    /**
     * Constructeur par défaut.
     */
    SequenceMessages();

    /**
     * Constructeur avec un seul message.
     */
    SequenceMessages(Message message);

    /**
     * Constructeur de clonage.
     */
    SequenceMessages(const SequenceMessages& sequence);

    /**
     * taille()
     * \return le nombre de message contenu dans la sequence.
     */
    int taille() const;

    /**
     * operator==
     * Opérateur de comparaison.
     * L'opérateur permet d'utiliser correctement l'opération ==
     * Exemple : seq1 == seq2; //L'opérateur '==' est bien défini.
     */
    bool operator== (const SequenceMessages& seq2) const;

    /**
     * operator=
     * Opérateur d'affectation.
     * L'opérateur permet d'utiliser correctement l'opération =
     * Exemple : seq1 = seq2; //L'opérateur '=' est bien défini.
     */
    SequenceMessages& operator= (const SequenceMessages& seq);

    /**
     * operator+
     * Opérateur permettant d'effectuer la concaténation.
     * Exemple : seq1 + seq2; //seq1 concaténé avec la séq2
     */
    SequenceMessages operator+ (const SequenceMessages& seq) const;

    /**
     * operator+=
     * Opérateur permettant d'effectuer la concaténation et l'affectation
     * Exemple : seq1 = seq1 + seq2; //seq1 concaténé avec la séq2
     * Plus rapide que de faire :
     * seq1.operator= (seq1.operator+( seq2 ) ).
     */
     SequenceMessages& operator+= (const SequenceMessages& seq);

     /**
      * operator[]
      * Opérateur permettant d'accéder au ième message de la liste.
      * Exemple : mess0 = seq1[0] ; //mess0 est le premier message de la séquence.
      */
     Message operator[] (int pos) const;
        
    /**
     * operator<<
     * L'opérateur permet de faire l'affichage d'une sequence de messages.
     */
    friend ostream& operator << (ostream& os, const SequenceMessages& seq);
    
    /**
     * test
     * Permet de tester la classe en supposant que le type de Message est un 'char'.
     * Cette fonction est une fonction statique amie.
     */    
     static void test();

};    
#endif //SEQUENCEMESSAGES_HPP
