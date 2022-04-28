// playTicTacToe.cpp
// This set of functions are used to actually play the game.
// Play starts with the function: playTicTacToe() which is defined below

#include "Nim.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <ChString.h>
#include <vector>
#include <iomanip>
using namespace std;

bool emptyBoard(vector<int>board) {
	bool hasNoRocks = false;
	for (int i = 0; i < board.size(); i++) {
		if (board[i] != 0) {
			return false;
		}
		else {
			hasNoRocks = true;
		}
	}
	return hasNoRocks;
}

int check4Win(vector<int>board, int player, bool wrongInput)
{
	bool ifEmpty = emptyBoard(board);

	if (ifEmpty == true && wrongInput == false) {
		return 1;
	}
	else if (wrongInput == true) {
		return 2;
	}
	else {
		return 0;
	}

}
int concate(int a, int b)
{
	string s1 = to_string(a);
	string s2 = to_string(b);
	string s = s1 + s2;
	int c = stoi(s);
	return c;
}

void displayBoard(vector<int> board) {
	cout << "\nNIM BOARD" << endl;
	int amount;
	for (int i = 0; i < board.size(); i++) {
		amount = board[i];
		cout << "Pile:" << i + 1 << " -> ";
		for (int j = 0; j < amount; j++) {
			cout << "O ";
		}
		cout << endl;
	}
}

void initializeBoard(vector<int>board) {
	cout << "\nNIM BOARD" << endl;
	int amount;
	for (int i = 0; i < board.size(); i++) {
		amount = board[i];
		cout << "Pile:" << i + 1 << " -> ";
		for (int j = 0; j < amount; j++) {
			cout << "O ";
		}
		cout << endl;
	}
}

vector<int> storeInput(string input) {
	vector<int>pairs;
	for (int i = 0; i < input.size(); i++) {
		int fdigit = input[i] - '0';
		int sdigit = input[i + 1] - '0';
		if (fdigit == 0) {
			if (sdigit >= 1 && sdigit <= 20) {
				pairs.push_back(sdigit);
			}
		}
		else {
			int pair = concate(input[i] - '0', input[i + 1] - '0');
			if (pair >= 1 && pair <= 20) {
				pairs.push_back(pair);
			}

		}
		input.erase(0, 1);
	}
	//for (int i = 0; i < pairs.size(); i++) {
	//	cout << pairs[i] << endl;
	//}
	return pairs;
}

int getMove(vector<int>&board, int player) {
	int columns;
	int numOfRocks;
	bool check = true;
	cout << "Input pile number you want to take rocks from: ";
	cin >> columns;

	if (columns > board.size()) {
		cout << "Pile not available. Try again: "; //when they lose
		cin >> columns;
		if (!(board[columns - 1])) {
			check4Win(board, player, true);
		}
	}
	if (board[columns-1] ) {
		cout << "How many rocks would you like to take from pile " << columns << "? ";
		cin >> numOfRocks;
		if (board[columns - 1] < numOfRocks) {
			cout << "Too many rocks to ask for, try again: ";
			cin >> numOfRocks;
			if (board[columns - 1] < numOfRocks) {
				check4Win(board, player, true);
			}
		}
	}
	return concate(columns, numOfRocks);
}

void updateBoard(vector<int>&board, int move, int player) {

	string a = to_string(move);
	string col = a.substr(0,1);
	string num = a.substr(1,2);
	int c = stoi(col);
	int n = stoi(num);

	board[c-1] = board[c-1] - n;
}

/*****Chat functions******/
void displayActionPrompt() {
	cout << "What would you like to do?\n";
	cout << "(M)ove, (C)hat, (F)orfeit\n";
}

void displayChatPrompt() {
	cout << "Type a message to send\n";
}

void displayChatMessage(char message[], int player) {
	cout << "Player " << player << " - " << "chat: " << message << endl << endl;
}

void sendChat(string message, string host, string port, SOCKET s) {
	UDP_send(s, const_cast<char*>(message.c_str()), strlen(message.c_str()), (char*)host.c_str(), (char*)port.c_str());
}

