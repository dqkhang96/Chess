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
   

    Piece(bool isWhite,Board* board);
  
    virtual ~Piece();

   
    virtual int moveTo(Player& byPlayer, Square& toSquare);

  
    virtual void setLocation(Square* location);


    bool isWhite() const;
    
    
    virtual bool canMoveTo(Square& location) const = 0;

    bool isOnSquare() const;

    Square* location() const;

 protected:
    bool _isWhite;
	Board* _board;
    
 private:

    // Private attributes
    Square* _square;


}; // Piece

#endif
