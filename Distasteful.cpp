// Distasteful.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
//#include <Windows.h>
#include <ctime>

using namespace std;

void initBoard();
void printBoard();
bool isHumanFirst();
void humanMove();
bool isValidMove(char[], char, bool);
int miniMax(int, char*);
void checkGameOver();
int checkWinner(int, int);
int evaluate();
void swapSides();
int minFunction(int, int, int);
int maxFunction(int, int, int);
void colorGreen();
void colorWhite();
void computerMove();
void sortTrees();

const int moveLength = 4;
const int rowCount = 8;
const int colCount = 7;
const char human = 'h', computer = 'c';
char board[rowCount][colCount];
char players[rowCount][colCount];
clock_t clockStart;
char moveTree[20][200];
int scoreTree[20][200];

int main()
{
	initBoard();
	printBoard();

	if (isHumanFirst()) {
		while (true) {
			humanMove();
			checkGameOver();

			swapSides();

			computerMove();
			//miniMax(int);
			checkGameOver();

			swapSides();
		}
	}
	else {
		while (true) {
			swapSides();

			computerMove();
			//miniMax(int);
			checkGameOver();

			swapSides();

			humanMove();
			checkGameOver();
		}
	}

	return 0;
}

void initBoard() {
	int i, j;
	for (i = 0; i < rowCount; i++) {
		for (j = 0; j < colCount; j++) {
			board[i][j] = ' ';
			players[i][j] = ' ';
		}
	}
	board[0][3] = 'K'; players[0][3] = computer;
	board[1][0] = 'J'; players[1][0] = computer;
	board[1][1] = 'J'; players[1][1] = computer;
	board[1][2] = 'J'; players[1][2] = computer;
	board[1][4] = 'S'; players[1][4] = computer;
	board[1][5] = 'S'; players[1][5] = computer;
	board[1][6] = 'S'; players[1][6] = computer;
	board[2][0] = 's'; players[2][0] = computer;
	board[2][1] = 's'; players[2][1] = computer;
	board[2][2] = 's'; players[2][2] = computer;
	board[2][4] = 'j'; players[2][4] = computer;
	board[2][5] = 'j'; players[2][5] = computer;
	board[2][6] = 'j'; players[2][6] = computer;
	board[5][0] = 'j'; players[5][0] = human;
	board[5][1] = 'j'; players[5][1] = human;
	board[5][2] = 'j'; players[5][2] = human;
	board[5][4] = 's'; players[5][4] = human;
	board[5][5] = 's'; players[5][5] = human;
	board[5][6] = 's'; players[5][6] = human;
	board[6][0] = 'S'; players[6][0] = human;
	board[6][1] = 'S'; players[6][1] = human;
	board[6][2] = 'S'; players[6][2] = human;
	board[6][4] = 'J'; players[6][4] = human;
	board[6][5] = 'J'; players[6][5] = human;
	board[6][6] = 'J'; players[6][6] = human;
	board[7][3] = 'K'; players[7][3] = human;
}

void printBoard() {
	int i, j;
	//colorGreen();
	//cout << "----------------- " << "Computer" << endl;
	//colorWhite();
	cout << "\033[32m----------------- Computer\033[0m\n";
	for (i = 0; i < rowCount; i++) {
		cout << rowCount - i << " ";
		for (j = 0; j < colCount; j++) {
			cout << "|";
			if (players[i][j] == computer) {
				//colorGreen();
				//cout << board[i][j];
				//colorWhite();
				cout << "\033[32m" << board[i][j] << "\033[0m";
			}
			else
				cout << board[i][j];
		}
		cout << "|" << endl;
	}
	cout << "----------------- Human" << endl << "  ";
	for (i = 65; i < (65 + colCount); i++) {
		cout << " " << char(i);
	}
	cout << endl;
}

bool isHumanFirst() {
	int start;
	cout << "Would you like to go first or second? (1/2)" << endl;
	cin >> start;
	while (start != 1 && start != 2) {
		cout << "Enter 1 or 2" << endl;
		cin >> start;
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
		}
	}
	if (start == 1)
		return true;
	else
		return false;
}

