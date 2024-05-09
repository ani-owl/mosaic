#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

void test_game_new_ext() {
  uint nb_rows = 4;
  uint nb_cols = 4;
  bool wrapping = true;
  neighbourhood nb_type = FULL;

  constraint constraints[nb_rows * nb_cols];
  color colors[nb_rows * nb_cols];
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      constraints[i * nb_rows + j] = UNCONSTRAINED;
      colors[i * nb_rows + j] = EMPTY;
    }
  }
  game g =
      game_new_ext(nb_rows, nb_cols, constraints, colors, wrapping, nb_type);
  game g1 = game_new_empty_ext(nb_rows, nb_cols, wrapping, nb_type);
  assert(game_equal(g, g1));
  game_delete(g);
  game_delete(g1);
}

void test_game_new_empty_ext() {
  uint nb_rows = 4;
  uint nb_cols = 4;
  bool wrapping = true;
  neighbourhood nb_type = FULL;

  constraint constraints[nb_rows * nb_cols];
  color colors[nb_rows * nb_cols];
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      constraints[i * nb_rows + j] = UNCONSTRAINED;
      colors[i * nb_rows + j] = EMPTY;
    }
  }
  game g =
      game_new_ext(nb_rows, nb_cols, constraints, colors, wrapping, nb_type);

  game g1 = game_new_empty_ext(nb_rows, nb_cols, wrapping, nb_type);
  assert(game_equal(g, g1));
  game_delete(g1);
  game_delete(g);
}

void test_game_nb_rows() {
  game g = game_new_empty_ext(2, 3, false, FULL);
  assert(game_nb_rows(g) == 2);
  game_delete(g);
}

void test_game_nb_cols() {
  game g = game_new_empty_ext(3, 4, false, FULL);
  assert(game_nb_cols(g) == 4);
  game_delete(g);
}

void test_game_is_wrapping() {
  game g_wrap_true = game_new_empty_ext(3, 3, true, FULL);
  game g_wrap_false = game_new_empty_ext(3, 3, false, FULL);
  assert(game_is_wrapping(g_wrap_true) == true &&
         game_is_wrapping(g_wrap_false) == false);
  game_delete(g_wrap_true);
  game_delete(g_wrap_false);
}

void test_game_get_neighbourhood() {
  game g_full = game_new_empty_ext(3, 3, true, FULL);
  game g_ortho = game_new_empty_ext(3, 3, true, ORTHO);
  game g_full_exclude = game_new_empty_ext(3, 3, true, FULL_EXCLUDE);
  game g_ortho_exclude = game_new_empty_ext(3, 3, true, ORTHO_EXCLUDE);

  assert(game_get_neighbourhood(g_full) == FULL &&
         game_get_neighbourhood(g_ortho) == ORTHO &&
         game_get_neighbourhood(g_full_exclude) == FULL_EXCLUDE &&
         game_get_neighbourhood(g_ortho_exclude) == ORTHO_EXCLUDE);
  game_delete(g_full);
  game_delete(g_ortho);
  game_delete(g_full_exclude);
  game_delete(g_ortho_exclude);
}

bool test_game_undo() {
  game g = game_new_empty();

  int i = 0;
  int j = 0;
  game_play_move(g, i, j, BLACK);

  game_undo(g);

  if (game_get_color(g, i, j) == EMPTY) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

void test_game_redo() {
  game g = game_new_empty();

  int i = 0;
  int j = 0;
  game_play_move(g, i, j, BLACK);

  game_undo(g);
  game_redo(g);

  assert(game_get_color(g, i, j) == BLACK);
  game_delete(g);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
    return EXIT_FAILURE;
  }

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);

  if (strcmp("game_new_ext", argv[1]) == 0)
    test_game_new_ext();
  else if (strcmp("game_new_empty_ext", argv[1]) == 0)
    test_game_new_empty_ext();
  else if (strcmp("game_nb_rows", argv[1]) == 0)
    test_game_nb_rows();
  else if (strcmp("game_nb_cols", argv[1]) == 0)
    test_game_nb_cols();
  else if (strcmp("game_is_wrapping", argv[1]) == 0)
    test_game_is_wrapping();
  else if (strcmp("game_get_neighbourhood", argv[1]) == 0)
    test_game_get_neighbourhood();
  else if (strcmp("game_undo", argv[1]) == 0) {
    if (test_game_undo()) return EXIT_SUCCESS;
  } else if (strcmp("game_redo", argv[1 == 0]))
    test_game_redo();

  fprintf(stderr, "Test \"%s\" finished\n", argv[1]);
  return EXIT_SUCCESS;
}
