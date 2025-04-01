#ifndef LZW_HPP
#define LZW_HPP

#include <iostream>
#include <stdlib.h>
using namespace std;

#include "BitIO.hpp"
#include "Dictionnaire.hpp"
#include "SequenceMessages.hpp"
#include <fstream>

typedef unsigned long Code; //D�finition du type Code.

/**
 *  \author Marc Chaumont
 *  \date   23 mars 2006
 *
 *  La classe lzw permet le codage et le d�codage en utilisant l'algorithme lzw.
 *
 */
 
class Lzw {
    
public :
    enum InOutStatus{CODAGE, DECODAGE};
private :
    
    InOutStatus iostatus;  /**< Status indiquant s'il l'on effectue un codage ou un d�codage.*/
    TBitStream *bitStream; /**< Flux pour les Codes.*/
    ifstream* ifs;         /**< Flux pour les messages (iostatus == CODAGE).*/
    ofstream* ofs;         /**< Flux pour les messages (iostatus == DECODAGE).*/
    
    Dictionnaire dico;     /**< Dictionnaire contenant les sequences de messages.*/
    Code SP;               /**< Code de controle : permet d'indiquer au d�codeur 
                                que la taille des codes est maintenant plus grande 
                                de 1 bit.*/
    int tailleCode;        /**< Taille des codes en nombre de bits. Cette taille 
                                grandit au cours du codage.*/
    int valMaxCode;        /**< = 2^tailleCode.*/
    int coutCodage;        /**< cout de codage total en nombre de bits.*/

    /**
     * endOfInputFlux()
     * Indicateur de fin de flux des Messages (utile pour le codage : lecture des messages sur ifs).
    */
    bool endOfInputFlux();


    /********************************/
    /* METHODE POUR LA COMPRESSION * /
    /********************************/
    
    /**
     * litMessage()
     * Lit un message dans le flux d'entr�e.
     *
     * Remarque : M�thode utilis�e lors de la compression.
     */
    Message litMessage();

    /**
     * ecritCode()
     * Ecrit un code dans le flux de sortie.
     * \param le code � �crire. Ce code est un indice correspond � une entr�e dans 
     * le dictionnaire.
     *
     * Remarque : M�thode utilis�e lors de la compression.
     */
    void ecritCode(Code code);

    /**********************************/
    /* METHODE POUR LA DECOMPRESSION * /
    /**********************************/

    /**
     * litCode()
     * Lit un code dans le flux d'entr�e.
     * \return le code lu dans le flux d'entr�e. Ce code est un indice correspondant 
     * � une entr�e dans le dictionnaire.
     *
     * Remarque : M�thode utilis�e lors de la d�compression.
     */
    Code litCode();

    /**
     * ecritMessage()
     * Ecrit les s�quences de messages dans le flux de sortie.
     * \param seq : la s�quence de messages � �crire.
     *
     * Remarque : M�thode utilis�s lors de la d�compression.
     */
     void ecritMessages(SequenceMessages seq);
    
public:
    
    /**
     * Constructeur par d�faut.
     */
    Lzw();

    /**
     * Constructeur par valeur.
     */
    Lzw(ifstream* ifs, ofstream* ofs, InOutStatus iostatus);

    /**
     * initDictionnaire()
     * Remplit le dictionnaire avec les messages initialement connu au codage et 
     * au d�codage.
     */
    void initDictionnaire();


    /**
     * codage()
     * Compression lzw. 
     */
    void codage();

    /**
     * decodage()
     * Decompression lzw. 
     */
     void decodage(unsigned long nbBitTotal);
};    
#endif //LZW_HPP
