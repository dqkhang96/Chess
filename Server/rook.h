/**
 * rook.h
 *
 * This class represents a chess rook piece.
 *
 */

#ifndef ROOK_H
#define ROOK_H

#include <iostream>
#include "restrictedPiece.h"
#include "square.h"

using namespace std;

class Rook : public RestrictedPiece
{
 public:
    
    /**
    * Creates a Rook
    * with color the color of the piece
    */
    Rook(bool isWhite,Board* board);
    
    /**
     * Default destructor
     */
    ~Rook();

    /**
    * check this rook can move to location
    */
    bool canMoveTo(Square& location) const;
    
}; // Rook

#endif
