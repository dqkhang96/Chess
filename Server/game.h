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
    ~Game();
    void initialize();


    Player* getNextPlayer();
	Player* currentPlayer();

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
