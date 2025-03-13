/*
*Debusschere Mathieu
 *Fernandez Nolahn
 *TP1 Partie 1
 */

#ifndef TABLE_HPP
#define TABLE_HPP

#include <iostream>
#include <stdlib.h>
using namespace std;

#include <vector>
#include "Node.hpp"

/**
 *  \author Marc Chaumont
 *  \date   20 mars 2006
 *
 *  La classe Table permet de g�rer la table contenant les messages 
 *  (symboles) et les probabilit�s associ�es. La classe Table ne sert que de 
 *  conteneur et ne met � disposition que trois accesseurs : 
 *  - ajout d'un noeud : m�thode addEntry.
 *  - suppression d'un noeud : m�thode delEntry.
 *  - nombre d'entr�es dans la table (nombre de noeuds): m�thode nbEntry.
 *
 *  Remarque : L'algorithme d'Huffman it�re sur cette table (message <-> proba) 
 *  pour construire l'arbre d'Huffman.
 */
class Table {
private :
    vector<Node*> table; /**< table contenant les associations message <-> proba 
    contenus dans un noeud.*/
    
    /**
     * setEntry()
     * positionne en i�me position le noeud.
     */
    void setEntry(int i, Node* node);    
    
public:
    Table();
    ~Table();

    /**
     * nbEntry()
     * \return Retourne le nombre d'entr�e dans la table.
     */
    int nbEntry();

    /**
     * addEntry()
     * Ajoute dans la table un noeud (message et probabilit� associ�e)
     * \attention l'ajout doit �tre fait de mani�re � conserver l'ordre 
     * d�croissant des probabilit�s.
     */
    void addEntry(Node* node);

    /**
     * delEntry()
     * Supprime le dernier �l�ment de la table.
     * \attention La suppression concerne le dernier �l�ment de la table.
     * \attention La suppression ne d�salloue pas le noeud...
     * \return pointeur sur le noeud que l'on vient d'enlever de la table.
     */
    Node* delEntry();

    /**
     * getEntry(...)
     * \return pointeur sur i�me noeud de la table.
     */
    Node* getEntry(int i);
};    
#endif //TABLE_HPP
