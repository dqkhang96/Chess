/*
 *  queen.cpp
 *  ChessProject
 *
 *
 */
#include "stdafx.h"
#include <sstream>
#include "queen.h"

Queen::Queen(bool isWhite,Board* board) : Piece(isWhite,board)
{
}

Queen::~Queen()
{
}

bool Queen::canMoveTo(Square& location) const
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
    
    // valid move if moving on a clear diagonal
    else if(_board->isClearDiagonal(*(this->location()), location))
    {
        validMove = true;
    }
    
    
    return validMove;
}
