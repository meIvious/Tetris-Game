#include "pieces.h"

Pieces::Pieces()
{
}


void Pieces::GetPieceColor(int pID){
	switch(pID)
	{
		case 0: {
			Window::Format(0); break;
			} // Empty
		case 1: {
			Window::Format(6); break;
			} 
		case 2: {
			Window::Format(3); break;
			} 
		case 3: case 8: {
			Window::Format(5); break;
			} 
		case 4: case 9: {
			Window::Format(2); break;
			} 
		case 5: case 10: {
			Window::Format(1); break;
			} 
		case 6: case 11: {
			Window::Format(4); break;
			} 
		case 7: case 12: {
			Window::Format(8); break;
			} 
	}
}


int Pieces::GetNewPiece(int pID, Board* field, Board* currentPiece){
	for (int y = 0; y < 2; y++) {
		for (int x = 0; x < 4; x++) {
			if (field->Get(x,y) != 0)
				return 1;
			currentPiece->Set(F_WIDTH/2-2+x,shapes[pID][y][x],y);
		}
	}
	
	return 0;
}

//move in array xy  direction, returns 1 if success
int Pieces::MovePiece(int dy, int dx, Board* field, Board* currentPiece){

	//initial
	int tempField[F_HEIGHT+4][F_WIDTH+2] = {0};

	//copy currentPiece to temp array
	for (int y = 0; y < F_HEIGHT+2; y++) {
		for (int x = 0; x < F_WIDTH; x++) {
			tempField[y+1][x+1] = currentPiece->Get(x,y);
			//check 
			if (tempField[y+1][x+1] != 0 && (y-dy >= F_HEIGHT || x-dx >= F_WIDTH || x-dx < 0)) {
				return 0;
			}
		}
	}

	for (int y = 0; y < F_HEIGHT; y++) {
		for (int x = 0; x < F_WIDTH; x++) {
			if (tempField[y+1+dy][x+1+dx] != 0 && field->Get(x,y) != 0) {
				return 0;
			}
		}
	}

	//Allow movement
	for (int y = 0; y < F_HEIGHT+2; y++) {
		for (int x = 0; x < F_WIDTH; x++) {
			currentPiece->Set(x,y,tempField[y+1+dy][x+1+dx]);
		}
	}

	return 1;
}

//SRS offset algorithm 
int Pieces::Offset3(int rot, int state, Board* field, Board* currentPiece, int cY, int cX)
{
	int tempPiece[3][3] = {};
	int rotatedPiece[3][3] = {};

	//copy currentPiece to 3x3 temp array
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			if (cY-1+y >= F_HEIGHT+2 || cY-1+y < 0 || cX-1+x >= F_WIDTH || cX-1+x < 0) {
				tempPiece[x][y] = 0;
			} else {
				tempPiece[x][y] = currentPiece->Get(cX-1+x,cY-1+y);
			}
		}
	}

	//rotate temporary array
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			switch(rot) {
				case 1: rotatedPiece[x][y] = tempPiece[y][3-x-1]; break;
				case -1: rotatedPiece[x][y] = tempPiece[3-y-1][x]; break;
			}
		}
	}

	int offsetYX[2] = {0,0};

	//check whether if piece fits
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			if (rotatedPiece[x][y] != 0 && (field->Get(cX-1+x,cY-1+y) != 0 || cY-1+y > F_HEIGHT+2 || cX-1+x > F_WIDTH-1 || cX-1+x < 0))
			{
				int final;
				switch (rot) {
					case 1: {
						final = Window::Modulo((state+rot),4); 
						break;
						}
					case -1: {
						final = Window::Modulo((state+rot),4)+4; 
						break;
						}
				}

				bool offsetSuccess;

				for (int test = 0; test < 5; test++) {
					offsetSuccess = true;
					for (int y = 0; y < 3; y++) {
						for (int x = 0; x < 3; x++) {
							if (cY-1+y-srs[final][test][0] > F_HEIGHT+2 ||
								cX-1+x+srs[final][test][1] > F_WIDTH-1 ||
								cX-1+x+srs[final][test][1] < 0){
								offsetSuccess = false;
								break;
							} else {
								if (rotatedPiece[x][y] != 0 &&
									(field->Get(cX-1+x+srs[final][test][1],cY-1+y-srs[final][test][0]) != 0))
								{
									 offsetSuccess = false;
									 break;
								}
							}
						}
						if (!offsetSuccess)
							break;
					}


					if (offsetSuccess) {
						offsetYX[0] = srs[final][test][0];
						offsetYX[1] = srs[final][test][1];
						//clear currentPiece
						for (int y = 0; y < F_HEIGHT+2; y++) {
							for (int x = 0; x < F_WIDTH; x++) {
								currentPiece->Set(x,y,0);
							}
						}
						//copy rotatedPiece back to currentPiece
						for (int y = 0; y < 3; y++) {
							for (int x = 0; x < 3; x++) {
								currentPiece->Set(cX-1+x+offsetYX[1],cY-1+y-offsetYX[0],rotatedPiece[y][x]);
							}
						}
						return Window::Modulo((state+rot),4);
					}
				}

				return state;
			}							
		}
	}

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			currentPiece->Set(cX-1+x,cY-1+y,rotatedPiece[x][y]);
		}
	}
			
	return Window::Modulo((state+rot),4);
}

