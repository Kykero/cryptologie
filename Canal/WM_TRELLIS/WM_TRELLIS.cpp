#include <iostream>
#include <stdlib.h>
using namespace std;

#include "WM_TRELLIS.hpp"

int WM_TRELLIS::NUM_STATES        = 8;
int WM_TRELLIS::NUM_BITSBYARC     = 4;
int WM_TRELLIS::STATE_NOT_REACHED = -1; //Utilis� pour le d�ocdeur de Viterbi

/* nextState -- description of the graph that defines the code.
   For each state, nextState contains two entries.
   nextState[ s ][ 0 ] gives the state reached by crossing the 0 arc from
   state s.  nextState[ s ][ 1 ] gives the state reached by crossing
   the 1 arc.
   nextState is global so that it can be used in other routines based on
   this trellis (TrellisDemodulate, TrellisEncode, and TrellisDecode) */
int WM_TRELLIS::nextState[ 8 ][ 2 ] =
{
  0, 1,
  2, 3,
  4, 5,
  6, 7,
  0, 1,
  2, 3,
  4, 5,
  6, 7,
};

/* trellisBits -- Labels on arcs in graph.  For each state, there are two
   4-bit codes: one for the arc traversed by a 0 in the message, and
   one for the arc traversed by a 1.  This table is global so that it can
   be used in TrellisDecode. */
//int WM_TRELLIS::trellisBits[ 8 ][ 2 ][ 4 ] = //Version qui semble erron�e
//{
//  0, 0, 0, 0,
//  0, 0, 0, 1,
//  0, 0, 1, 0,
//  0, 0, 1, 1,
//  0, 1, 0, 0,
//  0, 1, 0, 1,
//  0, 1, 1, 0,
//  0, 1, 1, 1,
//  1, 0, 0, 0,
//  1, 0, 0, 1,
//  1, 0, 1, 0,
//  1, 0, 1, 1,
//  1, 1, 0, 0,
//  1, 1, 0, 1,
//  1, 1, 1, 0,
//  1, 1, 1, 1,
//};

int WM_TRELLIS::trellisBits[ 8 ][ 2 ][ 4 ] = //Version livre p.150 ...
{
  0, 0, 0, 0, //A
  0, 0, 0, 1,
  0, 0, 1, 1, //B
  1, 1, 0, 0,
  1, 0, 1, 0, //C
  1, 1, 1, 1,
  1, 0, 0, 0, //D
  0, 1, 0, 1,
  0, 0, 1, 0, //E
  1, 0, 0, 1,
  1, 1, 1, 0, //F
  1, 1, 0, 1,
  0, 1, 0, 0, //G
  1, 0, 1, 1,
  0, 1, 1, 0, //H
  0, 1, 1, 1,
};


/*--------------------------------------------------------------------------*
 | TrellisEncode -- encode a message with a sequence of bits, using a       |
 |                  trellis code                                            |
 |                                                                          |
 | Arguments:                                                               |
 |   m -- message to be encoded (expressed as an array of bits)             |
 |   mLen -- length of message                                              |
 |   mc -- where to store resulting coded bit sequence                      |
 |                                                                          |
 | Return value:                                                            |
 |   none                                                                   |
 *--------------------------------------------------------------------------*/

void WM_TRELLIS::TrellisEncode( int *m, int mLen, int *mc ){
    int current_state = 0;  // On d�marre � l'�tat 0
    int out_index = 0;      // Position courante dans le tableau de sortie mc

    // Pour chaque bit du message d'entr�e
    for (int i = 0; i < mLen; i++)
    {
        int input_bit = m[i];
        
        // Copie des 4 bits de sortie correspondant � la transition
        // Pour l'�tat actuel et selon le bit d'entr�e, on r�cup�re la s�quence de 4 bits
        for (int j = 0; j < NUM_BITSBYARC; j++)
        {
            mc[out_index++] = trellisBits[current_state][input_bit][j];
        }
        
        // Mise � jour de l'�tat en fonction du bit d'entr�e et de la table nextState
        current_state = nextState[current_state][input_bit];
    }
}

/*--------------------------------------------------------------------------*
 | TrellisDecode -- use Viterbi's algorithm to decode a trellis-coded       |
 |                  bit sequence                                            |
 |                                                                          |
 | Arguments:                                                               |
 |   mc -- coded bits                                                       |
 |   mcLen -- length of mc                                                  |
 |   m -- where to store decoded bits                                       |
 |                                                                          |
 | Return value:                                                            |
 |   length of decoded message                                              |
 *--------------------------------------------------------------------------*/

