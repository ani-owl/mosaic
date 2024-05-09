#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
bool test_dummy(void) {
  printf("test running!!\n");
  return true;
}

bool test_game_new(void) {
  constraint constraints[DEFAULT_SIZE * DEFAULT_SIZE];
  color colors[DEFAULT_SIZE * DEFAULT_SIZE];
  for (uint i = 0; i < DEFAULT_SIZE;
       i++) {  // initializes all contraints to UNCONSTRAINED and colors to
               // EMPTY
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      constraints[i * DEFAULT_SIZE + j] = UNCONSTRAINED;
      colors[i * DEFAULT_SIZE + j] = EMPTY;
    }
  }
  game g = game_new(constraints, colors);
  game g0 = game_new_empty();
  if (game_equal(g, g0) ==
      true) {  // game_new equals to game_new_empty if the constraints are
               // UNCONSTRAINED and colors are EMPTY
    game_delete(g);
    game_delete(g0);
    return true;
  }
  game_delete(g);
  game_delete(g0);
  return false;
}
bool test_game_equal(void) {
  game g1 = game_default();
  game g2 = game_default();
  printf("test on equal games g1 and g2 SUCCESS\n");
  if (game_equal(g1, g2) == false) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_set_color(g1, 0, 1, WHITE);
  game_set_color(g2, 0, 1, BLACK);
  if (game_equal(g1, g2) == true) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  printf("test on different games g1 and g2 SUCCES\n");
  game_delete(g1);
  game_delete(g2);
  return true;
}
bool test_game_set_color(void) {
  game g = game_default();
  game_set_color(g, 0, 1, WHITE);
  if (game_get_color(g, 0, 1) == WHITE) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

bool test_game_get_next_square_not_wrapping(void) {
  game g = game_new_empty_ext(5, 5, false, FULL);
  uint pi_next = 0;
  uint pj_next = 0;
  printf("test on valid square\n");
  if (game_get_next_square(g, 0, 0, RIGHT, &pi_next, &pj_next) == true) {
    if (pi_next != 0 && pj_next != 1) return false;
    printf("test on invalid square\n");
    if (game_get_next_square(g, 0, 0, LEFT, &pi_next, &pj_next) != false) {
      game_delete(g);
      return false;  //
    }
    if (game_get_next_square(g, 0, 0, UP, &pi_next, &pj_next) != false) {
      game_delete(g);
      return false;
    }
    if (game_get_next_square(g, 0, 0, UP_LEFT, &pi_next, &pj_next) != false) {
      game_delete(g);
      return false;
    }
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}
bool test_game_get_next_square_wrapping(void) {
  game g = game_new_empty_ext(3, 4, true, FULL);
  uint i = 0;
  uint j = 0;
  uint *pi_next = &i;
  uint *pj_next = &j;
  bool test1 = (game_get_next_square(g, 0, 0, LEFT, pi_next, pj_next) &&
                (*pi_next == 0 || *pj_next == 3));
  bool test2 = (game_get_next_square(g, 0, 3, RIGHT, pi_next, pj_next) &&
                (*pi_next == 0 || *pj_next == 0));
  bool test3 = (game_get_next_square(g, 0, 4, UP, pi_next, pj_next) &&
                (*pi_next == 2 || *pj_next == 1));
  bool test4 = (game_get_next_square(g, 2, 1, DOWN, pi_next, pj_next) &&
                (*pi_next == 0 || *pj_next == 1));
  bool test5 = (game_get_next_square(g, 0, 0, UP_LEFT, pi_next, pj_next) &&
                (*pi_next == 2 || *pj_next == 3));
  bool test6 = (game_get_next_square(g, 0, 3, UP_RIGHT, pi_next, pj_next) &&
                (*pi_next == 2 || *pj_next == 0));
  bool test7 = (game_get_next_square(g, 0, 0, LEFT, pi_next, pj_next) &&
                (*pi_next == 0 || *pj_next == 3));
  bool test8 = (game_get_next_square(g, 0, 2, DOWN_LEFT, pi_next, pj_next) &&
                (*pi_next == 0 || *pj_next == 3));
  bool test9 = (game_get_next_square(g, 2, 3, DOWN_RIGHT, pi_next, pj_next) &&
                (*pi_next == 0 || *pj_next == 0));
  bool test10 = (game_get_next_square(g, 1, 2, LEFT, pi_next, pj_next) &&
                 (*pi_next == 1 || *pj_next == 1));
  bool test11 = (game_get_next_square(g, 1, 2, UP, pi_next, pj_next) &&
                 (*pi_next == 1 || *pj_next == 3));
  return (test1 && test2 && test3 && test4 && test5 && test6 && test7 &&
          test8 && test9 && test10 && test11);
  return true;
}

bool test_game_play_move(void) {
  game g = game_default();
  game_play_move(g, 0, 1, WHITE);
  if (game_get_color(g, 0, 1) == WHITE) {
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}
bool test_game_print(void) {
  game g = game_default();
  game_print(g);
  game_delete(g);
  return true;
}
bool test_game_load(void) {
  game g1 = game_load("default.txt");
  game g2 = game_load("solution.txt");
  game g3 = game_default();
  game g4 = game_default_solution();
  if (game_equal(g1, g3) && game_equal(g2, g4)) {
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    game_delete(g4);
    return true;
  }
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g4);
  return false;
}
bool test_game_save() {
  game g = game_default();

  game_save(g, "save.txt");
  game g1 = game_load("save.txt");
  if (game_equal(g1, g)) {
    game_delete(g);
    game_delete(g1);
    return true;
  }
  game_delete(g);
  game_delete(g1);
  return false;
}
bool test_game_solve(void) {
  game g = game_default();

  if (game_solve(g) && game_won(g)) {
    game_delete(g);
    return true;
  } else {
    game_delete(g);
    return false;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0) ok = test_dummy();
  if (strcmp("game_equal", argv[1]) == 0)
    ok = test_game_equal();
  else if (strcmp("game_set_color", argv[1]) == 0)
    ok = test_game_set_color();
  else if (strcmp("game_get_next_square_not_wrapping", argv[1]) == 0)
    ok = test_game_get_next_square_not_wrapping();
  else if (strcmp("game_get_next_square_wrapping", argv[1]) == 0)
    ok = test_game_get_next_square_wrapping();
  else if (strcmp("game_new", argv[1]) == 0)
    ok = test_game_new();
  else if (strcmp("game_play_move", argv[1]) == 0)
    ok = test_game_play_move();
  else if (strcmp("game_print", argv[1]) == 0)
    ok = test_game_print();
  else if (strcmp("game_load", argv[1]) == 0)
    ok = test_game_load();
  else if (strcmp("game_save", argv[1]) == 0)
    ok = test_game_save();
  else if (strcmp("game_solve", argv[1]) == 0)
    ok = test_game_solve();
  // print test result
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
