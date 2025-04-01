#include "Lzw.hpp"

/**
 *  \author Marc Chaumont
 *  \date   23 mars 2006
 *
 *  La classe lzw permet le codage et le d codage utilisant l'alforithme lzw
 *
 */
 
/********************************/
/* METHODE POUR LA COMPRESSION * /
/********************************/
    
/**
 * litMessage()
 * Lit un message dans le flux d'entr e.
 *
 * Remarque : M thode utilis  lors de la compression
 */
Message Lzw::litMessage() {
  char mess;
  (*ifs).get(mess);
  //cout<<"Lzw::litMessage: Lecture de "<<mess<<endl;
  return mess;
}    

/**
 * endOfInputFlux()
 * Indicateur de fin de flux.
 */
bool Lzw::endOfInputFlux() {
  return ( (*ifs).eof() || ((*ifs).peek() == EOF) );
}    

/**
 * ecritCode()
 *  crit un code dans le flux de sortie.
 * \param code : le code    crire. Ce code est un indice correspond   une entr e dans 
 * le dictionnaire.
 *
 * Remarque : M thode utilis e lors de la compression.
 */
void Lzw::ecritCode(Code code) {
                          
  //Ecrire sur nbBit un entier dans un vecteur par exemple ...
  //Le flushing sera fait   la fin...
  if(code >= valMaxCode) {
    coutCodage += tailleCode;         
    cout<<"Lzw::ecritCode : @"<<SP<<"(taille="<<tailleCode<<", message SP)\n";
    bitStream->WriteBits(SP, tailleCode);
    tailleCode += 1;             //On augmente d'un bit la taille des codes
    valMaxCode = 2 * valMaxCode; //La valeur du code maxi est donc 2 fois plus grande...
  }  
  cout<<"Lzw::ecritCode : @"<<code<<"(taille="<<tailleCode<<", message= "<<dico.getSequence(code)<<")\n";
  bitStream->WriteBits(code, tailleCode);
  coutCodage += tailleCode;         
}    

/**********************************/
/* METHODE POUR LA DECOMPRESSION * /
/**********************************/

/**
 * litCode()
 * Lit un code dans le flux d'entr e.
 * \return le code lu dans le flux d'entr e. Ce code est un indice correspondant 
 *   une entr e dans le dictionnaire.
 *
 * Remarque : M thode utilis e lors de la d compression.
 */
Code Lzw::litCode() {

  Code code;
  
  //LECTURE DU CODE (tenant sur 'tailleCode' bits)
  code = bitStream->ReadBits(tailleCode);

  while (code == SP) {
    //cout<<"Lzw::litCode : @"<<SP<<"(taille="<<tailleCode<<", message SP)\n";    
    tailleCode += 1;             //On augmente d'un bit la taille des codes
    valMaxCode = 2 * valMaxCode; //La valeur du code maxi est donc 2 fois plus grande...
    //LECTURE DU CODE (tenant sur 'tailleCode' bits)
    code = bitStream->ReadBits(tailleCode);
  }
   
  //cout<<"Lzw::litCode : @"<<code<<"(taille="<<tailleCode<<")\n";    
  return code; //Retourne le code lu
}

/**
 * ecritMessage()
 * Ecrit des s quences de messages dans le flux de sortie.
 * \param seq : la s quence de messages    crire.
 *
 * Remarque : M thode utilis  lors de la d compression.
 */
void Lzw::ecritMessages(SequenceMessages seq) {
  
  int i;
  for (i=0; i<seq.taille(); i++) { 
    (*ofs) << seq[i]; //Pas optimal car on utilise un parcour sur liste dans operator[]
  }
}    
        
/**
 * Constructeur par d faut.
 */
Lzw::Lzw() {
  cout<<"Lzw::Lzw: Ce contructeur n'est pas   utiliser\n";
  exit(-1);
}    

/**
 * Constructeur par valeur
 */
Lzw::Lzw(ifstream* ifs, ofstream* ofs, Lzw::InOutStatus iostatus) {

  /*TEST D'OUVERTURE*/
  if (!ifs->is_open()) {
    cout<<"Lzw::Lzw(...): Erreur, le flux d'entr e n'est pas ouvrable\n";
    exit(-1);
  }    
  if (!ofs->is_open()) {
    cout<<"Lzw::Lzw(...): Erreur, le flux de sortie n'est pas ouvrable\n";
    exit(-1);
  }    

  (*this).iostatus   = iostatus; //Indique si l'on fait une compression ou une d compression.
  
  //SI L'ON EFFECTUE UN CODAGE
  if (iostatus == CODAGE) {
    (*this).ifs  = ifs;                 //Flux d'entr e Messages
    (*this).ofs  = NULL;
    bitStream    = new TBitStream(ofs); //Flux de sortie Codes
  }
  //SI L'ON EFFECTUE UN DECODAGE
  else {
    (*this).ofs  = ofs;                 //Flux de sortie Messages
    (*this).ifs  = NULL;
    bitStream    = new TBitStream(ifs); //Flux d'entr e Codes
  }  
  
  (*this).coutCodage = 0;   //Comptabilisation du nombre de bits n cessaire au codage.
  
  //SES VALEURS SONT EN RAPPORT AVEC LA FACON DONT EST INITIALISE LE DICTIONNAIRE
  (*this).SP         = 7;   //Valeur du caract re de controle ASCII = 07 CARACTERE BELL 
  (*this).tailleCode = 8;   //Taille des codes en nombre de bits
  (*this).valMaxCode = 256; // = 2^tailleCode
  
  /*INITIALISATION DU DICTIONNAIRE*/
  initDictionnaire();
}    

/**
 * initDictionnaire()
 * Remplit le dictionnaire avec les messages initialement connu au codage et 
 * au d codage
 */
void Lzw::initDictionnaire() {
  
  int i;
  SequenceMessages seq;
  
  //Tous les caract re ASCII (valeur sur 8 bits) sont ajout s au dictionnaire
  for (i=0; i <= 255; i++) {
    seq = (Message) i;
    dico.addEntry(seq);
  }

  (*this).SP         = 7;   //Valeur du caract re de controle ASCII = 07 CARACTERE BELL 
  (*this).tailleCode = 8;   //Taille des codes
  (*this).valMaxCode = 256; // = 2^tailleCode
}

/**
 * codage()
 * Compression lzw 
 */
void Lzw::codage() {// On se base sur le pseudo code

  Message t;
  SequenceMessages s,u;

  s = litMessage();
  while (!endOfInputFlux()) {
    t = litMessage();
    u = s+t;
    if (!dico.isPresent(u)) {
      ecritCode(dico.getIndice(s));
      dico.addEntry(u);
      s = t;
    }else {
      s = u;
    }
  }
  ecritCode(dico.getIndice(s));
  delete bitStream;           //Pour s'assurer du flush des valeurs
}

/**
 * decodage()
 * Decompression lzw
 */
void Lzw::decodage(unsigned long nbBitTotal) { // On se base sur le pseudo code

  Message t;
  SequenceMessages s,u;
  Code code;

  code = litCode();

  u = dico.getIndice(code);
  ecritMessages(u);

    for (int i =0; i < nbBitTotal; i++){ // On ne peut pas manipuler directement nbBitTotal, on utilise une boucle for
    Code code2 = litCode();
    if (dico.isPresent(code2)) {
      s = dico.getSequence(code2);
    }else {
      s = u + t;
    }
    ecritMessages(s);
    t = s[0];
    dico.addEntry(u+t);
    u = s;
  }

}

