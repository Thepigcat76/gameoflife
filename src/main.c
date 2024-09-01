#include <unistd.h>

#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

#define SCREEN_W 1920
#define SCREEN_H 1080

#define WINDOW_NAME "Game of life"

#define FPS 60

#define CELL_W 25
#define CELL_H 25

#define CELL_COLS SCREEN_H / CELL_H
#define CELL_ROWS SCREEN_W / CELL_W

typedef struct {
  int32_t x;
  int32_t y;
} Vec2i;

typedef struct {
  bool cells[CELL_COLS][CELL_ROWS];
  bool next_gen[CELL_COLS][CELL_ROWS];
  bool paused;
} Game;

Game new_game() {
  Game game;
  for (int16_t y = 0; y < CELL_COLS; y++) {
    for (int16_t x = 0; x < CELL_ROWS; x++) {
      game.cells[y][x] = false;
      game.next_gen[y][x] = false;
    }
  }
  return game;
}

void create_next_cell(Game *game, int16_t x, int16_t y);

void kill_next_cell(Game *game, int16_t x, int16_t y);

void add_cell(Game *game, int16_t x, int16_t y);

void remove_cell(Game *game, int16_t x, int16_t y);

void run_game(Game *game);

void render_game(Game *game);

void update_generation(Game *game) {
  for (int16_t y = 0; y < CELL_COLS; y++) {
    for (int16_t x = 0; x < CELL_ROWS; x++) {
      game->cells[y][x] = game->next_gen[y][x];
      game->next_gen[y][x] = false;
    }
  }
}

int16_t get_alive_neighbors(Game *game, int16_t origin_cell_x,
                            int16_t origin_cell_y) {
  int16_t amount = 0;
  for (int8_t y = -1; y < 2; y++) {
    for (int8_t x = -1; x < 2; x++) {
      int8_t x_new = x + origin_cell_x;
      int8_t y_new = y + origin_cell_y;
      if (x_new >= 0 && y_new >= 0) {
        if (game->cells[y_new][x_new]) {
          amount++;
        }
      }
    }
  }
  amount -= game->cells[origin_cell_y][origin_cell_x];
  return amount;
}

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);

  InitWindow(SCREEN_W, SCREEN_H, WINDOW_NAME);

  SetTargetFPS(FPS);

  SetExitKey(0);

  Game game = new_game();

  int16_t timer = 0;

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_ESCAPE)) {
      game.paused = !game.paused;
    }

    float delta = GetFrameTime();
    if (game.paused) {
      if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        int16_t x = GetMouseX() / CELL_W;
        int16_t y = GetMouseY() / CELL_H;

        if (!game.cells[y][x]) {
          add_cell(&game, x, y);
        }
      } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        int16_t x = GetMouseX() / CELL_W;
        int16_t y = GetMouseY() / CELL_H;

        if (game.cells[y][x]) {
          remove_cell(&game, x, y);
        }
      }
    } else if (timer > 20) {
      run_game(&game);
      update_generation(&game);
      timer = 0;
    }

    render_game(&game);
    timer++;
  }

  CloseWindow();
}

void run_game(Game *game) {
  for (int16_t y = 0; y < CELL_COLS; y++) {
    for (int16_t x = 0; x < CELL_ROWS; x++) {
      int8_t amount = get_alive_neighbors(game, x, y);
      if (!game->cells[y][x] && amount == 3) {
        game->next_gen[y][x] = true;
      } else if (game->cells[y][x] && (amount < 2 || amount > 3)) {
        game->next_gen[y][x] = false;
      } else {
        game->next_gen[y][x] = game->cells[y][x];
      }
    }
  }
}

void render_game(Game *game) {
  BeginDrawing();

  ClearBackground(BLACK);

  if (game->paused) {
    DrawText("Paused", 10, 10, 25, WHITE);
  }

  for (int16_t y = 0; y < CELL_COLS; y++) {
    for (int16_t x = 0; x < CELL_ROWS; x++) {
      if (game->cells[y][x]) {
        DrawRectangle(x * CELL_W, y * CELL_H, CELL_W, CELL_H, WHITE);
      }
    }
  }

  EndDrawing();
}

void create_next_cell(Game *game, int16_t x, int16_t y) {
  game->next_gen[y][x] = true;
}

void kill_next_cell(Game *game, int16_t x, int16_t y) {
  game->next_gen[y][x] = false;
}

void add_cell(Game *game, int16_t x, int16_t y) { game->cells[y][x] = true; }

void remove_cell(Game *game, int16_t x, int16_t y) {
  game->cells[y][x] = false;
}
