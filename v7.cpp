// using multithreading

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

bool isValid(int (&grid)[9][9], int row, int col, int num)
{
    // Check the row
    for (int x = 0; x < 9; ++x)
    {
        if (grid[row][x] == num)
            return false;
    }

    // Check the column
    for (int x = 0; x < 9; ++x)
    {
        if (grid[x][col] == num)
            return false;
    }

    // Check the 3x3 subgrid
    int startRow = row - row % 3;
    int startCol = col - col % 3;
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

// Function to solve Sudoku using backtracking with multithreading
bool parallelSudokuSolver(int (&grid)[9][9], int row, int col)
{
    if (row == 9)
        return true; // Solved
    if (col == 9)
        return parallelSudokuSolver(grid, row + 1, 0); // Move to next row
    if (grid[row][col] != 0)
        return parallelSudokuSolver(grid, row, col + 1); // Skip filled cells

    vector<thread> threads;
    atomic<bool> solved(false);
    mutex gridLock;

    for (int num = 1; num <= 9; ++num)
    {
        if (isValid(grid, row, col, num))
        {
            threads.emplace_back([&, num]()
                                 {
                // Local copy of the grid to avoid conflicts
                int localGrid[9][9];
                {
                    lock_guard<mutex> lock(gridLock);
                    copy(&grid[0][0], &grid[0][0] + 81, &localGrid[0][0]);
                }

                // Place the number
                localGrid[row][col] = num;

                // Recursively solve the rest of the grid
                if (parallelSudokuSolver(localGrid, row, col + 1)) {
                    lock_guard<mutex> lock(gridLock);
                    if (!solved) {
                        copy(&localGrid[0][0], &localGrid[0][0] + 81, &grid[0][0]);
                        solved = true;
                    }
                } });
        }
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        if (t.joinable())
            t.join();
    }

    return solved;
}

bool sudokuSolver(int (&grid)[9][9])
{
    return parallelSudokuSolver(grid, 0, 0);
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