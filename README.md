# Conway's Game of Life
> Implementing Conway's Game of Life in C

After reading about Conway's Game of Life on Wikipedia, I was interested
in implementing the algorithm myself to better understand it. I hadn't
written anything in C recently, so I decided to take this as an
opportunity to brush up on C.

## Compiling
* git clone https://github.com/tewidis/Conway.git
* make clean
* make

## Running Examples
* ./conway random <width> <height> <n> creates a board of size width by height
  and runs for n iterations
* ./conway <initial_configuration> <n> runs with the specified initial
  condition for <n> iterations

## List of Initial Configurations
* block
* beehive
* loaf
* boat
* tub
* blinker
* beacon
* pulsar
* pentadecathlon
* toad
* rpentomino
* acorn
* diehard
* glider
* lwss (Light-weight Spaceship)
* mwss (Middle-weight Spaceship)
* hwss (Heavy-weight Spaceship)
* simkin_glider_gun
* gosper_glider_gun

## Conclusion
Through completing this project, I gained a better understanding of how to
debug C code using common tools such as gdb and valgrind. Additionally, I was
able to refresh my understanding of C syntax and memory management. This
project was also my first time creating my own makefile, so I gained a deeper
understanding of the compiling and linking process.
