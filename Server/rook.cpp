/*
 *  rook.cpp
 *  ChessProject
 *
 */
#include "stdafx.h"
#include <sstream>
#include "rook.h"

Rook::Rook(bool isWhite,Board* board) : RestrictedPiece(isWhite,board)
{
}

Rook::~Rook()
{
}


bool Rook::canMoveTo(Square& location) const
{
    bool validMove = false;
    
    // valid move if moving on a clear vertical
    if(_board->isClearVertical(*(this->location()), location))
    {
        validMove = true;
    }
    
    // valid move if moving on a clear horizontal
    else if(_board->isClearHorizontal(*(this->location()), location))
    {
        validMove = true;
    }
    
    return validMove;
}