void humanMove() {
	char move[moveLength];
	do {
		cout << "Enter your move" << endl;
		cin >> move[0];
		cin >> move[1];
		cin >> move[2];
		cin >> move[3];
	} while (!isValidMove(move, human, true));

	int convRowS = '8' - move[1];
	int convColS = tolower(move[0]) - 'a';
	int convRowE = '8' - move[3];
	int convColE = tolower(move[2]) - 'a';

	board[convRowE][convColE] = board[convRowS][convColS];
	players[convRowE][convColE] = players[convRowS][convColS];
	board[convRowS][convColS] = ' ';
	players[convRowS][convColS] = ' ';
	if (players[convRowE - 1][convColE] == computer) {
		if (board[convRowE - 1][convColE] >= 'a') {
			board[convRowE - 1][convColE] = ' ';
			players[convRowE - 1][convColE] = ' ';
		}
		else
			board[convRowE - 1][convColE] = tolower(board[convRowE - 1][convColE]);
		cout << "Hi-Ya!" << endl;
	}
}

void computerMove() {
	int best = -9999, score, startRow, startCol, bestRow, bestCol, depth = 1;
	char bestMove[moveLength], move[moveLength];
	double duration;

	clockStart = clock();
	while (best < 5000) {
		score = miniMax(depth, move);
		
		if (score > best) {
			best = score;
			for (int a = 0; a < moveLength; a++) bestMove[a] = move[a];
		}
		//sortTrees();
		duration = (clock() - clockStart) / (double)CLOCKS_PER_SEC;
		//cout << "Depth: " << depth << " Duration: " << duration << " Best: " << best << endl;
		if (duration >= 5.0)
			break;
		depth++;
	}
	startRow = '8' - bestMove[1];
	startCol = tolower(bestMove[0]) - 'a';
	bestRow = '8' - bestMove[3];
	bestCol = tolower(bestMove[2]) - 'a';
	//cout << "Best: " << best << endl;
	board[bestRow][bestCol] = board[startRow][startCol];
	players[bestRow][bestCol] = players[startRow][startCol];
	board[startRow][startCol] = ' ';
	players[startRow][startCol] = ' ';
	char a = 'a' + ('g' - bestMove[0]);
	char b = '8' - (bestMove[1] - '1');
	char c = 'a' + ('g' - bestMove[2]);
	char d = '8' - (bestMove[3] - '1');
	cout << "My move is " << a << b << c << d << " (" << bestMove[0] << bestMove[1] << bestMove[2] << bestMove[3] << ")" << endl;
	if (players[bestRow - 1][bestCol] == human) {
		if (board[bestRow - 1][bestCol] >= 'a') {
			board[bestRow - 1][bestCol] = ' ';
			players[bestRow - 1][bestCol] = ' ';
		}
		else
			board[bestRow - 1][bestCol] = tolower(board[bestRow - 1][bestCol]);
		cout << "Hi-Ya!" << endl;
	}
}

