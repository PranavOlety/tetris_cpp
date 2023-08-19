// Simple and Fast Multimedia Library
#include <SFML/Graphics.hpp>
#include "grid.hpp"
#include <iostream>

// Define world parameters
const int CellSize = 20;
const int borderSize = 2;
// Define world

namespace
{

    void createWindowAndGame(sf::RenderWindow &window, tetris::GameBoard &game)
    {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

        // Define the ASCII art for each letter as vector of strings
        std::vector<std::string> tetrisArt = {
            " _______ ______ _______ _____  _____  _____  ",
            " |__   __|  ____|__   __|  __ \\_   _|/ ____| ",
            "    | |  | |__     | |  | |__) | | | | (___   ",
            "    | |  |  __|    | |  |  _  /  | |  \\___ \\  ",
            "    | |  | |____   | |  | | \\ \\ _| |_ ____) | ",
            "    |_|  |______|  |_|  |_|  \\_\\_____|_____/  "};

        // Print each line of ASCII art
        for (const std::string &line : tetrisArt)
        {
            std::cout << line << std::endl;
        }
        std::cout << "Welcome to tetris_cpp!" << std::endl;
        std::cout << "Please select your difficulty" << std::endl;

        bool valid_conditions = false;

        std::string game_type;

        int height;
        int width;

        do
        {
            std::cout << "Enter game type (E for easy, M for medium, H for hard): ";
            std::cin >> game_type;

            if (game_type.size() >= 1)
            {
                char game_choice = tolower(game_type[0]);

                switch (game_choice)
                {
                case 'e':
                    window.create(sf::VideoMode(15 * CellSize, 25 * CellSize), "Tetris");

                    width = 15;
                    height = 25;
                    valid_conditions = true;
                    break;
                case 'm':
                    width = 10;
                    height = 20;
                    valid_conditions = true;
                    break;
                case 'h':
                    window.create(sf::VideoMode(7 * CellSize, 15 * CellSize), "Tetris");
                    width = 7;
                    height = 15;
                    valid_conditions = true;
                    break;
                default:
                    std::cout << "Invalid game type. Please enter E, M, or H.\n";
                    // Introduce a delay to avoid continuous looping and reduce lag
                }
            }
            else
            {
                std::cout << "Invalid input. Please enter a valid game type.\n";
                // Introduce a delay to avoid continuous looping and reduce lag
            }
        } while (!valid_conditions);

        window.create(sf::VideoMode(width * CellSize, height * CellSize), "Tetris");
        game = tetris::GameBoard(height, width);

        int windowPosX = (desktop.width - width * CellSize) / 2;
        int windowPosY = (desktop.height - height * CellSize) / 2;
        window.setPosition(sf::Vector2i(windowPosX, windowPosY));
    }

    void drawCellWithBorder(sf::RenderWindow &window, int x, int y, const sf::Color &color)
    {
        sf::RectangleShape cell(sf::Vector2f(CellSize, CellSize));
        cell.setFillColor(color);
        cell.setPosition(sf::Vector2f(x * CellSize, y * CellSize));
        window.draw(cell);

        // Draw the border
        sf::RectangleShape borderRect;
        borderRect.setSize(sf::Vector2f(CellSize, CellSize));
        borderRect.setFillColor(sf::Color::Black); // Border color
        borderRect.setPosition(x * CellSize, y * CellSize);

        // Draw additional rectangles for the border (top, left, right, bottom)
        borderRect.setSize(sf::Vector2f(CellSize, 2));
        window.draw(borderRect);
        borderRect.setSize(sf::Vector2f(borderSize, CellSize + 2 * 2));
        window.draw(borderRect);
        borderRect.setPosition((x + 1) * CellSize - borderSize, y * CellSize - borderSize);
        window.draw(borderRect);
        borderRect.setSize(sf::Vector2f(CellSize, borderSize));
        borderRect.setPosition(x * CellSize, (y + 1) * CellSize - borderSize);
        window.draw(borderRect);
    }

}

int main()
{

    // a window that can render 2D drawings
    sf::RenderWindow window;

    // draw a cell in the world
    sf::RectangleShape cell(sf::Vector2f(CellSize, CellSize));

    tetris::GameBoard game;
    // populate a block

    createWindowAndGame(window, game);

    game.generate_new_piece();

    bool gameOver = false;
    sf::Clock clock;

    while (window.isOpen() && !gameOver)
    {

        // start clock
        static float prev = clock.getElapsedTime().asSeconds();
        if (clock.getElapsedTime().asSeconds() - prev >= 0.5)
        {
            prev = clock.getElapsedTime().asSeconds();
            game.move_down();
        }

        // Define system event
        sf::Event e;

        // polling event (eg. key pressed)
        while (window.pollEvent(e))
        {
            // close window
            if (e.type == sf::Event::Closed)
                window.close();

            // keyboard interrupt
            if (e.type == sf::Event::KeyReleased)
            {
                if (e.key.code == sf::Keyboard::Left)
                {
                    --game.b_x;
                    if (!game.in_bounds())
                        ++game.b_x;
                }
                else if (e.key.code == sf::Keyboard::Right)
                {
                    ++game.b_x;
                    if (game.in_bounds() == false)
                        --game.b_x;
                }
                else if (e.key.code == sf::Keyboard::Down)
                {
                    game.move_down();
                }
                else if (e.key.code == sf::Keyboard::Space)
                {
                    // fall down until reaches the bottom
                    while (game.move_down() == true)
                        ;
                }
                else if (e.key.code == sf::Keyboard::Up)
                {
                    game.rotate();

                    // if rotation hits boundary, do not allow to rotate
                    if (!game.in_bounds())
                    {
                        game.rotate();
                        game.rotate();
                        game.rotate();
                    }
                }
            }
        }

        // clear window every frame
        window.clear();
        for (int y = 0; y < game.getHeight(); ++y)
        {
            for (int x = 0; x < game.getWidth(); ++x)
            {
                int cellValue = game.getGameState()[y][x];
                if (cellValue)
                {
                    drawCellWithBorder(window, x, y, tetris::colors.at(cellValue));
                }
            }
        }

        for (int y = 0; y < 4; ++y)
        {
            for (int x = 0; x < 4; ++x)
            {
                if (game.get_current_shape()[y][x])
                {
                    int drawX = game.b_x + x;
                    int drawY = game.b_y + y;
                    drawCellWithBorder(window, drawX, drawY, tetris::colors.at(game.getBlock()));
                }
            }
        }
        // display rendered object on screen
        window.display();

        gameOver = game.is_game_over();
    }

    std::cout << "Your Score was " << game.get_score() << std::endl;
    std::cout << "You cleared " << game.lines_cleared_count() << " line(s)" << std::endl;

    return 0;
}