#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

bool test_dummy(void) {
  printf("Running dummy test...\n");

  printf("Dummy test passed!\n");
  return true;
}

bool test_game_default_solution() {
  game g = game_default_solution();

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_status(g, i, j) != SATISFIED) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

void test_game_restart() {
  game g = game_new_empty();

  // initialize g and set 0,0 to black
  game_set_color(g, 0, 0, BLACK);
  // check 0,0 is black
  assert(game_get_color(g, 0, 0) == BLACK);

  game_restart(g);
  // restart game and check 0,0 has become empty
  assert(game_get_color(g, 0, 0) == EMPTY);

  game_delete(g);
}

int test_game_nb_neighbors() {
  game g = game_new_empty_ext(4, 6, false, FULL);
  game_set_color(g, 1, 1, BLACK);
  game_set_color(g, 1, 2, WHITE);
  game_set_color(g, 1, 3, BLACK);
  game_set_color(g, 2, 1, BLACK);
  game_set_color(g, 2, 2, EMPTY);
  game_set_color(g, 2, 3, BLACK);
  game_set_color(g, 3, 1, BLACK);
  game_set_color(g, 3, 2, WHITE);
  game_set_color(g, 3, 3, BLACK);
  bool test1 = (game_nb_neighbors(g, 2, 2, BLACK) == 4);
  bool test2 = (game_nb_neighbors(g, 2, 2, WHITE) == 4);
  bool test11 = (game_nb_neighbors(g, 2, 2, EMPTY) == 1);
  g->neigh = ORTHO;
  bool test3 = (game_nb_neighbors(g, 2, 2, BLACK) == 0);
  bool test4 = (game_nb_neighbors(g, 2, 2, WHITE) == 4);
  g->neigh = ORTHO_EXCLUDE;
  bool test5 = (game_nb_neighbors(g, 2, 2, BLACK) == 0);
  bool test6 = (game_nb_neighbors(g, 2, 2, WHITE) == 4);
  bool test7 = (game_nb_neighbors(g, 2, 2, EMPTY) == 0);
  g->neigh = FULL_EXCLUDE;
  bool test8 = (game_nb_neighbors(g, 2, 2, BLACK) == 4);
  bool test9 = (game_nb_neighbors(g, 2, 2, WHITE) == 4);
  bool test10 = (game_nb_neighbors(g, 2, 2, EMPTY) == 0);

  game_delete(g);
  if (test1 && test2 && test3 && test4 && test5 && test6 && test7 && test8 &&
      test9 && test10 && test11)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

void test_game_get_color() {
  game g = game_new_empty();

  game_set_color(g, 1, 2, WHITE);

  assert(game_get_color(g, 1, 2) == WHITE);

  game_delete(g);
}

void test_game_set_constraint() {
  game g = game_new_empty();

  game_set_constraint(g, 0, 0, 3);

  assert(game_get_constraint(g, 0, 0) == 3);

  game_delete(g);
}
int grid_4x6[] = {
    4,  -1, -1, 6,  4,  -1, /* row 0 */
    -1, 6,  7,  7,  -1, -1, /* row 1 */
    -1, -1, 6,  6,  7,  -1, /* row 2 */
    1,  -1, -1, -1, -1, -1, /* row 3 */
};
bool check_game_options(game g1, game g2) {
  return (game_get_neighbourhood(g1) == game_get_neighbourhood(g2) &&
          game_nb_cols(g1) == game_nb_cols(g2) &&
          game_nb_rows(g1) == game_nb_rows(g2) &&
          ((game_is_wrapping(g1) && game_is_wrapping(g2)) ||
           (!game_is_wrapping(g1) && !game_is_wrapping(g2))));
}
int test_game_copy(void) {
  // copy constraints
  game g0 = game_new_ext(4, 6, grid_4x6, NULL, false, FULL);
  game c0 = game_copy(g0);
  bool test1 = (game_equal(g0, c0) && check_game_options(g0, c0));
  game_delete(g0);
  game_delete(c0);

  // copy colors
  game g1 = game_new_ext(4, 6, grid_4x6, NULL, false, FULL);
  game_set_color(g1, 0, 0, BLACK);
  game_set_color(g1, 3, 5, BLACK);
  game c1 = game_copy(g1);
  bool test2 =
      game_get_color(c1, 0, 0) == BLACK && game_get_color(c1, 3, 5) == BLACK;
  game_delete(g1);
  game_delete(c1);

  // copy options
  game g2 = game_new_empty_ext(4, 4, true, ORTHO);
  game c2 = game_copy(g2);
  bool test3 = (game_equal(g2, c2) && check_game_options(g2, c2));
  game_delete(g2);
  game_delete(c2);

  if (test1 && test2 && test3) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}
int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
    return EXIT_FAILURE;
  }

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);

  if (strcmp("dummy", argv[1]) == 0)
    test_dummy();
  else if (strcmp("game_default_solution", argv[1]) == 0) {
    if (test_game_default_solution()) return EXIT_SUCCESS;
  } else if (strcmp("game_restart", argv[1]) == 0)
    test_game_restart();
  else if (strcmp("game_nb_neighbors", argv[1]) == 0)
    test_game_nb_neighbors();
  else if (strcmp("game_get_color", argv[1]) == 0)
    test_game_get_color();
  else if (strcmp("game_set_constraint", argv[1]) == 0)
    test_game_set_constraint();
  else if (strcmp("game_copy", argv[1]) == 0)
    test_game_copy();

  fprintf(stderr, "Test \"%s\" finished\n", argv[1]);
  return EXIT_SUCCESS;
}
