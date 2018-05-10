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
#include "game.h"
#include "player.h"
#include <ctime>

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
	int score;
};
struct PlayerInfor {
	User* user;
	SOCKET connSock;
	status status;
	PlayerInfor* rival;
	Game* game;
	time_t startTime;
	time_t endTime;
	bool play;
};
struct pakage {
	string type;
	string content;
};

//declare variable
vector<User> listUser;
vector<PlayerInfor> listPlayerInfor;

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
int recvCode(SOCKET socket,int *code) {
	return recvData(socket,code, sizeof(int));
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

//Get user from string
User getUser(string line) {
	int begin = 0, end = 0;
	User user;
	while (line.at(end) != ' ')//find white space
		end++;
	//Get userId
	user.userId = line.substr(begin, end);
	end++;
	begin = end;
	while (line.at(end) != ' ')//find white space
		end++;
	//Get password
	user.pass = line.substr(begin, end - begin);
	//Get status
	user.score = atoi(line.substr(end + 1, line.length() - end).c_str());
	return user;
}

void updateUser() {
	ofstream outFile("account.txt");
	for (int i = 0; i < listUser.size(); i++)
		outFile << listUser[i].userId + " " + listUser[i].pass + " " << listUser[i].score << "\n";
	outFile.close();
}
//load user from file account.txt
void loadUser() {
	ifstream inFile("account.txt");
	string line;
	while (getline(inFile, line)) {
		//Get user from line
		listUser.push_back(getUser(line));
	}
	inFile.close();
}

void logFile(PlayerInfor playerInfor) {
	ofstream logFile;
	logFile.open(LOG_FILE, ios_base::app);
	struct tm  tstructStart,tstructEnd;
	char       start[80],end[80];
	tstructStart = *localtime(&playerInfor.startTime);
	tstructEnd = *localtime(&playerInfor.endTime);
	strftime(start, sizeof(start), "%d-%m-%Y.%X", &tstructStart);
	strftime(end, sizeof(end), "%d-%m-%Y.%X", &tstructEnd);
	sockaddr_in SockAddr;
	int addrlen = sizeof(SockAddr);
	logFile << start << " "<<end<<" ";
	getsockname(playerInfor.connSock, (LPSOCKADDR)&SockAddr, &addrlen);
	logFile << inet_ntoa(SockAddr.sin_addr)<<" ";
	getsockname(playerInfor.rival->connSock, (LPSOCKADDR)&SockAddr, &addrlen);
	logFile << inet_ntoa(SockAddr.sin_addr) << endl;
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
void login(PlayerInfor* PlayerInfor, string userId_pass) {
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
		PlayerInfor->status = ONLINE;
		PlayerInfor->user = user;
		sendCode(PlayerInfor->connSock, 01);
	}
	else if (code == 1) {
		sendCode(PlayerInfor->connSock, -02);
	}
	else //code==2
	{
		sendCode(PlayerInfor->connSock,-01);
	}
}


int logout(PlayerInfor &PlayerInfor, pakage pak) {
	int result;
	//Logout success
	result = sendCode(PlayerInfor.connSock, 11);
	PlayerInfor.status = UNIEN;
	delete PlayerInfor.game;
	delete PlayerInfor.user;
	return result;
}

//send list PlayerInfor to user
void getListPlayerInfor(PlayerInfor PlayerInfor) {
	string sendData;
	for (int i = 0; i < listPlayerInfor.size(); i++)
		if ((listPlayerInfor[i].user->userId != PlayerInfor.user->userId) 
			&& (listPlayerInfor[i].status == WAITING)
			&&(abs(listPlayerInfor[i].user->score-PlayerInfor.user->score)<10))
			sendData.append(listPlayerInfor[i].user->userId+'|');
	cout << sendData << endl;
	sendAllData(PlayerInfor.connSock, sendData);
}

int findPlayerInfor(string userId, PlayerInfor *&PlayerInfor) {
	for (int i = 0; i<listPlayerInfor.size(); i++)
		if (listPlayerInfor[i].user->userId == userId)
			if (listPlayerInfor[i].status == WAITING) {
				PlayerInfor = &listPlayerInfor[i];
				return 0;//PlayerInfor is waiting
			}
			else {
				return 1;//PlayerInfor busy
			}
			return 2;//can't find PlayerInfor
}
void joinTo(PlayerInfor* playerInfor, string userIdRival) {
	PlayerInfor* rival;
	int code = findPlayerInfor(userIdRival, rival);
	if (code == 0) {
		if (abs(rival->user->score - playerInfor->user->score) > 10) {
			sendCode(playerInfor->connSock, -24);
		}else {
			sendAllData(rival->connSock, "JOIN_FROM|" + playerInfor->user->userId);
			int code;
			recvCode(rival->connSock, &code);
			if (code == 31) {
				playerInfor->startTime = time(0);
				rival->startTime = playerInfor->startTime;
				playerInfor->status = PLAYING;
				rival->status = PLAYING;
				playerInfor->rival = rival;
				rival->rival = playerInfor;
				playerInfor->play = false;
				rival->play = true;
				Game* game = new Game();
				game->getNextPlayer();
				playerInfor->game = game;
				rival->game = game; 
				sendCode(playerInfor->connSock, 21);
			}
			else {
				sendCode(playerInfor->connSock, -21);
			}
		}
	}
	else if (code == 1) {
		sendCode(playerInfor->connSock, -22);
	}
	else {
		sendCode(playerInfor->connSock, -23);
	}
}

void createRoom(PlayerInfor* PlayerInfor) {
	PlayerInfor->status = WAITING;
}
void move(PlayerInfor playerInfor, string go) {
	if (playerInfor.game == NULL)
		return;
	if (playerInfor.play) {
		int code;
		cout << go.substr(0, 2) << " " << go.substr(3);
		code = playerInfor.game->currentPlayer()->makeMove(go.substr(0, 2), go.substr(3));
		if (code == 0) {
			playerInfor.game->getNextPlayer();
			sendCode(playerInfor.connSock, 41);
			playerInfor.play =false;
			playerInfor.rival->play = true;
			sendAllData(playerInfor.rival->connSock, "MOVE|" + go);
		}
		else if (code == 1) {
			sendCode(playerInfor.connSock, 43);
			sendAllData(playerInfor.rival->connSock, "WIN|"+go);
			playerInfor.endTime = time(0);
			playerInfor.rival->endTime = playerInfor.endTime;
			playerInfor.user->score -= 3;
			playerInfor.rival->user->score += 3;
			delete playerInfor.game;
			logFile(playerInfor);
			updateUser();
		}
		else if (code == 2)
			sendCode(playerInfor.connSock, -41);
	}
	
}
void cancel(PlayerInfor playerInfor) {
	playerInfor.user->score -= 3;
	playerInfor.rival->user->score += 3;
	playerInfor.endTime = time(0);
	playerInfor.rival->endTime = playerInfor.endTime;
	sendAllData(playerInfor.rival->connSock, "WIN");
	delete playerInfor.game;
	logFile(playerInfor);
	updateUser();
}
void exitConnect(PlayerInfor PlayerInfor) {
	//Delete session
	for (int i = 0; i<listPlayerInfor.size(); i++)
		if (listPlayerInfor[i].connSock == PlayerInfor.connSock) {
			closesocket(listPlayerInfor[i].connSock);
			listPlayerInfor.erase(listPlayerInfor.begin() + i);
			break;
		}
	return;
}

int main(int argc, char* argv[]) {
	const int SERVER_PORT = 5500;//atoi(argv[1]);//Get port from argument command line 
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
				PlayerInfor playerInfor;
				playerInfor.connSock = connSock;
				playerInfor.status = UNIEN;
				//add session 
				listPlayerInfor.push_back(playerInfor);
				FD_SET(connSock, &initfds);


				if (listPlayerInfor.size() == FD_SETSIZE) {
					printf("\nToo many clients.");
					closesocket(connSock);
				}

				if (--nEvents == 0)
					continue; //no more event
			}
		}

		//receive data from clients
		for (int i = 0; i < listPlayerInfor.size(); i++) {
			if (FD_ISSET(listPlayerInfor[i].connSock, &readfds)) {
				string s_pakage;
				int result = recvAllData(listPlayerInfor[i].connSock, s_pakage);
				if (result != SOCKET_ERROR) {
					pakage pak = getPakage(s_pakage);
					cout << "Receive from client " << pak.type << " " << pak.content << "\n";
					if (pak.type == "LOGIN") {
						login(&listPlayerInfor[i], pak.content);
					}
					else if (pak.type == "LIST_PLAYER") {
						getListPlayerInfor(listPlayerInfor[i]);
					}
					else if (pak.type == "CREATE_ROM") {
						createRoom(&listPlayerInfor[i]);
					}
					else if (pak.type == "JOIN_TO") {
						joinTo(&listPlayerInfor[i], pak.content);
					}
					else if (pak.type == "MOVE") {
						move(listPlayerInfor[i], pak.content);
					}
					else if (pak.type == "CANCEL") {
						cancel(listPlayerInfor[i]);
					}
				}
				else {
					FD_CLR(listPlayerInfor[i].connSock, &initfds);
					exitConnect(listPlayerInfor[i]);
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


