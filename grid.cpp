
#include <SFML/Graphics.hpp>
#include <numeric>
#include "grid.hpp"
#include <iostream>
#include <stdexcept>

namespace tetris
{

  // Constructors
  GameBoard::GameBoard() {}
  GameBoard::GameBoard(int &height, int &width) : grid(height, std::vector<int>(width, 0)),
                                                  m_height(height), m_width(width), score(0), lines_cleared(0)
  {
    try
    {
      if (height < 5 || height > 50 || width < 5 || width > 50)
      {
        throw std::exception();
      }
    }
    catch (const std::exception &e)
    {
      std::cerr << "Cannot build a board of " << width << " x " << height << " " << std::endl;
    }
  }

  void GameBoard::generate_new_piece()
  {
    std::srand(std::time(nullptr));
    block = std::rand() % 7 + 1;
    b_x = std::rand() % (m_width - 4);
    b_y = 0;

    current_piece = shapes[block];
  }
  bool GameBoard::in_bounds()
  {
    for (int y = 0; y < 4; ++y)
    {
      for (int x = 0; x < 4; ++x)
      {
        if (current_piece[y][x] == 0)
          continue;

        int grid_x = x + b_x;
        int grid_y = y + b_y;

        if (grid_x < 0 || grid_x >= m_width || grid_y < 0 || grid_y >= m_height)
          return false;

        if (grid_y >= 0 && grid[grid_y][grid_x])
          return false;

        if (grid_x < 0)
          return false; // Check if the piece is too far to the left
      }
    }
    return true;
  }

  bool GameBoard::has_hit_pile()
  {
    for (int y = 0; y < 4; ++y)
    {
      for (int x = 0; x < 4; ++x)
      {
        if (current_piece[y][x] == 0)
          continue;

        int gridY = y + b_y;
        int gridX = x + b_x;

        if (gridY < 0 || gridY >= m_height || gridX < 0 || gridX >= m_width || grid[gridY][gridX])
          return true;
      }
    }
    return false;
  }

  // clears the lines
  void GameBoard::shift_down()
  {
    int deleted_line = m_height - 1;
    int linesCleared = 0; // Initialize linesCleared to count cleared lines

    for (int undeleted_line = m_height - 1; undeleted_line >= 0; undeleted_line--)
    {
      int count_width = 0;
      for (int x = 0; x < m_width; ++x)
      {
        if (grid[undeleted_line][x])
          count_width++;
      }

      if (count_width < m_width)
      {
        for (int x = 0; x < m_width; ++x)
        {
          grid[deleted_line][x] = grid[undeleted_line][x];
        }
        --deleted_line;
      }
      else
      {
        ++linesCleared;
        sf::sleep(sf::milliseconds(20)); // Increment linesCleared for each cleared line
      }
    }

    lines_cleared += linesCleared;
    score += (linesCleared * linesCleared) * 100;

    // Update your score here with the number of lines cleared
    // For example: score += linesCleared;
  }

  bool GameBoard::move_down()
  {
    // moves the piece down
    ++b_y;

    if (has_hit_pile())
    {
      // moves it back up
      --b_y;
      for (int y = 0; y < 4; ++y)
      {
        for (int x = 0; x < 4; ++x)
        {
          if (current_piece[y][x])
          {
            int grid_row = b_y + y;
            int grid_col = b_x + x;

            // Check boundary conditions before accessing grid elements
            if (grid_row >= 0 && grid_row < m_height && grid_col >= 0 && grid_col < m_width)
            {
              // this aids for color generate of the pile
              grid[grid_row][grid_col] = block;
            }
          }
        }
      }

      shift_down();
      generate_new_piece();
      return false;
    }
    return true;
  }

  void GameBoard::rotate()
  {

    // create a temp 2d vector for the rotated_piece
    std::vector<std::vector<int>> rotated_block(4, std::vector<int>(4, 0));

    for (int y = 0; y < 4; ++y)
    {
      for (int x = 0; x < 4; ++x)
      {
        if (current_piece[y][x])
        {
          rotated_block[3 - x][y] = 1;
        }
      }
    }

    // this makes the current_piece rotated
    current_piece = std::move(rotated_block);
  }

  Grid &GameBoard::getGameState()
  {
    return grid;
  }

  const int &GameBoard::getBlock()
  {
    return block;
  }

  bool GameBoard::is_game_over()
  {
    for (int y = 0; y < 4; y++)
    {
      for (int x = 0; x < 4; x++)
      {
        if (current_piece[y][x] == 1)
        {
          int world_x = b_x + x;
          int world_y = b_y + y;

          // Check if the block is out of the top boundary
          if (world_y < 0)
          {
            return true;
          }

          // Check if the block collides with existing blocks in the world
          if (world_y >= 0 && grid[world_y][world_x] != 0)
          {
            return true;
          }
        }
      }
    }

    return false;
  }

}