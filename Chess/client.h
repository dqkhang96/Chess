#pragma once
#include "stdafx.h"
#include<string>
#include<winsock.h>
#include<vector>
#include <SFML/Graphics.hpp>
#include <time.h> 

using namespace sf;

class Client {
private:
	int size = 56;
	Vector2f offset=Vector2f(28, 28);

	Sprite f[32]; //figures
	Sprite aBoard;
	Texture figures, tBoard;

	int board[8][8] =
	{ -1,-2,-3,-4,-5,-3,-2,-1,
		-6,-6,-6,-6,-6,-6,-6,-6,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		6, 6, 6, 6, 6, 6, 6, 6,
		1, 2, 3, 4, 5, 3, 2, 1 };

	SOCKET connSocket;
public:
	Client();
	Client(const char * address, const int port);
	void closeSocket();
	int sendCode(int code);
	int sendAllData(std::string data);
	int recvCode(int *code);
	int recvAllData(std::string &data);
	void playGame(bool first);
	int login(std::string userId, std::string pass);
	void createRoom();
	bool join(std::string userIdConn);
	std::vector<std::string> getListRoom();
	std::string getProfile();

private:
	void move(std::string str);
	void draw(RenderWindow& window, Sprite* top);
	std::string toChessNote(Vector2f p);
	Vector2f toCoord(char a, char b);
	void loadPosition();
	int sendData(const void * data, int data_size);
	int recvData(const void *data, int data_size);
};