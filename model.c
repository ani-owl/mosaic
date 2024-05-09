
#include "model.h"

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_random.h"
#include "game_tools.h"

#define BUTTON_WIDTH_RATIO 0.1  // Adjust the ratio as needed
#define BUTTON_ASPECT_RATIO 1.0
#define NUM_BUTTONS 7  // Define the total number of buttons
#define BACKGROUND "res/game.jpg"
#define BUTTON "res/button.jpg"
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
/* **************************************************************** */

typedef enum {
  UNDO_BUTTON,
  REDO_BUTTON,
  RESTART_BUTTON,
  SOLVE_button,
  RANDOM_BUTTON,
  SAVE_BUTTON,
  QUIT_BUTTON
} ButtonType;

typedef struct {
  int x;       // x-coordinate of the button
  int y;       // y-coordinate of the button
  int width;   // Width of the button
  int height;  // Height of the button
  bool clicked;
} Button;  // button to presss to play a move

struct Env_t {
  SDL_Texture *background;
  SDL_Rect *rect;
  int RectSize;
  game g;
  Button *buttons;
  char *filename;  // file to save the game into
};

/* **************************************************************** */

void render_buttons(SDL_Window *win, SDL_Renderer *ren, Env *env, int marginX,
                    int marginY) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  // Array of button labels corresponding to each button type
  const char *buttonLabels[NUM_BUTTONS] = {"Undo",   "Redo", "Restart", "Solve",
                                           "Random", "Save", "Quit"};

  int initial_size = 18;  // Base text size
  double textScaleFactor = sqrt(
      (double)(w * h) / (800 * 600));  // calculate how big the text will change
                                       // depending on the window size
  int FontSize = (int)(initial_size * textScaleFactor);
  TTF_Font *Sans =
      TTF_OpenFont("res/arial.ttf", FontSize);  // Adjust font size as needed
  SDL_Color textColor = {255, 255, 255};

  // Load the button image
  SDL_Surface *buttonSurface = IMG_Load(BUTTON);
  SDL_Texture *buttonTexture = SDL_CreateTextureFromSurface(ren, buttonSurface);
  SDL_FreeSurface(buttonSurface);

  for (int i = 0; i < NUM_BUTTONS; i++) {
    // set button width and height based on window size
    env->buttons[i].width = (int)(w * BUTTON_WIDTH_RATIO) + 20;
    env->buttons[i].height =
        (int)(env->buttons[i].width * BUTTON_ASPECT_RATIO) - 30;

    // set coressponding button position
    int posX, posY;
    if (i == RANDOM_BUTTON) {
      posX = w / 2 - env->buttons[i].width / 2;  // Center horizontally
      posY = h - 0.99 * h;                       // Above the game grid
    } else if (i == SAVE_BUTTON) {
      posX = w - 0.99 * w;  // Left
      posY = h - 0.99 * h;  // Top
    } else if (i == QUIT_BUTTON) {
      posX = w - env->buttons[i].width;  // Right
      posY = h - 0.99 * h;               // Top
    } else {
      // For other buttons, position them below the game grid
      posX =
          marginX + i * (1.9 * env->buttons[i].width) - env->buttons[i].width;
      posY = marginY + env->RectSize * game_nb_rows(env->g) + marginY / 2 -
             env->buttons[i].height / 2;
    }

    env->buttons[i].x = posX;
    env->buttons[i].y = posY;

    // Render button rectangle
    SDL_Rect buttonRect = {posX, posY, env->buttons[i].width,
                           env->buttons[i].height};
    SDL_RenderCopy(ren, buttonTexture, NULL, &buttonRect);

    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(
        Sans, buttonLabels[i], textColor);  // using the button label array
    SDL_Texture *message = SDL_CreateTextureFromSurface(ren, surfaceMessage);

    // Positioning  text in the center of the button
    SDL_Rect textRect;
    textRect.x = posX + (env->buttons[i].width - surfaceMessage->w) / 2;
    textRect.y = posY + (env->buttons[i].height - surfaceMessage->h) / 2;
    textRect.w = surfaceMessage->w;
    textRect.h = surfaceMessage->h;

    // Render text
    SDL_RenderCopy(ren, message, NULL, &textRect);

    // Free resources
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
  }
  SDL_DestroyTexture(buttonTexture);  // Destroy the button texture
  TTF_CloseFont(Sans);                // Close the font
}

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));
  PRINT("Game Instructions:\n");
  PRINT("To play the game, click on the grid cells:\n");
  PRINT("- Click once to fill a cell with white.\n");
  PRINT("- Click again to fill a cell with black.\n");
  PRINT("- Click a third time to empty a cell.\n");
  PRINT("Red cells indicate errors in your moves.\n");
  PRINT("Press the Save button to save your progress.\n");
  PRINT("Default save file is 'save.txt'.\n");
  PRINT(
      "You can specify a different file by providing it as the third "
      "argument.\n");
  PRINT(
      "Use the second argument to load a game from a file of your choice with "
      "options of your choice.\n");
  PRINT("Press Quit button or ESC to exit the game.\n");
  PRINT("Enjoy the mosaic game!\n \n");

  env->filename = NULL;
  // options of  game to load and save
  switch (argc) {
    case 1:
      env->g =
          game_default();  // if no additional arguments default game, and
                           // pressing on save button saves game into save.txt
      break;

    case 2:
      env->g = game_load(argv[1]);  // load game from file argv[1]
      PRINT("GAME LOADED FROM FILE : %s \n", argv[1]);
      break;
    case 3:
      env->g = game_load(argv[1]);  // first arg is game to be loaded,
      env->filename = argv[2];      // second arg  is where th game is saved
      PRINT("GAME LOADED FROM FILE : %s \n", argv[1]);
      break;
    default:
      fprintf(stderr, "Error: Invalid number of arguments.\n");
      break;
  }
  // handling error loading the game
  if (env->g == NULL) {
    fprintf(stderr, "Error: Failed to load or create game.\n");
    return NULL;
  }
  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* init background texture from  image */
  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  // Initialize the array of rectangles
  env->rect =
      malloc(game_nb_rows(env->g) * game_nb_cols(env->g) * sizeof(SDL_Rect));
  env->buttons = malloc(NUM_BUTTONS * sizeof(Button));

  return env;
}

