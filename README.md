## 1. Overview
Mosaic Game is a C-based project that implements basic game functions for a mosaic puzzle. The provided file "game_text.c" serves as a textual interface to interact with the game.

## 2. Installation Instructions
To install the Mosaic game, follow these steps:

bash
Copy code
$ git clone https://gitlab.emi.u-bordeaux.fr/pt2/teams/mosaic-a17
$ cd mosaic-a17
$ make install

$ mkdir build ; cd build ; cmake .. ; make 

# running tests
$ make test
# checking memory state
$ make Experimental Memcheck  

## Running the mosaic game on the terminal 

Run: ./game_text to play the mosaic game on the terminal

## 3. User Guide

The file game_text.c provides a textual interface to interact with the Mosaic game. Follow these commands when running the program:

- Press "w <i> <j>" to set square (i, j) to white.
- Press "b <i> <j>" to set square (i, j) to black.
- Press "e <i> <j>" to empty square (i, j).
- Press "e" to restart the game.
- Press "q" to quit the game.
- Press "z" to undo the move.
- Press "y" to redo the move.


## Additional functionalities:

- Run: ./game_solve -s arg1 to show the first solution to the game stored in the file named arg1 in the terminal.
- Run: ./game_solve -s arg1 arg2 to show the first solution to the game stored in the file named arg1 in the file arg2.
- Run: ./game_solve -c arg1 to show the number of solutions to the game in arg1 in the terminal.
- Run: ./game_solve -c arg1 arg2 to show the number of solutions to the game in arg1 in the file arg2.


## Playing in a graphic interface:

'''
- Run: ./game_sdl to play the game_default in a graphic interface. If you don't specify the file to be saved to in argument and you press the save button, it's automatically saved into save.txt.
- Run: ./game_sdl arg1 arg2, where arg1 is the game to be loaded and arg2 is the file where the game is saved after pressing the save button.

Authors:
- Anissa Hadj Amar

- Nikita Semenov
- Irina Kozak
