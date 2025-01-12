# C++ Sudoku Solver With Different Techniques

This project uses a variety of optimization techniques to solve a simple 9x9 sudoku puzzle.

## How to Run

Download and use the make commands to run, it will output the solved sudoku puzzle and the time (in milliseconds) the algorithm took to solve it; the program outputs "No solution found!" if the sudoku puzzle is unvalid.

General version:

```
make v{version number}
./v{version number}
```

Specific example:

```
make v1
./v1
```

Remove all compiled files with:

```
make clean
```

## Results

Tested on Mabook Air M1 2020.

On average, the version using backtracking and lists (v2) was the fastest closely followed by version using only the LCV/MRV heuristics (v4), and the version using the AC-3 algorithm (v5). The slowest was the version using multithreading (v7) by a wide margin.

Overall, I'm surprised at these results - I wasn't expecting v2 to outperform the version using bitmasks (v3) since bitmasks seemed like a strictly better version of v2 but it seems making the masking variables took a bit too long. I also didn't expect the AC-3 version to get worse when we add backtracking in, I suppose naive backtracking is just that bad.

Multithreading was horrendously slow - I thought waiting for all those threads to finish would take a while but not this bad. I suppose this problem is too small to be worth all the effort of splitting it into pieces and waiting for each thread to end before continuing.

## Versions

### v1

Naive backtracking using unoptimized data structures.

Ran in ~0.60 milliseconds.

### v2

Uses simple backtracking and lists to store which numbers are available in each cell instead of checking to see if column/row/3x3 grid is valid after inserting a new number.

Ran in ~0.015 milliseconds.

### v3

Uses simple backtracking and bitmasks to keep track of which numbers are available in each row/column/3x3 grid.

Ran in ~0.05 milliseconds.

### v4

Uses heuristics like Least Constraining Value (LCV) and Minimum Remaining Values (MRV).

Ran in ~0.02 milliseconds.

### v5

Uses the AC-3 algorithm.

Ran in ~0.23 milliseconds.

### v6

Uses the AC-3 algorithm and backtracking.

Ran in ~1.00 milliseconds.

### v7

Uses multithreading to divide the work among multiple threads.

Ran in ~150.00 milliseconds.

### v8

Uses Iterative Deepening to combine Breadth First Search (BFS) and Depth First Search (DFS).

Ran in ~2.00 milliseconds.

## Future Updates (To Do)

- Make central program that runs/tests each version of the algorithm based on user input
- Make sudoku puzzle generator with adjustable parameters (like number of starting values)
- Make sudoku verifier to ensure outputs are correct
- Combine AC-3 preprocessing with the list version in v2
