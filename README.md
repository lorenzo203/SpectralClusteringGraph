# Description
This repository includes a project of Numerical Linear Algebra, whose goal is to evaluate the *Algebraic Connectivity* of a graph and perform a *Spectral Clustering* using the **Fiedler Vector** for the **Laplacian** associated to the **Adjacency Matrix** of the Graph.

The project implements a complete pipeline for spectral clustering on a social‑network graph (351 nodes) using:
- Eigen (dense/sparse linear algebra), used to perform any operation associated to Linear Algebra objects
- LIS (Library of Iterative Solvers), used to implement iterative solvers and iterative eigenvalue computations
- Automated build/run via Makefile with consolidated report
  
Take a quick look on [Spectral Clustering](https://en.wikipedia.org/wiki/Spectral_clustering)

## System requirements
- Linux x86_64 (tested on Ubuntu/Debian‑like systems)
- GCC toolchain with glibc (g++ ≥ 9 recommended) and make
- Eigen3 headers
- MPI runtime (Open MPI) for running LIS tests with mpirun

Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install -y build-essential g++ make libeigen3-dev openmpi-bin libopenmpi-dev
```

If Eigen headers are not in a standard include path, you can pass `mkEigenInc=/path/to/eigen3` to make.

## Quick start
```bash
make all           # build LIS (MPI-enabled) and project binaries
make run           # run full pipeline (uses mpirun for LIS tests)
make clean         # remove build artifacts and generated outputs
```

## What the pipeline does
- Builds symmetric adjacency As and Laplacian Ls (stores them in Matrix Market format .mtx)
- Runs LIS eigensolvers via mpirun:
  - Power Iteration (with/without shift)
  - Subspace Iteration for the smallest eigenpairs
- Computes Fiedler‑based clustering 

## Repository layout
```
SpectralClusteringGraph/
├── Makefile
├── README.md
├── data/
│   └── social.mtx
├── src/
│   ├── challenge2.cpp
│   └── task10_11.cpp
├── matrices/                  # generated (gitignored)
└──reports/
    └── challenge2_report.txt
```

## Configuration
- `mkEigenInc=/path/to/eigen3`   Override Eigen include directory
- `NP=integer`                    MPI process count for LIS runs (default: 1)

Examples:
```bash
make all mkEigenInc=/usr/include/eigen3
make run NP=4
```

## Outputs
- `matrices/`: `As.mtx`, `Ls.mtx`, `evalues9.txt`, `evectors9.mtx`, …
- `reports/challenge2_report.txt`: norms, eigenvalues, LIS logs, cluster sizes (np, nn), and nnz in A12 for both A_ord and As
### Full report:
```
----------------------------------------------
Task 1: Frobenius norm of Ag: ||Ag||_F = 4.89898
----------------------------------------------
Task 2: Euclidean norm of y: ||y||_2 = 0
Task 2: Lg is not symmetric positive definite
----------------------------------------------
Task 3: Smallest eigenvalue of Lg:-2.04895e-16
Task 3: Largest eigenvalue of Lg: 5.67038
----------------------------------------------
Task 4: Smallest non-zero eigenvalue of Lg: 0.28031
Task 4: Eigenvector associated with the smallest non-zero eigenvalue: ( 0.455542  0.391696  0.218053  0.391696 -0.190354 -0.308704 -0.340521 -0.308704 -0.308704)^T
----------------------------------------------
Task 5: Frobenius norm of As: ||As||_F = 93.819
----------------------------------------------
Task 6: Is the matrix Ls symmetric? YES
Task 6: Number of non-zero entries in Ls: 9153
----------------------------------------------
Task 7: Matrix Ls with perturbation saved into ../matrices/Ls.mtx
Task 7: Matrix As saved into ../matrices/As.mtx

=====================================
LIS EIGENVALUE COMPUTATIONS
=====================================

--- Task 7: Power Iteration (default shift) ---

number of processes = 1
matrix size = 351 x 351 (123201 nonzero entries)

initial vector x      : all components set to 1
precision             : double
eigensolver           : Power
convergence condition : ||lx-(B^-1)Ax||_2 <= 1.0e-08 * ||lx||_2
matrix storage format : CSR
shift                 : 0.000000e+00
eigensolver status    : normal end

Power: mode number          = 0
Power: eigenvalue           = 6.013370e+01
Power: number of iterations = 2007
Power: elapsed time         = 5.246770e-01 sec.
Power:   preconditioner     = 0.000000e+00 sec.
Power:     matrix creation  = 0.000000e+00 sec.
Power:   linear solver      = 0.000000e+00 sec.
Power: relative residual    = 9.940435e-09


--- Task 8: Power Iteration (shift=29.6) ---

number of processes = 1
matrix size = 351 x 351 (123201 nonzero entries)

initial vector x      : all components set to 1
precision             : double
eigensolver           : Power
convergence condition : ||lx-(B^-1)Ax||_2 <= 1.0e-08 * ||lx||_2
matrix storage format : CSR
shift                 : 2.960000e+01
eigensolver status    : normal end

Power: mode number          = 0
Power: eigenvalue           = 6.013370e+01
Power: number of iterations = 1076
Power: elapsed time         = 2.845280e-01 sec.
Power:   preconditioner     = 0.000000e+00 sec.
Power:     matrix creation  = 0.000000e+00 sec.
Power:   linear solver      = 0.000000e+00 sec.
Power: relative residual    = 9.814119e-09


--- Task 9: Subspace Iteration ---

number of processes = 1
matrix size = 351 x 351 (123201 nonzero entries)

initial vector x      : all components set to 1
precision             : double
eigensolver           : Subspace
convergence condition : ||lx-(B^-1)Ax||_2 <= 1.0e-10 * ||lx||_2
matrix storage format : CSR
shift                 : 0.000000e+00
inner eigensolver     : Inverse
linear solver         : BiCG
preconditioner        : none
size of subspace      : 2

compute eigenpairs in subspace:

Subspace: mode number          = 0
Subspace: eigenvalue           = 5.669404e-04
Subspace: elapsed time         = 5.733800e-02 sec.
Subspace: number of iterations = 3
Subspace: relative residual    = 4.512604e-12

Subspace: mode number          = 1
Subspace: eigenvalue           = 1.789070e+00
Subspace: elapsed time         = 8.032122e-01 sec.
Subspace: number of iterations = 113
Subspace: relative residual    = 8.965692e-11

eigensolver status    : normal end

Subspace: mode number          = 0
Subspace: eigenvalue           = 5.669404e-04
Subspace: number of iterations = 3
Subspace: elapsed time         = 8.616228e-01 sec.
Subspace:   preconditioner     = 5.531311e-05 sec.
Subspace:     matrix creation  = 0.000000e+00 sec.
Subspace:   linear solver      = 5.586171e-02 sec.
Subspace: relative residual    = 4.512604e-12


Second smallest positive eigenvalue:
1.78907049141373897250e+00
Iterations count:
113

----------------------------------------------
TASK 10: Fiedler Vector Analysis
----------------------------------------------
Task 10: np = 52, nn = 299, n0 = 0
----------------------------------------------

Task 11: 
Non-zeros in off-diagonal blocks for A_ord: 332
Non-zeros in off-diagonal blocks for A_ord: 1162
=====================================
Report complete
=====================================
```

## Troubleshooting
- mpirun not found: install Open MPI (see System requirements) and ensure it is on PATH.
- “MPI_Comm_size before MPI_Init”: always use make run (it launches LIS tests via mpirun).
- Eigen headers not found: pass mkEigenInc to make with the correct include path.
