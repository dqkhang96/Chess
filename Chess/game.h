/**
 * game.h
 *
 * This class represents a game of chess.
 *
 */

#ifndef GAME_H
#define GAME_H

#include <set>
#include <iostream>
#include "board.h"
#include "piece.h"

using namespace std;

class Game 
{
 public:
	 Game();
    /**
     * Destructor
     */
    ~Game();

    /**
     * Set up a new game of chess
     *
     */
    void initialize();

    /**
    * Get the player whose turn is next
    *
    * @return the player
    */
    Player* getNextPlayer();

    /**
    * Get the player who is the opponent of the given player
    *
    * @param player the player
    * @return the opponent
    */
    Player* opponentOf(Player& player);

 private:

    // Private attributes
    Player* player1;
    Player* player2;
    Player* nextPlayer;
    set<Piece*> whitePieces;
    set<Piece*> blackPieces;

}; // Game

#endif
