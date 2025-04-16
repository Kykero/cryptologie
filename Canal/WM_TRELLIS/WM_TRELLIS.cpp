/*
 * Debusschere Mathieu
 * Fernandez Nolahn
 *
 */

#include <iostream>
#include <stdlib.h>
using namespace std;

#include "WM_TRELLIS.hpp"

int WM_TRELLIS::NUM_STATES        = 4;
int WM_TRELLIS::NUM_BITSBYARC     = 2;
int WM_TRELLIS::STATE_NOT_REACHED = -1; //Utilisé pour le déocdeur de Viterbi

/* nextState -- description of the graph that defines the code.
   For each state, nextState contains two entries.
   nextState[ s ][ 0 ] gives the state reached by crossing the 0 arc from
   state s.  nextState[ s ][ 1 ] gives the state reached by crossing
   the 1 arc.
   nextState is global so that it can be used in other routines based on
   this trellis (TrellisDemodulate, TrellisEncode, and TrellisDecode) */
int WM_TRELLIS::nextState[ 4 ][ 2 ] =
{
  0, 2,
  0, 2,
  1, 3,
  1, 3,
};

/* trellisBits -- Labels on arcs in graph.  For each state, there are two
   4-bit codes: one for the arc traversed by a 0 in the message, and
   one for the arc traversed by a 1.  This table is global so that it can
   be used in TrellisDecode. */
//int WM_TRELLIS::trellisBits[ 8 ][ 2 ][ 4 ] = //Version qui semble erronée
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

int WM_TRELLIS::trellisBits[ 4 ][ 2 ][ 2 ] = //Version livre p.150 ...
{
  0, 0,
  1, 1,
  1, 1,
  0, 0,
  1, 0,
  0, 1,
  0, 1,
  1, 0,
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
    int current_state = 0;  // On démarre à l'état 0
    int out_index = 0;      // Position courante dans le tableau de sortie mc

    // Pour chaque bit du message d'entrée
    for (int i = 0; i < mLen; i++)
    {
        int input_bit = m[i];
        
        // Copie des 4 bits de sortie correspondant à la transition
        // Pour l'état actuel et selon le bit d'entrée, on récupère la séquence de 4 bits
        for (int j = 0; j < NUM_BITSBYARC; j++)
        {
            mc[out_index++] = trellisBits[current_state][input_bit][j];
        }
        
        // Mise à jour de l'état en fonction du bit d'entrée et de la table nextState
        current_state = nextState[current_state][input_bit];
    }

    // Ajout de la queue
    int tail = 2;
    for(int i = 0; i < tail; i++){
      int input_bit = 0;
      for(int j = 0; j < NUM_BITSBYARC; j++){
        mc[out_index++] = trellisBits[current_state][input_bit][j];
      }
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
  for( state = 1; state < NUM_STATES; state = state + 1 )  // Modified: used NUM_STATES instead of 8
    h0[ state ] = STATE_NOT_REACHED;

  /* Apply the Viterbi algorithm to decode. */
  for( bitNum = 0; bitNum < mLen; bitNum = bitNum + 1 ) //mLen bits à récupérer...
  {
    /* Indicate that the states in the next iteration are not yet reached. */
    for( state = 0; state < NUM_STATES; state = state + 1 )  // Modified: used NUM_STATES instead of 8
      h1[ state ] = STATE_NOT_REACHED;

    /* Loop through all the states in the current iteration, updating
       the values for states in the next iteration. */
    for( state = 0; state < NUM_STATES; state = state + 1 )  // Modified: used NUM_STATES instead of 8
      if( h0[ state ] != STATE_NOT_REACHED )
      {
        /* Update values for the state connected to this state by a 0 arc.
           (Note: mc + NUM_BITSBYARC * bitNum is pointer arithmetic.  The NUM_BITSBYARC bits
           used to encode bit bitNum of m are (mc + NUM_BITSBYARC * bitNum)[ 0...NUM_BITSBYARC-1 ].) */  // Modified: replaced 4 by NUM_BITSBYARC
        next = nextState[ state ][ 0 ];
        //Calcul de la distance entre la valeur de l'arc et le code (on compare NUM_BITSBYARC bits)
        h = HammingDist( mc + NUM_BITSBYARC * bitNum, trellisBits[ state ][ 0 ], NUM_BITSBYARC );  // Modified: replaced 4 by NUM_BITSBYARC
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
        h = HammingDist( mc + NUM_BITSBYARC * bitNum, trellisBits[ state ][ 1 ], NUM_BITSBYARC );  // Modified: replaced 4 by NUM_BITSBYARC
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
    for( state = 0; state < NUM_STATES; state = state + 1 )  // Modified: used NUM_STATES instead of 8
    {
      h0[ state ] = h1[ state ];
      for( i = 0; i < mLen; i = i + 1 )
        m0[ state ][ i ] = m1[ state ][ i ];
    }
  }

  /* Find the state with the minimum hamming distance. */
  bestState = 0;
  for( state = 1; state < NUM_STATES; state = state + 1 )  // Modified: used NUM_STATES instead of 8
    if( h0[ state ] < h0[ bestState ] )
      bestState = state;

  /* Return the message for the best state. */
  for( i = 0; i < mLen; i = i + 1 )
    m[ i ] = m0[ bestState ][ i ];

  /*DELETE ALLOCATION*/
  for (j=0; j<NUM_STATES; j++) {  // Modified: used NUM_STATES instead of 8
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


