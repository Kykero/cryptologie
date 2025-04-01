#include "SequenceMessages.hpp"
#include <iterator>

/**
 *  \author Marc Chaumont
 *  \date   22 mars 2006
 *
 *  La classe SequenceMessage permet la manipulation de liste de messages.
 *
 */

    
/**
 * Constructeur par défaut.
 */
SequenceMessages::SequenceMessages() {
  //Efface tous les élements de la liste    
  listmessages.clear();   
}

/**
 * Constructeur avec un seul message.
 */
SequenceMessages::SequenceMessages(Message message) {
    
  //Efface tous les élements de la liste    
  listmessages.clear(); 
  
  //Ajout du message dans la liste
  listmessages.push_back( message );
}    

/**
 * Constructeur de clonage.
 */
SequenceMessages::SequenceMessages(const SequenceMessages& seq) {

  //Efface tous les élements de la liste    
  listmessages.clear(); 

  list<Message>::const_iterator it; //Utilisation de la notion d'itérateur pour le 
                                    //parcour de la liste
                                
  //PARCOURS DE LA LISTE DU DEBUT A LA FIN ET AJOUT DES messages DE seq
  for(it = seq.listmessages.begin(); it != seq.listmessages.end(); it++) {
    listmessages.push_back( (*it) );
  }
}    

/**
 * taille()
 * \return le nombre de message contenu dans la sequence.
 */
int SequenceMessages::taille() const {
  return listmessages.size();
}

/**
 * operator==
 * Opérateur de comparaison.
 * L'opérateur permet d'utiliser correctement l'opération ==
 * Exemple : seq1 == seq2; //L'opérateur '==' est bien défini.
 */
bool SequenceMessages::operator==(const SequenceMessages& seq) const {
  return ((*this).listmessages == seq.listmessages); //operator == des listes
}    

/**
 * operator=
 * Opérateur d'affectation.
 * L'opérateur permet d'utiliser correctement l'opération =
 * Exemple : seq1 = seq2; //L'opérateur '=' est bien défini.
 */
SequenceMessages& SequenceMessages::operator=(const SequenceMessages& seq) {

  listmessages.clear(); //Efface tous les élements de la liste

  list<Message>::const_iterator it; //Utilisation de la notion d'itérateur pour le 
                                    //parcour de la liste
                                
  //PARCOUR DE LA LISTE DU DEBUT A LA FIN ET AJOUT DES messages DE seq
  for(it = seq.listmessages.begin(); it != seq.listmessages.end(); it++) {
    listmessages.push_back( (*it) );
  }
  
  return (*this);
}

/**
 * operator+
 * Opérateur permettant d'effectuer la concaténation.
 * Exemple : seq1 + seq2; //seq1 concaténé avec la séq2.
 */
SequenceMessages SequenceMessages::operator+ (const SequenceMessages& seq) const {
    
  SequenceMessages seqRetour;

  list<Message>::const_iterator it; //Utilisation de la notion d'itérateurs pour le 
                                    //parcour de la liste

  //PARCOUR DE LA LISTE (*this).listmessages DU DEBUT A LA FIN ET AJOUT DES messages DANS seqRetour
  for(it = listmessages.begin(); it != listmessages.end(); it++) {
    seqRetour.listmessages.push_back( (*it) );
  }
                                
  //PARCOUR DE LA LISTE seq.listmessages DU DEBUT A LA FIN ET AJOUT DES messages DANS seqRetour
  for(it = seq.listmessages.begin(); it != seq.listmessages.end(); it++) {
    seqRetour.listmessages.push_back( (*it) );
  }
  
  return seqRetour;
}

/**
 * operator+
 * Opérateur permettant d'effectuer la concaténation et l'affectation
 * Exemple : seq1 = seq1 + seq2; //seq1 concaténé avec la séq2
 * Plus rapide que de faire :
 * seq1.operator= (seq1.operator+( seq2 ) ).
 */
SequenceMessages& SequenceMessages::operator+= (const SequenceMessages& seq) {
    
  list<Message>::const_iterator it; //Utilisation de la notion d'itérateurs pour le 
                                    //parcour de la liste

  //PARCOUR DE LA LISTE seq.listmessages DU DEBUT A LA FIN ET AJOUT DES messages DANS listmessages
  for(it = seq.listmessages.begin(); it != seq.listmessages.end(); it++) {
    listmessages.push_back( (*it) );
  }
  
  return (*this);
}

