/*
*Debusschere Mathieu
 *Fernandez Nolahn
 *TP1 Partie 1
 */

#ifndef BUILDHUFFMANTREE_HPP
#define BUILDHUFFMANTREE_HPP

#include <iostream>
#include <stdlib.h>
using namespace std;

#include "Table.hpp"
#include "CoupleMessageProba.hpp"

/**
 *  \author Marc Chaumont
 *  \date   20 mars 2006
 *
 *  La classe BuildHuffmanTree permet comme son nom l'indique de construire 
 *  l'arbre d'Huffman et ainsi attribuer � chaque message (symbole) un code 
 *  pr�fix�.
 *  Remarque : L'algorithme d'Huffman it�re sur la table (classe Table) pour 
 *  construire l'arbre d'Huffman.
 */
class BuildHuffmanTree {

private :
    
    Table table; /**< Table contenant les associations message <-> proba.*/
    
    /**
     * printTree()
     * Permet un parcours de l'arbre en profondeur (de mani�re r�cursive) avec 
     * construction du texte permettant la visualisation du graphe sous dotty.
     * \param numPere : le num�ro du noeud p�re.
     * \param node : le noeud courant.
     * \param bit : indique si le noeud est un fils gauche (0) ou un fils droit (1).
     */
    void printTree(int numPere, Node* node, int bit);

public :    

    /**
     * BuildArbreHuffman()
     * Constructeur par d�faut (rien � faire).
     */
    BuildHuffmanTree();  

    /**
     * initTable()
     * Initialise la table contenant les messages et les probabilit�s ceci 
     * � partir de l'histogramme pr�alablement construit.
     * \param histogram : ensemble des associations message <-> probabilit�. 
     */
    void initTable(const vector<CoupleMessageProba>& histogram); 

    /**
     * ~BuildArbreHuffman()
     * La destruction du contenu de la table est effectu�e par le destructeur 
     * de la classe Table.
     */
    ~BuildHuffmanTree();

    /**
     * buildTree()
     * Algorithme de Huffman :
     * M�thode permettant de construire l'arbre de d'association message <-> code.
     */
    void buildTree();

    /**
     * getTree()
     * M�thode permettant de r�cup�rer l'arbre de Huffman.
     */
    Node* getTree();
        
    /**
     * printTree()
     * Permet un parcours de l'arbre en profondeur (de mani�re r�cursive) avec 
     * construction du texte permettant la visualisation du graphe sous dotty.
     */
    void printTree();
    
};    
#endif //BUILDHUFFMANTREE_HPP
