// BlackWhiteChess.h: 標準系統 include 檔案的 include 檔案，
// 或專案專屬的 Include 檔案。

#pragma once

#include <iostream>
using std::cout;
using std::cin;
#include <iomanip>
#include <vector>
using std::vector;

using std::min;
using std::max;

// TODO: 在此參考您的程式所需的其他標頭。
class gameEngine {

	enum state {
		GAME_SETTING,
		WHITE_GO,
		BLACK_GO,
		GAME_END
	};

	enum {
		BLACK = -1,
		NO_CHESS = 0,
		WHITE = 1
	};

	enum {
		HAVE_CHESS,
		OUT_OF_BOUND,
		CANT_PLACE,
		VALID
	};

	enum {
		BLACK_NO_PLACE,
		WHITE_NO_PLACE,
		NO_PLACE,
		FINE
	};

	int state;

	int board[8 * 8];
public:
	gameEngine() {
		memset(board, 0, 8 * 8 * sizeof(int));

		board[3 * 8 + 3] = WHITE;
		board[3 * 8 + 4] = BLACK;
		board[4 * 8 + 3] = BLACK;
		board[4 * 8 + 4] = WHITE;

		state = GAME_SETTING;
	}

	void runGame()
	{
		if (state == GAME_SETTING)
			state = BLACK_GO;
		
		int x, y;
		vector<int> ate;
		int check;
		
		switch (state) {
			case BLACK_GO:
				printBoard();

				check = checkBoard(BLACK);

				if (check == NO_PLACE)
				{
					state = GAME_END;
					break;
				}
				else if (check != FINE)
				{
					state = WHITE_GO;
					cout << "Black no place\n";
					break;
				}
					

				cout << "black("<<BLACK<<") go:";
				cout << "\nx:";
			
				cin >> x;
				cout << "y:";
				cin >> y;

				if (checkValid(x * 8 + y) != VALID)
					break;

				ate = findAte(x * 8 + y, BLACK);

				if (ate.empty())break;
				
				board[x * 8 + y] = BLACK;

				for (auto i = ate.begin(); i != ate.end(); i++)
					board[*i] = BLACK;
				
				state = WHITE_GO;
				break;
			case WHITE_GO:
				printBoard();

				check = checkBoard(WHITE);

				if (check == NO_PLACE)
				{
					state = GAME_END;
					break;
				}
				else if (check != FINE)
				{
					state = BLACK_GO;
					cout << "White no place\n";
					break;
				}

				cout << "white(" << WHITE << ") go:";
				cout << "\nx:";
			
				cin >> x;
				cout << "y:";
				cin >> y;

				if (checkValid(x * 8 + y) != VALID)
					break;

				ate = findAte(x * 8 + y, WHITE);

				if (ate.empty())break;

				board[x * 8 + y] = WHITE;

				for (auto i = ate.begin(); i != ate.end(); i++)
					board[*i] = WHITE;

				state = BLACK_GO;
				break;
			case GAME_END:
				break;
		}
	}

