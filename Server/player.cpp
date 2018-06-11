/*
 *  player.cpp
 *  ChessProject
 *
 */
#include "stdafx.h"
#include <sstream>
#include <iostream>
#include "player.h"
#include "board.h"

#define X_MIN 'a'
#define X_MAX 'h'
#define Y_MIN '1'
#define Y_MAX '8'

Player::Player(string name,bool isWhite, King& myKing, set<Piece*>* myPieces,bool isPlay,Board* board) :
_name(name), _isWhite(isWhite), _myPieces(myPieces), _myKing(myKing),_isPlay(isPlay),_board(board)
{
}

Player::~Player()
{
	if (_myPieces != NULL)
		delete _myPieces;
}

void Player::setOpponent(Player* opponent) {
	_opponent = opponent;
}

void Player::play() {
	_isPlay = true;
}

void Player::notPlay() {
	_isPlay = false;
}
int Player::makeMove(string fromSquare,string toSquare)
{
	if (!_isPlay)
		return 2;//Can't play
    int fromX;
    int fromY;
    int toX;
    int toY;
    
    // move must be in algebraic notation 
    // (start and end squares denoted by 2 characters, a1-h8,
    // and separated from each other by whitespace)
    // move cannot start from an empty square
   if(fromSquare.length() != 2 ||
          toSquare.length() != 2 ||
          tolower(fromSquare.at(0)) < X_MIN ||
          tolower(fromSquare.at(0)) > X_MAX ||
          tolower(toSquare.at(0)) < X_MIN ||
          tolower(toSquare.at(0)) > X_MAX ||
          tolower(fromSquare.at(1)) < Y_MIN ||
          tolower(fromSquare.at(1)) > Y_MAX ||
          tolower(toSquare.at(1)) < Y_MIN ||
          tolower(toSquare.at(1)) > Y_MAX ||
          !(_board->squareAt(tolower(fromSquare.at(0)) - X_MIN, 
                                      tolower(fromSquare.at(1)) - Y_MIN)->occupied())
          )
    {
	   return 2;
    }
    
    // translate input (algebraic notation) into board coordinates
    fromX = tolower(fromSquare.at(0)) - X_MIN;
    fromY = tolower(fromSquare.at(1)) - Y_MIN;
    toX = tolower(toSquare.at(0)) - X_MIN;
    toY = tolower(toSquare.at(1)) - Y_MIN;
    
    // move the piece on fromSquare to toSquare
    return _board->squareAt(fromX, fromY)->occupiedBy()->moveTo(*this, 
                                                                  *(_board->squareAt(toX, toY)));
}

bool Player::inCheck()
{
    bool check = false;
    
    // for each piece in the opponent's set of pieces
    for (set<Piece*>::iterator itr = opponent()->myPieces()->begin(); 
         itr != opponent()->myPieces()->end(); ++itr)
    {
        // if a piece that is still located on the board
        // can move to my king's square
        if (((Piece*)*itr)->location() && 
            ((Piece*)*itr)->canMoveTo(*(myKing()->location())))
        {
            // I am in check
            check = true;
        }
    }
    
    return check;
}

string Player::getName() const
{
    return _name;
}

bool Player::isWhite() const
{
    return _isWhite;
}

set<Piece*>* Player::myPieces() const
{
    return  _myPieces;
}

King* Player::myKing() const
{
    return &_myKing;
}
Player* Player::opponent() const {
	return _opponent;
}