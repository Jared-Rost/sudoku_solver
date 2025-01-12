// AC-3
#include <vector>
#include <queue>
#include <set>
#include <utility>  // For std::pair
#include <iostream> // Includes the standard input/output stream library
#include <chrono>   // For timing

// Check if two variables (cells) are in conflict
bool inConflict(int row1, int col1, int row2, int col2)
{
    return row1 == row2 || col1 == col2 || (row1 / 3 == row2 / 3 && col1 / 3 == col2 / 3);
}

// Reduce the domain of `A` based on the domain of `B`
bool revise(std::vector<std::set<int>> &domains, int A, int B)
{
    bool revised = false;
    auto &domainA = domains[A];
    const auto &domainB = domains[B];

    // Check if a value in A has no supporting value in B
    for (auto it = domainA.begin(); it != domainA.end();)
    {
        bool supported = false;
        for (int value : domainB)
        {
            if (*it != value)
            {
                supported = true;
                break;
            }
        }

        if (!supported)
        {
            it = domainA.erase(it);
            revised = true;
        }
        else
        {
            ++it;
        }
    }

    return revised;
}

bool ac3(std::vector<std::set<int>> &domains, const std::vector<std::pair<int, int>> &constraints)
{
    std::queue<std::pair<int, int>> queue;

    // Add all constraints to the queue
    for (const auto &constraint : constraints)
    {
        queue.push(constraint);
    }

    // Process the queue
    while (!queue.empty())
    {
        auto [A, B] = queue.front();
        queue.pop();

        if (revise(domains, A, B))
        {
            if (domains[A].empty())
            {
                return false; // No solution
            }

            // Add related constraints back to the queue
            for (int i = 0; i < 81; ++i)
            {
                if (i != A && inConflict(A / 9, A % 9, i / 9, i % 9))
                {
                    queue.push({i, A});
                }
            }
        }
    }

    return true;
}

bool sudokuSolver(int (&grid)[9][9])
{
    // Step 1: Initialize domains
    std::vector<std::set<int>> domains(81);
    for (int i = 0; i < 81; ++i)
    {
        int row = i / 9, col = i % 9;
        if (grid[row][col] == 0)
        {
            domains[i] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        }
        else
        {
            domains[i] = {grid[row][col]};
        }
    }

    // Step 2: Create constraints
    std::vector<std::pair<int, int>> constraints;
    for (int i = 0; i < 81; ++i)
    {
        for (int j = 0; j < 81; ++j)
        {
            if (i != j && inConflict(i / 9, i % 9, j / 9, j % 9))
            {
                constraints.emplace_back(i, j);
            }
        }
    }

    // Step 3: Apply AC-3
    if (!ac3(domains, constraints))
    {
        return false; // No solution
    }

    // Step 4: Fill the grid with solved values
    for (int i = 0; i < 81; ++i)
    {
        if (domains[i].size() == 1)
        {
            grid[i / 9][i % 9] = *domains[i].begin();
        }
    }

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