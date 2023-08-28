#include "unit_test_framework.h"
#include "grid.hpp"
using std::operator""s;

TEST(TestGameBoardConstructor)
{
    int height = std::rand() % 25 + 5;
    int width = std::rand() % 25 + 5;
    tetris::GameBoard board(height, width);

    ASSERT_EQUAL(board.getHeight(), height);
    ASSERT_EQUAL(board.getWidth(), width);
    ASSERT_EQUAL(board.get_score(), 0);           // Using score variable directly
    ASSERT_EQUAL(board.lines_cleared_count(), 0); // Using lines_cleared variable directly
}

TEST(TestGenerateNewPiece)
{
    int height = std::rand() % 25 + 5;
    int width = std::rand() % 25 + 5;
    tetris::GameBoard board(height, width);
    board.generate_new_piece();

    auto piece = board.getBlock();

    // Assert that the block type is within a valid range 1 to 7
    ASSERT_TRUE(piece >= 1 && piece <= 7);

    int b_x = board.b_x;
    int b_y = board.b_y;
    ASSERT_TRUE(b_x >= 0 && b_x < width);
    ASSERT_TRUE(b_y >= 0 && b_y < height);

    for (int y = 0; y < 4; ++y)
    {
        for (int x = 0; x < 4; ++x)
        {
            ASSERT_EQUAL(tetris::shapes.at(piece)[y][x], board.get_current_shape()[y][x]);
        }
    }
}

TEST(TestInBounds)
{
    int height = std::rand() % 25 + 5;
    int width = std::rand() % 40 + 5;
    tetris::GameBoard board(height, width);
    board.generate_new_piece();

    for (int b_y = 0; b_y < height - 3; ++b_y)
    {
        for (int b_x = 0; b_x < width - 3; ++b_x)
        {
            // Set the position of the current piece
            board.b_x = b_x;
            board.b_y = b_y;

            bool isWithinBounds = board.in_bounds();

            ASSERT_EQUAL(isWithinBounds, (b_x >= 0 && b_x + 3 < width) &&
                                             (b_y >= 0 && b_y + 3 < height));
        }
    }
}

TEST(TestHasHitPile)
{
    int height = std::rand() % 40 + 5;
    int width = std::rand() % 45 + 5;
    tetris::GameBoard board(height, width);
    board.generate_new_piece(); // Ensure there's a current piece to test

    // Iterate through different positions of the current piece
    for (int b_y = -4; b_y < height; ++b_y)
    {
        for (int b_x = -4; b_x < width; ++b_x)
        {
            // Set the position of the current piece
            board.b_x = b_x;
            board.b_y = b_y;

            // Check if the current piece has hit the pile using the has_hit_pile() function
            bool hasHitPile = board.has_hit_pile();

            // Calculate the expected result based on position
            bool expected = false;
            for (int y = 0; y < 4; ++y)
            {
                for (int x = 0; x < 4; ++x)
                {
                    if (board.get_current_shape()[y][x])
                    {
                        int grid_row = b_y + y;
                        int grid_col = b_x + x;
                        if (grid_row < 0 || grid_row >= height || grid_col < 0 || grid_col >= width)
                        {
                            expected = true;
                            break;
                        }
                        if (board.getGameState()[grid_row][grid_col])
                        {
                            expected = true;
                            break;
                        }
                    }
                }
                if (expected)
                {
                    break;
                }
            }

            // Result matches expected
            ASSERT_EQUAL(hasHitPile, expected);
        }
    }
}

TEST(TestShiftDown)
{
    int height = std::rand() % 25 + 5;
    int width = std::rand() % 40 + 5;
    tetris::GameBoard board(height, width);

    for (int y = height / 2 - 1; y <= height / 2; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            board.getGameState()[y][x] = 1;
        }
    }

    int initialNumLines = 0;
    for (int y = 0; y < height; ++y)
    {
        bool hasLine = true;
        for (int x = 0; x < width; ++x)
        {
            if (board.getGameState()[y][x] == 0)
            {
                hasLine = false;
                break;
            }
        }
        if (hasLine)
        {
            ++initialNumLines;
        }
    }

    board.shift_down();

    int finalNumLines = 0;
    for (int y = 0; y < height; ++y)
    {
        bool hasLine = true;
        for (int x = 0; x < width; ++x)
        {
            if (board.getGameState()[y][x] == 0)
            {
                hasLine = false;
                break;
            }
        }
        if (hasLine)
        {
            ++finalNumLines;
        }
    }
    ASSERT_EQUAL(finalNumLines, 0);
}

TEST(TestRotate)
{

    int height = std::rand() % 40 + 5;
    int width = std::rand() % 35 + 5;
    tetris::GameBoard board(height, width);

    // Set up the game board with a piece
    board.generate_new_piece();

    // Get the initial orientation of the piece

    // the O is always the same, so handling that case
    if (board.getBlock() == 1)
    {
        board.get_current_shape() = tetris::L_SHAPE;
    }
    auto initialPiece = board.get_current_shape();

    // Rotate the piece
    board.rotate();
    board.rotate();

    // Get the new orientation of the piece after rotation
    auto rotatedPiece = board.get_current_shape();

    // Lambda function to compare two shapes (vectors)
    auto shapes_equal = [](const std::vector<std::vector<int>> &shape1,
                           const std::vector<std::vector<int>> &shape2)
    {
        if (shape1.size() != shape2.size())
        {
            return false;
        }

        for (size_t i = 0; i < shape1.size(); ++i)
        {
            for (size_t j = 0; j < shape1[i].size(); ++j)
            {
                if (shape1[i][j] != shape2[i][j])
                {
                    return false;
                }
            }
        }

        return true;
    };

    // Check if the rotated piece is not the same
    ASSERT_FALSE(shapes_equal(initialPiece, rotatedPiece));

    board.rotate();
    board.rotate();
    // Rotate it back
    auto restoredPiece = board.get_current_shape();

    // Checking that the it is back to its original spot
    ASSERT_TRUE(shapes_equal(initialPiece, restoredPiece));
}

TEST(TestGetScore)
{
    int height = 20;
    int width = 10;
    tetris::GameBoard board(height, width);

    for (int x = 0; x < width; ++x)
    {
        board.getGameState()[width - 1][x] = 1;
        board.getGameState()[width - 2][x] = 1;
    }
    board.shift_down();
    // Get the score from the board
    int actualScore = board.get_score();
    ASSERT_EQUAL(actualScore, 400);
}

TEST(TestCheckLinesCleared)
{
    int height = 43;
    int width = 22;
    tetris::GameBoard board(height, width);

    for (int x = 0; x < width; ++x)
    {
        board.getGameState()[width - 1][x] = 1;
        board.getGameState()[width - 3][x] = 1;
    }

    board.shift_down();

    // Get the score from the board
    int linesCleared = board.lines_cleared_count();

    ASSERT_EQUAL(linesCleared, 2);
}

// Define main function to run tests
TEST_MAIN()