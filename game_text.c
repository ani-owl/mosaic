#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char **argv) {
  game g;
  switch (argc) {
    case 1:
      g = game_default();
      break;

    case 2:
      printf("%s\n", argv[1]);
      g = game_load(argv[1]);
      break;

    default:
      return EXIT_FAILURE;
      break;
  }
  while (!game_won(g)) {
    game_print(g);
    for (int i = 0; i < game_nb_rows(g); i++) {
      for (int j = 0; j < game_nb_cols(g); j++) {
        if (game_get_status(g, i, j) == ERROR) {
          printf("Square(%d,%d) error\n", i, j);
        }
      }
    }
    char c;
    char filename[99];  // declaring max length as 99 hopefully no one wants to
                        // save to a huge file name
    printf("? [h for help]\n");
    if (scanf(" %c", &c) != 1) {
      fprintf(stderr, "Error reading input\n");
      // Handle the error, such as exiting the program or clearing the input
      // buffer
      exit(EXIT_FAILURE);
    }
    if (c == 'h') {
      printf("action: help\n");
      printf("- press 'W <i> <j>' to set square (i,j) white\n");
      printf("- press 'b <i> <j>' to set square (i,j) black\n");
      printf("- press 'e <i> <j>' to set square (i,j) empty\n");
      printf("- press 'z' to undo move\n");
      printf("- press 'y' to redo move\n");
      printf("- press 'r' to restart\n");
      printf("- press 'q' to quit\n");
      printf("- press 's <filename>' to save game\n");
    } else if (c == 'r') {
      game_restart(g);
    } else if (c == 'q') {
      printf("shame\n");
      return EXIT_SUCCESS;
    } else if (c == 'z') {
      game_undo(g);
    } else if (c == 'y') {
      game_redo(g);
    } else if (c == 'w' || c == 'b' || c == 'e') {
      int i, j;
      if (scanf("%d %d", &i, &j) != 2) {
        fprintf(stderr, "Error reading row and column\n");
        // Handle the error, such as exiting the program or clearing the input
        // buffer
        exit(EXIT_FAILURE);
      }  // Check if the provided coordinates are valid
      if (i < 0 || i >= game_nb_rows(g) || j < 0 || j >= game_nb_cols(g)) {
        fprintf(stderr, "Invalid coordinates\n");
        // Handle the error, such as exiting the program or clearing the input
        // buffer
        exit(EXIT_FAILURE);
      }
      if (game_get_status(g, i, j) != ERROR) {
        if (c == 'w') {
          game_play_move(g, i, j, WHITE);
        } else if (c == 'b') {
          game_play_move(g, i, j, BLACK);
        } else if (c == 'e') {
          game_play_move(g, i, j, EMPTY);
        }
      }
    } else if (c == 's') {
      printf("Enter filename: ");

      // Check if scanf successfully reads the filename
      if (scanf("%99s", filename) != 1) {
        fprintf(stderr, "Error reading filename\n");
        // Handle the error, such as exiting the program or clearing the input
        // buffer
        exit(EXIT_FAILURE);
      }
      game_save(g, filename);
      printf("game saved! \n");
    }
  }
  game_print(g);
  printf("congratulation\n");

  return EXIT_SUCCESS;
}