/* **************************************************************** */
void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  TTF_Font *Sans = TTF_OpenFont("res/arial.ttf", 100);
  /* render background texture */
  SDL_RenderCopy(ren, env->background, NULL, NULL);

  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  // set rectangle size
  env->RectSize = MIN(w, h) / (game_nb_rows(env->g) + 4);
  // compute position of the first rectangle in position (0,0) for the grid to
  // be in center of the window
  int marginX = (w - game_nb_cols(env->g) * env->RectSize) / 2;
  int marginY = (h - game_nb_rows(env->g) * env->RectSize) / 2;

  for (int i = 0; i < game_nb_rows(env->g); i++) {
    for (int j = 0; j < game_nb_cols(env->g); j++) {
      // first position the rectangles correctly so that the grid is in the
      // center
      env->rect[i * game_nb_cols(env->g) + j].x = j * env->RectSize + marginX;
      env->rect[i * game_nb_cols(env->g) + j].y = i * env->RectSize + marginY;
      env->rect[i * game_nb_cols(env->g) + j].w = env->RectSize;
      env->rect[i * game_nb_cols(env->g) + j].h = env->RectSize;

      // choose color according to the color set from process
      if (game_get_color(env->g, i, j) == EMPTY) {
        SDL_SetRenderDrawColor(ren, 37, 1, 73, 255);  // purple
      } else if (game_get_color(env->g, i, j) == WHITE) {
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);  // White
      } else if (game_get_color(env->g, i, j) == BLACK) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);  // Black
      } else {
        // error
      }

      if (game_get_status(env->g, i, j) == ERROR)  // check game status
        SDL_SetRenderDrawColor(ren, 255, 0, 0,
                               255);  // color cell in red if error in move
      // fill the color in cells
      SDL_RenderFillRect(ren, &env->rect[i * game_nb_cols(env->g) + j]);

      // drawing the constraint based on the color of the cell
      int constraint = game_get_constraint(env->g, i, j);
      if (constraint != -1) {
        SDL_Color constraintColor;

        if (game_get_color(env->g, i, j) == WHITE) {
          constraintColor.r = 0;  // color constraint in black
          constraintColor.g = 0;
          constraintColor.b = 0;
        } else if (game_get_color(env->g, i, j) == BLACK) {
          constraintColor.r = 255;  // color constraint in white
          constraintColor.g = 255;
          constraintColor.b = 255;
        } else {
          constraintColor.r = 152;  // empty is colored with purple
          constraintColor.g = 114;
          constraintColor.b = 191;
        }

        // Render text
        char constraintStr[10];  // Assuming constraints are within 10 digits
        snprintf(constraintStr, 10, "%d", constraint);
        SDL_Surface *surfaceMessage =
            TTF_RenderText_Solid(Sans, constraintStr, constraintColor);
        SDL_Texture *cons = SDL_CreateTextureFromSurface(ren, surfaceMessage);

        // Position the text in the center of the square
        SDL_Rect textRect;
        textRect.w = env->RectSize - 25;  // controls the width of the rect
        textRect.h = env->RectSize - 25;
        textRect.x = env->rect[i * game_nb_cols(env->g) + j].x +
                     (env->RectSize - textRect.w) / 2;
        textRect.y = env->rect[i * game_nb_cols(env->g) + j].y +
                     (env->RectSize - textRect.h) / 2;

        // Render text onto the screen
        SDL_RenderCopy(ren, cons, NULL, &textRect);
        SDL_FreeSurface(surfaceMessage);  // Free the surface
        SDL_DestroyTexture(cons);         // Destroy the texture
      }

      // draw the rectangle to show the grid
      SDL_SetRenderDrawColor(ren, 152, 114, 191, 255);
      SDL_RenderDrawRect(
          ren, &env->rect[i * game_nb_cols(env->g) + j]);  // Draw the rectangle
    }
  }
  if (game_won(env->g)) {
    TTF_Font *Sans = TTF_OpenFont("res/arial.ttf", 50);
    TTF_SetFontStyle(Sans, TTF_STYLE_BOLD);
    SDL_Color textColor = {152, 114, 191};
    const char *winMessage = "You have won!";
    SDL_Surface *surfaceMessage =
        TTF_RenderText_Solid(Sans, winMessage, textColor);
    SDL_Texture *winMessageTexture =
        SDL_CreateTextureFromSurface(ren, surfaceMessage);

    // Calculate position to center the message on the screen
    int textWidth, textHeight;
    SDL_QueryTexture(winMessageTexture, NULL, NULL, &textWidth, &textHeight);
    int posX = (w - textWidth) / 2;
    int posY = (h - textHeight) / 2;

    // Render the message
    SDL_Rect textRect = {posX, posY, textWidth, textHeight};
    SDL_RenderCopy(ren, winMessageTexture, NULL, &textRect);

    // Free resources
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(winMessageTexture);
    TTF_CloseFont(Sans);
  }
  render_buttons(win, ren, env, marginX, marginY);
}
/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (e->type == SDL_QUIT) {
    return true;  // Quit the application
  }

  if (e->type == SDL_MOUSEBUTTONDOWN) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Handle button clicks
    for (int i = 0; i < NUM_BUTTONS; i++) {
      if (mouseX >= env->buttons[i].x &&
          mouseX <= env->buttons[i].x + env->buttons[i].width &&
          mouseY >= env->buttons[i].y &&
          mouseY <= env->buttons[i].y + env->buttons[i].height) {
        switch (i) {
          case UNDO_BUTTON:
            game_undo(env->g);
            break;

          case REDO_BUTTON:
            game_redo(env->g);
            break;

          case RESTART_BUTTON:
            game_restart(env->g);
            break;
          case SOLVE_button:
            game_solve(env->g);
            break;
          case RANDOM_BUTTON:
            env->g =
                game_random(game_nb_rows(env->g), game_nb_cols(env->g),
                            game_is_wrapping(env->g),
                            game_get_neighbourhood(env->g), false, 0.5, 0.4);
            PRINT("RANDOM GAME GENERATED\n");
            break;
          case SAVE_BUTTON:
            if (env->filename != NULL) {
              game_save(env->g, env->filename);
              PRINT("CURRENT GAME STATE  SAVED INTO %s\n", env->filename);
            } else {
              game_save(env->g, "save.txt");
              PRINT("CURRENT GAME STATE  SAVED INTO save.txt\n");
            }
            break;
          case QUIT_BUTTON:
            if (game_won(env->g) == true) {
              PRINT("CONGRATULATIONS YOU WON THE GAME! \n");
            } else {
              PRINT("SHAME!\n");
            }

            PRINT("YOU HAVE QUIT THE GAME , GOODBYE !! UNTIL ANOTHER TIME\n");
            return true;

            break;

          default:
            break;
        }
      }
      env->buttons[i].clicked = true;
    }

    // Check if any cell is clicked
    for (int i = 0; i < game_nb_rows(env->g); i++) {
      for (int j = 0; j < game_nb_cols(env->g); j++) {
        SDL_Rect cell = env->rect[i * game_nb_cols(env->g) + j];

        if (mouseX >= cell.x && mouseX <= cell.x + cell.w && mouseY >= cell.y &&
            mouseY <= cell.y + cell.h) {
          if (game_get_color(env->g, i, j) == EMPTY) {
            game_play_move(env->g, i, j, WHITE);
          } else if (game_get_color(env->g, i, j) == WHITE) {
            game_play_move(env->g, i, j, BLACK);
          } else {
            game_play_move(env->g, i, j, EMPTY);
          }
        }
      }
    }
  }

  if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
      case SDLK_ESCAPE:
        return true;  // Quit the application
        break;
    }
  }

  return false;  // Continue running the application
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  SDL_DestroyTexture(env->background);
  free(env->rect);      // Clean up rectangles array
  free(env->buttons);   // Clean up buttons array
  game_delete(env->g);  // Clean up game state
  free(env);
}

/* **************************************************************** */
