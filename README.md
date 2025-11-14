# CS3113-OS-Project3

Project Description
In this project, you need to implement the deadlock avoidance algorithm using C++. The Banker’s algorithm
consists of:
- the safety algorithm
- the resource-request algorithm

Input explanation (Looking at input.txt):
- R 3 means there are 3 resource types: R0, R1, and R2.
- P 5 means there are 5 processes: P0, P1, P2, P3, and P4.
- Available is a vector with 3, 3, and 2 representing the number of available instances for R0, R1, and R2
respectively.
- Max is a matrix, where each row represents the maximum number of instances of each resource type (R0,
R1, R2) needed by the corresponding process.
- Allocation is a matrix, where each row represents the number of instances of each resource type (R0, R1,
R2) currently allocated to the corresponding process.
- P1 1 0 2 means P1 is requesting 1 instance of R0, 0 instances of R1, and 2 instances of R2 respectively.

The output should look as follows (Look at output.txt).

TODO
- Write the Banker’s algorithm in a file called project3.cpp.
- Assume that the input is read via redirection
- Compile and run project3.cpp. Make sure you keep the input file (e.g input.txt) in the same directory where
your program is located.
- Submit project3.cpp to GradeScope.
