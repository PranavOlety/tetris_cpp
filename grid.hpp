#ifndef GRID_HPP
#define GRID_HPP
#include <vector>
#include <SFML/Graphics.hpp>

namespace tetris
{

    // So I learned about making things inline, which allowed me to define the tetris global variables
    // ultimately, this allowed me to not worry about compiler warnings and when things expanded out
    // from when the definition was called. The other option I was considering was making these all
    // private variables within the GameBoard class. Storing them inside a function when required might
    // have also been interesting, but perhaps computationally unnecessary

    // this is a map of all the colors, the board keeps of track of the corresponding number
    // for color purposes, I think this was an interesting choice, perhaps I could have represented
    // it in the pieces, but that would require a rewrite of a few other functions (line_clearing, etc)
    inline std::unordered_map<int, sf::Color> colors = {
        {1, sf::Color::Yellow},
        {2, sf::Color::Cyan},
        {3, sf::Color::Green},
        {4, sf::Color::Red},
        {5, sf::Color::Magenta},
        {6, sf::Color(228, 138, 64)},
        {7, sf::Color::Blue}};

    // O
    inline std::vector<std::vector<int>> O_SHAPE = {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}};

    // I
    inline std::vector<std::vector<int>> I_SHAPE = {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}};

    // L
    inline std::vector<std::vector<int>> S_SHAPE = {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}};

    // Z
    inline std::vector<std::vector<int>> Z_SHAPE = {
        {0, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}};

    // T
    inline std::vector<std::vector<int>> T_SHAPE = {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}};

    // L
    inline std::vector<std::vector<int>> L_SHAPE = {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0}};

    // J
    inline std::vector<std::vector<int>> J_SHAPE = {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0}};

    // The shapes map allows me to quickly get the shape I want based on the number
    // I believe I would optimize this to be a struct of color and shape in the future
    // to couple state together
    inline std::unordered_map<int, std::vector<std::vector<int>>> shapes = {
        {1, O_SHAPE},
        {2, I_SHAPE},
        {3, S_SHAPE},
        {4, Z_SHAPE},
        {5, T_SHAPE},
        {6, L_SHAPE},
        {7, J_SHAPE}};

    // I thought typedef was cool, and convenient
    typedef std::vector<std::vector<int>> Grid;

    // This is the gameboard class
    class GameBoard
    {
    public:
        GameBoard();                        // default constructor
        GameBoard(int &height, int &width); // this is the usual constructor
        void generate_new_piece();          // a new piece is generate randomly
        bool in_bounds();                   // this checks if a piece is within the bounds of the game_board
        void shift_down();                  // this
        bool move_down();
        void rotate();        // this rotates a piece
        Grid &getGameState(); // this gets the actual game_state,
        // I should have kept it const, but it would be hard to simulate game_over working

        const int &getBlock(); // this is a const method that tracks the current piece/block falling, it is useful in keeping state
        int b_x;               // variable for falling piece's x pos
        int b_y;               // variable for falling piece's y pos

        // gets the height of the board
        const int &getHeight()
        {
            return m_height;
        }

        // gets the width of the board
        const int &getWidth()
        {
            return m_width;
        }

        // checks if the falling_piece has hit the pile
        bool has_hit_pile();

        // chekcs if this game is over
        bool is_game_over();

        // checks the score of the game, calculated when lines are cleared
        const int &get_score()
        {
            return score;
        }

        // returns the sum of all the lines cleared
        const int &lines_cleared_count()
        {
            return lines_cleared;
        }

        // gets the shape of the current_piece (a std::vector<std::vector<int>>)
        std::vector<std::vector<int>> get_current_shape()
        {
            return current_piece;
        }

    private:
        Grid grid;                                   // the game_board
        int m_height;                                // the game height
        int m_width;                                 // the game_width
        int block;                                   // k_value for piece for shape_gen and color_gen
        std::vector<std::vector<int>> current_piece; // the current_shape
        int score;                                   // the score
        int lines_cleared;                           // the lines
    };

}
#endif // GRID_HPP