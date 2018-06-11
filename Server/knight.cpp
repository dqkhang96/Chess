/*
 *  knight.cpp
 *  ChessProject
 */
#include "stdafx.h"
#include <sstream>
#include "knight.h"

Knight::Knight(bool isWhite,Board* board) : Piece(isWhite,board)
{
}

Knight::~Knight()
{
}


bool Knight::canMoveTo(Square& location) const
{
    bool validMove = false;
    int translationX = location.getX() - this->location()->getX();
    int translationY = location.getY() - this->location()->getY();
    
    // valid move if moving 1 square forward/backwards
    // and 2 squares sideways
    if(abs(translationY) == 1 && abs(translationX) == 2)
    {
        validMove = true;
    }
    
    // valid move if moving 1 square sideways and 2 squares
    // forward/backwards
    else if(abs(translationX) == 1 && abs(translationY) == 2)
    {
        validMove = true;
    }
    
    return validMove;
}




