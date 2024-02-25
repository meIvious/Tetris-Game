#include "config.h"
#include "window.h"
#include "board.h"
#include "pieces.h"
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <iostream>

using namespace std;


struct itimerval tv;   //timer


bool isGameOver;
bool isPieceActive;
bool pcFlag;
int rotState;
int holding;


void Quit(int score, struct termios oldAttr)
{
	Window::ShowCursor(true);
	tcsetattr(0, TCSANOW, &oldAttr);
	cout << endl << "   Game over! Your score is : " << score << endl << endl;
	return;

}


//holds the current piece and swaps the current piece with the held piece
int Hold(int holding, Board* field, Board* currentPiece)
{	
	//find active piece ID and wipe currentPiece
	int active;
	for (int y = 0; y < F_HEIGHT+2; y++) {
		for (int x = 0; x < F_WIDTH; x++) {
			//get ID value (not pivot)
			if (currentPiece->Get(x,y) != 0) {
				if (currentPiece->Get(x,y) > 7) {
					active = currentPiece->Get(x,y)-5;
				} else {
					active = currentPiece->Get(x,y);
				}
			}
			//delete cell
			currentPiece->Set(x,y,0);
		}
	}

	//check for existing piece in hold
	if (holding == 9) {
		//no piece in hold, get new piece
		isPieceActive = false;
	} else {
		//existing piece in hold, get held piece
		Pieces::GetNewPiece(holding-1, field, currentPiece);
	}
	rotState = 0;
	pcFlag = false;
	return active;
}


int GetKeyInput(Board* field, Board* currentPiece)
{
	int keyPress = getchar();

	switch(keyPress)
	{
		case KEY_LEFT: {
			Pieces::MovePiece(0,1, field, currentPiece); break;
			}
		case KEY_RIGHT: {
			Pieces::MovePiece(0,-1, field, currentPiece); break;
			}
		case KEY_DOWN: {
			Pieces::MovePiece(-1,0, field, currentPiece); break;
			}
		case KEY_UP:
		case KEY_ROTATE_CW: {
			rotState = Pieces::Rotate(1, rotState, field, currentPiece); break;
			}
		case KEY_ROTATE_CCW: {
			rotState = Pieces::Rotate(-1, rotState, field, currentPiece); break;
			}
		case KEY_ROTATE_180: {
			rotState = Pieces::Rotate(1, rotState, field, currentPiece);
			rotState = Pieces::Rotate(1, rotState, field, currentPiece);
			break;
		}
		case KEY_HARD_DROP: {
			Pieces::HardDrop(field, currentPiece); break;
			}
		case KEY_HOLD: {
			holding = Hold(holding, field, currentPiece); break;
			}
		case KEY_EXIT: {
			isGameOver = true; break;
			}
	}

	return keyPress;

}




