/*
 * Author: Luke Burger
 * Email: burger.178@buckeyemail.osu.edu
 * Date Created: 03/27/2020
 * Class: CSE 2451 SP20
 * Instructor: Max Taylor
 *
 * Purpose: This program implements the game "Genetic Engineering".  The ncurses library is used to manage I/O and the GUI.
 * 			For more info, see the README file.
 *
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include "tetrominoes.h"

int main () {
	//initialize the ncurses environment
	initscr();
	raw();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	WINDOW* WIN;
	curs_set(0);
	WIN = newwin(20, 53, 0, 0);

	char menuInput = ' ';
	//while continues from menu to game when p is pressed
	while (menuInput != 'p') {
		if (menuInput == 'c') {
			//print controls
			werase(WIN);
			printControls(WIN);
			wrefresh(WIN);
		} else {
			//prints menu
			werase(WIN);
			wprintw(WIN, "\tWelcome to Genetic Engineering!\n\n");
			wprintw(WIN, "Press 'c' to see the controls before you start.\n");
			wprintw(WIN, "Press 'p' at any time to begin playing.\n");
			wrefresh(WIN);
		}
		menuInput = wgetch(WIN);
	}


	//create 6 random tetrominoes
	struct tetromino tetrominoes[6];

	srand(time(0));		//use time(0) as seed for pseudo rng	
	for (int i = 0; i < 6; i++) {
		tetrominoes[i].pieceType = rand() % 7;
		tetrominoes[i].xCoord = 3;
		tetrominoes[i].yCoord = 1;
		tetrominoes[i].orientation = 0;
		tetrominoes[i].inUse  = 0;
	}


	/*2D array stores the representation of the game board
	 * 0 indicates empty block: 
	 * 1 indicates filled block: []
	 * 2 indicates an unallowed operation: XX
	 */
	unsigned short board [6][4];
	memset(board, 0, sizeof(board));

	int gameOver = 0;
	int gameWon = 0;
	while (!gameOver) {
		//ensure the window is updated
		werase(WIN);
		printBoard(board, WIN);
		printTetrominoes(tetrominoes, WIN);
		wrefresh(WIN);

		char input = wgetch(WIN);
		if ((input == '1' || input == '2' || input == '3' || input == '4' || input == '5' || input == '6') && tetrominoes[input - '1'].inUse == 0) {
			int turnOver = 0;
			while(!turnOver) {
				addTetromino(board, tetrominoes[input - '1']);
				tetrominoes[input - '1'].inUse = 1; 
				werase(WIN);
				printBoard(board, WIN);
				printTetrominoes(tetrominoes, WIN);
				wrefresh(WIN);

				//remove tetromino, make changes in swith, then eprint at start of while
				removeTetromino(board, tetrominoes[input - '1']);
				tetrominoes[input - '1'].inUse = 0;

				char pieceInput = wgetch(WIN);
				switch (pieceInput) {
					case 'r': 
						//ends the turn
						turnOver = 1;

						break;
					case 'e':
						//rotates the piece clockwise
						if (tetrominoes[input - '1'].pieceType == I || tetrominoes[input - '1'].pieceType == S || tetrominoes[input - '1'].pieceType == Z) {
							//I, S, Z have only two orientations
							tetrominoes[input - '1'].orientation = !(tetrominoes[input - '1'].orientation);
							//if the piece goes OOB, the orientation is returned;
							if(OOB(tetrominoes[input - '1'])) {
								tetrominoes[input - '1'].orientation = !(tetrominoes[input - '1'].orientation);
							}
						} else if (tetrominoes[input - '1'].pieceType == O) {
							//O has one orientation, do nothing
							;
						} else {
							//all other pieces have 4 orientations
							tetrominoes[input - '1'].orientation++;
							if(tetrominoes[input - '1'].orientation > 3) {
								tetrominoes[input - '1'].orientation = 0;
							}
							//if the piece goes OOB, the orientation is returned
							if(OOB(tetrominoes[input - '1'])) {
								tetrominoes[input - '1'].orientation--;
								if(tetrominoes[input - '1'].orientation < 0) {
									tetrominoes[input - '1'].orientation = 3;
								}
							}
						}

						break;
					case 'q':
						//rotates the piece counterclockwise
						if (tetrominoes[input - '1'].pieceType == I || tetrominoes[input - '1'].pieceType == S || tetrominoes[input - '1'].pieceType == Z) {
							//I, S, Z have only two orientations
							tetrominoes[input - '1'].orientation = !(tetrominoes[input - '1'].orientation);
							//if the piece goes OOB, the orientation is returned;
							if(OOB(tetrominoes[input - '1'])) {
								tetrominoes[input - '1'].orientation = !(tetrominoes[input - '1'].orientation);
							}
						} else if (tetrominoes[input - '1'].pieceType == O) {
							//O has one orientation, do nothing
							;
						} else {
							//all other pieces have 4 orientations
							tetrominoes[input - '1'].orientation--;
							if(tetrominoes[input - '1'].orientation < 0) {
								tetrominoes[input - '1'].orientation = 3;
							}
							//if the piece goes OOB, the orientation is returned
							if(OOB(tetrominoes[input - '1'])) {
								tetrominoes[input - '1'].orientation++;
								if(tetrominoes[input - '1'].orientation > 3) {
									tetrominoes[input - '1'].orientation = 0;
								}
							}
						}

						break;
					case 'w':
						//moves piece up
						tetrominoes[input - '1'].yCoord--;
						if(OOB(tetrominoes[input - '1'])) {
							tetrominoes[input - '1'].yCoord++;
						}

						break;
					case 'a':
						//moves piece left
						tetrominoes[input - '1'].xCoord--;
						if(OOB(tetrominoes[input - '1'])) {
							tetrominoes[input - '1'].xCoord++;
						}

						break;
					case 's':
						//moves piece down
						tetrominoes[input - '1'].yCoord++;
						if(OOB(tetrominoes[input - '1'])) {
							tetrominoes[input - '1'].yCoord--;
						}
						break;
					case 'd':
						//moves piece right
						tetrominoes[input - '1'].xCoord++;
						if(OOB(tetrominoes[input - '1'])) {
							tetrominoes[input - '1'].xCoord--;
						}
						break;
					case ' ':
						//adds piece to board
						addTetromino(board, tetrominoes[input - '1']);
						tetrominoes[input - '1'].inUse = 1;
						//if any index of game represenation is 2, the placement is not valid
						int validPlacement = 1;
						for(int i = 0; i < 6; i++) {
							for(int j = 0; j < 4; j++) {
								if(board[i][j] == 2) {
									validPlacement = 0;
								}
							}
						}

						//if placement is valid, end turn, else remove piece
						if(!validPlacement) {
							removeTetromino(board, tetrominoes[input - '1']);
							tetrominoes[input - '1'].inUse = 0;
						} else {
							turnOver = 1;
						}

						break;
					case 'c':
						//reset game board representaion
						memset(board, 0, sizeof(board));
						//reset pieces
						for (int i = 0; i < 6; i++) {
							tetrominoes[i].inUse = 0;
						}
						turnOver = 1;

						break;
					case 'z':
						//shuffles the piece types, clearing board
						memset(board, 0, sizeof(board));
						for(int i = 0; i < 6; i++) {
							tetrominoes[i].pieceType = rand() % 7;
							tetrominoes[i].inUse = 0;
							tetrominoes[i].xCoord = 3;
							tetrominoes[i].yCoord = 1;
							tetrominoes[i].orientation = 0;
						}
						turnOver = 1;
				}
			}

			//check if the game has been won
			gameWon = 1;
			gameOver = 1;
			for(int i = 0; i < 6; i++) {
				for(int j = 0; j < 4; j++) {
					if(board[i][j] == 0) {
						gameWon = 0;
						gameOver = 0;
					}
				}
			}					

		} else if (input == 'c') {
			memset(board, 0, sizeof(board));
			for(int i = 0; i < 6; i++) {
				tetrominoes[i].inUse = 0;
			}
		} else if (input == 'z') {
			//shuffles the piece types, clearing board
			memset(board, 0, sizeof(board));
			for(int i = 0; i < 6; i++) {
				tetrominoes[i].pieceType = rand() % 7;
				tetrominoes[i].inUse = 0;
				tetrominoes[i].xCoord = 3;
				tetrominoes[i].yCoord = 1;
				tetrominoes[i].orientation = 0;
			}
		} else if (input == 'x') {
			gameOver = 1;
		} else {
			;		//input ignored if not valid
		}
	}


	//Print final screen, different if game won
	if (gameWon) {
		werase(WIN);
		wmove(WIN, 0, 0);
		wprintw(WIN, "\n\tCongratulations! You win!\n");
		wprintw(WIN, "\t   Thanks For Playing");
		wrefresh(WIN);
		sleep(3);
	} else {
		werase(WIN);
		wmove(WIN, 0, 0);
		wprintw(WIN, "\n\tThanks for playing!");
		wrefresh(WIN);
		sleep(3);
	}

	delwin(WIN);
	endwin();

	return 0;
}

