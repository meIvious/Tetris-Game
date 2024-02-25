#ifndef PIECES_H
#define PIECES_H

#include "config.h"
#include "window.h"
#include "board.h"


const int shapes[10][3][3] = {
    {{0, 1, 0},   // Plus Sign
     {1, 1, 1},
     {0, 1, 0}},

    {{1, 1, 0},   // Small Square
     {1, 1, 0},
	 {0, 0, 0}},

    {{1, 0, 0},   // Corner Piece
     {1, 0, 0},
	 {1, 1, 0}},

    {{1, 0, 0},   // Tall Tower
     {1, 0, 0},
     {1, 0, 0}},

    {{0, 1, 1},   // Zigzag
     {1, 1, 0},
	 {0, 0, 0}},

    {{1, 0, 1},   // U Shape
     {1, 1, 1},
	 {0, 0, 0}},

    {{1, 0, 0},   // Step Shape
     {1, 1, 0},
     {0, 1, 0}},

    {{0, 1, 0},   // Arrow Pointing Up
     {1, 1, 1},
     {1, 0, 1}},

    {{1, 0, 0},   // Double Zigzag
     {1, 1, 1},
     {0, 0, 1}},

    {{0, 1, 0},   // Pyramid
     {1, 1, 1},
	 {0, 0, 0}}
};




//super rotation system
const int srs[16][5][2]  = {{{0,0},{0,-1},{-1,-1},{+2,0},{+2,-1}},	
								{{0,0},{0,-1},{+1,-1},{-2,0},{-2,-1}},	
								{{0,0},{0,+1},{-1,+1},{+2,0},{+2,+1}},	
								{{0,0},{0,+1},{+1,+1},{-2,0},{-2,+1}},
								{{0,0},{0,+1},{-1,+1},{+2,0},{+2,+1}},	
								{{0,0},{0,-1},{+1,-1},{-2,0},{-2,-1}},	
								{{0,0},{0,-1},{-1,-1},{+2,0},{+2,-1}},	
								{{0,0},{0,+1},{+1,+1},{-2,0},{-2,+1}},	
								{{0,0},{0,+1},{0,-2},{-2,+1},{+1,-2}},	
								{{0,0},{0,-2},{0,+1},{-1,-2},{+2,+1}},	
								{{0,0},{0,-1},{0,+2},{+2,-1},{-1,+2}},	
								{{0,0},{0,+2},{0,-1},{+1,+2},{-2,-1}},	
								{{0,0},{0,+2},{0,-1},{+1,+2},{-2,-1}},	
								{{0,0},{0,+1},{0,-2},{-2,+1},{+1,-2}},	
								{{0,0},{0,-2},{0,+1},{-1,-2},{+2,+1}},	
								{{0,0},{0,-1},{0,+2},{+2,-1},{-1,+2}}};	

class Pieces : public Board
{
	public:
		Pieces();
		~Pieces();

		static void GetPieceColor(int pID);
		static int GetNewPiece(int pID, Board* field, Board* currentPiece);
		static int MovePiece(int dy, int dx, Board* field, Board* currentPiece);
		static int Offset3(int rot, int state, Board* field, Board* currentPiece, int cY, int cX);
		static int Offset4(int rot, int state, Board* field, Board* currentPiece, int cY, int cX);
		static int Rotate(int rot, int state, Board* field, Board* currentPiece);
		static int SetPiece(Board* field, Board* currentPiece);
		static void HardDrop(Board* field, Board* currentPiece);
};

#endif