int miniMax(int maxDepth, char* bestMove) {
	char move[moveLength], start, end, pStart, pEnd, prevPiece, prevPlayer;
	int best = -9999, score, depth = 0, counter = 0;
	int convRowS, convColS, convRowE, convColE;
	bool attack, flag = false;

	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if (players[i][j] == computer) {
				move[0] = 'a' + j;
				move[1] = '8' - i;
				for (int k = 0; k < rowCount; k++) {
					for (int l = 0; l < colCount; l++) {
						move[2] = 'a' + l;
						move[3] = '8' - k;
						if (isValidMove(move, computer, false))
							for (int b = 0; b < moveLength; b++) moveTree[depth][counter++] = move[b];
					}
				}
			}
		}
	}
	int x = 0, y = 0;
	for (int i = 0; i < (counter / moveLength); i++) {
		convColS = tolower(moveTree[depth][x++]) - 'a';
		convRowS = '8' - moveTree[depth][x++];
		convColE = tolower(moveTree[depth][x++]) - 'a';
		convRowE = '8' - moveTree[depth][x++];
		start = board[convRowS][convColS];
		end = board[convRowE][convColE];
		pStart = players[convRowS][convColS];
		pEnd = players[convRowE][convColE];
		// Make move
		board[convRowE][convColE] = board[convRowS][convColS];
		players[convRowE][convColE] = players[convRowS][convColS];
		board[convRowS][convColS] = ' ';
		players[convRowS][convColS] = ' ';
		if (players[convRowE - 1][convColE] == human) {
			attack = true;
			prevPiece = board[convRowE - 1][convColE];
			prevPlayer = players[convRowE - 1][convColE];
			if (board[convRowE - 1][convColE] >= 'a') {
				board[convRowE - 1][convColE] = ' ';
				players[convRowE - 1][convColE] = ' ';
			}
			else
				board[convRowE - 1][convColE] = tolower(board[convRowE - 1][convColE]);
		}
		else
			attack = false;
		// Evaluate move
		score = minFunction(depth + 1, best, maxDepth);
		scoreTree[depth][y++] = score;

		//cout << "Score: " << score << endl;
		if (score > best) {
			for (int a = 0; a < moveLength; a++) bestMove[a] = moveTree[depth][x - (moveLength - a)];
			best = score;
		}
		// Retract move
		board[convRowS][convColS] = start;
		board[convRowE][convColE] = end;
		players[convRowS][convColS] = pStart;
		players[convRowE][convColE] = pEnd;
		if (attack) {
			board[convRowE - 1][convColE] = prevPiece;
			players[convRowE - 1][convColE] = prevPlayer;
		}
		double duration = (clock() - clockStart) / (double)CLOCKS_PER_SEC;
		if (duration >= 5.0)
			return best;
	}
	return best;
}

int minFunction(int depth, int upperBest, int maxDepth) {
	char move[moveLength], start, end, pStart, pEnd, prevPiece, prevPlayer;
	int best = 9999, score, counter = 0;
	int convRowS, convColS, convRowE, convColE;
	bool attack;

	if (checkWinner(depth, maxDepth) != -1)
		return checkWinner(depth, maxDepth);
	if (depth == maxDepth)
		return evaluate();

	swapSides();
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if (players[i][j] == human) {
				move[0] = 'a' + j;
				move[1] = '8' - i;
				for (int k = 0; k < rowCount; k++) {
					for (int l = 0; l < colCount; l++) {
						move[2] = 'a' + l;
						move[3] = '8' - k;
						if (isValidMove(move, human, false)) {
							for (int b = 0; b < moveLength; b++) moveTree[depth][counter++] = move[b];
						}
					}
				}
			}
		}
	}
	int x = 0, y = 0;
	for (int i = 0; i < (counter / moveLength); i++) {
		convColS = tolower(moveTree[depth][x++]) - 'a';
		convRowS = '8' - moveTree[depth][x++];
		convColE = tolower(moveTree[depth][x++]) - 'a';
		convRowE = '8' - moveTree[depth][x++];
		start = board[convRowS][convColS];
		end = board[convRowE][convColE];
		pStart = players[convRowS][convColS];
		pEnd = players[convRowE][convColE];
		// Make move
		board[convRowE][convColE] = board[convRowS][convColS];
		players[convRowE][convColE] = players[convRowS][convColS];
		board[convRowS][convColS] = ' ';
		players[convRowS][convColS] = ' ';
		if (players[convRowE - 1][convColE] == human) {
			attack = true;
			prevPiece = board[convRowE - 1][convColE];
			prevPlayer = players[convRowE - 1][convColE];
			if (board[convRowE - 1][convColE] >= 'a') {
				board[convRowE - 1][convColE] = ' ';
				players[convRowE - 1][convColE] = ' ';
			}
			else
				board[convRowE - 1][convColE] = tolower(board[convRowE - 1][convColE]);
		}
		else
			attack = false;
		// Evaluate move
		score = maxFunction(depth + 1, best, maxDepth);
		scoreTree[depth][y++] = score;
		//cout << "Score: " << score;
		if (score < best)
			best = score;
		// Retract move
		board[convRowS][convColS] = start;
		board[convRowE][convColE] = end;
		players[convRowS][convColS] = pStart;
		players[convRowE][convColE] = pEnd;
		if (attack) {
			board[convRowE - 1][convColE] = prevPiece;
			players[convRowE - 1][convColE] = prevPlayer;
		}
		double duration = (clock() - clockStart) / (double)CLOCKS_PER_SEC;
		if (best < upperBest || duration >= 5.0) {
			swapSides();
			return best;
		}
	}
	swapSides();
	return best;
}

