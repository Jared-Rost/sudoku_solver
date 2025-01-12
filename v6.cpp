// AC-3 and backtracking algorithm for solving Sudoku puzzles

#include <vector>
#include <queue>
#include <set>
#include <utility>  // For std::pair
#include <iostream> // Includes the standard input/output stream library
#include <chrono>   // For timing

using namespace std;

typedef vector<vector<set<int>>> Domains;

bool isValid(int (&grid)[9][9], int row, int col, int num)
{
    for (int i = 0; i < 9; ++i)
    {
        if (grid[row][i] == num || grid[i][col] == num)
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

bool removeInconsistentValues(Domains &domains, int xiRow, int xiCol, int xjRow, int xjCol)
{
    bool removed = false;
    for (auto it = domains[xiRow][xiCol].begin(); it != domains[xiRow][xiCol].end();)
    {
        int value = *it;
        // Check if there's any valid value in xj's domain that satisfies the "not-equal" constraint
        bool isConsistent = false;
        for (int neighborValue : domains[xjRow][xjCol])
        {
            if (value != neighborValue)
            {
                isConsistent = true;
                break;
            }
        }

        if (!isConsistent)
        {
            it = domains[xiRow][xiCol].erase(it);
            removed = true;
        }
        else
        {
            ++it;
        }
    }
    return removed;
}

bool ac3(Domains &domains)
{
    queue<pair<pair<int, int>, pair<int, int>>> arcs;

    // Initialize arcs with all cell pairs
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (!domains[row][col].empty())
            {
                for (int i = 0; i < 9; ++i)
                {
                    if (i != col)
                        arcs.push({{row, col}, {row, i}}); // Same row
                    if (i != row)
                        arcs.push({{row, col}, {i, col}}); // Same column
                }

                int startRow = row - row % 3, startCol = col - col % 3;
                for (int i = 0; i < 3; ++i)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        int r = startRow + i, c = startCol + j;
                        if (r != row || c != col)
                            arcs.push({{row, col}, {r, c}});
                    }
                }
            }
        }
    }

    while (!arcs.empty())
    {
        auto [xi, xj] = arcs.front();
        arcs.pop();

        if (removeInconsistentValues(domains, xi.first, xi.second, xj.first, xj.second))
        {
            if (domains[xi.first][xi.second].empty())
                return false;

            for (int i = 0; i < 9; ++i)
            {
                if (i != xi.second)
                    arcs.push({{xi.first, xi.second}, {xi.first, i}});
                if (i != xi.first)
                    arcs.push({{xi.first, xi.second}, {i, xi.second}});
            }
        }
    }

    return true;
}

bool sudokuSolverRecursive(Domains &domains, int (&grid)[9][9])
{
    // Find the cell with the smallest domain (Most Constrained Variable)
    int row = -1, col = -1, minDomainSize = 10;
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (grid[i][j] == 0 && domains[i][j].size() < minDomainSize)
            {
                row = i;
                col = j;
                minDomainSize = domains[i][j].size();
            }
        }
    }

    // If no empty cell is found, the puzzle is solved
    if (row == -1)
        return true;

    // Try each value in the domain
    auto backupDomains = domains; // Backup domains for backtracking
    for (int value : domains[row][col])
    {
        grid[row][col] = value;

        // Propagate constraints using AC-3
        domains[row][col] = {value};
        if (ac3(domains) && sudokuSolverRecursive(domains, grid))
            return true;

        // Backtrack
        domains = backupDomains;
        grid[row][col] = 0;
    }

    return false;
}

bool sudokuSolver(int (&grid)[9][9])
{
    // Initialize domains
    Domains domains(9, vector<set<int>>(9));
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (grid[row][col] == 0)
            {
                for (int value = 1; value <= 9; ++value)
                {
                    if (isValid(grid, row, col, value))
                    {
                        domains[row][col].insert(value);
                    }
                }
            }
            else
            {
                domains[row][col] = {grid[row][col]};
            }
        }
    }

    // Preprocess with AC-3
    if (!ac3(domains))
    {
        cout << "No solution exists!" << endl;
        return 0;
    }

    return sudokuSolverRecursive(domains, grid);
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