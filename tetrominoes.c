#include <ncurses.h>

typedef enum {I, O, T, L, J, S, Z} PIECETYPE;

struct tetromino {
	PIECETYPE pieceType;
	short xCoord;
	short yCoord;
	short orientation;
	short inUse;
};	

void
printBoard(unsigned short board[6][4], WINDOW* WIN) {
	//set cursor to start, print top border
	wmove(WIN, 0, 0);
	wprintw(WIN, "--------------\n");
	//print side borders and pieces based on representation
	for(int i = 0; i < 4; i++) {
		wprintw(WIN, "|");
		for(int j = 0; j < 6; j++){
			if (board[j][i] == 0) {
				wprintw(WIN, "  ");
			} else if (board[j][i] == 1) {
				wprintw(WIN, "[]");
			} else {
				wprintw(WIN, "XX");
			}
		}
		wprintw(WIN, "|\n");
	}
	//print bottom border
	wprintw(WIN, "--------------\n");
}

void
printTetrominoes(struct tetromino tetrominoes[6], WINDOW* WIN) {
	//cursor set to print tetrominoes to the right of the game window
	int cursorX = 16;
	int cursorY = 1;


	for(int i = 0; i < 6; i++) {
		//print the piece number
		mvwprintw(WIN, cursorY, cursorX, "%d)", i + 1);
		cursorX += 3;

		//print the corresponding piece
		if (tetrominoes[i].inUse == 0) {
			switch (tetrominoes[i].pieceType) {
				case I:
					mvwprintw(WIN, cursorY++, cursorX, "[]");
					mvwprintw(WIN, cursorY++, cursorX, "[]");
					mvwprintw(WIN, cursorY++, cursorX, "[]");
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 7;

					break;
				case O:
					mvwprintw(WIN, cursorY++, cursorX, "[]");
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 2;
					mvwprintw(WIN, cursorY--, cursorX, "[]");
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 5;

					break;
				case T:
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 2;
					mvwprintw(WIN, cursorY++, cursorX, "[]");
					mvwprintw(WIN, cursorY--, cursorX, "[]");
					cursorX += 2;
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 3;
					break;
				case L:
					mvwprintw(WIN, cursorY++, cursorX, "[]");
					mvwprintw(WIN, cursorY++, cursorX, "[]");
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 2;
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 5;

					break;
				case J:
					cursorY += 2;
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 2;
					mvwprintw(WIN, cursorY--, cursorX, "[]");
					mvwprintw(WIN, cursorY--, cursorX, "[]");
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 5;
					break;
				case S:
					mvwprintw(WIN, ++cursorY, cursorX, "[]");
					cursorX += 2;
					mvwprintw(WIN, cursorY--, cursorX, "[]");
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 2;
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 3;

					break;
				case Z:
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 2;
					mvwprintw(WIN, cursorY++, cursorX, "[]");
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 2;
					mvwprintw(WIN, cursorY, cursorX, "[]");
					cursorX += 3;
					break;
			}
		} else {
			cursorX += 7;
		}
		//Set cursor position for next piece, pieces printed in two rows.
		if( i < 2) {
			cursorY = 1;
		} else { 
			cursorY = 6;
		}
		if(i == 2) {
			cursorX = 16;
		}

	}
}

void
printControls(WINDOW* WIN) {
	move(1, 1);
	wprintw(WIN, "######CONTROLS######\n");
	wprintw(WIN, " 1-6: select piece\n");
	wprintw(WIN, " wasd: move piece\n");
	wprintw(WIN, " r: remove active piece\n");
	wprintw(WIN, " e: rotate clockwise\n");
	wprintw(WIN, " q: rotate counterclockwise\n");
	wprintw(WIN, " SPACE: place piece\n");
	wprintw(WIN, " c: clear board\n");
}

