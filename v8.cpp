// using iterative deepening

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

// Check if placing a number is valid
bool isValid(int (&grid)[9][9], int row, int col, int num)
{
    for (int x = 0; x < 9; ++x)
    {
        if (grid[row][x] == num || grid[x][col] == num)
            return false;
    }
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (grid[startRow + i][startCol + j] == num)
                return false;
        }
    }
    return true;
}

// Depth-limited DFS
bool depthLimitedDFS(int (&grid)[9][9], vector<pair<int, int>> &emptyCells, int depthLimit, int currentDepth = 0)
{
    if (currentDepth == depthLimit)
        return false; // Reached depth limit

    // If all empty cells are filled, the puzzle is solved
    if (emptyCells.empty())
        return true;

    // Get the next empty cell
    auto [row, col] = emptyCells.back();
    emptyCells.pop_back();

    // Try placing numbers 1-9
    for (int num = 1; num <= 9; ++num)
    {
        if (isValid(grid, row, col, num))
        {
            grid[row][col] = num;

            // Recur with the next empty cell
            if (depthLimitedDFS(grid, emptyCells, depthLimit, currentDepth + 1))
            {
                return true;
            }

            // Backtrack
            grid[row][col] = 0;
        }
    }

    // Restore the empty cell
    emptyCells.push_back({row, col});
    return false;
}

// Iterative deepening solver
bool sudokuSolver(int (&grid)[9][9])
{
    vector<pair<int, int>> emptyCells;

    // Collect all empty cells
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (grid[row][col] == 0)
            {
                emptyCells.emplace_back(row, col);
            }
        }
    }

    // Iteratively increase the depth limit
    for (int depthLimit = 1; depthLimit <= 81; ++depthLimit)
    {
        vector<pair<int, int>> tempEmptyCells = emptyCells;

        if (depthLimitedDFS(grid, tempEmptyCells, depthLimit))
        {
            return true; // Solution found
        }
    }

    return false; // No solution found
}

int main()
{
    // set up inputs
    // Unsolved Sudoku puzzle (0 represents empty cells)
    int sudoku[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}};

    // Record the start time
    auto start = std::chrono::high_resolution_clock::now();

    // Call the function
    bool solved = sudokuSolver(sudoku);

    // Record the end time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration in milliseconds
    std::chrono::duration<double, std::milli> elapsed = end - start;

    // Output the time taken in milliseconds
    std::cout << "Execution time: " << elapsed.count() << " milliseconds\n"
              << std::endl;

    // output solved sudoku if it was solved
    if (solved)
    {
        for (int row = 0; row < 9; ++row)
        {
            for (int col = 0; col < 9; ++col)
            {
                std::cout << sudoku[row][col] << ", ";
            }
            std::cout << "\n";
        }
    }
    else
    {
        std::cout << "No solution found!\n";
    }

    return 0; // Indicates successful program termination
}