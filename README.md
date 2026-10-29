# Lab 7 – Vector Calculator

**Course:** CPE2600  
**Project:** Vector Calculator (Lab 7)  
**Author:** Emanuel Hernandez

## Description

This project is a command-line vector calculator written in C. It supports common operations on 3D vectors and lets the user store named vectors, view and clear them, and load or save vectors using CSV files. Lab 7 adds dynamic memory management and file operations to the vector calculator.

## Features

- Add and subtract vectors.
- Multiply a vector by a scalar.
- Calculate the dot product and cross product of vectors.
- Assign names to vectors and display their values.
- List all stored vectors.
- Clear stored vectors and release allocated memory.
- Load vectors from a CSV file.
- Save stored vectors to a CSV file.
- Exit the program and release allocated memory.

## Project Files

| File | Purpose |
| --- | --- |
| `main.c` | Entry point for the calculator. |
| `interface.c` / `interface.h` | User interaction and command processing. |
| `vector.c` / `vector.h` | Vector operations and related declarations. |
| `storage.c` / `storage.h` | Storage and management of named vectors. |
| `Makefile` | Compiles and links the program. |
| `example_vectors.csv` | Example input for testing CSV loading. |
| `README.md` | Project overview and instructions. |

## Requirements

- Linux or another environment with a C compiler
- GCC
- `make`
- Valgrind (optional, for checking memory use)

## How to Build

Open a terminal in the folder containing the `Makefile`, then run:

```bash
make
```

The Makefile compiles the C source files and links them into the `minimat` executable.

To rebuild from scratch, run:

```bash
make clean
make
```

## How to Run and Command-Line Options

Start the interactive calculator with:

```bash
./minimat
```

**Command-line options:** The standard invocation above does not require any options. The calculator accepts its vector operations as **interactive commands after startup**, not as shell options. Consult `main.c` for any additional arguments supported by your particular version; no extra flags are documented here.

Once started, enter a command at the prompt. Type `help` for built-in usage information and `quit` to exit.

## Available Commands

| Command or operator | Description |
| --- | --- |
| `help` | Show available commands and syntax. |
| Vector assignment | Create or update a named vector. |
| Vector display | Show the value of a named vector. |
| `+` | Add two vectors. |
| `-` | Subtract two vectors. |
| `*` | Multiply a vector by a scalar. |
| `.` | Calculate the dot product. |
| `x` | Calculate the cross product. |
| `list` | Display all saved vectors. |
| `clear` | Remove stored vectors and free their memory. |
| `load <filename>` | Read vectors from a CSV file. |
| `save <filename>` | Write stored vectors to a CSV file. |
| `quit` | Exit the calculator. |

Run `help` inside the program for the exact vector-entry and expression syntax.

## CSV File Format

Each line contains a vector name followed by its three components, separated by commas:

```csv
A,1,2,3
B,4,5,6
C,-2,0,7
```

The CSV file has **no header row**. For example, to load the supplied file, enter:

```text
load example_vectors.csv
```

To save the current collection of named vectors, enter:

```text
save saved_vectors.csv
```

When loading a vector whose name already exists, the stored vector with that name is replaced.

## Testing / Demonstration

1. Run `make` and `./minimat`.
2. Enter `help` to view supported syntax.
3. Create two vectors and perform addition, subtraction, scalar multiplication, dot product, and cross product.
4. Use `list` to view the stored vectors.
5. Run `save saved_vectors.csv` and inspect the output file.
6. Run `clear`, followed by `list`, to check that the stored vectors have been removed.
7. Run `load example_vectors.csv`, followed by `list`, to verify CSV loading.
8. Run `quit` to exit.

## How Dynamic Memory Is Used

Lab 7 uses **dynamic memory allocation** to manage stored vectors while the program runs. The storage module (`storage.c`) manages memory for the collection of named vectors rather than relying only on a fixed set of local vector variables.

- As vectors are added or loaded, the program allocates or adjusts storage for the saved data.
- Saved vectors remain available for later calculations, listing, and CSV output until they are removed or the program exits.
- Clearing stored vectors releases the associated dynamically allocated memory.
- Before the program exits, allocated storage should be released so it is not leaked.

This makes memory management part of the program's responsibility: each successful allocation must eventually have a corresponding release. The exact allocation functions and data structure can be inspected in `storage.c`.

## Memory Checking

Because this lab uses dynamic memory allocation, check for memory leaks with Valgrind if it is installed:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./minimat
```

Try creating and clearing vectors, loading a CSV file, and quitting while running the program under Valgrind. Review its report for any leaked or invalid memory accesses.

## Cleanup

```bash
make clean
```

This removes the executable and generated object/dependency files according to the Makefile.