/*
 *  restrictedPiece.cpp
 *  ChessProject
 */
#include "stdafx.h"
#include <sstream>
#include "player.h"
#include "restrictedPiece.h"

RestrictedPiece::RestrictedPiece(bool isWhite,Board *board) : Piece(isWhite,board)
{
    _moved = false;
}
    
RestrictedPiece::~RestrictedPiece()
{
}

int RestrictedPiece::moveTo(Player& byPlayer, Square& to)
{
    int validMove = Piece::moveTo(byPlayer, to);
    
    // only change _moved if a valid move is made 
    // and this piece hasn't already been moved
    if(validMove==0 && !_moved)
    {
        _moved = true;
    }
    
    return validMove;
}

bool RestrictedPiece::hasMoved() const
{
    return _moved;
}

