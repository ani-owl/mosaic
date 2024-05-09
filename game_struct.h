#ifndef __GAME_STRUCT_H__
#define __GAME_STRUCT_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"
typedef struct history_moves {
  color before;
  color after;
  uint i;
  uint j;
} moves;

struct game_s {
  moves *history;
  queue *s_do;    // queue that stocks the done moves
  queue *s_undo;  // the undone moves
  bool wrapping;
  neighbourhood neigh;
  uint height;
  uint width;
  constraint *constraints;
  color *colors;
};

#endif