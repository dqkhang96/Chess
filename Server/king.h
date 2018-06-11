/**
 * king.h
 *
 * This class represents a chess king piece.
 *
 */

#ifndef KING_H
#define KING_H

#include <iostream>
#include "restrictedPiece.h"

using namespace std;

class King : public RestrictedPiece
{
 public:

   /**
    Create King with color
    */
    King(bool isWhite,Board* board);
    
    /**
     * Default destructor
     */
    ~King();

 
    /**
    * Determines if the piece can legally move to a given square
    *
    * @param square the square to move to
    * @return if move is legal
    */
    bool canMoveTo(Square& location) const;

}; // King

#endif
