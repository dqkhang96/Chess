/**
 * knight.h
 *
 * This class represents a chess knight piece.
 *
 */

#ifndef KNIGHT_H
#define KNIGHT_H

#include <iostream>
#include "square.h"
#include "piece.h"

using namespace std;

class Knight : public Piece
{
 public:
    
    /**
    * Creates a Knight
	* with color the color of the piece
    */
    Knight(bool isWhite,Board* board);
    
    /**
     * Default destructor
     */
    ~Knight();

 
    /**
    * Determines if the piece can legally move to a given square
    *
    * @param square the square to move to
    * @return if move is legal
    */
    bool canMoveTo(Square& location) const;
   
}; // Knight

#endif
