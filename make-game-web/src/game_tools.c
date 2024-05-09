#include "game_tools.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "assert.h"

game game_load(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "couldn't load file!!");
    exit(EXIT_FAILURE);
  }
  uint nb_rows = 0;
  uint nb_cols = 0;
  uint wrapping = 0;
  uint neigh = 0;
  int scanned =
      fscanf(file, "%u %u %u %u\n", &nb_rows, &nb_cols, &wrapping, &neigh);
  // Check if all four values were successfully scanned
  if (scanned != 4) {
    if (scanned == EOF) {
      fprintf(stderr, "End of file reached before reading all parameters");
    } else {
      fprintf(stderr, "Error reading parameters from file");
    }
    fclose(file);
    exit(EXIT_FAILURE);
  }
  if ((wrapping != 0 && wrapping != 1) ||
      (neigh != 0 && neigh != 1 && neigh != 2 && neigh != 3)) {
    fprintf(stderr, "not the right parameters of the game");
    exit(EXIT_FAILURE);
  }
  switch (wrapping) {
    case 0:
      wrapping = FULL;
      break;

    case 1:
      wrapping = ORTHO;
      break;
    case 2:
      wrapping = ORTHO_EXCLUDE;
      break;
    case 3:
      wrapping = FULL_EXCLUDE;
      break;
  }
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, neigh);
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      char constraint;
      char color;
      int scanned = fscanf(file, "%c%c", &constraint, &color);
      if (scanned != 2) {
        if (scanned == EOF) {
          fprintf(stderr, "End of file reached before reading all parameters");
        } else {
          fprintf(stderr, "Error reading parameters from file");
        }
        fclose(file);
        exit(EXIT_FAILURE);
      }

      if (constraint == '-') {
        game_set_constraint(g, i, j, UNCONSTRAINED);
      } else
        game_set_constraint(g, i, j, constraint - '0');

      switch (color) {
        case 'e':
          game_set_color(g, i, j, EMPTY);
          break;
        case 'w':
          game_set_color(g, i, j, WHITE);
          break;
        case 'b':
          game_set_color(g, i, j, BLACK);
          break;
      }
    }
    fgetc(file);
  }
  fclose(file);
  return g;
}

void game_save(cgame g, char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    exit(EXIT_FAILURE);
  }
  fprintf(file, "%u %u %u %u\n", game_nb_rows(g), game_nb_cols(g),
          game_is_wrapping(g), game_get_neighbourhood(g));

  for (uint i = 0; i < game_nb_rows(g); i++) {
    for (uint j = 0; j < game_nb_cols(g); j++) {
      if (game_get_constraint(g, i, j) == UNCONSTRAINED) {
        fprintf(file, "-");
      } else {
        fprintf(file, "%c", game_get_constraint(g, i, j) + '0');
      }

      switch (game_get_color(g, i, j)) {
        case EMPTY:
          fprintf(file, "%c", 'e');
          break;
        case WHITE:
          fprintf(file, "%c", 'w');
          break;
        case BLACK:
          fprintf(file, "%c", 'b');
          break;
        default:
          break;
      }
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

// fonction pour vérifier le status des voisins ,for 0 it starts by checking not
// the previous neighbors but the neighbors after on the right , it depends on
// the kind of place you're at so you do specefic places first then

// so to optimize

// the game_solve i'll have to check each time i do a moove if the status is
// satisfied with the right neighbors , if there's an error just stop the
// current combination and move to the next one ,
bool has_error(cgame g, uint i, uint j) {
  status current_status = game_get_status(g, i, j);

  if (current_status == ERROR) {
    return true;  // If current square has error return true
  }

  // Checking the status of neighbours
  for (direction d = HERE; d <= DOWN_RIGHT; d++) {
    uint i_next, j_next;
    if (game_get_next_square(g, i, j, d, &i_next, &j_next)) {
      status neighbour_status = game_get_status(g, i_next, j_next);

      // Check if  neighbor has  error
      if (neighbour_status == ERROR) {
        return true;
      }
    }
  }

  return false;  // No error found neither in current square nor the neighbours
}

bool gen_combinations(int pos, int len, game g) {
  int register i = pos / game_nb_cols(g);
  int register j = pos % game_nb_cols(g);
  if (pos == len) {
    if (game_won(g)) return true;
    return false;
  }

  game_set_color(g, i, j, WHITE);
  if (!has_error(g, i, j) &&
      gen_combinations(pos + 1, len,
                       g)) {  // if no error in current square and its neighbors
                              // continue with combinasion
    return true;
  }
  game_set_color(g, i, j, EMPTY);

  game_set_color(g, i, j, BLACK);
  if (!has_error(g, i, j) && gen_combinations(pos + 1, len, g)) {
    return true;
  }
  game_set_color(g, i, j,
                 EMPTY);  // reset the color to empty in case it was error and
                          // the combinasion  is not a solution
  return false;

}  // do the thing is , instead of creating a new game every single time ,
//, i just have to create one a t the beginning and then change the  colors
// directly with game_set_colors and convert from the position in the linear to
// get i and j and use set color

bool game_solve(game g) {
  int pos = 0;
  int len = game_nb_cols(g) * game_nb_rows(g);
  if (gen_combinations(pos, len, g)) {
    return true;
  }
  game_restart(g);
  return false;
}

void gen_combinations_count(int pos, int len, game g, uint *count) {
  if (pos == len) {
    if (game_won(g)) {
      (*count)++;

      return;
    }
    return;
  }
  int i = pos / game_nb_cols(g);
  int j = pos % game_nb_cols(g);
  game_set_color(g, i, j, WHITE);
  if (!has_error(g, i, j)) {
    gen_combinations_count(pos + 1, len, g, count);
  }
  game_set_color(g, i, j, EMPTY);

  game_set_color(g, i, j, BLACK);
  if (!has_error(g, i, j)) {
    gen_combinations_count(pos + 1, len, g, count);
  }
  game_set_color(g, i, j, EMPTY);
}

uint game_nb_solutions(game g) {
  uint count = 0;
  int pos = 0;
  int len = game_nb_cols(g) * game_nb_rows(g);

  gen_combinations_count(pos, len, g, &count);

  game_restart(g);

  return count;
}
game game_random(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh,
                 bool with_solution, float black_rate, float constraint_rate) {
  assert(black_rate >= 0.0f && black_rate <= 1.0f);
  assert(constraint_rate >= 0.0f && constraint_rate <= 1.0f);
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, neigh);
  assert(g);

  // fill the grid with random colors
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      color c = (rand() < black_rate * (float)RAND_MAX) ? BLACK : WHITE;
      game_set_color(g, i, j, c);
    }
  }

  // fill the grid with actual constraint at random positions
  uint nb_squares = nb_rows * nb_cols;
  uint nb_constraints = constraint_rate * nb_squares;
  for (uint i = 0; i < nb_constraints; i++) {
    uint row = rand() % nb_rows;
    uint col = rand() % nb_cols;
    int nb_blacks = game_nb_neighbors(g, row, col, BLACK);
    game_set_constraint(g, row, col, nb_blacks);
  }

  // check solution
  if (!game_won(g)) {
    game_delete(g);
    return NULL;
  }

  if (!with_solution) game_restart(g);
  return g;
}

/* ************************************************************************** */