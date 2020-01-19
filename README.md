# Conway's Game of Life
> Implementing Conway's Game of Life in C

After reading about Conway's Game of Life on Wikipedia, I was interested
in implementing the algorithm myself. I hadn't written anything in C
recently, so I decided to take this as an opportunity to

## Compiling
* git clone https://github.com/tewidis/Conway.git
* gcc -g conway.c -o conway

## Running Examples
* ./conway will run with a random initial condition
* ./conway <initial_configuration> runs with the specified initial condition

## List of Initial Configurations
* block

## Future Work
* Write a make file for compiling
* Remove memory leaks
* Accept number of iterations as an input as opposed to a global variable
* Refactor duplicate pieces
* Make header files and split code into different .c files

## Conclusion
