/**
 * player.h
 *
 * This class represents a chess player.
 *
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <set>
#include "piece.h"
#include "king.h"

using namespace std;

class Player 
{
public:
    
    Player(string name, bool isWhite, King& myKing, set<Piece*>* myPieces,bool isPlay,Board* _board);
	void setOpponent(Player* opponent);
    ~Player();
    
    int makeMove(string fromSquare, string toSquare);
    
    
    bool inCheck();
    
   
    string getName() const;
    
  
    bool isWhite() const;
    

    set<Piece*>* myPieces() const;
    
    King* myKing() const;
	Player* opponent()const;

	void play();
	void notPlay();
    
    
private:
    
    // Private attributes
    string _name;
    bool _isWhite;
    set<Piece*>* _myPieces=NULL;
    //set<Piece*> _capturedPieces;
    King& _myKing;
	Player* _opponent;
	bool _isPlay;
	Board* _board;
    
}; // Player

#endif
