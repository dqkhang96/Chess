/*
 *  bishop.cpp
 *  ChessProject
 *
 *
 */
#include "stdafx.h"
#include <sstream>
#include "bishop.h"


Bishop::Bishop(bool isWhite,Board* board) : Piece(isWhite, board)
{
}

Bishop::~Bishop()
{
}

bool Bishop::canMoveTo(Square& location) const
{
    bool validMove = false;
    
    // valid move if moving on a clear diagonal
    if(_board->isClearDiagonal(*(this->location()), location))
    {
        validMove = true;
    }
    
    return validMove;
}
