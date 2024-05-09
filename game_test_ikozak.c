#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int test_dummy(int argc, const char *argv[]) {
  if (argc != 2 || strcmp(argv[1], "dummy") != 0) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

bool test_game_delete(void) {
  game g = game_default();
  if (g != NULL) {
    game_delete(g);
    g = NULL;
  }
  return (g == NULL);
}

bool test_game_default(void) {
  game g = game_default();

  bool cons =
      (game_get_constraint(g, 0, 0) == 0 && game_get_constraint(g, 0, 3) == 3 &&
       game_get_constraint(g, 1, 1) == 5 && game_get_constraint(g, 2, 2) == 4 &&
       game_get_constraint(g, 2, 4) == 1 && game_get_constraint(g, 3, 0) == 6 &&
       game_get_constraint(g, 3, 2) == 6 && game_get_constraint(g, 3, 3) == 3);

  bool result = true;

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if ((game_get_status(g, i, j) == ERROR) ||
          (game_get_color(g, i, j) != EMPTY)) {
        result = false;
      }
    }
  }
  game_delete(g);
  return (result && cons);
}

bool test_game_won(void) {
  game g;
  g = game_default_solution();
  game g1 = game_default();
  bool test = true;
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_color(g, i, j) != WHITE &&
          game_get_color(g, i, j) != BLACK) {
        test = false;
      }
      if ((game_get_status(g, i, j) != SATISFIED) ||
          (game_get_constraint(g, i, j) != game_get_constraint(g1, i, j))) {
        test = false;
      }
      if (game_get_constraint(g, i, j) != -1) {
        if (game_nb_neighbors(g, i, j, BLACK) != game_get_constraint(g, i, j)) {
          test = false;
        }
      }
    }
  }
  if (game_won(g) != test) {
    game_delete(g);
    game_delete(g1);
    return false;
  }
  game_delete(g);
  game_delete(g1);
  return true;
}

bool test_game_new_empty(void) {
  game g = game_new_empty();
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_status(g, i, j) == ERROR ||
          game_get_color(g, i, j) != EMPTY ||
          game_get_constraint(g, i, j) < MIN_CONSTRAINT ||
          game_get_constraint(g, i, j) > MAX_CONSTRAINT) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_get_constraint(void) {
  game g = game_new_empty();
  game_set_constraint(g, 1, 1, 1);
  game_set_constraint(g, 1, 2, 2);
  game_set_constraint(g, 1, 3, 3);

  if (game_get_constraint(g, 1, 1) == 1 && game_get_constraint(g, 1, 2) == 2 &&
      game_get_constraint(g, 1, 3) == 3) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

bool test_get_status(void) {
  game g = game_new_empty();

  // test UNCONSTRAINED
  game_set_constraint(g, 2, 2, UNCONSTRAINED);
  game_set_color(g, 2, 2, WHITE);
  bool test1a = (game_get_status(g, 2, 2) == UNSATISFIED);
  game_set_color(g, 1, 1, BLACK);
  game_set_color(g, 1, 2, BLACK);
  game_set_color(g, 1, 3, BLACK);
  game_set_color(g, 2, 1, BLACK);
  game_set_color(g, 2, 3, BLACK);
  game_set_color(g, 3, 1, BLACK);
  game_set_color(g, 3, 2, BLACK);
  bool test1b = (game_get_status(g, 2, 2) == UNSATISFIED);
  game_set_color(g, 3, 3, BLACK);
  bool test1c = (game_get_status(g, 2, 2) == SATISFIED);

  // test constraint MAX
  game_set_constraint(g, 2, 2, 9);
  game_set_color(g, 2, 2, EMPTY);
  bool test2a = (game_get_status(g, 2, 2) == UNSATISFIED);
  game_set_color(g, 2, 2, BLACK);
  bool test2b = (game_get_status(g, 2, 2) == SATISFIED);
  game_set_color(g, 2, 2, WHITE);
  bool test2c = (game_get_status(g, 2, 2) == ERROR);

  game_delete(g);

  if (test1a && test1b && test1c && test2a && test2b && test2c) return true;

  return false;
}

bool test_game_nb_solutions(void) {
  game g1 = game_default();
  game g2 = game_default();
  unsigned int res = game_nb_solutions(g1);

  if (res == 1 && game_equal(g1, g2)) {
    game_delete(g1);
    game_delete(g2);
    return true;
  }
  printf("A possible error:\nNb of solutions: %u\nGame state unchanged: %d\n",
         res, game_equal(g1, g2));

  game_delete(g1);
  game_delete(g2);
  return false;
}

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0) return test_dummy(argc, argv);
  if (strcmp("game_delete", argv[1]) == 0)
    ok = test_game_delete();
  else if (strcmp("game_default", argv[1]) == 0)
    ok = test_game_default();
  else if (strcmp("game_won", argv[1]) == 0)
    ok = test_game_won();
  else if (strcmp("game_new_empty", argv[1]) == 0)
    ok = test_game_new_empty();
  else if (strcmp("game_get_constraint", argv[1]) == 0)
    ok = test_game_get_constraint();
  else if (strcmp("game_get_status", argv[1]) == 0)
    ok = test_get_status();
  else if (strcmp("game_nb_solutions", argv[1]) == 0) {
    ok = test_game_nb_solutions();
  } else {
    printf("Unknown test \n");
  }
  // print test result
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}