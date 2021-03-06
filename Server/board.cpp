/*
 *  board.cpp
 *  ChessProject
 */
#include "stdafx.h"
#include "board.h"
#include "piece.h"

Board::Board()
{
    // Set up squares (_DIMENSION x _DIMENSION)
    for (int i = 0; i < _DIMENSION; i++)
    {
        for (int j = 0; j < _DIMENSION; j++)
        {
            _squares[i][j] = new Square(i, j);
        }
    }
}

Board::~Board()
{
    // Delete squares
    for (int i = 0; i < _DIMENSION; i++)
    {
        for (int j = 0; j < _DIMENSION; j++)
        {
            delete[] _squares[i][j];
        }
        delete[] _squares[i];
    }
    delete[] _squares;
}


Square* Board::squareAt(int x, int y) const
{
    return _squares[x][y];
}

bool Board::isClearVertical(Square& from, Square& to) const
{
    Square* start = NULL;
    Square* end = NULL;
    bool valid = true;
    
    // determine which square has the smaller y-value, 
    // (this helps to determine which direction to travel in)
    if(from.getY() <= to.getY())
    {
        start = &from;
        end = &to;
    }
    else
    {
        start = &to;
        end = &from;
    }
    
    // check that there is no horizontal movement
    if(start->getX() != end->getX())
    {
        valid = false;
    }
    else 
    {
        // iterate over vertical interval between squares
        for(int i = start->getY() + 1; i < end->getY(); i++)
        {
            // if a square is occupied, the vertical is not clear
            if(squareAt(start->getX(), i)->occupied())
            {
                valid = false;
            }
        } 
    }
    
    return valid;
}

bool Board::isClearHorizontal(Square& from, Square& to) const
{
    Square* start = NULL;
    Square* end = NULL;
    bool valid = true;
    
    // determine which square has the smaller x-value, 
    // (this helps to determine which direction to travel in)
    if(from.getX() <= to.getX())
    {
        start = &from;
        end = &to;
    }
    else
    {
        start = &to;
        end = &from;
    }
    
    // check that there is no vertical movement
    if(start->getY() != end->getY())
    {
        valid = false;
    }
    else 
    {
        // iterate over horizontal interval between squares
        for(int i = start->getX() + 1; i < end->getX(); i++)
        {
            // if a square is occupied, the vertical is not clear
            if(squareAt(i, start->getY())->occupied())
            {
                valid = false;
            }
        } 
    }
    
    return valid;
}

bool Board::isClearDiagonal(Square& from, Square& to) const
{
    bool valid = true;
    int translationX = to.getX() - from.getX();
    int translationY = to.getY() - from.getY();
    int xDir = 1;
    int yDir = 1;
    
    if(translationX < 0)
    {
        xDir = -1;
    }
    
    if(translationY < 0)
    {
        yDir = -1;
    }
    
    // not a diagonal if absolute value of x and y translations don't match
    if(abs(translationX) != abs(translationY))
    {
        valid = false;
    }
    else 
    {
        // iterate over diagonal interval between squares
        for(int i = 1; i < abs(translationX); i++)
        {
            // if a square is occupied, the diagonal is not clear
            if(squareAt(from.getX() + (i * xDir), from.getY() + (i * yDir))->occupied())
            {
                valid = false;
            }
        }
    }
    
    return valid;
}

bool Board::isEndRow(Square& location) const
{
    return (location.getY() == 0 || location.getY() == (_DIMENSION - 1));
}

Board* Board::_theBoard = NULL;