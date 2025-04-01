#include "Dictionnaire.hpp"

#include <iomanip> //Permet de formatter l'affichage de sortie cout

/**
 *  \author Marc Chaumont
 *  \date   24 mars 2006
 *
 *  La classe dictionnaire permettant la gestion des Sequences de messages.
 *
 */
    
/**
 * Constructeur par défaut.
 */
Dictionnaire::Dictionnaire() {
}    

/**
 * addEntry()
 * Ajoute une entrée au dictionnaire c'est à dire une sequence de messages. 
 * \param seq : une séquence de messages.
 */
void Dictionnaire::addEntry(const SequenceMessages& seq) {
    
#ifndef RELEASE
  //PRECONDITION
  if (isPresent(seq)) {
    cout<<"Dictionnaire::addEntry: Erreur, la sequence est déjà présente dans le dictionnaire\n";
    exit(-1);  
  }  
#endif //RELEASE

  //AJOUT DE LA SEQUENCE AU DICTIONNAIRE
  dico.push_back(seq);
}    

/**
 * isPresent()
 * \param seq : une séquence de message.
 * \return vrai si une sequence de message est présente dans le dictionnaire.
 */
bool Dictionnaire::isPresent(const SequenceMessages& seq) const {

  int taille   = dico.size();      //Taille du dico
  int i        = 0;                //Indice de parcour
  bool present = false;            //Booleen indiquant la présence
  while ( (i<taille) && (!present) ) {
    present = (seq == dico[i]);    //SequenceMessages::Operator==
    i++;
  }
  
  return present;
}    
    
/**
 * getIndice()
 * \return l'indice de la séquence si elle est présente dans le dictionnaire 
 * retourne -1 sinon.
 */
int Dictionnaire::getIndice(const SequenceMessages& seq) const {
    
  int taille   = dico.size();      //Taille du dico
  int i        = 0;                //Indice de parcour
  int indice   = -1;               //Indice de retour
  bool present = false;            //Booleen indiquant la présence
  
  while ( (i<taille) && (!present) ) {
      
    if (seq == dico[i]) {          //SequenceMessages::Operator==
      present = true;              //La séquence est présente
      indice  = i;                 //Positionnement de l'indice de retour
    }    
    i++;
  }
  
  return indice;
}


/**
 * indiceValide()
 * \params incide : indice d'entrée dans le dictionnaire.
 * \return vrai si l'indice est valide et faux sinon.
 */
bool Dictionnaire::indiceValide(int indice) const {
    
  return ( (indice>=0) && (indice < dico.size()) );
}

/**
 * getSequence()
 * \return la séquence de message à l'indice indice.
 */
const SequenceMessages& Dictionnaire::getSequence(int indice) const {

#ifndef RELEASE
  //PRECONDITION
  int taille = dico.size(); //Taille du dico
  if ( (indice<0) || (indice >= taille) ) {
    cout<<"Dictionnaire::getSequence: Erreur, débordement d'indice.\n";
    cout<<"  indice = "<<indice<<", or les indices vont de 0 à "<<taille-1<<endl;
    exit(-1);  
  }  
#endif //RELEASE
  
  return dico[indice];
}


/**
 * operator<<
 * L'opérateur permet de faire l'affichage du dictionnaire.
 * 
 * Remarque : l'opérateur est une fonction amie et ne fait donc pas partie de 
 * la classe SequenceMessages
 */
ostream& operator<< (ostream& os, const Dictionnaire& d) {

  int i;
  int taille = d.dico.size();
                                
  //PARCOUR DU DICTIONNAIRE
  for(i = 0; i < taille; i++) {
    os<<"-------------------------------------------------------------------------\n";
    os<< " | " <<setw(5) << i <<" | "; //Ecriture dans le flux de sortie
    os<<d.dico[i]<<endl;               //Ecriture dans le flux de sortie
  }
  os<<"-------------------------------------------------------------------------";
  
  //RETOURN LE FLUX DE SORTIE      
  return os;
}

/**
 * test()
 * Fonction de test de la classe dictionnaire
 */
void Dictionnaire::test() {
    
  Dictionnaire dico;
  SequenceMessages seqmess1;
  SequenceMessages seqmess2;
  SequenceMessages seqmess3;
  Message m1 = 'a';
  Message m2 = 'b';
  Message m3 = 'c';
  Message m4 = 'd';
  Message m5 = 'e';
  seqmess1  += m1 ;
  seqmess1  += m2 ;
  seqmess1  += m3 ;
  cout<<seqmess1<<endl;
  seqmess2  += m4; 
  seqmess2  += m5;
  cout<<seqmess2<<endl;
  seqmess3   = seqmess1;
  cout<<seqmess3<<endl;
  dico.addEntry(m1);
  dico.addEntry(seqmess1);
  dico.addEntry(seqmess2);
  cout<<dico<<endl;
  
  cout<<"Indice de seqmess2   : "<<dico.getIndice(seqmess2) <<endl;
  cout<<"Présence de seqmess3 : "<<dico.isPresent(seqmess3) <<endl;
  cout<<"Présence de m4       : "<<dico.isPresent(m4) <<endl;
  cout<<"Indice de m4         : "<<dico.getIndice(m4) <<endl;
  cout<<"Indice 2 valide ?    : "<<dico.indiceValide(2)<<endl;
  cout<<"Indice 5 valide ?    : "<<dico.indiceValide(5)<<endl;
}