void Update(Board* field, Board* currentPiece, int* bag1, int* bag2, int order, int level, int score, int lines, int combo)
{
    Window::Format(0);
    cout << endl;

    for (int y = 0; y < F_HEIGHT+2; y++) {
        Window::Format(0);
        cout << "  ";

        for (int x = 0; x < F_WIDTH+8; x++) {
           //defult
            Window::Format(0);
            if (x == 0 || x == F_WIDTH+1) {
                //render gamefield 
                Window::Format(0);
                cout << "||";
            } 
			else if ((bag1[order] < 2) && (x == F_WIDTH+2 && y >= 2 && y < 4)) {
                //center preview 
                Window::Format(0);
                cout << "   ";
            } 
			else if ((bag1[order] >= 2) && (x == F_WIDTH+2 && y >= 2 && y < 4)) {
                //center preview 
                Window::Format(0);
                cout << "    ";
            } 
			else if ((holding-1 < 2) && (x == F_WIDTH+2 && y >= 7 && y < 9)) {
                //center hold 
                Window::Format(0);
                cout << "   ";
            } 
			else if ((holding-1 >= 2) && (x == F_WIDTH+2 && y >= 7 && y < 9)) {
                //center hold 
                Window::Format(0);
                cout << "    ";
            } 
			else if (y == 12 && x == F_WIDTH+3) {

            } 
			else if (y == 13 && x == F_WIDTH+3) {
                switch (lines) {
                    case 8: {cout << " CLEAR "; break;}
                }
            }
			else if (y == 15 && x == F_WIDTH+3) {
                if (combo > 1)
                    cout << " x" << combo;
            } 
			else if (y == 17 && x == F_WIDTH+3) {
                cout << "LEVEL: " << level;
            } 
			else if (y == 19 && x == F_WIDTH+3) {
                cout << "SCORE: "<< score ;
            } 
			else if (y == 20 && x == F_WIDTH+3) {
                cout << " ";
            } 
			else if ((y == 0 && x < F_WIDTH) || x == F_WIDTH+2) {
                //rendering field top and vertical 
                Window::Format(0);
                cout << "  ";
            } 
			else if ((y == F_HEIGHT+1 && x < F_WIDTH+1) ||
                       (y == 0 && x > F_WIDTH+2) ||
                       (y == F_HEIGHT-15 && x > F_WIDTH+2) ) {
                //rendering field bottom and horizontal 
                Window::Format(0);
                cout << "==";
            } 
			else if (x >= F_WIDTH+3 && x < F_WIDTH+7 && y >= 2 && y < 4) {
                //render preview
                Pieces::GetPieceColor(shapes[bag1[order]][y-2][x-(F_WIDTH+3)]);
                cout << "  ";
            } 
			else if (x >= F_WIDTH+3 && x < F_WIDTH+7 && y >= 7 && y < 9 && holding != 9) {
                //render hold
                Pieces::GetPieceColor(shapes[holding-1][y-7][x-(F_WIDTH+3)]);
                cout << "  ";
            } 
			else if (currentPiece->Get(x-1,y-1) != 0) {
                //render current piece
                Pieces::GetPieceColor(currentPiece->Get(x-1,y-1));
                cout << "  ";
            } 
			else {
                //render field
                Pieces::GetPieceColor(field->Get(x-1,y-1));
                cout << "  ";
            }
        }
        Window::Format(0);
        cout << endl;
    }
}




void SignalHandler(int signal){  //timer icin
	switch(signal) {
		case SIGTERM:
		case SIGINT:
		case SIGSEGV:
			isGameOver = true;
			break;
		case SIGALRM:
			//increase game speed by 0.1% every tick
			tv.it_value.tv_usec -= tv.it_value.tv_usec / 10000;
			setitimer(0, &tv, NULL);
			break;
	}
}

