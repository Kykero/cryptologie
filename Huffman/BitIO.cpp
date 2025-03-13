
/**
 *  \author Stéphane Pateux
 *  \date  1998 ?
 *
 *  Ensembles de routines pour la lecture au niveau du bit
 *  dans des fichiers.
 */

#include <iostream>
#include <fstream>
using namespace std;

#include "BitIO.hpp"


//////////////////////////////////////////////////////
//
//   definition des methodes de la classe TBitStream
//
///////////////////////////////////////////////////////


// constructeur
TBitStream::TBitStream(istream *pStream)
{
  taille = 0;
  status = READING;
  posInOctet = 0;
  octetWaiting = 0;
  eofRead = 0;

  IStream = pStream;
  OStream = 0;
};

// constructeur
TBitStream::TBitStream(ostream *pStream)
{
  taille = 0;
  status = WRITING;
  posInOctet = 0;
  octetWaiting = 0;
  eofRead = 0;

  OStream = pStream;
  IStream = 0;
};

// destructeur
TBitStream::~TBitStream()
{
  cout << "positionnement en fin de flux binaire: " << taille << endl;

  if (status!=READING)
  {/* controle qu'il ne reste pas des bits a flusher */
    if (posInOctet!=0)
    {
      for(; posInOctet!=8; posInOctet++)
	octetWaiting <<=1;

      (*OStream) << octetWaiting;
    }
  }

}

// ecriture d'un bit
void TBitStream::WriteBit(uchar bit)
{
  octetWaiting <<=  1;
  if (bit) octetWaiting += 1;
  posInOctet ++;

  if (posInOctet==8)
  {
    (*OStream) << octetWaiting;
    posInOctet = 0;
    octetWaiting = 0;
  }

  taille ++;
}


// ecriture de n bits
void TBitStream::WriteBits(unsigned long val, int nbBits)
{
  unsigned long masque;
  int i;

  masque = 1;
  // decalage du masque
  for(i=1; i<nbBits; i++)
    masque <<= 1;

  for(i=0; i<nbBits; i++)
  {
    if ((val&masque)!=0)
      WriteBit(1);
    else
      WriteBit(0);
    // decalage du masque
    masque >>= 1;
  }
}


// ecriture de n bits signe's
void TBitStream::WriteSignedBits(long val, int nbBits)
{
  unsigned char bitSigne;
  unsigned long absVal;
  unsigned long maxVal;
  int i;

  bitSigne = (unsigned char)( val<0 ? 1 : 0 );
  absVal = ( val<0 ? -val : val);

  if (bitSigne==1)
  { /* nombre negatif */
    // ecriture du bit de signe
    WriteBit(bitSigne);
    // "negation" des bits et ecriture (-> compatibilite norme ISO)
    maxVal = 1;
    for(i=1; i<nbBits-1; i++)
      maxVal <<= 1;
    absVal = maxVal-absVal;
    WriteBits(absVal, nbBits-1);
  }
  else
  { /* nombre positif */
    WriteBit(bitSigne);
    WriteBits(absVal, nbBits-1);
  }
}


// lecture d'un bit
uchar TBitStream::ReadBit()
{
  uchar result;

  taille ++;

  if ((posInOctet==0)||(posInOctet==8))
  {
    //(*IStream) >> octetWaiting;
    IStream->get(octetWaiting);
    //cout << "octet lu: " << (int)octetWaiting << endl;
    if (IStream->eof()) {
    //cout<<EOF<<endl;
      cout << "TBitStream::ReadBit: EOF rencontre!" << endl;
      /*Modif Marc Chaumont*/exit(-1);
	//Modif Marc Chaumont : return EOF_BIT;
    }
    posInOctet = 0;
  }


  //if (octetWaiting>127)
if(octetWaiting < 0)
  result = 1;
  else
    result = 0;

  octetWaiting <<= 1;
  posInOctet ++;


  /*  static int num=0;

  cout << num << " lit bit: " ;
  num++;
  if (result)
    cout << "1" << endl;
  else
    cout << "0" << endl;
    */
  return result;
}

// lecture d'un bit
void TBitStream::UnReadBit(uchar bit)
{
  taille --;

  // mise a jour des bits lus au dela de la fin du fichier
  if (eofRead)
  {
    eofRead--;
    return ;
  }

  //-- mise à jour de l'octet
  octetWaiting /= 2;
  if (bit) octetWaiting += 128;
  posInOctet--;


  if (posInOctet<=0) 
  {
    IStream->putback(octetWaiting);
    posInOctet=8;
  }

}


// lecture de n bits
unsigned long  TBitStream::ReadBits(int nbBits)
{
  unsigned long masque;
  unsigned long result;
  int i;
  uchar bitLu;

  masque = 1;
  result = 0;
  // decalage du masque
  for(i=1; i<nbBits; i++)
    masque <<= 1;

  for(i=0; i<nbBits; i++)
  {
    bitLu = ReadBit();

    /*if (bitLu==EOF_BIT)
    {
      cerr << "tentative de lecture de " << nbBits << " en fin de fichier!" << endl;
      exit(-1);
    }*/

    if (bitLu)
      result += masque;
    // decalage du masque
    masque >>= 1;
  }

  return result;
}