/**
 * operator[]
 * Opérateur permettant d'accéder au ième message de la liste.
 * Exemple : mess0 = seq1[0] ; //mess0 est le premier message de la séquence.
 */
Message SequenceMessages::operator[] (int pos) const {
    
#ifndef RELEASE
  if ( (pos<0) || (pos>=listmessages.size()) ) {
    cout<<"SequenceMessages::operator[]: Erreur: le "<<pos<<"eme message n'existe pas\n";
    cout<<"   le paramètre doit être compris entre 0 et "<<listmessages.size()-1<<endl;
    exit(-1);
  }
#endif //RELEASE

  list<Message>::const_iterator it; //Utilisation de la notion d'itérateurs pour le 
                                    //parcour de la liste

  //PARCOUR DE LA LISTE listmessages DU DEBUT AU ième MESSAGE
  int indice = 0;
  it = listmessages.begin();
  while (indice < pos) {
    it++;     //On avance dans la liste ...
    indice++; //On avance l'indice
  }    
  //RETOURNE LE pos-ème MESSAGE
  return (*it);
}

/**
 * operator<<
 * L'opérateur permet de faire l'affichage d'une sequence de messages.
 * 
 * Remarque : l'opérateur est une fonction amie et ne fait donc pas partie de 
 * la classe SequenceMessages.
 */
ostream& operator<< (ostream& os, const SequenceMessages& seq) {

  list<Message>::const_iterator it; //Utilisation de la notion d'itérateurs pour le 
                                    //parcour de la liste
                                
  //PARCOUR DE LA LISTE DU DEBUT A LA FIN...                                
  os <<" . ";
  for(it = seq.listmessages.begin(); it != seq.listmessages.end(); it++) {
    os<< (*it) <<" . ";  //Ecriture dans le flux de sortie
  }
  
  //RETOURN LE FLUX DE SORTIE      
  return os;
}

/**
 * test
 * Permet de tester la classe en supposant que le type de message est char
 * Fonction statique.
 */
void SequenceMessages::SequenceMessages::test() {

  SequenceMessages seqmess1;
  SequenceMessages seqmess2;
  Message m1 = 'a';
  Message m2 = 'b';
  Message m3 = 'c';
  Message m4 = 'd';
  Message m5 = 'e';  
  
  seqmess1 = m1;                       //Test operator=
  cout<<"seqmess1 = m1;\n";            
  cout<<"seqmess1 : "<<seqmess1<<endl;  //Test operator<<
  
  seqmess1 = seqmess1 + m2;            //Test operator+ sur sequence et message et test operator=
  cout<<"seqmess1 = seqmess1 + m2\n";
  cout<<"seqmess1 : "<<seqmess1<<endl;  //Test operator<<
  
  seqmess1 += m3;                      //Test operator+=
  cout<<"seqmess1 += m3;\n";
  cout<<"seqmess1 : "<<seqmess1<<endl;  //Test operator<<
  
  seqmess2 = m4;                       //Test operator=
  cout<<"seqmess2 = m4;\n";
  cout<<"seqmess2 : "<<seqmess2<<endl;  //Test operator<<
  
  seqmess1 = seqmess1 + seqmess2;      //Test operator+ sur des séquences et test operator=
  cout<<"seqmess1 = seqmess1 + seqmess2;\n";
  cout<<"seqmess1 : "<<seqmess1<<endl;  //Test operator<<
  
  SequenceMessages seqmess3(seqmess1);  //Test constructeur de type clonage
  cout<<"SequenceMessage seqmess3(seqmess1);\n";
  cout<<"seqmess3 : "<<seqmess3<<endl;  //Test operator<<

  //Test operator==
  cout<<"seqmess3("<<seqmess3<<") == seqmess1 ("<<seqmess1<<") :"<< (seqmess3 == seqmess1)  <<endl;
  cout<<"seqmess2("<<seqmess2<<") == seqmess1 ("<<seqmess1<<") :"<< (seqmess2 == seqmess1)  <<endl;
  
  SequenceMessages seqmess4('z');       //Test constructeur par valeur
  cout<<"SequenceMessage seqmess4('z');\n";
  cout<<"seqmess4 : "<<seqmess4<<endl;  //Test operator<<
  
  cout<<"seqmess3[0] = "<<seqmess3[0]<<endl; //Test operator[]
  cout<<"seqmess3[1] = "<<seqmess3[1]<<endl;
  cout<<"seqmess3[2] = "<<seqmess3[2]<<endl;
  cout<<"seqmess3[3] = "<<seqmess3[3]<<endl;
}  
  
  
  
