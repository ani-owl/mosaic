#include "game.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

// creates a new game with given constraints and colors with a default size
game game_new(constraint *constraints, color *colors) {  // Anissa
  game g;                                                // Anissa
  g = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, constraints, colors, false,
                   FULL);
  return g;
}

// creates an empty game
game game_new_empty(void) {  // Anissa
  return game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false, FULL);
}

// copies a given game
game game_copy(cgame g) {  // Anissa
  game new_game = game_new_ext(g->height, g->width, g->constraints, g->colors,
                               g->wrapping, g->neigh);
  return new_game;
}

// tests if two games are equal
bool game_equal(cgame g1, cgame g2) {  // Anissa
  if (g1->height != g2->height || g1->width != g2->width ||
      g1->wrapping != g2->wrapping || g1->neigh != g2->neigh) {
    return false;
  }

  for (int i = 0; i < g1->height * g1->width; i++) {
    if (g1->colors[i] != g2->colors[i] ||
        g1->constraints[i] != g2->constraints[i]) {
      return false;
    }
  }
  return true;
}

// deletes a given game by freing its memory
void game_delete(game g) {
  if (g != NULL) {
    free(g->constraints);
    free(g->colors);

    if (g->history != NULL) {
      free(g->history);
    }
    queue_free_full(g->s_do, free);
    queue_free_full(g->s_undo, free);

    free(g);
  }
}

// Sets the constraint at position (i, j) to the given value
void game_set_constraint(game g, uint i, uint j, constraint n) {  // IRINA
  if (g == NULL || i >= g->height || j >= g->width) {
    fprintf(stderr,
            "Error: Invalid game pointer or indices in game_nb_neighbors\n");
    exit(EXIT_FAILURE);
  }

  if (n < MIN_CONSTRAINT || n > MAX_CONSTRAINT) {
    fprintf(stderr, "Error: Invalid constraint in game_nb_neighbors\n");
    exit(EXIT_FAILURE);
  }

  g->constraints[i * g->width + j] = n;
}

// Gets the color at position (i, j)
color game_get_color(cgame g, uint i, uint j) {  // IRINA
  uint index = i * g->width + j;
  return g->colors[index];
}

// Sets the color at position (i, j) to the given value
void game_set_color(game g, uint i, uint j, color c) {  // Anissa
  if (g == NULL || i >= g->height || j >= g->width) {
    fprintf(stderr, "NULL game or invalid column");
    exit(EXIT_FAILURE);
  }
  if (c == WHITE || c == BLACK || c == EMPTY) {
    g->colors[i * g->width + j] = c;
  } else {
    fprintf(stderr, "INVALID COLOR!!\n");
  }
}

// Gets the constraint at position (i, j)
constraint game_get_constraint(cgame g, uint i, uint j) {  // Anissa
  if (g == NULL) {
    fprintf(stderr, "Error: game is NULL\n");
    exit(EXIT_FAILURE);
  }

  return g->constraints[i * g->width + j];
}

// Gets the next square in the specified direction
bool game_get_next_square(cgame g, uint i, uint j, direction dir, uint *pi_next,
                          uint *pj_next) {  // Anissa
  int dirchange_i[9] = {
      0,  -1, 1, 0, 0,
      -1, -1, 1, 1};  // how the index changes after moving in a direction
  int dirchange_j[9] = {
      0,  0, 0,  -1, 1,
      -1, 1, -1, 1};  // if moving forward you add one else going back is -1
  if (!g->wrapping) {
    *pi_next = i + dirchange_i[dir];
    *pj_next = j + dirchange_j[dir];

    if (*pi_next < g->height &&
        *pj_next < g->width) {  // checks if the next square is within the grid
      return true;
    }
    return false;
  } else if (g->wrapping) {
    *pi_next = (i + dirchange_i[dir] + g->height) % g->height;
    *pj_next = (j + dirchange_j[dir] + g->width) % g->width;
  }

  return true;
}

