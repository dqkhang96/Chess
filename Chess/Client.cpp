#include "stdafx.h"
#include "stdio.h"
#include <SFML/Graphics.hpp>
#include <time.h>
#include "winsock2.h"
#include<iostream>
#include<string>
#include<conio.h>
#include <vector>
#include "client.h"
#include "afxdialogex.h"

#pragma comment(lib,"Ws2_32.lib")
#pragma warning(disable:4996)
#define BUFF_SIZE 2048
using namespace sf;

bool logined = false;

struct pakage {
	std::string type;
	std::string content;
};

struct moving {
	std::string from;
	std::string to;
};
Client::Client() {

};
Client::Client(const char* address, const int port) {
	WSADATA	wsaData;
	WORD wVestion = MAKEWORD(2, 2);
	if (WSAStartup(wVestion, &wsaData))
		printf("Version is not supported.\n");
	printf("Client started!\n");

	//Construct socke
	connSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int tv = 1000000;
	setsockopt(connSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(int));

	//Spectify server address
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(address);

	//Request to connect server
	if (connect(connSocket, (sockaddr *)&serverAddr, sizeof(serverAddr))) {
		printf("Error! Cannot connect server. %d", WSAGetLastError());
		_getch();
	}
	printf("Connected server!\n");
	///Board
	tBoard;
	tBoard.loadFromFile("images/board.png");
	aBoard=Sprite(tBoard);
	figures.loadFromFile("images/figures.png");
	for (int i = 0; i < 32; i++) f[i].setTexture(figures);
}
void Client::closeSocket() {
	closesocket(connSocket);
}

//Send data_size byte
int Client::sendData(const void * data, int data_size) {
	char *sendbuff = (char*)data;
	int ret, nLeft = data_size;
	int idx = 0;

	while (nLeft > 0) {
		// Send data from nLeft position
		ret = send(connSocket, &sendbuff[idx], nLeft, 0);
		if (ret == SOCKET_ERROR) {
			return -1;
		}
		nLeft -= ret;
		idx += ret;
	}
	return 1;
}

//Send a std::string
int Client::sendAllData(const std::string data) {
	//Covert host order to big-endian
	ULONG data_size = htonl(data.size());

	//Send data size
	int result = sendData(&data_size, sizeof(data_size));
	if (result == 1) {
		//Send data
		result = sendData(data.c_str(), data.size());
	}
	return result;
}

//Send code
int Client::sendCode(int code) {
	int ret = sendData(&code, sizeof(int));
	return ret;
}