void 
addTetromino(unsigned short board[6][4], struct tetromino piece) {
	switch (piece.pieceType) {
		case I:
			if(piece.orientation == 0) {
				board[piece.xCoord][piece.yCoord - 1]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord + 1]++;
				board[piece.xCoord][piece.yCoord + 2]++;
			} else {
				board[piece.xCoord - 2][piece.yCoord]++;
				board[piece.xCoord - 1][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord + 1][piece.yCoord]++;
			}

			break;
		case O:
			board[piece.xCoord][piece.yCoord]++;
			board[piece.xCoord + 1][piece.yCoord]++;
			board[piece.xCoord + 1][piece.yCoord + 1]++;
			board[piece.xCoord][piece.yCoord + 1]++;

			break;
		case T:
			if (piece.orientation == 0) {
				board[piece.xCoord - 1][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord + 1][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord + 1]++;
			} else if (piece.orientation == 1) {
				board[piece.xCoord - 1][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord + 1]++;
				board[piece.xCoord][piece.yCoord - 1]++;
			} else if (piece.orientation == 2) {
				board[piece.xCoord + 1][piece.yCoord]++;
				board[piece.xCoord - 1][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord - 1]++;
			} else {
				board[piece.xCoord][piece.yCoord + 1]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord - 1]++;
				board[piece.xCoord + 1][piece.yCoord]++;
			}

			break;
		case L:
			if (piece.orientation == 0) {
				board[piece.xCoord][piece.yCoord - 1]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord + 1]++;
				board[piece.xCoord + 1][piece.yCoord + 1]++;
			} else if (piece.orientation == 1) {
				board[piece.xCoord - 1][piece.yCoord + 1]++;
				board[piece.xCoord - 1][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord + 1][piece.yCoord]++;
			} else if (piece.orientation == 2) {
				board[piece.xCoord - 1][piece.yCoord - 1]++;
				board[piece.xCoord][piece.yCoord - 1]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord + 1]++;
			} else {
				board[piece.xCoord - 1][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord + 1][piece.yCoord]++;
				board[piece.xCoord + 1][piece.yCoord - 1]++;
			}

			break;
		case J:
			if (piece.orientation == 0) {
				board[piece.xCoord - 1][piece.yCoord + 1]++;
				board[piece.xCoord][piece.yCoord + 1]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord - 1]++;
			} else if (piece.orientation == 1) {
				board[piece.xCoord - 1][piece.yCoord - 1]++;
				board[piece.xCoord - 1][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord + 1][piece.yCoord]++;
			} else if (piece.orientation == 2) {
				board[piece.xCoord][piece.yCoord + 1]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord - 1]++;
				board[piece.xCoord + 1][piece.yCoord - 1]++;
			} else {
				board[piece.xCoord - 1][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord + 1][piece.yCoord]++;
				board[piece.xCoord + 1][piece.yCoord + 1]++;
			}

			break;
		case S:
			if (piece.orientation == 0) {
				board[piece.xCoord - 1][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord - 1]++;
				board[piece.xCoord + 1][piece.yCoord - 1]++;
			} else {
				board[piece.xCoord - 1][piece.yCoord - 1]++;
				board[piece.xCoord - 1][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord][piece.yCoord + 1]++;
			}



			break;
		case Z:
			if (piece.orientation == 0) {
				board[piece.xCoord - 1][piece.yCoord - 1]++;
				board[piece.xCoord][piece.yCoord - 1]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord + 1][piece.yCoord]++;
			} else {
				board[piece.xCoord][piece.yCoord + 1]++;
				board[piece.xCoord][piece.yCoord]++;
				board[piece.xCoord + 1][piece.yCoord]++;
				board[piece.xCoord + 1][piece.yCoord - 1]++;
			}

			break;
	}
}

