#ifndef __GAME_EXT_H__
#define __GAME_EXT_H__

#include <stdbool.h>

#include "game.h"
typedef enum { FULL, ORTHO, FULL_EXCLUDE, ORTHO_EXCLUDE } neighbourhood;

/**
 * @brief Creates a new game with extended options and initializes it.
 *
 * @param nb_rows Number of rows in the game.
 * @param nb_cols Number of columns in the game.
 * @param constraints An array describing the initial constraint of each square
 * using row-major storage (of size nb_rows*nb_cols).
 * @param colors An array describing the initial color of each square using
 * row-major storage (of size nb_rows*nb_cols) or NULL to set all colors as
 * EMPTY.
 * @param wrapping Wrapping option.
 * @param neigh Neighbourhood option.
 * @pre constraints must be an initialized array of default size squared.
 * @pre colors must be an initialized array of default size squared or NULL.
 * @return The created game.
 */
game game_new_ext(uint nb_rows, uint nb_cols, constraint *constraints,
                  color *colors, bool wrapping, neighbourhood neigh);
/**
 * @brief Creates a new empty game with extended options.
 *
 * All squares are initialized with empty squares.
 *
 * @param nb_rows Number of rows in the game.
 * @param nb_cols Number of columns in the game.
 * @param wrapping Wrapping option.
 * @param neigh Neighbourhood option.
 * @return The created game.
 */
game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        neighbourhood neigh);

/**
 * @brief Gets the number of rows (or height).
 *
 * @param g The game.
 * @return The number of rows on this game.
 * @pre g is a valid pointer toward a cgame structure.
 */
uint game_nb_rows(cgame g);

/**
 * @brief Gets the number of columns (or width).
 *
 * @param g The game.
 * @return The number of columns on this game.
 * @pre g is a valid pointer toward a cgame structure.
 */
uint game_nb_cols(cgame g);

/**
 * @brief Checks if the game has the wrapping option.
 *
 * @param g The game.
 * @return true, if wrapping option is enabled, false otherwise.
 * @pre g is a valid pointer toward a cgame structure.
 */
bool game_is_wrapping(cgame g);

/**
 * @brief Gets the neighbourhood option.
 *
 * @param g The game.
 * @return The neighbourhood option.
 * @pre g is a valid pointer toward a cgame structure.
 */
neighbourhood game_get_neighbourhood(cgame g);

/**
 * @brief Undoes the last move.
 *
 * Searches in the history for the last move played (by calling game_play_move
 * or game_redo), and restores the state of the game before that move. If no
 * moves have been played, this function does nothing. The game_restart function
 * clears the history.
 *
 * @param g The game.
 * @pre g is a valid pointer toward a cgame structure.
 */
void game_undo(game g);

/**
 * @brief Redoes the last move.
 *
 * Searches in the history for the last cancelled move (by calling game_undo),
 * and replays it. If there are no more moves to be replayed, this function does
 * nothing. After playing a new move with game_play_move, it is no longer
 * possible to redo an old cancelled move.
 *
 * @param g The game.
 * @pre g is a valid pointer toward a cgame structure.
 */
void game_redo(game g);

#endif  // __GAME_EXT_H__