int maxFunction(int depth, int upperBest, int maxDepth) {
	char move[moveLength], start, end, pStart, pEnd, prevPiece, prevPlayer;
	int best = -9999, score, counter = 0;
	int convRowS, convColS, convRowE, convColE;
	bool attack;

	if (checkWinner(depth, maxDepth) != -1)
		return checkWinner(depth, maxDepth);
	if (depth == maxDepth)
		return evaluate();

	swapSides();
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if (players[i][j] == human) {
				move[0] = 'a' + j;
				move[1] = '8' - i;
				for (int k = 0; k < rowCount; k++) {
					for (int l = 0; l < colCount; l++) {
						move[2] = 'a' + l;
						move[3] = '8' - k;
						if (isValidMove(move, human, false)) {
							for (int b = 0; b < moveLength; b++) moveTree[depth][counter++] = move[b];
						}
					}
				}
			}
		}
	}
	int x = 0, y = 0;
	for (int i = 0; i < (counter / moveLength); i++) {
		convColS = tolower(moveTree[depth][x++]) - 'a';
		convRowS = '8' - moveTree[depth][x++];
		convColE = tolower(moveTree[depth][x++]) - 'a';
		convRowE = '8' - moveTree[depth][x++];
		start = board[convRowS][convColS];
		end = board[convRowE][convColE];
		pStart = players[convRowS][convColS];
		pEnd = players[convRowE][convColE];
		// Make move
		board[convRowE][convColE] = board[convRowS][convColS];
		players[convRowE][convColE] = players[convRowS][convColS];
		board[convRowS][convColS] = ' ';
		players[convRowS][convColS] = ' ';
		if (players[convRowE - 1][convColE] == human) {
			attack = true;
			prevPiece = board[convRowE - 1][convColE];
			prevPlayer = players[convRowE - 1][convColE];
			if (board[convRowE - 1][convColE] >= 'a') {
				board[convRowE - 1][convColE] = ' ';
				players[convRowE - 1][convColE] = ' ';
			}
			else
				board[convRowE - 1][convColE] = tolower(board[convRowE - 1][convColE]);
		}
		else
			attack = false;
		// Evaluate move
		score = minFunction(depth + 1, best, maxDepth);
		scoreTree[depth][y++] = score;
		//cout << "Score: " << score << endl;
		if (score > best)
			best = score;
		// Retract move
		board[convRowS][convColS] = start;
		board[convRowE][convColE] = end;
		players[convRowS][convColS] = pStart;
		players[convRowE][convColE] = pEnd;
		if (attack) {
			board[convRowE - 1][convColE] = prevPiece;
			players[convRowE - 1][convColE] = prevPlayer;
		}
		double duration = (clock() - clockStart) / (double)CLOCKS_PER_SEC;
		if (best > upperBest || duration >= 5.0) {
			swapSides();
			return best;
		}
	}
	swapSides();
	return best;
}

int evaluate() {
	int eval, hCount = 0, cCount = 0;
	char move[moveLength];

	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if (players[i][j] == computer) {
				switch (board[i][j]) {
				case 'S': cCount += 4; break;
				case 'J': cCount += 3; break;
				case 's': cCount += 2; break;
				case 'j': cCount += 1; break;
				default: break;
				}
				if (j > 1 && j < 5)
					cCount += 5;
			}
			if (players[i][j] == human) {
				switch (board[i][j]) {
				case 'S': hCount += 4; break;
				case 'J': hCount += 3; break;
				case 's': hCount += 2; break;
				case 'j': hCount += 1; break;
				default: break;
				}
				if (j > 1 && j < 5)
					hCount += 5;
			}
		}
	}

	
	char p;
	if (players[0][3] == human) {
		p = computer;
	}
	else {
		p = human;
	}
	move[2] = 'd';
	move[3] = '7';
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if (players[i][j] == p) {
				move[0] = 'a' + j;
				move[1] = '8' - i;
				if (isValidMove(move, p, false)) {
					if (p == computer)
						cCount += 20;
					else
						hCount += 20;
				}
			}
		}
	}
	//move[2] = 'd';
	//move[3] = '2';


	eval = cCount - hCount;
	return eval;
}

