#ifndef WM_TRELLIS_HPP
#define WM_TRELLIS_HPP

/**
 *  La classe est purement static.
 */
class WM_TRELLIS {        
     
private:     
  static int NUM_STATES;
  static int NUM_BITSBYARC;
  static int STATE_NOT_REACHED; //Utilisé pour le déocdeur de Viterbi
  static int nextState[ 8 ][ 2 ];
  /* trellisBits -- Labels on arcs in graph.  For each state, there are two
     4-bit codes: one for the arc traversed by a 0 in the message, and
     one for the arc traversed by a 1.  This table is global so that it can
     be used in TrellisDecode;
     8 states, 2 arcs by state, 4 bits on an arc*/
  static int trellisBits[ 8 ][ 2 ][ 4 ];
  static int HammingDist( int *b1, int *b2, int len );
  
public:
  static void TrellisEncode( int *m, int mLen, int *mc );
  static int TrellisDecode( int *mc, int mcLen, int *m );
};    
#endif //WM_TRELLIS_HPP
