// BlackWhiteChess.h: 標準系統 include 檔案的 include 檔案，
// 或專案專屬的 Include 檔案。

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
using std::cout;
using std::cin;
#include <iomanip>
#include <vector>
using std::vector;

using std::min;
using std::max;
// TODO: 在此參考您的程式所需的其他標頭。
class gameEngine {

	sf::SoundBuffer buffer;
	
	sf::Sound sound;

	enum validdation{
		HAVE_CHESS,
		OUT_OF_BOUND,
		CANT_PLACE,
		VALID
	};

	enum check{
		BLACK_NO_PLACE,
		WHITE_NO_PLACE,
		NO_PLACE,
		FINE
	};

public:
	
	enum state {
		GAME_SETTING,
		WHITE_GO,
		BLACK_GO,
		GAME_END
	};

	int state;


	enum symble{
		BLACK = -1,
		NO_CHESS = 0,
		WHITE = 1,
		CAN_GO//not differ from black or white
	};


	int board[8 * 8];

	gameEngine() {
		memset(board, 0, 8 * 8 * sizeof(int));

		board[3 * 8 + 3] = WHITE;
		board[3 * 8 + 4] = BLACK;
		board[4 * 8 + 3] = BLACK;
		board[4 * 8 + 4] = WHITE;

		state = GAME_SETTING;
		runGame(-1, -1);

		buffer.loadFromFile("audio/sound.wav");
		sound.setBuffer(buffer);
	}

	void runGame(int x,int y)//col,row
	{
		if (state == GAME_SETTING)
			state = BLACK_GO;

		vector<int> ate;
		int check;

		switch (state) {
		case BLACK_GO:
			//printBoard();
			check = checkBoard(BLACK);

			if (checkValid(x * 8 + y) != VALID)
				break;

			ate = findAte(x * 8 + y, BLACK);

			if (ate.empty())break;

			board[x * 8 + y] = BLACK;

			for (auto i = ate.begin(); i != ate.end(); i++)
				board[*i] = BLACK;

			sound.play();

			state = WHITE_GO;

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
				check = checkBoard(BLACK);
				break;
			}

			break;
		case WHITE_GO:
			//printBoard();

			check = checkBoard(WHITE);

			if (checkValid(x * 8 + y) != VALID)
				break;

			ate = findAte(x * 8 + y, WHITE);

			if (ate.empty())break;

			board[x * 8 + y] = WHITE;

			for (auto i = ate.begin(); i != ate.end(); i++)
				board[*i] = WHITE;

			sound.play();

			state = BLACK_GO;

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
				check = checkBoard(WHITE);
				break;
			}
			break;
		case GAME_END:
			break;
		}
	}

	bool gameEnd()
	{
		return state == GAME_END;
	}

public:
	int checkValid(int pos)
	{
		int x = pos / 8;
		int y = pos % 8;

		if (x < 0 || x>7 || y < 0 || y>7)
			return OUT_OF_BOUND;

		if (board[x * 8 + y] == WHITE|| board[x * 8 + y] == BLACK)
			return HAVE_CHESS;

		int counter = 0;
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				int tx = max(0, min(7, i + x));
				int ty = max(0, min(7, j + y));

				if (board[tx * 8 + ty] == WHITE|| board[tx * 8 + ty]== BLACK)//check around
					counter++;
			}
		}

		if (counter == 0)//no chess around
			return CANT_PLACE;

		return VALID;
	}

	vector<int> findAte(int pos, int type) {
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
			while (x - i > 0 && y - i > 0 && board[(x - i) * 8 + (y - i)] == -type)
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
			if (board[i] == NO_CHESS || board[i] == CAN_GO)
			{
				counter++;
				board[i] = NO_CHESS;//clear up
			}
		if (counter == 0)
			return NO_PLACE;

		vector<int>placeAble;

		for (int i = 0; i < 8 * 8; i++)
		{
			if (board[i] == NO_CHESS && !findAte(i, type).empty())
				placeAble.push_back(i);
		}
		//add placeable info to board
		for (auto i = placeAble.begin(); i != placeAble.end(); i++)
			board[*i] = CAN_GO;

		if (placeAble.empty())//maybe end
		{
			vector<int>placeAble;

			for (int i = 0; i < 8 * 8; i++)
			{
				if (board[i] == NO_CHESS && !findAte(i, -type).empty())
					placeAble.push_back(i);
			}
			if (placeAble.empty())
				return NO_PLACE;//end

			switch (type)
			{
			case WHITE:
				return WHITE_NO_PLACE;
				break;
			case BLACK:
				return BLACK_NO_PLACE;
				break;
			}
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
			cout << std::setw(3) << "==";
		}
		cout << "\n";
		for (int i = 0; i < 8; i++)
		{
			cout << std::setw(3) << i << "|";
			for (int j = 0; j < 8; j++)
			{
				cout << std::setw(3) << board[i * 8 + j];
			}
			cout << "\n";
		}
	}

	int getWinner()
	{
		int b = 0,w = 0;
		for (int i = 0; i < 64 ; i++)
		{
			if (board[i] == WHITE)
				w++;

			if (board[i] == BLACK)
				b++;
		}

		cout << "white:"<<w<<"\n";
		cout << "black:" << b << "\n";

		if (w == b)
		{
			cout << "draw\n";
			return NO_CHESS;//draw

		}

		if (b > w)
		{
			cout << "black win!\n";
			return BLACK;
		}

		cout << "white win!\n";
		return WHITE;
	}

};



class AI {
public:
	gameEngine* game;
	int type;
	AI(gameEngine* game, int type) :game(game), type(type) {};

	void go() {
		sf::Clock clock;
		clock.restart();

		vector<int>can_go;
		for (int i = 0; i < 64; i++)
		{
			if (game->board[i] == game->CAN_GO)
				can_go.push_back(i);
		}

		if (can_go.empty())
			return;

		int go;
		int max_eat = 0;
		for (auto i = can_go.begin(); i != can_go.end(); i++)
		{
			int s = game->findAte(*i, type).size();
			if (s > max_eat)
			{
				max_eat = s;
				go = *i;
			}
		}
		
		while (clock.getElapsedTime() < sf::seconds(0.5));

		game->runGame(go / 8, go % 8);

	}

};