	bool gameEnd()
	{
		return state == GAME_END;
	}

private:
	int checkValid(int pos)
	{
		int x = pos / 8;
		int y = pos % 8;

		if (x < 0 || x>7 || y < 0 || y>7)
			return OUT_OF_BOUND;

		if (board[x * 8 + y] != NO_CHESS)
			return HAVE_CHESS;

		int counter = 0;
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (max(0, min(7, i + x)) * 8 + max(0, min(7, j + y)) != NO_CHESS)//check around
					counter++;
			}
		}

		if (counter == 0)//no chess around
			return CANT_PLACE;

		return VALID;
	}

	vector<int> findAte(int pos, int type){
		int x = pos / 8;
		int y = pos % 8;

		vector<int> ate;

		if (y - 2 > 0)//left,at least two space to eat
		{
			int i = 1;
			while (y - i > 0 && board[x * 8 + y - i] == -type)
			{
				ate.push_back(x * 8 + y - i);
				i++;
			}
			if (i > 1 && board[x * 8 + y - i] != type)
				ate.erase(ate.end() - (i - 1), ate.end());
		}
		if (y + 2 < 8)//right
		{
			int i = 1;
			while (y + i < 8 && board[x * 8 + y + i] == -type)
			{
				ate.push_back(x * 8 + y + i);
				i++;
			}
			if (i > 1 && board[x * 8 + y + i] != type)
				ate.erase(ate.end() - (i - 1), ate.end());
		}
		if (x - 2 > 0)//up
		{
			int i = 1;
			while (x - i > 0 && board[(x - i) * 8 + y] == -type)
			{
				ate.push_back((x - i) * 8 + y);
				i++;
			}
			if (i > 1 && board[(x - i) * 8 + y] != type)
				ate.erase(ate.end() - (i - 1), ate.end());
		}
		if (x + 2 < 8)//down
		{
			int i = 1;
			while (x + i < 8 && board[(x + i) * 8 + y] == -type)
			{
				ate.push_back((x + i) * 8 + y);
				i++;
			}
			if (i > 1 && board[(x + i) * 8 + y] != type)
				ate.erase(ate.end() - (i - 1), ate.end());
		}

		// diagonals
		if (x - 2 > 0 && y - 2 > 0)//left-up
		{
			int i = 1;
			while( x - i > 0 && y - i > 0 && board[(x - i) * 8 + (y - i)] == -type)
			{
				ate.push_back((x - i) * 8 + (y - i));
				i++;
			}
			if (i > 1 && board[(x - i) * 8 + (y - i)] != type)
				ate.erase(ate.end() - (i - 1), ate.end());
		}
		if (x - 2 > 0 && y + 2 < 8)//right-up
		{
			int i = 1;
			while (x - i > 0 && y + i < 8 && board[(x - i) * 8 + (y + i)] == -type)
			{
				ate.push_back((x - i) * 8 + (y + i));
				i++;
			}
			if (i > 1 && board[(x - i) * 8 + (y + i)] != type)
				ate.erase(ate.end() - (i - 1), ate.end());
		}
		if (x + 2 < 8 && y + 2 < 8)//right-down
		{
			int i = 1;
			while (x + i < 8 && y + i < 8 && board[(x + i) * 8 + (y + i)] == -type)
			{
				ate.push_back((x + i) * 8 + (y + i));
				i++;
			}
			if (i > 1 && board[(x + i) * 8 + (y + i)] != type)
				ate.erase(ate.end() - (i - 1), ate.end());
		}
		if (x + 2 < 8 && y - 2 > 0)//left-down
		{
			int i = 1;
			while (x + i < 8 && y - i > 0 && board[(x + i) * 8 + (y - i)] == -type)
			{
				ate.push_back((x + i) * 8 + (y - i));
				i++;
			}
			if (i > 1 && board[(x + i) * 8 + (y - i)] != type)
				ate.erase(ate.end() - (i - 1), ate.end());
		}
		return ate;
	}

	int checkBoard(int type) 
	{
		int counter = 0;
		for (int i = 0; i < 8 * 8; i++)
			if (board[i] == NO_CHESS)
				counter++;
		if (counter == 0)
			return NO_PLACE;

		vector<int>placeAble;

		for (int i = 0; i < 8 * 8; i++)
		{
			if (board[i] == NO_CHESS && !findAte(i, type).empty())
				placeAble.push_back(i);
		}

		if(placeAble.empty())
			switch (type)
			{
			case WHITE:
				return WHITE_NO_PLACE;
				break;
			case BLACK:
				return BLACK_NO_PLACE;
				break;
			}

		return FINE;
	}
public:
	void printBoard()
	{
		cout << std::setw(4) << " ";
		for (int i = 0; i < 8; i++)
		{
			cout << std::setw(3) << i;
		}
		cout << "\n";
		cout << std::setw(4) << " ";
		for (int i = 0; i < 8; i++)
		{
			cout << std::setw(3) <<"==";
		}
		cout << "\n";
		for (int i = 0; i < 8; i++)
		{
			cout << std::setw(3) << i<<"|";
			for (int j = 0; j < 8; j++)
			{
				cout <<std::setw(3)<< board[i * 8 + j];
			}
			cout << "\n";
		}
	}

};