int checkWinner(int depth, int maxDepth) {
	int kingRow = 0, kingCol = 3;
	char p;
	char move[moveLength];
	bool anyMovesLeft = false;

	if (players[kingRow][kingCol] == 'c')
		p = human;
	else
		p = computer;

	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if (players[i][j] == p) {
				move[0] = 'a' + j;
				move[1] = '8' - i;
				for (int k = 0; k < rowCount; k++) {
					for (int l = 0; l < colCount; l++) {
						move[2] = 'a' + l;
						move[3] = '8' - k;
						if (isValidMove(move, p, false)) {
							anyMovesLeft = true;
							goto here;
						}
					}
				}
			}
		}
	}
	if (!anyMovesLeft) {
		if (p == human)
			return 5000;// +(maxDepth - depth);
		else
			return -5000;// +-(maxDepth - depth);
	}
	here:
	move[2] = 'a' + kingCol;
	move[3] = '8' - kingRow;
	if (players[kingRow + 1][kingCol] == p) {
		if (p == human)
			return -5000;// +-(maxDepth - depth);
		else
			return 5000;// +(maxDepth - depth);
	}
	return -1;
}

void checkGameOver() {
	if (players[0][3] == computer)
		printBoard();
	else {
		swapSides();
		printBoard();
		swapSides();
	}
	if (checkWinner(0, 0) <= -5000) {
		cout << "ayee you won gj lol" << endl;
		cout << "Press enter to quit" << endl;
		cin.ignore();
		cin.get();
		exit(0);
	}
	if (checkWinner(0, 0) >= 5000) {
		cout << "Git gud." << endl;
		cout << "Press enter to quit" << endl;
		cin.ignore();
		cin.get();
		exit(0);
	}
	if (checkWinner(0, 0) == 0) {
		cout << "Draw..." << endl;
		cout << "Press enter to quit" << endl;
		cin.ignore();
		cin.get();
		exit(0);
	}
}

void swapSides() {
	char tempBoard[rowCount][colCount];
	char tempPlayers[rowCount][colCount];
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			tempBoard[i][j] = board[i][j];
			tempPlayers[i][j] = players[i][j];
		}
	}

	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			board[i][j] = tempBoard[(rowCount - i) - 1][(colCount - j) - 1];
			players[i][j] = tempPlayers[(rowCount - i) - 1][(colCount - j) - 1];
		}
	}
}

void sortTrees() {
	int i = 0, j = 0;
	while (moveTree[i][j] != 'z') {
		while (moveTree[i][j] != 'z') {
			i++;
			j++;
		}
	}
}

