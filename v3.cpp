// bitmasks are used to keep track of the numbers that are already used in the row, column, and subgrid.

#include <iostream>      // Includes the standard input/output stream library
#include <chrono>        // For timing
#include <unordered_set> // For hashsets

#define N 9 // Size of the grid

int rowMasks[N] = {0};
int colMasks[N] = {0};
int subgridMasks[N] = {0};

bool sudokuSolverRecursive(int (&grid)[9][9])
{
    // loop through everything twice and try inserting new
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            // check to see if the cell is empty
            if (grid[row][col] == 0)
            {
                int subgrid = (row / 3) * 3 + col / 3; // Subgrid index (0 to 8)

                // Determine the valid candidates using bitmask
                int validCandidates = ~(rowMasks[row] | colMasks[col] | subgridMasks[subgrid]) & 0x1FF;

                while (validCandidates)
                {
                    // Extract the least significant bit
                    int candidateBit = validCandidates & -validCandidates;
                    int candidate = __builtin_ctz(candidateBit) + 1; // Get number (1-9)

                    // Place the number
                    grid[row][col] = candidate;
                    rowMasks[row] |= candidateBit;
                    colMasks[col] |= candidateBit;
                    subgridMasks[subgrid] |= candidateBit;

                    // Recursively try to solve the next cells
                    if (sudokuSolverRecursive(grid))
                        return true;

                    // Backtrack: undo placing the number
                    grid[row][col] = 0;
                    rowMasks[row] &= ~candidateBit;
                    colMasks[col] &= ~candidateBit;
                    subgridMasks[subgrid] &= ~candidateBit;

                    // Remove the processed candidate
                    validCandidates &= (validCandidates - 1);
                }

                // If no candidates are valid, backtrack
                return false;
            }
        }
    }

    // puzzle has been solved
    return true;
}

bool sudokuSolver(int (&grid)[9][9])
{
    // loop through it once and ensure masks match what is already there
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            // check to see if the cell is empty
            if (grid[row][col] != 0)
            {
                int subgrid = (row / 3) * 3 + col / 3; // Subgrid index (0 to 8)

                // Set the appropriate bits indicating that the number is taken
                rowMasks[row] |= (1 << (grid[row][col] - 1));         // Mark number as used in the row
                colMasks[col] |= (1 << (grid[row][col] - 1));         // Mark number as used in the column
                subgridMasks[subgrid] |= (1 << (grid[row][col] - 1)); // Mark number as used in the subgrid
            }
        }
    }

    return sudokuSolverRecursive(grid);
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