void sendForfeit(string host, string port, SOCKET s) {
	string f = "F";
	UDP_send(s, const_cast<char*>(f.c_str()), strlen(f.c_str()), (char*)host.c_str(), (char*)port.c_str());
}

string getChatInput() {
	char c[MAX_CHAT_SIZE + 1];
	cin.ignore();
	cin.getline(c, MAX_CHAT_SIZE);
	string s = c;
	return s;
}

void chat(string host, string port, SOCKET s) {
	displayChatPrompt();
	string chatInput = getChatInput();
	chatInput = /*"C" +*/ chatInput + "\0";
	sendChat(chatInput, host, port, s);
}

int getUserAction() {
	displayActionPrompt();
	char input;
	bool inputCheck = false;
	cin >> input;
	do {
		if (input == 'M' || input == 'm') {
			inputCheck = true;
			return MOVE;
		}
		else if (input == 'C' || input == 'c') {
			inputCheck = true;
			return CHAT;
		}
		else if (input == 'F' || input == 'f') {
			inputCheck = true;
			return FORFEIT;
		}
		//if inputCheck
	} while (inputCheck != true);

}

/*****Forfeit Function********/
void endGame() {
	cout << "Player forfeitted the game" << endl;
	cout << "You win!\n";
}

/*****Chat functions END******/

int playNim(SOCKET s, std::string serverName, std::string host, std::string port, int player, char* boardInput)
{
	int winner = noWinner;
	vector<int>board;
	int opponent;
	int move;
	int userAction = NULL;
	bool myMove;
	string input;
	
	if (player == X_PLAYER) { 
		//write input here for piles
		std::cout << "Playing as Player 1" << std::endl;
		opponent = O_PLAYER;
		myMove = true;
	}
	else { //host
		std::cout << "Playing as Player 2" << std::endl;
		opponent = X_PLAYER;
		myMove = false;
		
	}
		input = string(boardInput);
		board = storeInput(input);
		displayBoard(board);

	while (winner == noWinner) {

		if (myMove) {
			//getting the user's option: chat, move, or forfeit
			userAction = getUserAction();
			if (userAction == MOVE) {
				// Get my move & display board
				move = getMove(board, player);

				std::cout << "Board after your move:" << std::endl;
				updateBoard(board, move, player);
				displayBoard(board);
				// Send move to opponent

				char charmove[MAX_SEND_BUF] = "";
				_itoa_s(move, charmove, 10);
				UDP_send(s, charmove, strlen(charmove) + 1, (char*)host.c_str(), (char*)port.c_str());
			}
			else if (userAction == CHAT) {
				//call chat functionality
				chat(host, port, s);
			}
			else if (userAction == FORFEIT) {
				//Need to let the opponent know that the player forfeitted the game
				sendForfeit(host, port, s);
				return winner = ABORT;
			}

		}
		else {
			std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
			//Get opponent's move & display board
			int status = wait(s, 40, 0);
			if (status > 0) {

				char newmove[MAX_RECV_BUF] = "";
				UDP_recv(s, newmove, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				move = atoi(newmove);
				char temp = *newmove;
				//need to make an indication to not update the board if you sent a chat
				if (temp ==  'F') {
					endGame();
					break;
				}
				else if (move == 0) {
					displayChatMessage(newmove, player);
				}
				else {
					updateBoard(board, move, player);
					displayBoard(board);
				}

			}
			else {
				winner = ABORT;
			}
		}
		//change the move so the opponent can play the game
		myMove = !myMove;

		if (winner == ABORT) {
			std::cout << "No response from opponent.  Aborting the game..." << std::endl;
		}
		else {
			winner = check4Win(board, player, false);
		}

		if (winner == player)
			std::cout << "You WIN!" << std::endl;
		else if (winner == opponent)
			std::cout << "I'm sorry.  You lost" << std::endl;
	}

	return winner;
}