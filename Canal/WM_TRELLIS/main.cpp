/*
  Name: main.cpp
  Author: Marc CHAUMONT
  Date: 22/03/16
  Description: Permet d'appeler le codeur et le décodeur convolutif
               Les routines sont extraites de livre de Cox-Miller-Bloom (Digital Watermarking)
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "WM_TRELLIS.hpp"
using namespace std;

/*VARIABLE GLOBALES*/
const int STR_LEN = 1000;
char message[STR_LEN];         // Message à coder sous sa version caractère
int message_binaire[STR_LEN];  // Message à coder sous sa version caractère
int messageLength;             // Longueur du message en nombre de bits

//==============================================================================
void printUsage(char* name) {

  cout << "\nUsage : "
       <<name
       <<" message_binaire\n";
  exit(-1);
}

//==============================================================================
void processOptionsInLine(int argc, char** argv){

  if (argc != 2) {
    printUsage(argv[0]);
  }

  //RECOPIE DE LA CHAINE DE CARACTERE CONTENANT DES CARACTERES '0' ET '1'
  strcpy(message, argv[1]);
}

//==============================================================================
void processAttribute(){

  messageLength = strlen(message);   //Longueur du message

  cout<<"Le mot de source (\'a coder) est : \n";
  for(int i=0; i<messageLength; i++) {
    message_binaire[i] = message[i]-'0'; //Transformation de la chaine de caractère en suite d'entier 0 ou 1 (les bits ...)
    if ((message_binaire[i] != 0) && (message_binaire[i] != 1)) {
      cout<<"main: Erreur, le message n'est pas correct; cela doit etre une succession de 0 et de 1...\n";
      exit(-1);
    }
    cout<<message_binaire[i]<<" , ";
  }
  cout<<endl<<endl;
}

//==============================================================================
int main(int argc, char *argv[]) {

  processOptionsInLine(argc, argv);
  processAttribute();

  int taille_message_code = messageLength*4;
  int* mc = new int[taille_message_code]; // Le message codé

  WM_TRELLIS::TrellisEncode(message_binaire, messageLength, mc);

  cout<<"Mot de code r\'esultant du codage convolutif :\n";
  for(int i=0; i<taille_message_code; i++) {
    cout<<mc[i]<<" , ";
  }
  cout<<endl<<endl;

  cout<<"Bruit sur le mot de code :\n";
  mc[4] = ((mc[4]+1)%2); // On inverse la quatrième position
  for(int i=0; i<taille_message_code; i++) {
    cout<<mc[i]<<" , ";
  }
  cout<<endl<<endl;

// -- Décodage --
  int* decode = new int[messageLength]; // Le message décodé

  WM_TRELLIS::TrellisDecode( mc, taille_message_code, decode);
  cout<<"D\'ecodage par Viterbi :\n";
  for(int i=0; i<messageLength; i++) {
    cout<<decode[i]<<" , ";
  }
  cout<<endl<<endl;

  delete [] mc;
  delete [] decode;
}
