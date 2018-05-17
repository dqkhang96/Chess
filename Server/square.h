/**
 * square.h
 *
 * This class represents a square on a game board.
 *
 */

#ifndef SQUARE_H
#define SQUARE_H

class Piece;

using namespace std;

class Square 
{
 public:
   
 
    Square(int x, int y);
    Square();

    ~Square();
    
   
    void setOccupier(Piece* piece);

    int getY() const;
	int getX()const;

    
    bool occupied() const;   

   
    Piece* occupiedBy() const;


 private:

    // Private attributes
    int _x;
    int _y;
    Piece* _piece;

}; // Square

#endif
