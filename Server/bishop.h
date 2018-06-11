/**
 * bishop.h
 *
 * This class represents a chess bishop piece.
 *
 */

#ifndef BISHOP_H
#define BISHOP_H

#include <iostream>
#include "piece.h"
#include "square.h"

using namespace std;

class Bishop : public Piece
{
 public:
    
    //Constructor
    Bishop(bool isWhite,Board* board);
    
	//Desconstructor
    ~Bishop();
    //return true if can't move this bishop
    bool canMoveTo(Square& location) const;
    

}; // Bishop

#endif
