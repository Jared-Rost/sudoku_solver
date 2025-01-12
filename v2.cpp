// using lists to store which numbers are available in each cell

#include <iostream>      // Includes the standard input/output stream library
#include <chrono>        // For timing
#include <unordered_set> // For hashsets

// remove selected number from all available number lists in the same row, col, and 3x3 grid
void removeNumberFromLists(bool (&availableNumbers)[9][9][9], int row, int col, int num)
{
    // check row
    for (int i = 0; i < 9; i++)
        availableNumbers[i][col][num - 1] = false;

    // check col
    for (int i = 0; i < 9; i++)
        availableNumbers[row][i][num - 1] = false;

    // check 3x3 grid
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            availableNumbers[startRow + i][startCol + j][num - 1] = false;
}

// add selected number to all available number lists in the same row, col, and 3x3 grid
void addNumberFromLists(bool (&availableNumbers)[9][9][9], int row, int col, int num)
{
    // check row
    for (int i = 0; i < 9; i++)
        availableNumbers[i][col][num - 1] = true;

    // check col
    for (int i = 0; i < 9; i++)
        availableNumbers[row][i][num - 1] = true;

    // check 3x3 grid
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            availableNumbers[startRow + i][startCol + j][num - 1] = true;
}

bool sudokuSolverRecursive(int (&grid)[9][9], bool (&availableNumbers)[9][9][9])
{
    // loop through everything twice and try inserting new
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            // check to see if the cell is empty
            if (grid[row][col] == 0)
            {
                // try each number
                for (int i = 1; i <= 9; i++)
                {
                    // check to see if that number is available
                    if (availableNumbers[row][col][i - 1] == true)
                    {
                        // insert into array
                        grid[row][col] = i;

                        // remove it as an option for all other slots in the same row, col, and grid
                        removeNumberFromLists(availableNumbers, row, col, i);

                        // recursively solve
                        if (sudokuSolverRecursive(grid, availableNumbers))
                            return true;

                        // if it was not the correct number, add it back to all the lists, backtrack, and try something else
                        addNumberFromLists(availableNumbers, row, col, i);
                        grid[row][col] = 0;
                    }
                }
                // if we looped through all numbers and none of them are valid time to try something else
                return false;
            }
        }
    }

    // puzzle has been solved
    return true;
}

bool sudokuSolver(int (&grid)[9][9])
{
    // loop through the board once to set the numbers to false that are not contained in the hashset
    bool availableNumbers[9][9][9];

    // Set each element to true
    for (int row = 0; row < 9; ++row)
        for (int col = 0; col < 9; ++col)
            for (int index = 0; index < 9; index++)
                availableNumbers[row][col][index] = true;

    // loop through the board once to remove the numbers from the available numbers lists that are already present in same row/col/3x3 grid
    for (int row = 0; row < 9; ++row)
        for (int col = 0; col < 9; ++col)
            if (grid[row][col] != 0)
                removeNumberFromLists(availableNumbers, row, col, grid[row][col]);

    return sudokuSolverRecursive(grid, availableNumbers);
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