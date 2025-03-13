#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <stdlib.h>
using namespace std;

/**
 *  \author Marc Chaumont
 *  \date   20 mars 2006
 *
 *  La classe Node permet la construction d'un arbre binaire. Un noeud contient : 
 *  - un fils gauche : fils_0 (fils à qui l'on attribuera la valeur 0 lors du 
 *    parcours de l'arbre). Ce fils est un pointeur sur un autre noeud.
 *  - un fils droit : fils_1 (fils à qui l'on attribuera la valeur 1 lors du 
 *    parcours de l'arbre). Ce fils est un pointeur sur un autre noeud.
 *  - un message (caractère). Ce message a une signification uniquement 
 *    pour les feuilles de l'arbre.
 *
 */
class Node {
private :
    
    Node* fils_0; /**< fils gauche */
    Node* fils_1; /**< fils droit */
    char message; /**< message associé au noeud */
    double proba; /**< probabilité associé au noeud */
    
public:
    
    /**
     * Constructeur par défaut.
     */
    Node();

    /**
     * Constructeur d'une feuille.
     */
    Node(char message, double proba);

    /**
     * Constructeur d'un noeud intermédiaire.
     */
    Node(Node* node1, Node* node2, double proba);

    /**
     * isFeuille()
     * \return vrai si le noeud est une feuille.
     */
    bool isFeuille();

    /**
     * getFils0()
     * \return un pointeur sur le fils gauche.
     */
    Node* getFils0();

    /**
     * getFils1()
     * \return un pointeur sur le fils droit.
     */
    Node* getFils1();

    /**
     * getMessage()
     * \return le message associé au noeud.
     */
    char getMessage();

    /**
     * getProba()
     * \return la proba associée au noeud.
     */
    double getProba();

};    
#endif //NODE_HPP