bool isValidMove(char myMove[moveLength], char player, bool printError)
{
	char colStart = tolower(myMove[0]);
	char rowStart = (myMove[1]);
	char colEnd = tolower(myMove[2]);
	char rowEnd = myMove[3];
	int convRowS = '8' - rowStart;
	int convColS = colStart - 'a';
	int convRowE = '8' - rowEnd;
	int convColE = colEnd - 'a';
	int curRow = convRowS;
	int curCol = convColS;

	if (colStart >= 'a' && colStart <= 'g' && rowStart >= '1' && rowStart <= '8' &&
		colEnd >= 'a' && colEnd <= 'g' && rowEnd >= '1' && rowEnd <= '8' && sizeof(myMove) == moveLength) {
		if (players[convRowS][convColS] == player && board[convRowS][convColS] != ' ' && board[convRowE][convColE] == ' ') {
			switch (board[convRowS][convColS]) {
			case 'K':
				if (printError) cout << "King cannot move..." << endl;
				return false;
			case 'J':
				if (abs(convRowS - convRowE) == abs(convColE - convColS)) {
					if (convRowS - convRowE > 0) {
						if (convColE - convColS > 0) {
							while (board[--curRow][++curCol] == ' ') {
								if (curRow == convRowE && curCol == convColE)
									return true;
							}
							if (printError) cout << "Piece in the way..." << endl;
							return false;
						}
						else {
							while (board[--curRow][--curCol] == ' ') {
								if (curRow == convRowE && curCol == convColE)
									return true;
							}
							if (printError) cout << "Piece in the way..." << endl;
							return false;
						}
					}
					else {
						if (convColE - convColS > 0) {
							while (board[++curRow][++curCol] == ' ') {
								if (curRow == convRowE && curCol == convColE) {
									if (players[convRowE - 1][convColE] != player && players[convRowE - 1][convColE] != ' ')
										return true;
									else {
										if (printError) cout << "Cannot move backward unless attacking..." << endl;
										return false;
									}
								}
							}
							if (printError) cout << "Piece in the way..." << endl;
							return false;
						}
						else {
							while (board[++curRow][--curCol] == ' ') {
								if (curRow == convRowE && curCol == convColE) {
									if (players[convRowE - 1][convColE] != player && players[convRowE - 1][convColE] != ' ')
										return true;
									else {
										if (printError) cout << "Cannot move backward unless attacking..." << endl;
										return false;
									}
								}
							}
							if (printError) cout << "Piece in the way..." << endl;
							return false;
						}
					}
				}
				else {
					if (printError) cout << "Invalid Ninja move..." << endl;
					return false;
				}
			case 'S':
				if (convRowS - convRowE > 0 && convColS - convColE == 0) {
					while (board[--curRow][convColS] == ' ') {
						if (curRow == convRowE)
							return true;
					}
					if (printError) cout << "Piece in the way..." << endl;
					return false;
				}
				else if (convColS - convColE > 0 && convRowS - convRowE == 0) {
					while (board[convRowS][--curCol] == ' ') {
						if (curCol == convColE) {
							if (players[convRowE - 1][curCol] != player && players[convRowE - 1][curCol] != ' ')
								return true;
							else {
								if (printError) cout << "Cannot move horizontal without attacking..." << endl;
								return false;
							}
						}
					}
					if (printError) cout << "Piece in the way..." << endl;
					return false;
				}
				else if (convColS - convColE < 0 && convRowS - convRowE == 0) {
					while (board[convRowS][++curCol] == ' ') {
						if (curCol == convColE) {
							if (players[convRowE - 1][curCol] != player && players[convRowE - 1][curCol] != ' ')
								return true;
							else {
								if (printError) cout << "Cannot move horizontal without attacking..." << endl;
								return false;
							}
						}
					}
					if (printError) cout << "Piece in the way..." << endl;
					return false;
				}
				else {
					if (printError) cout << "Samurai can only move up vertical or horizontal..." << endl;
					return false;
				}
			case 'j':
				if (convRowS - 1 == convRowE && (convColS - 1 == convColE || convColS + 1 == convColE))
					return true;
				else if (convRowS + 1 == convRowE && (convColS - 1 == convColE || convColS + 1 == convColE)) {
					if (players[convRowE - 1][convColE] != player && players[convRowE - 1][convColE] != ' ')
						return true;
					else {
						if (printError) cout << "Cannot move backwards unless attacking..." << endl;
						return false;
					}
				}
				else {
					if (printError) cout << "Invalid Mini-Ninja move..." << endl;
					return false;
				}
			case 's':
				if (convRowS - 1 == convRowE && convColS - convColE == 0)
					return true;
				else if (convColS - 1 == convColE && convRowS - convRowE == 0) {
					if (players[convRowE - 1][convColE] != player && players[convRowE - 1][convColE] != ' ')
						return true;
					else {
						if (printError) cout << "Cannot move horizontal without attacking..." << endl;
						return false;
					}
				}
				else if (convColS + 1 == convColE && convRowS - convRowE == 0) {
					if (players[convRowE - 1][convColE] != player && players[convRowE - 1][convColE] != ' ')
						return true;
					else {
						if (printError) cout << "Cannot move horizontal without attacking..." << endl;
						return false;
					}
				}
				else {
					if (printError) cout << "Invalid Mini-Samurai move..." << endl;
					return false;
				}
			}
		}
		else {
			if (printError) cout << "Invalid starting space or another piece in destination space..." << endl;
			return false;
		}
	}
	else {
		if (printError) cout << "A move is outside board range..." << endl;
		return false;
	}
}
/*
void colorGreen() {
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
}

void colorWhite() {
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);
}
*/
