#include "Node.hpp"

/**
 *  \author Marc Chaumont
 *  \date   20 mars 2006
 *
 *  La classe Node permet la construction d'un arbre binaire. Un noeud contient : 
 *  - un fils gauche : fils_0 (fils à qui l'on attribuera la valeur 0 lors du 
 *    parcour de l'arbre). Ce fils est un pointeur sur un autre noeud.
 *  - un fils droit : fils_1 (fils à qui l'on attribuera la valeur 1 lors du 
 *    parcour de l'arbre). Ce fils est un pointeur sur un autre noeud.
 *  - un message (caractère). Ce message a une signification uniquement 
 *    pour les feuilles de l'arbre.
 *
 */
    
    /**
     * Constructeur par défaut.
     */
    Node::Node() {
      fils_0  = NULL;
      fils_1  = NULL;
      message = ' ';
      proba   = 0;
    }
    
    /**
     * Constructeur d'une feuille.
     */
    Node::Node(char message, double proba) {
      fils_0  = NULL;
      fils_1  = NULL;
      (*this).message = message;
      (*this).proba   = proba;
    }        
              
    /**
     * Constructeur d'un noeud intermédiaire.
     */
    Node::Node(Node* node1, Node* node2, double proba) {
      fils_0          = node1;    //Recopie par pointeur ...
      fils_1          = node2;    //Recopie par pointeur ...
      (*this).proba   = proba;
    }              

    /**
     * isFeuille()
     * \return vrai si le noeud est une feuille.
     */
    bool Node::isFeuille() {
        return ( (fils_0 == NULL) && (fils_1 == NULL) );
    }
    
    /**
     * getFils0()
     * \return un pointeur sur le fils gauche.
     */
    Node* Node::getFils0() {
      return fils_0;
    }
    /**
     * getFils1()
     * \return un pointeur sur le fils droit.
     */
    Node* Node::getFils1() {
      return fils_1;
    }
    
    /**
     * getMessage()
     * \return le message associé au noeud.
     */
    char Node::getMessage() {
      return message;  
    }    

    /**
     * getProba()
     * \return la proba associée au noeud.
     */
    double Node::getProba() {
      return proba;  
    }    