int WM_TRELLIS::TrellisDecode( int *mc, int mcLen, int *m )
{

  int j;
  /* Find length of message */
  int mLen = mcLen / NUM_BITSBYARC;    /* length of message */
  int h0[ NUM_STATES ];                /* hamming distance obtained from best
                                          path (so far) to each state */
  int** m0 = new int*[ NUM_STATES ]; /* message obtained from best path (so far) to each state */
  int** m1 = new int*[ NUM_STATES ]; /* values of m0 in next iteration */
  for (j=0; j<NUM_STATES; j++) {
    m0[j] = new int [mLen];
    m1[j] = new int [mLen];
  }
  int h1[ NUM_STATES ];                /* values of h0 in next iteration */
  int bitNum;                          /* bit number */
  int state;                           /* state index */
  int next;                            /* index of state at the other end
                                          of an arc */
  int h;                               /* hamming distance between portion
                                          of mc and label for an arc */
  int bestState;                       /* state with the lowest hamming
                                          distance at the end of the
                                          algorithm */
  int i;                               /* index into messages */


  /* All paths must start from state 0, so we initialize that state to
     0 hamming distance, and label the remaining states as unreached. */
  h0[ 0 ] = 0;
  for( state = 1; state < 8; state = state + 1 )
    h0[ state ] = STATE_NOT_REACHED;

  /* Apply the Viterbi algorithm to decode. */
  for( bitNum = 0; bitNum < mLen; bitNum = bitNum + 1 ) //mLen bits � r�cup�rer...
  {
    /* Indicate that the states in the next iteration are not yet reached. */
    for( state = 0; state < 8; state = state + 1 )
      h1[ state ] = STATE_NOT_REACHED;

    /* Loop through all the states in the current iteration, updating
       the values for states in the next iteration. */
    for( state = 0; state < 8; state = state + 1 )
      if( h0[ state ] != STATE_NOT_REACHED )
      {
        /* Update values for the state connected to this state by a 0 arc.
           (Note: mc + 4 * bitNum is pointer arithmetic.  The four bits
           used to encode bit bitNum of m are (mc + 4 * bitNum)[ 0...3 ]. */
        next = nextState[ state ][ 0 ];
        //Calcul de la distance entre la valeur de l'arc et le code (sur 4 bits)
        h = HammingDist( mc + 4 * bitNum, trellisBits[ state ][ 0 ], 4 );
        if( h1[ next ] == STATE_NOT_REACHED ||
            h1[ next ] > h0[ state ] + h )
        {
          h1[ next ] = h0[ state ] + h;
          for( i = 0; i < bitNum; i = i + 1 )
            m1[ next ][ i ] = m0[ state ][ i ];
          m1[ next ][ bitNum ] = 0;
        }

        /* Update values for the state connected to this state by a 1 arc. */
        next = nextState[ state ][ 1 ];
        h = HammingDist( mc + 4 * bitNum, trellisBits[ state ][ 1 ], 4 );
        if( h1[ next ] == STATE_NOT_REACHED ||
            h1[ next ] > h0[ state ] + h )
        {
          h1[ next ] = h0[ state ] + h;
          for( i = 0; i < bitNum; i = i + 1 )
            m1[ next ][ i ] = m0[ state ][ i ];
          m1[ next ][ bitNum ] = 1;
        }
      }

    /* Go on to the next iteration. */
    for( state = 0; state < 8; state = state + 1 )
    {
      h0[ state ] = h1[ state ];
      for( i = 0; i < mLen; i = i + 1 )
        m0[ state ][ i ] = m1[ state ][ i ];
    }
  }

  /* Find the state with the minimum hamming distance. */
  bestState = 0;
  for( state = 1; state < 8; state = state + 1 )
    if( h0[ state ] < h0[ bestState ] )
      bestState = state;

  /* Return the message for the best state. */
  for( i = 0; i < mLen; i = i + 1 )
    m[ i ] = m0[ bestState ][ i ];

  /*DELETE ALLOCATION*/
  for (j=0; j<NUM_STATES; j++) {
    delete[] m0[j];
    delete[] m1[j];
  }
  delete[] m0;
  delete[] m1;

  return mLen;
}

/*--------------------------------------------------------------------------*
 | HammingDist -- compute the hamming distance between two bit sequences    |
 |                                                                          |
 | Arguments:                                                               |
 |   b1 -- one bit sequence                                                 |
 |   b2 -- other bit sequence                                               |
 |   len -- length of sequences                                             |
 |                                                                          |
 | Return value:                                                            |
 |   hamming distance                                                       |
 *--------------------------------------------------------------------------*/

int WM_TRELLIS::HammingDist( int *b1, int *b2, int len ) {

  int h;
  int i;

  h = 0;

  for( i = 0; i < len; i = i + 1 )
    if( b1[ i ] != b2[ i ] )
      h = h + 1;

  return h;
}


