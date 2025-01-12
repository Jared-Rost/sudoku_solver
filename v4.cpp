// Heuristics like MRV and LCV are used to solve the Sudoku puzzle.

#include <iostream> // Includes the standard input/output stream library
#include <chrono>   // For timing

// Function to find the cell with the fewest valid options
std::pair<int, int> findMostConstrainedCell(int grid[9][9], bool (&availableNumbers)[9][9][9])
{
    int minOptions = INT_MAX;
    std::pair<int, int> mostConstrainedCell = {-1, -1};

    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (grid[row][col] == 0)
            { // Only consider empty cells
                int validCount = 0;
                for (int num = 0; num < 9; ++num)
                {
                    if (availableNumbers[row][col][num])
                    {
                        ++validCount;
                    }
                }

                // Update the most constrained cell if fewer options are found
                if (validCount < minOptions)
                {
                    minOptions = validCount;
                    mostConstrainedCell = {row, col};
                }
            }
        }
    }

    return mostConstrainedCell;
}

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

// Function to get the least constraining value for a specific cell
std::vector<int> getLeastConstrainingValues(int grid[9][9], bool (&availableNumbers)[9][9][9], int row, int col)
{
    std::vector<std::pair<int, int>> candidates; // Pair of (number, impact)

    for (int num = 1; num <= 9; ++num)
    {
        if (availableNumbers[row][col][num - 1])
        {
            int impact = 0;

            // remove it as an option for all other slots in the same row, col, and grid
            removeNumberFromLists(availableNumbers, row, col, num);

            // Calculate impact: count how many valid options are removed for neighbors
            for (int i = 0; i < 9; ++i)
            {
                // Check row
                if (grid[row][i] == 0)
                {
                    for (int n = 0; n < 9; ++n)
                    {
                        if (availableNumbers[row][i][n])
                        {
                            ++impact;
                        }
                    }
                }

                // Check column
                if (grid[i][col] == 0)
                {
                    for (int n = 0; n < 9; ++n)
                    {
                        if (availableNumbers[i][col][n])
                        {
                            ++impact;
                        }
                    }
                }
            }

            // Check subgrid
            int startRow = row - row % 3, startCol = col - col % 3;
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    if (grid[startRow + i][startCol + j] == 0)
                    {
                        for (int n = 0; n < 9; ++n)
                        {
                            if (availableNumbers[startRow + i][startCol + j][n])
                            {
                                ++impact;
                            }
                        }
                    }
                }
            }

            // add it back to all the lists
            addNumberFromLists(availableNumbers, row, col, num);

            // Add the candidate number and its impact
            candidates.push_back({num, impact});
        }
    }

    // Sort candidates by their impact (ascending)
    std::sort(candidates.begin(), candidates.end(),
              [](const std::pair<int, int> &a, const std::pair<int, int> &b)
              {
                  return a.second < b.second;
              });

    // Extract only the numbers
    std::vector<int> values;
    for (const auto &candidate : candidates)
    {
        values.push_back(candidate.first);
    }

    return values;
}

bool sudokuSolverRecursive(int (&grid)[9][9], bool (&availableNumbers)[9][9][9])
{
    // Step 1: Find the most constrained cell (MRV)
    auto [row, col] = findMostConstrainedCell(grid, availableNumbers);
    if (row == -1)
    {
        return true; // No empty cells left, puzzle solved
    }

    // Step 2: Get the least constraining values (LCV)
    std::vector<int> values = getLeastConstrainingValues(grid, availableNumbers, row, col);

    // Step 3: Try each value in the sorted order
    for (int num : values)
    {
        if (availableNumbers[row][col][num - 1])
        {
            grid[row][col] = num;

            // remove it as an option for all other slots in the same row, col, and grid
            removeNumberFromLists(availableNumbers, row, col, num);

            // Recursively solve
            if (sudokuSolverRecursive(grid, availableNumbers))
            {
                return true;
            }

            // if it was not the correct number, add it back to all the lists, backtrack, and try something else
            addNumberFromLists(availableNumbers, row, col, num);
            grid[row][col] = 0;
        }
    }

    return false; // No solution found

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