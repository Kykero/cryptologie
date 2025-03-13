/*
*Debusschere Mathieu
 *Fernandez Nolahn
 *TP1 Partie 1
 */

#include "BuildHuffmanTree.hpp"

#include <list>
#include <queue>

/**
 *  \author Marc Chaumont
 *  \date   20 mars 2006
 *
 *  La classe BuildHuffmanTree permet comme son nom l'indique de construire 
 *  l'arbre d'Huffman et ainsi attribuer   chaque message (symbole) un code 
 *  pr fix .
 *  Remarque : L'algorithme d'Huffman it re sur la table (classe Table) pour 
 *  construire l'arbre d'Huffman.
 */

    /**
     * BuildHuffmanTree()
     * Constructeur par d faut (rien   faire).
     */
    BuildHuffmanTree::BuildHuffmanTree() {
    }    

    /**
     * initTable()
     * Inititialise la table contenant les messages et les probabilit  ceci 
     *   partir de l'histogramme pr alablement construit.
     * \param histogram : ensemble des association message <-> probabilit . 
     */
    void BuildHuffmanTree::initTable(const vector<CoupleMessageProba>& histogram) {
      for (const auto& couple : histogram) {
        Node* newNode = new Node(couple.getMessage(), couple.getProba());
        table.addEntry(newNode);
      }
      cout << "Table initialisé avec : " << table.nbEntry() << " entries";
    }    

    /**
     * ~BuildHuffmanTree
     * La destruction du contenu de la table est effectu e par le destructeur 
     * de la classe Table.
     */
    BuildHuffmanTree::~BuildHuffmanTree() {
    }

    /**
     * buildTree()
     * Algorithme de Huffman :
     * M thode permettant de construire l'arbre de d'association message <-> code.
     */
    void BuildHuffmanTree::buildTree() {

      // Récupération des probabilités dans un vecteur différent afin de pouvoir le manipuler sans risque
      vector<Node*> list_nodes;
      while (table.nbEntry() > 0) {
        list_nodes.push_back(table.delEntry());  // Prend le dernier élément, donc plus petite proba (voir la méthode addEntry dans Table.cpp)
      } // On obtient un vecteur trié par ordre croissant des probabilités


      //Construction de l'arbre
      while (list_nodes.size() > 1) {
        //On commence par récupérer les deux plus petites probabilités
        Node* fils_gauche = list_nodes.front();
        list_nodes.erase(list_nodes.begin());
        Node* fils_droit = list_nodes.front();
        list_nodes.erase(list_nodes.begin());

        //On crée maintenant un parent en sommant les deux
        Node* parent = new Node(fils_gauche,fils_droit,fils_gauche->getProba()+fils_droit->getProba());

        //On réinsère les données en gardant l'ordre décroissant qu'on a fait a l'entrée
        auto index = list_nodes.begin();
        while (index != list_nodes.end() && (*index)->getProba() < parent->getProba()) {
          ++index;
        }
        list_nodes.insert(index, parent);
      }

      //On ajoute à la racine les nouveaux noeuds
      if (!list_nodes.empty()) {
        table.addEntry(list_nodes[0]);
      }
    }

                   
    /**
     * getTree()
     * M thode permettant de r cup rer l'arbre de Huffman.
     */
    Node* BuildHuffmanTree::getTree() {
        return table.getEntry(0);
    }    

    /**
     * printTree()
     * Affiche l'arbre (appel r cursif).
     */
    static int cmpt;
    void BuildHuffmanTree::printTree() {
       Node* node = getTree();
       cmpt = 0;
       cout<<"Affichage pour Dotty\n";
       cout<<"digraph g {\n";
       printTree(cmpt, node, 0);
       cout<<"}\n";       
    }    
    /**
     * printTree()
     * Affiche l'arbre.
     */
    void BuildHuffmanTree::printTree(int numPere, Node* node, int bit) {

      int numNoeud = ++cmpt; //Numero attribu  au noeud
      
      if (numNoeud > 1) {
        cout<<"  node_"<<numPere<<" -> node_"<<cmpt<<" [label ="<<bit<<"];\n";
      }    
      if (node->isFeuille()) {
        cout<<"  node_"<<cmpt<<" [label ="<<'"'<<"message : "<<node->getMessage()
                                <<"\\"<<"nproba : "<<node->getProba()<<'"'<<"];\n";
      }
      else {
        printTree(numNoeud, node->getFils0(), 0);    
        printTree(numNoeud, node->getFils1(), 1);
      }
    }