// Gets the status of a square at position (i, j)
status game_get_status(cgame g, uint i, uint j) {  // Anissa
  if (!g || i >= g->height || j >= g->width) {
    return ERROR;
  }

  constraint c = game_get_constraint(g, i, j);

  if (c == UNCONSTRAINED && game_nb_neighbors(g, i, j, EMPTY) == 0) {
    return SATISFIED;
  } else if (c == UNCONSTRAINED && game_nb_neighbors(g, i, j, EMPTY) != 0) {
    return UNSATISFIED;
  }

  else if (game_nb_neighbors(g, i, j, BLACK) == c &&
           game_nb_neighbors(g, i, j, EMPTY) == 0) {
    return SATISFIED;
  } else if (game_nb_neighbors(g, i, j, BLACK) == c &&
             game_nb_neighbors(g, i, j, EMPTY) != 0) {
    return UNSATISFIED;
  }

  else if (game_nb_neighbors(g, i, j, BLACK) < c &&
           game_nb_neighbors(g, i, j, EMPTY) == 0) {
    return ERROR;
  } else if (game_nb_neighbors(g, i, j, BLACK) < c &&
             game_nb_neighbors(g, i, j, EMPTY) != 0) {
    return UNSATISFIED;
  }

  return ERROR;
}
// returns the number of neighborsof color c  of a given square
int game_nb_neighbors(cgame g, uint i, uint j, color c) {  // Anissa
  if (!g || i >= g->height || j >= g->width) {
    fprintf(stderr, "Invalid game or indices\n");
    exit(1);
  }

  int count = 0;
  uint i_next = 0;
  uint j_next = 0;

  if (g->neigh == FULL) {  // goes through the neighbours in all directions
    for (direction d = HERE; d <= DOWN_RIGHT; d++) {
      if ((game_get_next_square(g, i, j, d, &i_next, &j_next)) &&
          game_get_color(g, i_next, j_next) == c) {
        count += 1;
      }
    }
  }
  if (g->neigh == ORTHO) {  // goes through the neighbours only in five the
                            // orthogonal directions
    for (direction d = HERE; d <= RIGHT; d++) {
      if ((game_get_next_square(g, i, j, d, &i_next, &j_next)) &&
          game_get_color(g, i_next, j_next) == c) {
        count += 1;
      }
    }
  }
  if (g->neigh == ORTHO_EXCLUDE) {  // same as orthogonal but doesn't include
                                    // the current square
    for (direction d = UP; d <= RIGHT; d++) {
      if ((game_get_next_square(g, i, j, d, &i_next, &j_next)) &&
          game_get_color(g, i_next, j_next) == c) {
        count += 1;
      }
    }
  }
  if (g->neigh ==
      FULL_EXCLUDE) {  // same as full but doesn't include the current square
    for (direction d = UP; d <= DOWN_RIGHT; d++) {
      if ((game_get_next_square(g, i, j, d, &i_next, &j_next)) &&
          game_get_color(g, i_next, j_next) == c) {
        count += 1;
      }
    }
  }
  return count;
}

// plays a move in the game by setting the color c at the given position
void game_play_move(game g, uint i, uint j, color c) {  // Anissa
  color c_before = game_get_color(g, i, j);
  game_set_color(g, i, j, c);
  // saves move into the queue that records history
  moves *hist = malloc(sizeof(moves));
  hist->before = c_before;
  hist->after = c;
  hist->i = i;
  hist->j = j;
  queue_push_head(g->s_do, hist);
  queue_clear_full(g->s_undo, &free);
}

// checks if the game is won by checking the status and the color
bool game_won(cgame g) {  // Anissa

  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (game_get_status(g, i, j) != SATISFIED ||
          game_get_color(g, i, j) == EMPTY) {
        return false;
      }
    }
  }
  return true;
}

// resets the game by setting colors to empty and frees the history of moves
void game_restart(game g) {  // Anissa
  if (!g) {
    fprintf(stderr, "Invalid game\n");
    exit(1);
  }
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      game_set_color(g, i, j, EMPTY);
    }
  }
  queue_free(g->s_do);
  queue_free(g->s_undo);
  g->s_do = queue_new();
  g->s_undo = queue_new();
}
