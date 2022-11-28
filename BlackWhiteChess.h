// BlackWhiteChess.h: 標準系統 include 檔案的 include 檔案，
// 或專案專屬的 Include 檔案。

#pragma once

#include <iostream>
using std::cout;
using std::cin;
#include <iomanip>

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

	void startGame()
	{
		if (state == GAME_SETTING)
			state = BLACK_GO;
		
		int x, y;

		switch (state) {
		case BLACK_GO:
			printBoard();
			cout << "black go:";
			cout << "\nx:";
			
			cin >> x;
			cout << "\ny:";
			cin >> y;

			board[x * 8 + y] = BLACK;
			state = WHITE_GO;
			break;
		case WHITE_GO:
			printBoard();
			cout << "white go:";
			cout << "\nx:";
			
			cin >> x;
			cout << "\ny:";
			cin >> y;

			board[x * 8 + y] = WHITE;
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