# Sudoku Solver - Performance Optimization Study 🧩⚡

> Exploring and benchmarking various algorithmic optimization techniques for solving 9×9 Sudoku puzzles

A personal project comparing the performance of different constraint satisfaction and search algorithms.

## 📋 Overview

This project implements multiple approaches to solving Sudoku puzzles, from naive backtracking to advanced constraint satisfaction algorithms. Each version explores different optimization strategies, providing insights into their real-world performance characteristics on a classic computational problem.

### Key Features

- 🔄 **Multiple Algorithm Implementations** - 8 different solving approaches
- ⚡ **Performance Benchmarking** - Precise timing measurements in milliseconds
- 📊 **Comparative Analysis** - Side-by-side performance comparisons
- 🎯 **Optimization Techniques** - Backtracking, heuristics, constraint propagation, and more
- 🛠️ **Easy Testing** - Simple Makefile commands to run each version

## 🛠️ Tech Stack

### Implementation (98.7% C++, 1.3% Makefile)
- **C++** - High-performance implementation
- **Makefile** - Build automation and version management
- Standard Template Library (STL)
- Multithreading support

## 🚀 Getting Started

### Prerequisites

- C++ compiler (g++ recommended)
- Make utility
- Unix-like environment (Linux, macOS, WSL)

### Installation & Usage

1. **Clone the Repository**
   ```bash
   git clone https://github.com/Jared-Rost/sudoku_solver.git
   cd sudoku_solver
   ```

2. **Build and Run a Specific Version**
   ```bash
   make v{version_number}
   ./v{version_number}
   ```

   **Example:**
   ```bash
   make v2
   ./v2
   ```

3. **Clean Build Artifacts**
   ```bash
   make clean
   ```

### Output

Each solver outputs:
- The solved Sudoku puzzle
- Execution time in milliseconds
- "No solution found!" if the puzzle is unsolvable

## 📊 Performance Results

**Test Environment:** MacBook Air M1 2020

| Version | Algorithm | Average Time | Performance Rank |
|---------|-----------|--------------|------------------|
| v2 | Backtracking + Lists | **~0.015 ms** | 🥇 Fastest |
| v4 | LCV/MRV Heuristics | ~0.020 ms | 🥈 2nd |
| v5 | AC-3 Algorithm | ~0.230 ms | 🥉 3rd |
| v3 | Backtracking + Bitmasks | ~0.050 ms | Fast |
| v1 | Naive Backtracking | ~0.600 ms | Baseline |
| v6 | AC-3 + Backtracking | ~1.000 ms | Moderate |
| v8 | Iterative Deepening | ~2.000 ms | Moderate |
| v7 | Multithreading | ~150.00 ms | Slowest |

### Key Insights

**Surprising Results:**
- **Lists beat Bitmasks** - v2 (lists) outperformed v3 (bitmasks), despite bitmasks theoretically being more efficient. The overhead of mask operations appears to outweigh the benefits for this problem size.

- **Multithreading Overhead** - v7 was dramatically slower due to thread management overhead. Sudoku solving is too small and fast for multithreading to provide benefits—the coordination cost exceeds the computation savings.

- **Heuristics Win** - v4's LCV (Least Constraining Value) and MRV (Minimum Remaining Values) heuristics provided excellent performance with minimal complexity.

## 🧠 Algorithm Versions

### v1 - Naive Backtracking
Basic depth-first search with backtracking. Validates constraints after each insertion.
- **Time:** ~0.60 ms
- **Technique:** Brute force exploration

### v2 - Backtracking + Lists ⭐ Fastest
Maintains lists of available values for each cell, eliminating redundant constraint checks.
- **Time:** ~0.015 ms
- **Technique:** Constraint tracking with lists

### v3 - Backtracking + Bitmasks
Uses bit manipulation to track available values in rows, columns, and 3×3 grids.
- **Time:** ~0.05 ms
- **Technique:** Bitwise operations

### v4 - LCV/MRV Heuristics
Implements intelligent variable and value ordering heuristics.
- **Time:** ~0.02 ms
- **Technique:** Least Constraining Value, Minimum Remaining Values

### v5 - AC-3 Algorithm
Arc Consistency algorithm for constraint propagation.
- **Time:** ~0.23 ms
- **Technique:** Constraint satisfaction preprocessing

### v6 - AC-3 + Backtracking
Combines constraint propagation with search.
- **Time:** ~1.00 ms
- **Technique:** Hybrid approach

### v7 - Multithreading
Parallelizes the search across multiple threads.
- **Time:** ~150.00 ms ⚠️
- **Technique:** Parallel computation (ineffective for small problems)

### v8 - Iterative Deepening
Combines Breadth-First and Depth-First Search strategies.
- **Time:** ~2.00 ms
- **Technique:** IDDFS (Iterative Deepening DFS)

## 🔮 Future Enhancements

- [ ] Central program to run/test all versions with user selection
- [ ] Sudoku puzzle generator with difficulty parameters
- [ ] Puzzle validator to verify solution correctness
- [ ] Combine AC-3 preprocessing with v2's list approach
- [ ] Benchmarking suite with various difficulty levels
- [ ] Visualization of solving process
- [ ] Support for larger puzzle sizes (16×16, 25×25)