void removeTetromino(unsigned short board[6][4], struct tetromino piece) {	
	switch (piece.pieceType) {
		case I:
			if(piece.orientation == 0) {
				board[piece.xCoord][piece.yCoord - 1]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord + 1]--;
				board[piece.xCoord][piece.yCoord + 2]--;
			} else {
				board[piece.xCoord - 2][piece.yCoord]--;
				board[piece.xCoord - 1][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord + 1][piece.yCoord]--;
			}

			break;
		case O:
			board[piece.xCoord][piece.yCoord]--;
			board[piece.xCoord + 1][piece.yCoord]--;
			board[piece.xCoord + 1][piece.yCoord + 1]--;
			board[piece.xCoord][piece.yCoord + 1]--;

			break;
		case T:
			if (piece.orientation == 0) {
				board[piece.xCoord - 1][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord + 1][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord + 1]--;
			} else if (piece.orientation == 1) {
				board[piece.xCoord - 1][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord + 1]--;
				board[piece.xCoord][piece.yCoord - 1]--;
			} else if (piece.orientation == 2) {
				board[piece.xCoord + 1][piece.yCoord]--;
				board[piece.xCoord - 1][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord - 1]--;
			} else {
				board[piece.xCoord][piece.yCoord + 1]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord - 1]--;
				board[piece.xCoord + 1][piece.yCoord]--;
			}

			break;
		case L:
			if (piece.orientation == 0) {
				board[piece.xCoord][piece.yCoord - 1]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord + 1]--;
				board[piece.xCoord + 1][piece.yCoord + 1]--;
			} else if (piece.orientation == 1) {
				board[piece.xCoord - 1][piece.yCoord + 1]--;
				board[piece.xCoord - 1][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord + 1][piece.yCoord]--;
			} else if (piece.orientation == 2) {
				board[piece.xCoord - 1][piece.yCoord - 1]--;
				board[piece.xCoord][piece.yCoord - 1]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord + 1]--;
			} else {
				board[piece.xCoord - 1][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord + 1][piece.yCoord]--;
				board[piece.xCoord + 1][piece.yCoord - 1]--;
			}

			break;
		case J:
			if (piece.orientation == 0) {
				board[piece.xCoord - 1][piece.yCoord + 1]--;
				board[piece.xCoord][piece.yCoord + 1]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord - 1]--;
			} else if (piece.orientation == 1) {
				board[piece.xCoord - 1][piece.yCoord - 1]--;
				board[piece.xCoord - 1][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord + 1][piece.yCoord]--;
			} else if (piece.orientation == 2) {
				board[piece.xCoord][piece.yCoord + 1]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord - 1]--;
				board[piece.xCoord + 1][piece.yCoord - 1]--;
			} else {
				board[piece.xCoord - 1][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord + 1][piece.yCoord]--;
				board[piece.xCoord + 1][piece.yCoord + 1]--;
			}

			break;
		case S:
			if (piece.orientation == 0) {
				board[piece.xCoord - 1][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord - 1]--;
				board[piece.xCoord + 1][piece.yCoord - 1]--;
			} else {
				board[piece.xCoord - 1][piece.yCoord - 1]--;
				board[piece.xCoord - 1][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord][piece.yCoord + 1]--;
			}



			break;
		case Z:
			if (piece.orientation == 0) {
				board[piece.xCoord - 1][piece.yCoord - 1]--;
				board[piece.xCoord][piece.yCoord - 1]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord + 1][piece.yCoord]--;
			} else {
				board[piece.xCoord][piece.yCoord + 1]--;
				board[piece.xCoord][piece.yCoord]--;
				board[piece.xCoord + 1][piece.yCoord]--;
				board[piece.xCoord + 1][piece.yCoord - 1]--;
			}

			break;
	}
}

