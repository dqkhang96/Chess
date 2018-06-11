/**
 * queen.h
 *
 * This class represents a chess queen piece.
 *
 */

#ifndef QUEEN_H
#define QUEEN_H

#include <iostream>
#include "square.h"
#include "piece.h"

using namespace std;

class Queen : public Piece
{
 public:
    
    /**
    * Creates a Queen
    * with color the color of the piece
    */
    Queen(bool isWhite,Board* board);
    
    /**
     * Default destructor
     */
    ~Queen();

    
    /**
    check this queen can move to location
    */
    bool canMoveTo(Square& location) const;
    
  
}; // Queen

#endif