//SRS offset algorithm for I piece
int Pieces::Offset4(int rot, int state, Board* field, Board* currentPiece, int cY, int cX)
{
	int tempPiece[4][4] = {};
	int rotatedPiece[4][4] = {};
	
	//check whether if piece is horizontal or vertical
	switch(state) {
		case 0: case 2: //horizontal
		{
			for (int x = 0; x < 4; x++) {
				tempPiece[(state == 0) ? 1 : 2][x] = currentPiece->Get(cY,cX+x);
			}
			break;
		}
		case 1: case 3: //vertical
		{
			for (int y = 0; y < 4; y++) {
				tempPiece[y][(state == 3) ? 2 : 1] = currentPiece -> Get(cY+y,cX);
			}
		}
	}

	//rotate temp array
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			switch(rot) {
				case 1: rotatedPiece[x][y] = tempPiece[y][4-x-1]; break;
				case -1: rotatedPiece[x][y] = tempPiece[4-y-1][x]; break;
			}
		}
	}


	int offsetYX[2] = {0,0};

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (rotatedPiece[x][y] != 0 && (field->Get(cX+x,cY+y) != 0 ||
				cY+y > F_HEIGHT+2 || cX+x > F_WIDTH-1 || cX+x < 0))
			{
				int final;
				switch (rot) {
					case 1: {
						final = 8+Window::Modulo((state+rot),4); 
						break;
						}
					case -1: {
						final = 8+Window::Modulo((state+rot),4)+4; 
						break;
						}
				}

				bool offsetSuccess;
				
				for (int test = 0; test < 5; test++) {

					offsetSuccess = true;

					for (int y = 0; y < 4; y++) {
						for (int x = 0; x < 4; x++) {

							if (cY+y-srs[final][test][0] > F_HEIGHT+2 ||
								cX+x+srs[final][test][1] > F_WIDTH-1 ||
								cX+x+srs[final][test][1] < 0){
								offsetSuccess = false;
								break;
							} else {
								if (rotatedPiece[x][y] != 0 &&
									(field->Get(cX+x+srs[final][test][1],cY+y-srs[final][test][0]) != 0))
								{
									 offsetSuccess = false;
									 break;
								}
							}
						}
						if (!offsetSuccess)
							break;
					}

					if (offsetSuccess) {
						offsetYX[0] = srs[final][test][0];
						offsetYX[1] = srs[final][test][1];
						//clear currentPiece
						for (int y = 0; y < F_HEIGHT+2; y++) {
							for (int x = 0; x < F_WIDTH; x++) {
								currentPiece->Set(x,y,0);
							}
						}
						//copy rotatedPiece back to currentPiece
						for (int y = 0; y < 4; y++) {
							for (int x = 0; x < 4; x++) {
								currentPiece->Set(cY+y-offsetYX[0],cX+x+offsetYX[1],rotatedPiece[y][x]);
							}
						}
						return Window::Modulo((state+rot),4);
					}
				}

				return state;
			}							
		}
	}

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			currentPiece->Set(cX+x,cY+y,rotatedPiece[x][y]);
		}
	}
			
	return Window::Modulo((state+rot),4);
}

//rotate CW or CCW
int Pieces::Rotate(int rot, int state, Board* field, Board* currentPiece)
{
	int cX, cY;

	//scan currentPiece for piece type
	for (int y = 0; y < F_HEIGHT+2; y++) {
		for (int x = 0; x < F_WIDTH; x++) {
			if (currentPiece->Get(x,y) > 7) { 
				//gget pivot position
				cY = y;
				cX = x;
				//rotate
				return Offset3(rot, state, field, currentPiece, cY, cX);
			} else if (currentPiece->Get(x,y) == 1) { // I
				//get first point
				cY = y;
				cX = x;
				//rotate
				return Offset4(rot, state, field, currentPiece, cY, cX);
			} else if (currentPiece->Get(x,y) == 2) { 
				return state;
			}
		}
	}
}

//copy currentPiece to field array
int Pieces::SetPiece(Board* field, Board* currentPiece)
{
	for (int y = 0; y < F_HEIGHT; y++) {
		for (int x = 0; x < F_WIDTH; x++) {
			if (field->Get(x,y) == 0) {
				field->Set(x,y,currentPiece->Get(x,y));
			}
		}
	}

	//clear currentPiece field
	for (int y = 0; y < F_HEIGHT+2; y++) {
		for (int x = 0; x < F_WIDTH; x++) {
			currentPiece->Set(x,y,0);
		}
	}

	return 0;
}

//drop current piece to bottom
void Pieces::HardDrop(Board* field, Board* currentPiece)
{
	bool isFalling = true;

	while(isFalling)
	{
		isFalling = Pieces::MovePiece(-1, 0, field, currentPiece);
	}

	Pieces::SetPiece(field, currentPiece);
}

Pieces::~Pieces()
{
}
