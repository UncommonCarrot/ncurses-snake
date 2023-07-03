# ncurses-snake
Snake made with C++ and ncurses

working and tested on Debian `GNU/Linux 11 (bullseye)`

## Controls

`q`  quit the game

`p`  pause the game

`Arrow Keys` control the snake, pressing same direction speeds up the snake

## Rules

Running into the white border is a loss.

Running into the snake is a loss.

Every apple eaten adds to snake length.

The game is won at a snake Lenght of 1517.

## Compiling

To compile the source code use:
`g++ main.cpp -o snake -lncurses`

You probably need the `libncurses-dev` package to compile.
