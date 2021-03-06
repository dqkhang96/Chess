#include "stdafx.h"
#include "stdio.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#include<iostream>
#include<string>
#include<conio.h>
#include "vector"
#include <iostream>
#include <windows.h>
#include <fstream>
#include<process.h>
#include <ctime>
#include <algorithm>

#include "queen.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "pawn.h"
#include "king.h"
#include "square.h"
#include "player.h"

#pragma comment(lib,"Ws2_32.lib")
#pragma warning(disable:4996)
#define BUFF_SIZE 2048
#define LOG_FILE "log.txt"

enum status { WAITING, PLAYING, ONLINE, UNIEN };
using namespace std;
//type declaration
struct User {
	string userId;
	string pass;
	int numberGame;
	int numberGameWin;
	int score;
};
struct Session {
	User* user;
	SOCKET connSock;
	status status;
	Session* joinTo=NULL;
	Player* player=NULL;
	time_t startTime;
	time_t endTime;
};
struct pakage {
	string type;
	string content;
};

//declare variable
vector<User> listUser;
vector<Session> listSession;

//Send data has data_size byte
int sendData(SOCKET socket, const void * data, int data_size) {
	char *sendbuff = (char*)data;
	int ret, nLeft = data_size;
	int idx = 0;

	while (nLeft > 0) {
		// Send data from nLeft position
		ret = send(socket, &sendbuff[idx], nLeft, 0);
		if (ret == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		nLeft -= ret;
		idx += ret;
	}
	return 1;
}

//Send code
int sendCode(SOCKET socket, int code) {
	int ret = sendData(socket, &code, sizeof(int));
	return ret;
}

//Send a string
int sendAllData(SOCKET socket, const string &data) {
	//Covert host order to big-endian
	ULONG data_size = htonl(data.size());
	//Send data size
	int result = sendData(socket, &data_size, sizeof(data_size));
	if (result == 1) {
		//Send data
		result = sendData(socket, data.c_str(), data.size());
	}
	return result;
}

//Receive data_size byte
int recvData(SOCKET socket, const void *data, int data_size) {
	char *recvbuff = (char*)data;
	int ret, nLeft, idx;
	nLeft = data_size; // length of the data needs to be
					   // received
	idx = 0;
	while (nLeft > 0) {
		//Receive data from nLeft position
		ret = recv(socket, &recvbuff[idx], nLeft, 0);
		if (ret == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		nLeft -= ret;
		idx += ret;
	}
	return 1;
}

//Recv code
int recvCode(SOCKET socket, int *code) {
	return recvData(socket, code, sizeof(int));
}

//Receive a string
int recvAllData(SOCKET socket, string &data) {
	ULONG data_size;
	data = "";
	int result = recvData(socket, &data_size, sizeof(data_size));
	if (result == 1) {
		//Covert big-endian to host order 
		data_size = ntohl(data_size);
		if (data_size > 0) {
			//Resize data output
			data.resize(data_size);
			//Receive data 
			result = recvData(socket, &data[0], data_size);
			if (result != 1) {
				data.clear();
			}
		}
	}
	return result;
}

bool compareUser(User user1,User user2) {
	return user1.score > user2.score;
}
void sortListUser() {
	sort(listUser.begin(), listUser.end(), compareUser);
}

//Get user from string
User getUser(string line) {
	int begin = 0, end = 0;
	User user;
	while (line.at(end) != ' ')//find white space
		end++;
	//Get userId
	user.userId = line.substr(begin, end);
	//Get password
	end++;
	begin = end;
	while (line.at(end) != ' ')//find white space
		end++;
	user.pass = line.substr(begin, end - begin);
	//Get score
	end++;
	begin = end; 
	
	while (line.at(end) != ' ')
		end++;
	user.score = atoi(line.substr(begin, end-begin).c_str());
	//Get number game
	end++;
	begin = end;
	while (line.at(end) != ' ')
		end++;
	user.numberGame = atoi(line.substr(begin, end - begin).c_str());
	//Get number game win
	end++;
	begin = end;
	user.numberGameWin = atoi(line.substr(begin, line.size() - begin).c_str());
	return user;
}

void updateUser() {
	ofstream outFile("account.txt");
	for (int i = 0; i < listUser.size(); i++)
		outFile << listUser[i].userId + " " + listUser[i].pass + " " << listUser[i].score <<" "<<listUser[i].numberGame<<" "<<listUser[i].numberGameWin<<endl;
	outFile.close();
}
//load user from file account.txt
void loadUser() {
	ifstream inFile("account.txt");
	if (inFile.fail()) {
		cout << "File account.txt not found!";
		return;
	}
	string line;
	while (getline(inFile, line)) {
		//Get user from line
		listUser.push_back(getUser(line));
	}
	sortListUser();
	inFile.close();
}

void logFile(Session Session) {
	ofstream logFile;
	logFile.open(LOG_FILE, ios_base::app);
	struct tm  tstructStart, tstructEnd;
	char       start[80], end[80];
	tstructStart = *localtime(&Session.startTime);
	tstructEnd = *localtime(&Session.endTime);
	strftime(start, sizeof(start), "%d-%m-%Y.%X", &tstructStart);
	strftime(end, sizeof(end), "%d-%m-%Y.%X", &tstructEnd);
	sockaddr_in SockAddr;
	int addrlen = sizeof(SockAddr);
	logFile <<"Start: "<< start << " End: " << end << endl;
	getsockname(Session.connSock, (LPSOCKADDR)&SockAddr, &addrlen);
	logFile << "  Player win: "<<endl;
	logFile<<"    User address:" << inet_ntoa(SockAddr.sin_addr) <<endl<< "    User id: " << Session.user->userId <<endl;
	getsockname(Session.joinTo->connSock, (LPSOCKADDR)&SockAddr, &addrlen);
	logFile << "  Player lost:"<<endl;
	logFile << "    User address:" << inet_ntoa(SockAddr.sin_addr)<<endl<< "    User id: " << Session.user->userId <<endl;
	logFile.close();
}
//Get pakage from string pakage
pakage getPakage(string s) {
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
//find user in listUser
int validUser(string userId, string pass, User* &user) {
	for (int i = 0; i < listUser.size(); i++)
		if (listUser[i].userId == userId)
			if (listUser[i].pass == pass) {
				user = &listUser[i];
				return 0;//valided
			}
			else return 1;//password fail
			return 2;//can't find user
}

//login fun
void login(Session* Session, string userId_pass) {
	int i;
	for (i = 0; i < userId_pass.length(); i++)
		if (userId_pass[i] == '|')
			break;
	string userId = userId_pass.substr(0, i);
	string pass = userId_pass.substr(i + 1, userId_pass.size() - i - 1);
	User *user;
	int code = validUser(userId, pass, user);
	if (code == 0)//success
	{
		Session->status = ONLINE;
		Session->user = user;
		sendCode(Session->connSock, 01);
	}
	else if (code == 1) {
		sendCode(Session->connSock, -02);
	}
	else //code==2
	{
		sendCode(Session->connSock, -01);
	}
}


int logout(Session &Session, pakage pak) {
	int result;
	//Logout success
	result = sendCode(Session.connSock, 11);
	Session.status = UNIEN;
	if (Session.player != NULL) {
		delete Session.player;
		delete Session.joinTo->player;
	}
	delete Session.user;
	return result;
}

//send list Session to user
void getListPlayer(Session Session) {
	string sendData;
	for (int i = 0; i < listSession.size(); i++)
		if ((listSession[i].user->userId != Session.user->userId)
			&& (listSession[i].status == WAITING)
			&& (abs(listSession[i].user->score - Session.user->score) < 10))
			sendData.append(listSession[i].user->userId + '|');
	cout << sendData << endl;
	sendAllData(Session.connSock, sendData);
}
void getProfile(Session session) {
	string sendData;
	int index;
	for (index = 0; index < listUser.size(); index++)
		if (listUser[index].userId == session.user->userId)
			break;
	sendData=session.user->userId+"|"+to_string(session.user->score)+"|"+to_string(session.user->numberGame)+"|"+to_string(session.user->numberGameWin)+"|"+to_string(index+1);
	
	sendAllData(session.connSock,sendData);
}
int findSession(string userId, Session *&Session) {
	for (int i = 0; i < listSession.size(); i++)
		if (listSession[i].user->userId == userId)
			if (listSession[i].status == WAITING) {
				Session = &listSession[i];
				return 0;//Session is waiting
			}
			else {
				return 1;//Session busy
			}
			return 2;//can't find Session
}

void initializeGame(Session* session)
{
	Piece* aPiece;
	King* aKing;
	Square* aSquare;
	Board* board;

	// create piece sets
	set<Piece*>*whitePieces = (new set<Piece*>);
	set<Piece*>*blackPieces = (new set<Piece*>);
	board = new Board();
	// Create each piece
	// Set their locations
	// Put them in their respective collections
	aPiece = new Queen(true,board);
	aSquare = board->squareAt(3, 0);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	whitePieces->insert(aPiece);
	aPiece = new Queen(false,board);
	aSquare = board->squareAt(3, 7);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	blackPieces->insert(aPiece);

	aPiece = new Bishop(true,board);
	aSquare = board->squareAt(2, 0);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	whitePieces->insert(aPiece);
	aPiece = new Bishop(false,board);
	aSquare = board->squareAt(2, 7);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	blackPieces->insert(aPiece);
	aPiece = new Bishop(true,board);
	aSquare = board->squareAt(5, 0);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	whitePieces->insert(aPiece);
	aPiece = new Bishop(false,board);
	aSquare = board->squareAt(5, 7);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	blackPieces->insert(aPiece);

	aPiece = new Knight(true,board);
	aSquare = board->squareAt(1, 0);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	whitePieces->insert(aPiece);
	aPiece = new Knight(false,board);
	aSquare = board->squareAt(1, 7);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	blackPieces->insert(aPiece);
	aPiece = new Knight(true,board);
	aSquare = board->squareAt(6, 0);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	whitePieces->insert(aPiece);
	aPiece = new Knight(false,board);
	aSquare = board->squareAt(6, 7);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	blackPieces->insert(aPiece);

	aPiece = new Rook(true,board);
	aSquare = board->squareAt(0, 0);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	whitePieces->insert(aPiece);
	aPiece = new Rook(false,board);
	aSquare = board->squareAt(0, 7);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	blackPieces->insert(aPiece);
	aPiece = new Rook(true,board);
	aSquare = board->squareAt(7, 0);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	whitePieces->insert(aPiece);
	aPiece = new Rook(false,board);
	aSquare = board->squareAt(7, 7);
	aSquare->setOccupier(aPiece);
	aPiece->setLocation(aSquare);
	blackPieces->insert(aPiece);

	for (int i = 0; i < 8; i++)
	{
		aPiece = new Pawn(true,board);
		aSquare = board->squareAt(i, 1);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		whitePieces->insert(aPiece);
		aPiece = new Pawn(false,board);
		aSquare = board->squareAt(i, 6);
		aSquare->setOccupier(aPiece);
		aPiece->setLocation(aSquare);
		blackPieces->insert(aPiece);
	}

	// create the white king
	aKing = new King(true,board);
	aSquare = board->squareAt(4, 0);
	aSquare->setOccupier(aKing);
	aKing->setLocation(aSquare);
	whitePieces->insert(aKing);

	// create White player and hand them their pieces
	session->player = new Player("White", true, *aKing, whitePieces,false,board);

	// create the black king
	aKing = new King(false,board);
	aSquare = board->squareAt(4, 7);
	aSquare->setOccupier(aKing);
	aKing->setLocation(aSquare);
	blackPieces->insert(aKing);

	// create Black player and hand them their pieces
	session->joinTo->player = new Player("Black", false, *aKing, blackPieces,true,board);
	session->player->setOpponent(session->joinTo->player);
	session->joinTo->player->setOpponent(session->player);
	
}

void joinTo(Session* session, string joinUserId) {
	Session* joinTo;
	int code = findSession(joinUserId, joinTo);
	if (code == 0) {
		if (abs(joinTo->user->score - session->user->score) > 10) {
			sendCode(session->connSock, -24);
		}
		else {
			sendAllData(joinTo->connSock, "JOIN_FROM|" + session->user->userId);
			int code;
			recvCode(joinTo->connSock, &code);
			if (code == 31) {
				session->startTime = time(0);
				joinTo->startTime = session->startTime;
				session->status = PLAYING;
				joinTo->status = PLAYING;
				session->joinTo = joinTo;
				joinTo->joinTo = session;
				initializeGame(session);
				sendCode(session->connSock, 21);
			}
			else {
				sendCode(session->connSock, -21);
			}
		}
	}
	else if (code == 1) {
		sendCode(session->connSock, -22);
	}
	else {
		sendCode(session->connSock, -23);
	}
}

void createRoom(Session* Session) {
	Session->status = WAITING;
}
void move(Session Session, string go) {
	if (Session.player == NULL)
		return;

	int code;
	code = Session.player->makeMove(go.substr(0, 2), go.substr(3));
	if (code == 0) {
		sendCode(Session.connSock, 41);
		sendAllData(Session.joinTo->connSock, "MOVE|" + go);
	}
	else if (code == 1) {
		sendCode(Session.connSock, 43);
		sendAllData(Session.joinTo->connSock, "WIN|" + go);
		//Update session
		Session.endTime = time(0);
		Session.joinTo->endTime = Session.endTime;
		Session.user->score -= 3;
		Session.joinTo->user->score += 3;
		Session.user->numberGame++;
		Session.joinTo->user->numberGame++;
		Session.joinTo->user->numberGameWin++;
		sortListUser();
		logFile(Session);
		updateUser();
	}
	else if (code == 2)
		sendCode(Session.connSock, -41);
}


void cancel(Session Session) {
	//Update session
	Session.user->score -= 3;
	Session.joinTo->user->score += 3;
	Session.user->numberGame++;
	Session.joinTo->user->numberGame++;
	Session.joinTo->user->numberGameWin++;
	Session.endTime = time(0);
	Session.joinTo->endTime = Session.endTime;
	sendAllData(Session.joinTo->connSock, "WIN");
	if(Session.player!=NULL)
		delete Session.player;
	logFile(Session);
	updateUser();
}
void exitConnect(Session Session) {
	if (Session.player != NULL)
		delete Session.player;
	//Delete session
	for (int i = 0; i < listSession.size(); i++)
		if (listSession[i].connSock == Session.connSock) {
			closesocket(listSession[i].connSock);
			listSession.erase(listSession.begin() + i);
			break;
		}
	return;
}

int main() {
	const int SERVER_PORT = 5500;//Get port from argument command line 
	const char* SERVER_ADDR = "127.0.0.1";

	//Initiate WinSock
	WSADATA	wsaData;
	WORD wVestion = MAKEWORD(2, 2);
	if (WSAStartup(wVestion, &wsaData))
		printf("Version is not supported.\n");

	//Construct socket
	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Bind address to socket
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	if (bind(listenSock, (sockaddr *)&serverAddr, sizeof(serverAddr))) {
		printf("Error! Cannot bind this address.\n");
		_getch();
		return 0;
	}

	//Listen request from client
	if (listen(listenSock, 10)) {
		printf("Error! Cannot listen.");
		_getch();
		return 0;
	}
	printf("Server started!\n");
	printf("Address: %s Port: %d\n", SERVER_ADDR, SERVER_PORT);
	loadUser();
	SOCKET connSock;
	fd_set readfds, initfds;//use inittfds to initiate readfds at the begining of every loop step
	sockaddr_in clientAddr;
	int ret, nEvents, clientAddrLen;

	FD_ZERO(&initfds);
	FD_SET(listenSock, &initfds);

	//Step 5: Communicate with clients
	while (1) {
		readfds = initfds;		/* structure assignment */
		nEvents = select(0, &readfds, 0, 0, 0);
		if (nEvents < 0) {
			printf("\nError! Cannot poll sockets: %d", WSAGetLastError());
			break;
		}

		//new client connection
		if (FD_ISSET(listenSock, &readfds)) {
			clientAddrLen = sizeof(clientAddr);
			if ((connSock = accept(listenSock, (sockaddr *)&clientAddr, &clientAddrLen)) < 0) {
				printf("\nError! Cannot accept new connection: %d", WSAGetLastError());
				break;
			}
			else {
				printf("You got a connection from %s\n", inet_ntoa(clientAddr.sin_addr));//print client's IP
																						 //new session
				Session Session;
				Session.connSock = connSock;
				Session.status = UNIEN;
				//add session 
				listSession.push_back(Session);
				FD_SET(connSock, &initfds);


				if (listSession.size() == FD_SETSIZE) {
					printf("\nToo many clients.");
					closesocket(connSock);
				}

				if (--nEvents == 0)
					continue; //no more event
			}
		}

		//receive data from clients
		for (int i = 0; i < listSession.size(); i++) {
			if (FD_ISSET(listSession[i].connSock, &readfds)) {
				string s_pakage;
				int result = recvAllData(listSession[i].connSock, s_pakage);
				if (result != SOCKET_ERROR) {
					pakage pak = getPakage(s_pakage);
					cout << "Receive from client " << pak.type << " " << pak.content << "\n";
					if (pak.type == "LOGIN") {
						login(&listSession[i], pak.content);
					}
					else if (pak.type == "LIST_PLAYER") {
						getListPlayer(listSession[i]);
					}
					else if (pak.type == "CREATE_ROM") {
						createRoom(&listSession[i]);
					}
					else if (pak.type == "JOIN_TO") {
						joinTo(&listSession[i], pak.content);
					}
					else if (pak.type == "MOVE") {
						move(listSession[i], pak.content);
					}
					else if (pak.type == "CANCEL") {
						cancel(listSession[i]);
					}
					else if (pak.type == "PROFILE") {
						getProfile(listSession[i]);
					}
				}
				else {
					FD_CLR(listSession[i].connSock, &initfds);
					exitConnect(listSession[i]);
				}
			}

			if (--nEvents <= 0)
				continue; //no more event
		}

	}

	//Terminate Winsock
	WSACleanup();
	return 0;
}


