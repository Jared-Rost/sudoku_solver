// basic backtracking

#include <iostream> // Includes the standard input/output stream library
#include <chrono>   // For timing

bool validateRowCol(int (&grid)[9][9], int row, int col, int num)
{
    // check row
    for (int i = 0; i < 9; i++)
        if (grid[i][col] == num)
            return false;

    // check col
    for (int i = 0; i < 9; i++)
        if (grid[row][i] == num)
            return false;

    // check 3x3 grid
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (grid[startRow + i][startCol + j] == num)
                return false;

    // otherwise return truw
    return true;
}

bool sudokuSolver(int (&grid)[9][9])
{
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
                    if (validateRowCol(grid, row, col, i))
                    {
                        // insert into array
                        grid[row][col] = i;

                        // recursively solve
                        if (sudokuSolver(grid))
                            return true;

                        // if it was not the correct number, backtrack and try something else
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