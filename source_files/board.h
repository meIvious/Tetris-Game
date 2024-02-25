#ifndef BOARD_H
#define BOARD_H

class Board : public Window
{
	public:
		Board();
		Board(int type);
		~Board();

		int Get(int x, int y);
		void Set(int x, int y, int val);
		
		static int* GenerateBag();
		static int ClearLine(Board* field, bool pc);
		
	protected:
		int** boardData;
		int maxY;
		int maxX;
};

#endif