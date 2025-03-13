/*
*Debusschere Mathieu
 *Fernandez Nolahn
 *TP1 Partie 1
 */

#include "Table.hpp"

/**
 *  \author Marc Chaumont
 *  \date   20 mars 2006
 *
 *  La classe Table permet de g�rer la table contenant les messages 
 *  (symboles) et les probabilit�s associ�es. La classe Table ne sert que de 
 *  conteneur et ne met � disposition que trois accesseurs : 
 *  - ajout d'un noeud : m�thode addEntry.
 *  - suppression d'un noeud : m�thode delEntry.
 *  - nombre d'entr�e dans la table (nombre de noeuds): m�thode nbEntry.
 *
 *  Remarque : L'algorithme d'Huffman it�re sur cette table (message <-> proba) 
 *  pour construire l'arbre d'Huffman.
 */

    Table::Table() {
    }    
    
    Table::~Table() {
      cout<<"Table::~Table: Le destructeur n'est pas écrit...\n";
    }    

    /**
     * nbEntry()
     * \return Retourne le nombre d'entr�e dans la table.
     */
    int Table::nbEntry() {
      return table.size();
    }    

    /**
     * getEntry()
     * \return pointeur sur i�me noeud de la table.
     */
    Node* Table::getEntry(int i) {
#ifndef RELEASE
      if ( (i<0) || ( i>= nbEntry()) ) {
        cout << "Nombre d'éléments dans Table : " << nbEntry() << endl; //Déboggage
        cout<<"Node::getEntry: Erreur, l'entrée "<<i<<" n'existe pas\n";
        exit(-1);
      }    
#endif
      return table[i];
    }    

    /**
     * setEntry()
     * positionne le i�me noeud de la table.
     */
    void Table::setEntry(int i, Node* node) {
#ifndef RELEASE
      if ( (i<0) || (i>=nbEntry()) ) {
        cout<<"Node::setEntry: Erreur, l'entr�e "<<i<<" n'existe pas\n";
        exit(-1);
      }    
#endif
      table[i] = node;
    }

    /**
     * addEntry()
     * Ajoute dans la table un noeud (message et probabilit� associ�e)
     * \attention l'ajout doit �tre fait de mani�re � conserver l'ordre 
     * d�croissant des probabilit�s.
     */
void Table::addEntry(Node* node) {
      //iterateur depuis le début de la table
      auto it = table.begin();

      // Trouver la position où insérer en gardant l'ordre décroissant des probabilités
      while (it != table.end() && (*it)->getProba() > node->getProba()) {
        ++it;
      }

      // Insérer le nœud à la bonne position
      table.insert(it, node);
    }

    /**
     * delEntry()
     * Supprime le dernier �l�ment de la table.
     * \attention La suppression concerne le dernier �l�ment de la table.
     * \attention La suppression ne d�salloue pas le noeud...
     * \return pointeur sur le noeud que l'on vient d'enlever de la table.
     */
    Node* Table::delEntry() {
#ifndef RELEASE
      if (nbEntry() == 0) {
        cout<<"Node::delEntry: Erreur, la table est vide\n";
        exit(-1);
      }    
#endif
      Node* node = table[nbEntry()-1];
      table.pop_back();
      return node;
    }
