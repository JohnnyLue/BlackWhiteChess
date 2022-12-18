
#include <SFML/Graphics.hpp>

#include "GameEngine.h"

using namespace sf;

int main()
{
    do {
        gameEngine* game = new gameEngine();
        AI* ai = nullptr;
        RenderWindow window(VideoMode(297 * 2, 297 * 2), "The Game!", sf::Style::Titlebar | sf::Style::Close);

        
        Texture t1, t2, t3, t4;
        t1.loadFromFile("images/chessBoard.jpg");
        t2.loadFromFile("images/chess2.jpg");
        t3.loadFromFile("images/flip2.jpg");
        t4.loadFromFile("images/winner2.jpg");
        Sprite backGround(t1);
        backGround.setScale(Vector2f(2.0, 2.0));

        Sprite c(t2);
        c.setScale(Vector2f(2.0, 2.0));
        Sprite ani(t3);
        ani.setScale(Vector2f(2.0, 2.0));

        int choose = -1;

        window.setVisible(false);

        while (choose != 1 && choose != 2)
        {
            std::string t;
            cout << "Choose mode (1)AI or (2)two players:";
            cin >> t;
            choose = atoi(t.c_str());
            system("CLS");
        }

        if (choose == 1)
        {
            choose = -1;
            while (choose != 1 && choose != 2)
            {
                std::string t;
                cout << "you want to be 1(black) or 2(white):";
                cin >> t;
                choose = atoi(t.c_str());
                system("CLS");
            }
            int type;
            if (choose == 1)
                type = game->WHITE;
            else
                type = game->BLACK;

            ai = new AI(game, type);
        }

        int oldBoard[8 * 8];
        int firstIn = true;

        window.setVisible(true);

        while (!game->gameEnd())
        {
            if (ai)
            {
                if (game->state == game->BLACK_GO && ai->type == game->BLACK)
                {
                    ai->go();
                }

                if (game->state == game->WHITE_GO && ai->type == game->WHITE)
                {
                    ai->go();
                }
            }

            Event e;
            while (window.pollEvent(e))
            {

                if (e.type == Event::Closed)
                    window.close();

                if (e.type == Event::KeyPressed)
                    if (e.key.code == Keyboard::Q)
                    {
                        game->state = game->state::GAME_END;
                    }

                if (e.type == Event::MouseButtonPressed)
                {
                    int x = e.touch.x, y = e.touch.y;

                    c.setTextureRect(IntRect(33, 0, 33, 33));

                    x -= 40, y -= 40;
                    x = floor(x / 64);
                    y = floor(y / 64);

                    //c.setPosition(x, y);
                    game->runGame(y, x);

                }

            }

            if (firstIn)
            {
                memcpy(oldBoard, game->board, 64 * sizeof(int));
                firstIn = false;
            }

            Time t;
            Clock clock;

            for (int f = 0; f < 7; f++)
            {
                while (t.asSeconds() < 0.05)
                {
                    t += clock.restart();
                }
                t = t.Zero;
                window.draw(backGround);

                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        ani.setPosition(40 + j * 64, 40 + i * 64);
                        c.setPosition(40 + j * 64, 40 + i * 64);
                        if (game->board[i * 8 + j] == game->WHITE)
                        {
                            if (oldBoard[i * 8 + j] == game->BLACK)//ate anime
                            {
                                ani.setTextureRect(IntRect(33 * f, 33, 33, 33));
                                window.draw(ani);
                            }
                            else
                            {
                                c.setTextureRect(IntRect(0, 0, 33, 33));
                                window.draw(c);
                            }

                        }
                        else if (game->board[i * 8 + j] == game->BLACK)
                        {
                            if (oldBoard[i * 8 + j] == game->WHITE)//ate anime
                            {
                                ani.setTextureRect(IntRect(33 * f, 0, 33, 33));
                                window.draw(ani);
                            }
                            else
                            {
                                c.setTextureRect(IntRect(33, 0, 33, 33));
                                window.draw(c);
                            }
                        }
                        else if (game->board[i * 8 + j] == game->CAN_GO)
                        {
                            if (game->state == game->WHITE_GO)
                                c.setTextureRect(IntRect(66, 0, 33, 33));

                            if (game->state == game->BLACK_GO)
                                c.setTextureRect(IntRect(99, 0, 33, 33));

                            window.draw(c);
                        }
                    }
                }
                window.display();
            }

            memcpy(oldBoard, game->board, 64 * sizeof(int));
        }

        Clock clock0;
        while (clock0.getElapsedTime() < sf::seconds(1));

        Sprite win(t4);
        win.setOrigin(16, 16);
        win.setPosition(40 + 64 * 4, 40 + 64 * 4);
        win.scale(10, 10);
        int winner = game->getWinner();
        if (winner == game->WHITE)
        {
            win.setTextureRect(IntRect(0, 0, 33, 33));
            window.draw(win);
        }
        if (winner == game->BLACK)
        {
            win.setTextureRect(IntRect(33, 0, 33, 33));

            window.draw(win);
        }
        if (winner == game->NO_CHESS)
        {
            win.setTextureRect(IntRect(66, 0, 33, 33));

            window.draw(win);
        }
        window.display();

        Clock clock;
        while (clock.getElapsedTime() < sf::seconds(2.5));

        window.setVisible(false);

        std::string a;
        char firstC='0';
        while (firstC != 'Q' && firstC != 'q' && firstC != 'Y' && firstC != 'y')
        {
            cout << "replay?(y or q):";
            cin >> a;
            if(!a.empty())
                firstC = a.at(0);
            system("CLS");
        }
        if (firstC == 'q' || firstC == 'Q')
            break;

    } while (1);

    cout << "Thanks for playing.\n";

    return 0;
}