//Receive data has data_size byte
int Client::recvData(const void *data, int data_size) {
	char *recvbuff = (char*)data;
	int ret, nLeft, idx;
	nLeft = data_size; // length of the data needs to be
					   // received
	idx = 0;
	while (nLeft > 0) {
		//Receive data from nLeft position
		ret = recv(connSocket, &recvbuff[idx], nLeft, 0);
		if (ret == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		nLeft -= ret;
		idx += ret;
	}
	return 1;
}

int Client::recvCode(int *code) {
	return recvData(code, sizeof(int));
}

//Receive a std::string
int Client::recvAllData(std::string &data) {
	ULONG data_size;
	data = "";
	int result = recvData(&data_size, sizeof(data_size));
	if (result == 1) {
		//Covert big-endian to host order 
		data_size = ntohl(data_size);
		if (data_size > 0) {
			//Resize data ouput
			data.resize(data_size);
			//Receive data 
			result = recvData(&data[0], data_size);
			if (result != 1) {
				data.clear();
			}
		}
	}
	return result;
}

//Connect std::string to package
std::string getPackage_s(std::string type, std::string content) {
	return type.append("|").append(content);
}

//Get pakage from std::string pakage
pakage getPakage(std::string s) {
	int i = 0;
	for (; i < s.size(); i++)
		if (s[i] == '|')
			break;
	pakage pak;
	if ((i == s.size()) && (s[i] != '|')) {
		pak.type = s;
		pak.content = "";
		return pak;
	}
	//Get type message
	pak.type = s.substr(0, i);
	//Get content message
	pak.content = s.substr(i + 1, s.size() - i - 1);
	return pak;
}

moving getMove(std::string s) {
	int i = 0;
	while (s[i] != '|')
		i++;
	moving result;
	result.from = s.substr(0, i);
	result.to = s.substr(i + 1);
	return result;
}
void Client::createRoom() {
	sendAllData("CREATE_ROM");
	printf("Created room. Let's wait to connect!\n");
	std::string acceptReq;
	recvAllData(acceptReq);
	pakage pak = getPakage(acceptReq);
	CString str;
	str.Format(L"Do you want connect to %s?", CString(pak.content.c_str()));
	int choose=MessageBox(NULL, str, L"Connect",MB_OKCANCEL);
	if (choose == IDOK) {
		sendCode(31);
		playGame(false);
	}
	else {
		sendCode(-31);
	}
}
bool Client::join(std::string userIdConn) {
	int code;
	sendAllData("JOIN_TO|" + userIdConn);
	recvCode(&code);
	if (code == 21) {
		printf("Connect success!\n");
		playGame(true);
		return true;
	}
	else if (code==-21) {
		printf("Player deny!\n");
		MessageBox(NULL, L"Player deny!", L"Accept", MB_OK);
		return false;
	}
	else if (code == -22) {
		MessageBox(NULL, L"Player busy!", L"Accept", MB_OK);
		return false;
	}
	else if (code == -23) {
		MessageBox(NULL, L"Can't find user!", L"Accept", MB_OK);
		return false;
	}
	else if (code == -24) {
		MessageBox(NULL, L"You can't player with his!", L"Accept", MB_OK);
		return false;
	}
}

int Client::login(std::string userId, std::string pass) {
	std::string res;
	int code;
	sendAllData("LOGIN|" + userId + "|" + pass);
	recvCode(&code);
	if (code == 01) {
		printf("Login success.\n");
		return 0;
	}
	else if (code == -02) {
		printf("Password fail!\n");
		return 1;
	}
	else if (code == -01) {
		printf("User is not exist!\n");
		return 2;
	}
	return 0;
}
std::vector<std::string> Client::getListRoom() {
	std::string res;
	sendAllData("LIST_PLAYER");
	recvAllData(res);
	std::vector<std::string> listPlayer;
	int start = 0, pos = 0;
	while (1) {
		while ((res[pos] != '|') && (pos <res.size()))
			pos++;
		if (res[pos] != '|')
			break;
		listPlayer.push_back(res.substr(start, pos - start));
		pos++;
		start = pos;
	}
	listPlayer.push_back(res.substr(start, pos - start));
	return listPlayer;
}
std::string Client::getProfile() {
	std::string res;
	sendAllData("PROFILE");
	recvAllData(res);
	return res;
}
std::string Client::toChessNote(Vector2f p)
{
	std::string s = "";
	s += char(p.x / size + 97);
	s += char(7 - p.y / size + 49);
	return s;
}
Vector2f Client::toCoord(char a, char b)
{
	int x = int(a) - 97;
	int y = 7 - int(b) + 49;
	return Vector2f(x*size, y*size);
}
void Client::move(std::string str)
{
	Vector2f oldPos = toCoord(str[0], str[1]);
	Vector2f newPos = toCoord(str[2], str[3]);

	for (int i = 0; i<32; i++)
		if (f[i].getPosition() == newPos) f[i].setPosition(-100, -100);

	for (int i = 0; i<32; i++)
		if (f[i].getPosition() == oldPos) f[i].setPosition(newPos);
}

void Client::loadPosition()
{
	int k = 0;
	for (int i = 0; i<8; i++)
		for (int j = 0; j<8; j++)
		{
			int n = board[i][j];
			if (!n) continue;
			int x = abs(n) - 1;
			int y = n>0 ? 1 : 0;
			f[k].setTextureRect(IntRect(size*x, size*y, size, size));
			f[k].setPosition(size*j, size*i);
			k++;
		}

}

void Client::draw(RenderWindow& window,Sprite* top) {
	window.clear();
	window.draw(aBoard);
	for (int i = 0; i < 32; i++) f[i].move(offset);
	for (int i = 0; i < 32; i++) window.draw(f[i]); window.draw(*top);
	for (int i = 0; i < 32; i++) f[i].move(-offset);
	window.display();
}
void Client::playGame(bool first) {
	std::string res, req;
	moving mov;
	bool isMove = false, next = false, checkMouse = false, closeWindow = false;
	float dx = 0, dy = 0;
	Vector2f oldPos, newPos;
	std::string str, s_oldPos, s_newPos;
	int n = 0, code = 2;
	pakage pak;

	// initialize a chess game and display the initial state
	
	RenderWindow window(VideoMode(504, 504), "The Chess! (press SPACE)");
	
	loadPosition();

	if (first) {
		draw(window,&f[n]);
		recvAllData(res);
		pak = getPakage(res);
		if (pak.type == "WIN") {
			MessageBox(NULL, L"You win!", L"Win", MB_OK);
			closeWindow = true;
		}
		else{
			mov = getMove(pak.content);
			for (int i = 0; i < 32; i++)
				if (f[i].getPosition() == toCoord(mov.from[0], mov.from[1])) {
					n = i;
					break;
				}
			move(mov.from + mov.to);
			f[n].setPosition(toCoord(mov.to[0], mov.to[1]));
		}
	}
	while (window.isOpen()&&!closeWindow)
	{
		Vector2i pos = Mouse::getPosition(window) - Vector2i(offset);

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) {
				sendAllData("CANCEL");
				closeWindow = true;
				break;
			}
			////move back//////
			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::BackSpace)
				{
				}

			/////drag and drop///////
			if (e.type == Event::MouseButtonPressed)
				if (e.key.code == Mouse::Left)
					for (int i = 0; i < 32; i++)
						if (f[i].getGlobalBounds().contains(pos.x, pos.y))
						{
							isMove = true; n = i;
							dx = pos.x - f[i].getPosition().x;
							dy = pos.y - f[i].getPosition().y;
							oldPos = f[i].getPosition();
							checkMouse = true;
							break;
						}

			if (e.type == Event::MouseButtonReleased)
				if (e.key.code == Mouse::Left)
				{
					isMove = false;
					Vector2f p = f[n].getPosition() + Vector2f(size / 2, size / 2);
					newPos = Vector2f(size*int(p.x / size), size*int(p.y / size));
					sendAllData("MOVE|" + toChessNote(oldPos) + "|" + toChessNote(newPos));
					str = toChessNote(oldPos) + toChessNote(newPos);
					recvCode(&code);
					if (code==41) {
						move(str);
						f[n].setPosition(newPos);
						next = true;
						checkMouse = false;
						break;
					}
					else if (code == 43) {
						move(str);
						f[n].setPosition(newPos);
						draw(window, &f[n]);
						MessageBox(NULL, L"You lost!", L"Lost", MB_OK);
						closeWindow = true;
					}
					if(code==-41) {
						if (checkMouse) { f[n].setPosition(oldPos); }
					}
				}
		}
		if (closeWindow) {
			window.close();
			window.pollEvent(e);
			break;
		}	
		if (isMove) f[n].setPosition(pos.x - dx, pos.y - dy);

		////// draw  ///////
		draw(window, &f[n]);
		if (next) {
			next=false;
			recvAllData(res);
			pak = getPakage(res);
			if (pak.content != "") {
				mov = getMove(pak.content);
				move(mov.from + mov.to);
				////// draw  ///////
				draw(window, &f[n]);
			}
			if (pak.type == "WIN") {
				closeWindow = true;
				MessageBox(NULL, L"You win!", L"Win", MB_OK);
				break;
			}
		}

	}
	
}

