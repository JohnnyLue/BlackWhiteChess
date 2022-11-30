// BlackWhiteChess.cpp: 定義應用程式的進入點。
//

#include "BlackWhiteChess.h"

using namespace std;

int main()
{
	gameEngine* game = new gameEngine();
	
	while (!game->gameEnd())
		game->runGame();

	return 0;
}
