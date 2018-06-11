/**
 * board.h
 *
 * This class represents a game board.
 *
 */

#ifndef BOARD_H
#define BOARD_H

#include <ostream>
#include "square.h"

using namespace std;

class Board 
{
 public:

	 /**
	 * Creates an 8x8 Board
	 *
	 */
	 Board();
    /**
     * Default destructor
     */
    ~Board();

    /**
	get square at (x,y) in board
    */
    Square* squareAt(int x, int y) const;

    /**
    * Determine if the path between two squares is a clear vertical
    *
    * @param from the starting square
    * @param to the ending square
    * @return if the path is a clear vertical
    */
    bool isClearVertical(Square& from, Square& to) const;

    /**
		return true if board's horizontal from 'from' to 'to' is clear 
	*/
    bool isClearHorizontal(Square& from, Square& to) const;

	/**
	return true if board's diagonal from 'from' to 'to' is clear
	*/
    bool isClearDiagonal(Square& from, Square& to) const;
    
    /**
     if square is end of row
     */
    bool isEndRow(Square& location) const;

 private:

    // Private attributes
    static Board* _theBoard;
    static const int _DIMENSION = 8;
    Square* _squares[_DIMENSION][_DIMENSION];

}; // Board

#endif
