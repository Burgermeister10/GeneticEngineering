#ifndef TETROMINOES_H
#define TETROMINOES_H

typedef enum {I, O, T, L, J, S, Z} PIECETYPE;

struct tetromino {
	PIECETYPE pieceType;
	short xCoord;
	short yCoord;
	short orientation;
	short inUse;
};	

void printBoard(unsigned short board[6][4], WINDOW* WIN);
void printTetrominoes(struct tetromino tetrominoes[6], WINDOW* WIN);
void printControls(WINDOW* WIN);
void addTetromino(unsigned short board[6][4], struct tetromino piece);
void removeTetromino(unsigned short board[6][4], struct tetromino piece);
int OOB(struct tetromino piece);

#endif
