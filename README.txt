Code used for obtaining numerical results presented in https://arxiv.org/abs/2012.08536.

The error-correcting codes referred to as A,B and C in that paper are referred to here as Toric, Rhombic1 and Rhombic2 respectively.

To build the code:

- `cd path/to/JITdecoding-public/build`
- `cmake ..`
- `cmake --build .`

This will produce six executables, three which run unit tests on the code and three which actually run the simulations (one for each code).
Each simulation executable takes parameters: 

- Lattice size (L)
- Single-qubit X error probability (p)
- Z stabiliser measurement error probability (q)
- Number of runs
- decoder cutoff (parameter "c" defined in main text)
- decoder reduction (parameter "r" defined in main text)

The output will be five numbers, the first four of which are just the values given as input for L, p, q, and runs. The fifth is the number of runs in which a logical error was detected at the end of the simulation. 
