#include <stdlib.h>

#include "config.h"
#include "window.h"
#include "board.h"

//default constructor 
Board::Board()
{
	boardData = new int*[F_HEIGHT];
	for (int i = 0; i < F_HEIGHT; i++)
		boardData[i] = new int[F_WIDTH]();
}

//overload constructor
Board::Board(int type)
{
	switch (type) {
		case 0: //active playfield
		{
			maxY = F_HEIGHT;
			maxX = F_WIDTH;
			boardData = new int*[F_HEIGHT];
			for (int i = 0; i < F_HEIGHT; i++)
				boardData[i] = new int[F_WIDTH]();
			break;
		}
		case 1: //current piece
		{
			maxY = F_HEIGHT+2;
			maxX = F_WIDTH;
			boardData = new int*[F_HEIGHT+2];
			for (int i = 0; i < F_HEIGHT+2; i++)
				boardData[i] = new int[F_WIDTH]();
			break;
		}
	}
}

//boardData Getter
int Board::Get(int x, int y){
	if (!(y >= maxY || y < 0 || x >= F_WIDTH || x < 0))
		return boardData[y][x];
}

//boardData Setter
void Board::Set(int x, int y, int val){
	//bound
	if (!(y >= maxY || y < 0 || x >= F_WIDTH || x < 0))
		boardData[y][x] = val;
}

//randomly generates 
int* Board::GenerateBag(){
	
	int* bag = new int[7];
	for (int bagID = 0; bagID < 7; bagID++) {
		bag[bagID] = 7;
	}

	int i = 0;
	int newID;

	//get number 
	while (i != 7){
		// assume new id as valid
		bool isValidID = true;

		// get new random id [0-6]
		newID = rand() % 7;

		// if bag is full 
		for (int bagID = 0; bagID < 7; bagID++) {
			if (bag[bagID] == newID) {
				isValidID = false;
				break;
			}
		}

		//set new id 
		if (isValidID) {
			bag[i] = newID;
			i++;
		}

	}

	return bag;
}


int Board::ClearLine(Board* field, bool pc){
	int linesCleared = 0;

	for (int y = 0; y < F_HEIGHT; y++) {
		bool isClearable = true;
		for (int x = 0; x < F_WIDTH; x++) {
			if (field->Get(x,y) == 0) {
				isClearable = false;
				break;
			}
		}
		// clear line
		if (isClearable) {
			linesCleared++;
			for (int line = 0; line < y; line++) {
				for (int x = 0; x < F_WIDTH; x++) {
					field->Set(x,y-line,field->Get(x,y-line-1));
				}
			}
		}
	}

	for (int y = 0; y < F_HEIGHT; y++) {
		for (int x = 0; x < F_WIDTH; x++) {
			if (field->Get(x,y) != 0)
				return linesCleared;
		}
	}
	if (pc) {
		return 8;
	} else {
		return linesCleared;
	}
}

//destructor
Board::~Board()
{
	delete boardData;
}
