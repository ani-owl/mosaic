#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char** argv) {
  game g;
  if (argc == 3) {
    g = game_load(argv[2]);
    if (strcmp(argv[1], "-s") == 0) {
      game_solve(g);
      game_print(g);
    } else if (strcmp(argv[1], "-c") == 0)
      printf("%u\n", game_nb_solutions(g));

  } else if (argc == 4) {
    g = game_load(argv[2]);
    if (strcmp(argv[1], "-s") == 0) {
      game_solve(g);
      game_save(g, argv[3]);
    } else if (strcmp(argv[1], "-c") == 0) {
      FILE* file = fopen(argv[3], "w");
      if (file == NULL) {
        fprintf(stderr, "Error opening output file\n");
        exit(EXIT_FAILURE);
      }
      fprintf(file, "%u\n", game_nb_solutions(g));
      fclose(file);
    }
  }
  return EXIT_SUCCESS;
}
