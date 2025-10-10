# Makefile now also compiles the program!
**LIS library** (Library of Iterative Solvers for linear systems): 
  - Included in `../lis-2.1.10/` directory 
  - **Built automatically** by the Makefile when you run `make`
  - No manual installation required - the Makefile handles configuration and compilation
  - Used for computing the largest eigenvalue via the Power Iteration method

## Compilation
To compile the file just execute 
```bash
make
``` 
and to run it
```bash
./challenge2
```
To clean before compiling again
```bash
make clean
```
## Latest output reached with the program:
```
----------------------------------------------
Task 1: Frobenius norm of Ag: ||Ag||_F = 4
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
Task 5: Frobenius norm of As: ||As||_F = 93
----------------------------------------------
Task 6: Is the matrix Ls symmetric? YES
Task 6: Number of non-zero entries in Ls: 123201
----------------------------------------------
Task 7: Matrix Ls with perturbation saved into /lis-2.1.10/test/Ls.mtx
matrix size = 351 x 351 (123201 nonzero entries)

Task 7: Largest eigenvalue of the matrix Ls: 60.1337, number of iterations: 2007
----------------------------------------------
Task 8: shift mi = 28.6
Task 8: number of iterations to reach 1.e-8 accuracy: 1094
Task 8: largest eigenvalue result using a shift mi = 28.6: 60.1337
```
