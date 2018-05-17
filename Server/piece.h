/**
 * piece.h
 *
 * This class represents a game piece.
 *
 */

#ifndef PIECE_H
#define PIECE_H

#include <ostream>
#include "square.h"
#include "board.h"
class Player;

using namespace std;

class Piece 
{
 public:
   

    Piece(bool isWhite);
  
    virtual ~Piece();

   
    virtual int moveTo(Player& byPlayer, Square& toSquare);

  
    virtual void setLocation(Square* location);

    virtual int value() const = 0;

    bool isWhite() const;

    string color() const;    
    
    virtual void display() const = 0;   

    virtual bool canMoveTo(Square& location) const = 0;

    bool isOnSquare() const;

    Square* location() const;

 protected:
    bool _isWhite;
    string _color;
    
 private:

    // Private attributes
    Square* _square;

}; // Piece

#endif