// lecture de n bits signe's
long TBitStream::ReadSignedBits(int nbBits)
{
  unsigned char bitSigne;
  unsigned long absVal;
  unsigned long maxVal;
  long val;
  int i;

  // lecture du bit de signe
  bitSigne = ReadBit();
  /*if (bitSigne == EOF_BIT)
    {
      cerr << "tentative de lecture de " << nbBits << " en fin de fichier!" << endl;
      exit(-1);
  }*/

  absVal = ReadBits(nbBits-1);

  if (bitSigne==1)
  { /* nombre negatif */
    // "negation" des bits (-> compatibilite norme ISO)
    maxVal = 1;
    for(i=1; i<nbBits-1; i++)
      maxVal <<= 1;
    val = absVal-maxVal;
  }
  else
  { /* nombre positif */
    val = absVal;
  }

  return val;
}

////////////////////////////////////////////////////////////////////////////////



BIT_FILE *BIT_FOPEN_READ(char *filename)
{
  BIT_FILE *bitFile;

  if ((bitFile = (BIT_FILE *)malloc(sizeof(BIT_FILE)))==NULL)
    return NULL;

  bitFile->file = fopen(filename, "rb");
  if (bitFile->file==NULL)
  {
    free(bitFile);
    return NULL;
  }

  bitFile->taille = 0;
  bitFile->status = READING;
  bitFile->posInOctet = 0;

  return bitFile;
}



BIT_FILE *BIT_FOPEN_WRITE(char *filename)
{
  BIT_FILE *bitFile;

  if ((bitFile = (BIT_FILE *)malloc(sizeof(BIT_FILE)))==NULL)
    return NULL;

  bitFile->file = fopen(filename, "wb");
  if (bitFile->file==NULL)
  {
    free(bitFile);
    return NULL;
  }

  bitFile->taille = 0;
  bitFile->status = WRITING;
  bitFile->octetWaiting = 0;
  bitFile->posInOctet = 0;

  return bitFile;
}


void BIT_FWRITE(BIT_FILE *bitFile, uchar bit)
{
  bitFile->octetWaiting <<=  1;
  if (bit) bitFile->octetWaiting += 1;
  bitFile->posInOctet +=1;

  if (bitFile->posInOctet==8)
  {
    fwrite(&(bitFile->octetWaiting), 1, 1, bitFile->file);
    bitFile->posInOctet = 0;
    bitFile->octetWaiting = 0;
  }

  bitFile->taille += 1;
}

/*uchar BIT_FREAD(BIT_FILE *bitFile)
{
  uchar result;
  if ((bitFile->posInOctet==0)||(bitFile->posInOctet==8))
  {
    if (fread(&(bitFile->octetWaiting), 1, 1, bitFile->file) == 0)
      exit(1); //Modif Marc Chaumont return EOF_BIT;
    bitFile->posInOctet = 0;
  }


  if ((bitFile->octetWaiting)>127)
    result = 1;
  else
    result = 0;

  bitFile->octetWaiting <<= 1;
  bitFile->posInOctet += 1;

  bitFile->taille += 1;

  return result;
}
*/

void BIT_FUNREAD(BIT_FILE *bitFile, uchar bit)
{
  cout << "BIT_FUNREAD: fonction non implémentée!!" << endl;
  abort();
}


void BIT_FCLOSE(BIT_FILE *bitFile)
{
  if (bitFile->status!=READING)
  {/* controle qu'il ne reste pas des bits a flusher */
    if (bitFile->posInOctet!=0)
    {
      for(; bitFile->posInOctet!=8; bitFile->posInOctet++)
	bitFile->octetWaiting <<=1;
      fwrite(&(bitFile->octetWaiting), 1, 1, bitFile->file);
    }
  }

  fclose(bitFile->file);

  free(bitFile);
}



/* programme de test */
/*
void main()
{
  uchar bit;
  ifstream *ifs;
  ofstream *ofs;
  TBitStream *bitStream;
  unsigned long uval;
  long val;
  int nbBits;
  int key;

  printf("programme de test des IO niveau bits.\n");

  printf("ouverture en ecriture du fichier:  test.bit\n");

  ofs = new ofstream("test.bit");
  bitStream = new TBitStream(ofs);

  printf("entrer une suite de nombre (nbBits=0 pour sortir)\n");
  do
  {
    cout << "nombre de bits: ";
    cin >> nbBits;
    cout << "valeur: ";
    cin >> val;
    if (nbBits)
      bitStream->WriteSignedBits(val, nbBits);
  } while (nbBits);

  delete bitStream;
  delete ofs;

  printf("relecture du fichier!\n");

  ifs = new ifstream("test.bit");
  bitStream = new TBitStream(ifs);

  while (1)
  {
    cout << "nbBits: ";
    cin >> nbBits;
    val = bitStream->ReadSignedBits(nbBits);
    cout << "lecture de: " << val << endl;
  }

  delete bitStream;
  delete ifs;


  printf("relecture finie.\n");

}


*/
