# C Compiler
C Compiler compiles a subset of C into x86 assembly. It currently supports local
variables, multi statement functions and mathematical/boolean expressions.

It also has control flow statements like if/else blocks and while loops. Therfore 
the subset of supported keywords is turing complete.

An example program returning the number of integers less than 100 divisible by 
7 is provided in test.txt.  

## Instructions

These instructions will get you a copy of the project up and running on your machine.

### Prerequisites

1. g++ with version at least 5.2
2. gcc with any recent version.

### Run instructions

1. Fork this repo into a directory.
2. Open a terminal and navigate to the directory.
3. Type "make" to generate the assembly file called "assembly.s"
4. Type "make asm" to compile and run the assembly file.

To change the source code being compiled, open test.txt and edit the code there.