int main(int argc, char* argv[]){
	//initialise signal handler
	struct sigaction siga;

	sigemptyset(&siga.sa_mask);

	siga.sa_flags = 0;
	siga.sa_handler = SignalHandler;

	sigaction(SIGALRM, &siga, NULL);
	sigaction(SIGTERM, &siga, NULL);
	sigaction(SIGINT,  &siga, NULL);
	sigaction(SIGSEGV, &siga, NULL);

	
	tv.it_value.tv_usec = FRAME_USEC;  //timer signalhandler
	SignalHandler(SIGALRM);

	//initialise terminal attributes
	struct termios oldAttr;
	struct termios newAttr;

	tcgetattr(STDIN_FILENO, &oldAttr);
	newAttr = oldAttr;
	newAttr.c_lflag &= ~(ICANON|ECHO);
	newAttr.c_cc[VTIME] = 0;
	newAttr.c_cc[VMIN] = 0;
	tcsetattr(0, TCSANOW, &newAttr);
	
	Window::ShowCursor(false);

	
	srand(time(NULL));

	//initialise board
	Board* field = new Board(0);
	Board* currentPiece = new Board(1);
	
	//initialise game state
	int debugTick = 0, moveTick = 0, holdTick = 0, lockTick = 0, textDelay = 0;
	int score = 0, level = 1, currentLines = 0, linesCleared = 0, lastClear = 0, combo = 0;
	int bagID = 0;
	int *bag1 = Board::GenerateBag();
	int *bag2 = Board::GenerateBag();
	holding = 9;
	pcFlag = false;

	//Clear terminal output
	cout << "\033[2J";  //system ("clear");

	//game loop
	while (!isGameOver)
	{
		//game tick (1000000 = 1s)
		debugTick++;
		moveTick++;
		textDelay++;
		//Game logic
		//refresh game states
		isPieceActive = false;
		
		for (int y = 0; y < F_HEIGHT+2; y++) {
			for (int x = 0; x < F_WIDTH; x++) {
				if (currentPiece->Get(x,y) != 0) {
					isPieceActive = true;
				}
			}
		}

		//create new piece if no piece is active
		if (!isPieceActive && bagID < 7) {
			//game over if no piece can spawn
			isGameOver = Pieces::GetNewPiece(bag1[bagID], field, currentPiece);
			isPieceActive = true;
			rotState = 0;
			
			//clear lines and calculate the score
			linesCleared = Board::ClearLine(field, pcFlag);
	
			pcFlag = true;
			currentLines += linesCleared;
			lastClear = linesCleared;
			
			switch (linesCleared) {
				case 1: {
					score += 80*(level+combo); break;
					} 
				case 2: {
					score += 200*(level+combo); break;
					} 
				case 3: {
					score += 600*(level+combo); break;
					} 
				case 4: {
					score += 2400*(level+combo); break;
					} 
				case 8: {score += 7600*(level+combo); break;
				} 
			}
			if (lastClear > 0 && linesCleared > 0){
				combo++;
			} 
			else {
				combo = 0;
			}
			if (currentLines > 20) {
				level++;
				//speed up %5
				tv.it_value.tv_usec -= tv.it_value.tv_usec / 20;
				currentLines = 0;
			}
			//next piece
			bagID++;
		}

		//generate new bag on end of bag sequence
		if (bagID == 7) {
			//swap bag
			delete bag1;
			int *bag1 = new int[7];
			for (int i = 0; i < 7; i++)
				bag1[i] = bag2[i];
			//get new bag
			delete bag2;
			int *bag2 = Board::GenerateBag();
			bagID = 0;
		}

		//update
		cout << "\033[H";  //system(clear);
		Update(field, currentPiece, bag1, bag2, bagID, level, score, lastClear, combo);

	
		int key = GetKeyInput(field, currentPiece);

		//gravity
		if (moveTick == 50) {
			(Pieces::MovePiece(-1,0, field, currentPiece)) ? lockTick = 0 : lockTick++;
			if (lockTick == 2)
				Pieces::SetPiece(field, currentPiece);
			moveTick = 0;
		}

		//clear line  text timeout
		if (textDelay == 250) {
			lastClear = 0;
			textDelay = 0;
		}

	
		if (argv[1]) { //argument control

			for (int y = 0; y < F_HEIGHT; y++) {
				cout << "   ";
				//field values
				for (int x = 0; x < F_WIDTH; x++) {
					if (field->Get(x,y) > 7) {
						cout << field->Get(x,y)-5;
					} else {
						cout << field->Get(x,y);
					}
				}
				cout << "  ";
				//currentPiece values
				for (int x = 0; x < F_WIDTH; x++) {
					if (currentPiece->Get(x,y) > 7) {
						cout << currentPiece->Get(x,y)-5;
					} else {
						cout << currentPiece->Get(x,y);
					}
				}
				cout << endl;
			}
		} else {
		
			cout << "   <- Move left  |  -> Move right" << endl;
			cout << "   Z Rotate CCW  |  X Rotate CW" << endl;
			cout << "   A Rotate      |  C Hold" << endl;
			cout << "           Q Quit game" << endl << endl;

		}

	}

	delete bag1;
	delete bag2;

	delete field;
	delete currentPiece;

	Quit(score, oldAttr);

	return 0;

}
