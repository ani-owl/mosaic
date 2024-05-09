#include "game_ext.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_struct.h"
#include "queue.h"

game game_new_ext(uint nb_rows, uint nb_cols, constraint *constraints,
                  color *colors, bool wrapping,
                  neighbourhood neigh) {  // Anissa
  game g = malloc(sizeof(struct game_s));

  g->height = nb_rows;
  g->width = nb_cols;
  g->s_do = queue_new();
  g->s_undo = queue_new();
  g->wrapping = wrapping;
  g->neigh = neigh;
  g->constraints = malloc(nb_rows * nb_cols * sizeof(constraint));
  g->history = malloc(sizeof(moves));
  g->history->before = EMPTY;
  g->history->after = EMPTY;
  g->history->i = 0;
  g->history->j = 0;
  for (int i = 0; i < nb_rows * nb_cols; i++) {
    g->constraints[i] = constraints[i];
  }
  g->colors = malloc(nb_rows * nb_cols * sizeof(color));
  if (colors != NULL) {
    for (int i = 0; i < nb_rows * nb_cols; i++) {
      g->colors[i] = colors[i];
    }
  }

  else if (colors == NULL) {
    for (int i = 0; i < nb_rows * nb_cols; i++) {
      g->colors[i] = EMPTY;
    }
  }
  return g;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        neighbourhood neigh) {  // Anissa
  game g = malloc(sizeof(struct game_s));
  g->height = nb_rows;
  g->width = nb_cols;
  g->s_do = queue_new();
  g->s_undo = queue_new();
  g->wrapping = wrapping;
  g->neigh = neigh;
  g->history = malloc(sizeof(moves));
  g->constraints = malloc(nb_rows * nb_cols * sizeof(constraint));
  g->colors = malloc(nb_rows * nb_cols * sizeof(color));
  for (int i = 0; i < nb_rows * nb_cols; i++) {
    g->colors[i] = EMPTY;
    g->constraints[i] = UNCONSTRAINED;
  }
  return g;
}

uint game_nb_rows(cgame g) {  // ANISSA
  return g->height;
}

uint game_nb_cols(cgame g) {  // ANISSA
  return g->width;
}

bool game_is_wrapping(cgame g) {  // NICK
  return g->wrapping;
}

neighbourhood game_get_neighbourhood(cgame g) { return g->neigh; };  // NICK

void game_undo(game g) {  // Anissa
  if (queue_is_empty(g->s_do)) {
    fprintf(stderr, "no move yet nothing to undo\n");
    return;
  }
  // Pop last move from s_do
  moves *last_move = queue_pop_head(g->s_do);
  // Push undone move to  s_undo
  queue_push_head(g->s_undo, last_move);
  // Restore previous color
  game_set_color(g, last_move->i, last_move->j, last_move->before);
}

void game_redo(game g) {  // Anissa
  if (queue_is_empty(g->s_undo)) {
    fprintf(stderr, "no undone move yet nothing to redo\n");
    return;
  }
  // Pops the last undone move from s_undo
  moves *redone_move = queue_pop_head(g->s_undo);
  // Sets color to  color after the move
  game_set_color(g, redone_move->i, redone_move->j, redone_move->after);
  // Push the redone move back to s_do
  queue_push_head(g->s_do, redone_move);
}