int OOB (struct tetromino piece) {
	//initialize ints to hold x, y coord of each block
	int X1, Y1, X2, Y2, X3, Y3, X4, Y4;

	switch (piece.pieceType) {
		case I:
			if(piece.orientation == 0) {
				X1 = piece.xCoord;
				Y1 = piece.yCoord - 1;
				X2 = piece.xCoord;
				Y2 = piece.yCoord;
				X3 = piece.xCoord;
				Y3 = piece.yCoord + 1;
				X4 = piece.xCoord;
				Y4 = piece.yCoord + 2;
			} else {
				X1 = piece.xCoord - 2;
				Y1 = piece.yCoord;
				X2 = piece.xCoord - 1;
				Y2 = piece.yCoord;
				X3 = piece.xCoord;
				Y3 = piece.yCoord;
				X4 = piece.xCoord + 1;
				Y4 = piece.yCoord;
			}

			break;
		case O:
			X1 = piece.xCoord;
			Y1 = piece.yCoord;
			X2 = piece.xCoord + 1;
			Y2 = piece.yCoord;
			X3 = piece.xCoord + 1;
			Y3 = piece.yCoord + 1;
			X4 = piece.xCoord;
			Y4 = piece.yCoord + 1;

			break;
		case T:
			if (piece.orientation == 0) {
				X1 = piece.xCoord - 1;
				Y1 = piece.yCoord;
				X2 = piece.xCoord;
				Y2 = piece.yCoord;
				X3 = piece.xCoord + 1;
				Y3 = piece.yCoord;
				X4 = piece.xCoord;
				Y4 = piece.yCoord + 1;
			} else if (piece.orientation == 1) {
				X1 = piece.xCoord - 1;
				Y1 = piece.yCoord;
				X2 = piece.xCoord;
				Y2 = piece.yCoord;
				X3 = piece.xCoord;
				Y3 = piece.yCoord + 1;
				X4 = piece.xCoord;
				Y4 = piece.yCoord - 1;
			} else if (piece.orientation == 2) {
				X1 = piece.xCoord + 1;
				Y1 = piece.yCoord;
				X2 = piece.xCoord;
				Y2 = piece.yCoord;
				X3 = piece.xCoord - 1;
				Y3 = piece.yCoord;
				X4 = piece.xCoord;
				Y4 = piece.yCoord - 1;
			} else {
				X1 = piece.xCoord;
				Y1 = piece.yCoord + 1;
				X2 = piece.xCoord;
				Y2 = piece.yCoord;
				X3 = piece.xCoord;
				Y3 = piece.yCoord - 1;
				X4 = piece.xCoord + 1;
				Y4 = piece.yCoord;
			}

			break;
		case L:
			if (piece.orientation == 0) {
				X1 = piece.xCoord;
				Y1 = piece.yCoord - 1;
				X2 = piece.xCoord;
				Y2 = piece.yCoord;
				X3 = piece.xCoord;
				Y3 = piece.yCoord + 1;
				X4 = piece.xCoord + 1;
				Y4 = piece.yCoord + 1;
			} else if (piece.orientation == 1) {
				X1 = piece.xCoord - 1;
				Y1 = piece.yCoord + 1;
				X2 = piece.xCoord - 1;
				Y2 = piece.yCoord;
				X3 = piece.xCoord;
				Y3 = piece.yCoord;
				X4 = piece.xCoord + 1;
				Y4 = piece.yCoord;
			} else if (piece.orientation == 2) {
				X1 = piece.xCoord - 1;
				Y1 = piece.yCoord - 1;
				X2 = piece.xCoord;
				Y2 = piece.yCoord - 1;
				X3 = piece.xCoord;
				Y3 = piece.yCoord;
				X4 = piece.xCoord;
				Y4 = piece.yCoord + 1;
			} else {
				X1 = piece.xCoord - 1;
				Y1 = piece.yCoord;
				X2 = piece.xCoord;
				Y2 = piece.yCoord;
				X3 = piece.xCoord + 1;
				Y3 = piece.yCoord;
				X4 = piece.xCoord + 1;
				Y4 = piece.yCoord - 1;
			}

			break;
		case J:
			if (piece.orientation == 0) {
				X1 = piece.xCoord - 1;
				Y1 = piece.yCoord + 1;
				X2 = piece.xCoord;
				Y2 = piece.yCoord + 1;
				X3 = piece.xCoord;
				Y3 = piece.yCoord;
				X4 = piece.xCoord;
				Y4 = piece.yCoord - 1;
			} else if (piece.orientation == 1) {
				X1 = piece.xCoord - 1;
				Y1 = piece.yCoord - 1;
				X2 = piece.xCoord - 1;
				Y2 = piece.yCoord;
				X3 = piece.xCoord;
				Y3 = piece.yCoord;
				X4 = piece.xCoord + 1;
				Y4 = piece.yCoord;
			} else if (piece.orientation == 2) {
				X1 = piece.xCoord;
				Y1 = piece.yCoord + 1;
				X2 = piece.xCoord;
				Y2 = piece.yCoord;
				X3 = piece.xCoord;
				Y3 = piece.yCoord - 1;
				X4 = piece.xCoord + 1;
				Y4 = piece.yCoord - 1;
			} else {
				X1 = piece.xCoord - 1;
				Y1 = piece.yCoord;
				X2 = piece.xCoord;
				Y2 = piece.yCoord;
				X3 = piece.xCoord + 1;
				Y3 = piece.yCoord;
				X4 = piece.xCoord + 1;
				Y4 = piece.yCoord + 1;
			}

			break;
		case S:
			if (piece.orientation == 0) {
				X1 = piece.xCoord - 1;
				Y1 = piece.yCoord;
				X2 = piece.xCoord;
				Y2 = piece.yCoord;
				X3 = piece.xCoord;
				Y3 = piece.yCoord - 1;
				X4 = piece.xCoord + 1;
				Y4 = piece.yCoord - 1;
			} else {
				X1 = piece.xCoord - 1;
				Y1 = piece.yCoord - 1;
				X2 = piece.xCoord - 1;
				Y2 = piece.yCoord;
				X3 = piece.xCoord;
				Y3 = piece.yCoord;
				X4 = piece.xCoord;
				Y4 = piece.yCoord + 1;
			}

			break;
		case Z:
			if (piece.orientation == 0) {
				X1 = piece.xCoord - 1;
				Y1 = piece.yCoord - 1;
				X2 = piece.xCoord;
				Y2 = piece.yCoord - 1;
				X3 = piece.xCoord;
				Y3 = piece.yCoord;
				X4 = piece.xCoord + 1;
				Y4 = piece.yCoord;
			} else {
				X1 = piece.xCoord;
				Y1 = piece.yCoord + 1;
				X2 = piece.xCoord;
				Y2 = piece.yCoord;
				X3 = piece.xCoord + 1;
				Y3 = piece.yCoord;
				X4 = piece.xCoord + 1;
				Y4 = piece.yCoord - 1;
			}

			break;
	}

	if(X1 > 5 || X1 < 0 || X2 > 5 || X2 < 0 || X3 > 5 || X3 < 0 || X4 > 5 || X4 < 0) {
		return(1);
	} else if (Y1 > 3 || Y1 < 0 || Y2 > 3 || Y2 < 0 || Y3 > 3 || Y3 < 0 || Y4 > 3 || Y4 < 0) {
		return(1);
	} else {
		return(0);
	}
	
}

