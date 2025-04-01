#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

#include "SequenceMessages.hpp"
#include "Dictionnaire.hpp"
#include "Lzw.hpp"

char option;               //Vaut 'c' (pour codage) ou 'd' pour d�codage
char inputFileName [200];  //Le nom du fichier d'entr�e
char outputFileName [200]; //Le nom du fichier de sortie

/*FLUX DE LECTURE POUR LE FICHIER INPUT*/	
ifstream* ifs;
/*FLUX D'ECRITURE POUR LE FICHIER OUTPUT*/	
ofstream* ofs;

//==============================================================================
void printUsage(char* name) {
  cout << "\nUsage : "
       <<name
       <<" option inputFilename outputFilename\n"
       <<" option est le caract�re c (pour codage) ou d (pour decodage)\n";
  exit(-1);
}

//==============================================================================
void processOptionsInLine(int argc, char** argv){  
  if (argc != 4) {
    printUsage(argv[0]);
  }

  //RECOPIE DU NOM DU FICHIER INPUT DANS LA VARIABLE inputFileName
  option = (*argv[1]);
  
  //RECOPIE DU NOM DU FICHIER INPUT DANS LA VARIABLE inputFileName
  strcpy(inputFileName, argv[2]);  

  //RECOPIE DU NOM DU FICHIER OUTPUT DANS LA VARIABLE outputFileName
  strcpy(outputFileName, argv[3]);  
}

//==============================================================================
void processAttributes(char *name) {
    
  if ((option != 'c') && (option != 'd')) {
    cout<<"L'option de codage ou d�codage est mal positionn�e\n";
    printUsage(name);
  }
}

//==============================================================================
/**
 * main.
 * Programme principal ...
 */    
int main(int argc, char *argv[]) {
    
  /*TRAITE LES OPTIONS*/
  processOptionsInLine(argc, argv);

  /*VERIFIE LES OPTIONS*/
  processAttributes(argv[0]);
  
  
  /***************************************/
  /* EFFECTUER LES TESTS DE LA CLASSE :  */
  /*  - SEQUENCESMESSAGES                */
  /*  - DICTIONNAIRE                     */
  /***************************************/
  cout<<"\nTESTER LES CLASSES SEQUENCESMESSAGES et DICTIONNAIRE AVANT TOUTE CHOSE\n";



  if (option == 'c') {
    /*TEST D'OUVERTURE*/
    ifs = new ifstream(inputFileName,ios::in);
    if (!ifs->is_open()) {
      cout<<"main: Erreur, le fichier "<<inputFileName<<" n'est pas ouvrable\n";
      exit(-1);
    }
  
    /*TEST D'OUVERTURE*/
    //ATTENTION : ECRITURE EN MODE BINAIRE : tr�s important pour ne pas avoir des 
    //EOF pr�matur�es
    ofs = new ofstream(outputFileName,ios::out|ios::trunc|ios::binary); 
    if (!ofs->is_open()) {
      cout<<"main: Erreur, le fichier "<<outputFileName<<" n'est pas ouvrable\n";
      exit(-1);
    }  

    //CODAGE
    /*CREATION DE L'OBJET LZW*/
    Lzw algo_codage(ifs, ofs, Lzw::CODAGE);
    algo_codage.codage();
  }    
  else {
    /*TEST D'OUVERTURE*/
    //ATTENTION : LECTURE EN MODE BINAIRE : tr�s important pour ne pas avoir des 
    //EOF pr�matur�es
    ifs = new ifstream(inputFileName,ios::in|ios::binary);
    if (!ifs->is_open()) {
      cout<<"main: Erreur, le fichier "<<inputFileName<<" n'est pas ouvrable\n";
      exit(-1);
    }
  
    /*TEST D'OUVERTURE*/
    ofs = new ofstream(outputFileName,ios::out|ios::trunc);
    if (!ofs->is_open()) {
      cout<<"main: Erreur, le fichier "<<outputFileName<<" n'est pas ouvrable\n";
      exit(-1);
    }  

    //DECODAGE
    /*CREATION DE L'OBJET LZW*/
    Lzw algo_decodage(ifs, ofs, Lzw::DECODAGE);
    cout<<"Entrez le nombre de bits � d�coder\n";
    unsigned long nbBitsTotal;
    cin >> nbBitsTotal;
    algo_decodage.decodage(nbBitsTotal);
  }  
  
  ifs->close();
  ofs->flush(); //Flush des valeurs.
  ofs->close();
  
  delete ifs;
  delete ofs;

  cout<<"exit(1)\n";
  exit(1);
}
