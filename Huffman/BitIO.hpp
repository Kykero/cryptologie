
/**
 *  \author Stéphane Pateux
 *  \date  1998 ?
 *
 *  Ensembles de routines pour la lecture au niveau du bit
 *  dans des fichiers.
 */


#ifndef _BIT_IO__H_
#define _BIT_IO__H_


#include <stdio.h>
#include <stdlib.h>
//using namespace std;


/* definitions des statuts d'ouverture de fichier */
#define READING 0
#define WRITING 1
#define APPENDING 2

/* symbole de fin de fichier */
//Modif Marc Chaumont #define EOF_BIT 128


/*-------  DEFINITION DES TYPES ---------*/

typedef unsigned char uchar;

typedef struct tagBIT_FILE
{
  FILE *file;
  unsigned long taille; /* nombre de bits lus ou deja ecrits */

  int status;   /* READING ou WRITING */
  char octetWaiting;
  int posInOctet;
} BIT_FILE;


class TBitStream
{
  istream *IStream;
  ostream *OStream;

  unsigned long taille; /* nombre de bits lus ou deja ecrits */

  int status; /* READING ou WRITING */
  char octetWaiting;
  int posInOctet; 
  int eofRead; // nombre de bits lus en fin de fichier
public:
  TBitStream(istream *pStream);
  TBitStream(ostream *pStream);

  //       ATTENTION
  //
  //  le destructeur fait une operation de flush sur les derniers bits
  //  et comble au besoin par des 0 afin de remplir l'octetWaiting.
  ~TBitStream();
 
  void WriteBit(uchar bit);
  void WriteBits(unsigned long val, int nbBits);
  void WriteSignedBits(long val, int nbBits);

  unsigned char ReadBit();
  void UnReadBit(unsigned char bit);
  unsigned long  ReadBits(int nbBits);
  long ReadSignedBits(int nbBits);

  inline unsigned long Taille() { return taille; };

  int GetEofRead(){return eofRead;}

};



/*-------  ROUTINES -----------*/

extern BIT_FILE *BIT_FOPEN_READ(char *filename);
extern BIT_FILE *BIT_FOPEN_WRITE(char *filename);
extern void BIT_FWRITE(BIT_FILE *bitFile, uchar bit);
//Modif Marc Chaumont extern uchar BIT_FREAD(BIT_FILE *bitFile);
extern void BIT_FUNREAD(BIT_FILE *bitFile, uchar bit);
extern void BIT_FCLOSE(BIT_FILE *bitFile);


#endif /* _BIT_IO__H_ */




