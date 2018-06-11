/**
 * pawn.h
 *
 * This class represents a chess pawn piece.
 *
 */

#ifndef PAWN_H
#define PAWN_H

#include <iostream>
#include "square.h"
#include "restrictedPiece.h"

using namespace std;

class Pawn : public RestrictedPiece
{
 public:
    
    /**
    * Creates a Pawn
    * with color the color of the piece
    */
    Pawn(bool isWhite,Board* board);
    
    /**
     * Default destructor
     */
    ~Pawn();

    /**
    * Set which square the piece is located on
    */
    void setLocation(Square* location);


    /**
     * Move the piece to another square.
     */
    int moveTo(Player& byPlayer, Square& to);

    /**
    this pawn can move to location
    */
    bool canMoveTo(Square& location) const;

  
   
 private:

    // Private attributes
    Piece* _delegate;

}; // Pawn

#endif
