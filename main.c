#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800
#define GRID_SIZE 40
#define CELL_WIDTH SCREEN_WIDTH / GRID_SIZE
#define CELL_HEIGHT SCREEN_HEIGHT / GRID_SIZE
#define IS_TEST false

typedef enum State
{
  PLAYING = 1,
  PAUSE = 2,
  END = 3
} State;

typedef struct Apple
{
  int x;
  int y;
  int size;
  Color color;
  Texture2D texture;
} Apple;

typedef struct snake
{
  Vector2 body[GRID_SIZE * GRID_SIZE];
  int size;
  Color color;
  Vector2 dir;
  Texture2D headTexture[4];
} Snake;
typedef struct game
{
  Apple apple;
  Snake snake;
  int Score;
  double lastUpdateTime;
  char *scoreText;
  State state;
} Game;

Game createGame()
{
  Game game = {
      .Score = 0,
      .state = PLAYING,
  };
  game.scoreText = malloc(10);
  return game;
}

void renderGrid()
{
#if (IS_TEST)
  for (int i = 0; i < SCREEN_WIDTH; i += CELL_WIDTH)
  {
    for (int ii = 0; ii < SCREEN_HEIGHT; ii += CELL_HEIGHT)
    {
      DrawRectangleLines(i, ii, CELL_WIDTH, CELL_HEIGHT, GRAY);
    }
  }
#endif
  return;
}

void renderApple(Apple apple)
{
  int x = apple.x * CELL_WIDTH + CELL_WIDTH / 2 - apple.size / 2;
  int y = apple.y * CELL_HEIGHT + CELL_HEIGHT / 2 - apple.size / 2;
  // DrawRectangle(x, y, apple.size, apple.size, apple.color);
  DrawTexture(apple.texture, x, y, WHITE);
}

Apple createApple(Game *game)
{
  Apple apple = {
      .size = 25,
      .color = RED,
      .texture = game->apple.texture,
  };
  while (true)
  {
    apple.y = GetRandomValue(0, GRID_SIZE - 1);
    apple.x = GetRandomValue(0, GRID_SIZE - 1);
    bool isInSnake = false;
    for (int i = 0; i < game->snake.size; i++)
    {
      if (apple.x == game->snake.body[i].x && apple.y == game->snake.body[i].y)
      {
        isInSnake = true;
        break;
      }
    }
    if (isInSnake)
    {
      continue;
    }
    break;
  }
  return apple;
}

void renderSnake(Snake snake)
{
  for (int i = snake.size - 1; i >= 0; i--)
  {
    int x = snake.body[i].x * CELL_WIDTH;
    int y = snake.body[i].y * CELL_HEIGHT;
    Color color = snake.color;
    if (i == 0)
    {
      int headPath = 0;
      if (snake.dir.y == 1)
      {
        headPath = 1;
      }
      if (snake.dir.x == -1)
      {
        headPath = 2;
      }
      if (snake.dir.x == 1)
      {
        headPath = 3;
      }
      DrawTexture(snake.headTexture[headPath], x, y, WHITE);
    }
    else
    {
      DrawRectangle(x, y, CELL_WIDTH, CELL_HEIGHT, color);
    }
  }
  return;
}

Snake createSnake()
{
  Snake snake;
  snake.size = 3;
  snake.color = (Color){91, 123, 249, 255};
  snake.body[0] = (Vector2){3, 1};
  snake.body[1] = (Vector2){2, 1};
  snake.body[2] = (Vector2){1, 1};
  snake.dir = (Vector2){1, 0};
  return snake;
}

void updateSnake(Snake *snake)
{
  Vector2 lastPos = snake->body[0];
  snake->body[0].x += snake->dir.x;
  snake->body[0].y += snake->dir.y;
  if (snake->body[0].x >= GRID_SIZE)
  {
    snake->body[0].x = 0;
  }
  if (snake->body[0].x < 0)
  {
    snake->body[0].x = GRID_SIZE - 1;
  }

  if (snake->body[0].y >= GRID_SIZE)
  {
    snake->body[0].y = 0;
  }
  if (snake->body[0].y < 0)
  {
    snake->body[0].y = GRID_SIZE - 1;
  }

  for (int i = 1; i < snake->size; i++)
  {
    Vector2 holder = snake->body[i];
    snake->body[i] = lastPos;
    lastPos = holder;
  }
  return;
}

void growSnake(Snake *snake)
{
  snake->size++;
  Vector2 newHead = {snake->body[0].x + snake->dir.x,
                     snake->body[0].y + snake->dir.y};
  Vector2 lastBody = snake->body[0];
  snake->body[0] = newHead;
  for (int i = 1; i < snake->size; i++)
  {
    Vector2 holder = snake->body[i];
    snake->body[i] = lastBody;
    lastBody = holder;
  }
  return;
}

bool isEating(Snake snake, Apple apple)
{
  return snake.body[0].x == apple.x && snake.body[0].y == apple.y;
}

bool checkCollision(Snake snake)
{
  Vector2 head = snake.body[0];
  for (int i = 1; i < snake.size; i++)
  {
    if (snake.body[i].x == head.x && snake.body[i].y == head.y)
    {
      return true;
    }
  }
  return false;
}
bool triggerEvent(double *lastUpdateTime, double interval)
{
  double currentTime = GetTime();
  if (currentTime - interval >= *lastUpdateTime)
  {
    *lastUpdateTime = currentTime;
    return true;
  }
  return false;
}

// State
void resetGame(Game *game)
{
  game->snake = createSnake();
  game->apple = createApple(game);
  game->Score = 0;
  game->state = PLAYING;
}

void runGame(Game *game)
{
  switch (GetKeyPressed())
  {
  case KEY_UP:
  case KEY_W:
  {
    if (game->snake.dir.y == 1)
    {
      break;
    }
    game->snake.dir = (Vector2){0, -1};
  }
  break;
  case KEY_DOWN:
  case KEY_S:
  {
    if (game->snake.dir.y == -1)
    {
      break;
    }
    game->snake.dir = (Vector2){0, 1};
  }
  break;
  case KEY_LEFT:
  case KEY_A:
  {
    if (game->snake.dir.x == 1)
    {
      break;
    }
    game->snake.dir = (Vector2){-1, 0};
  }
  break;
  case KEY_RIGHT:
  case KEY_D:
  {
    if (game->snake.dir.x == -1)
    {
      break;
    }
    game->snake.dir = (Vector2){1, 0};
  }
  break;
  case KEY_SPACE:
  {
    game->state = PAUSE;
  }
  break;
  }

  if (triggerEvent(&game->lastUpdateTime, 0.1))
  {
    updateSnake(&game->snake);
  }

  if (isEating(game->snake, game->apple))
  {
    game->Score++;
    growSnake(&game->snake);
    game->apple = createApple(game);
  }
  if (checkCollision(game->snake))
  {
    game->state = END;
  }

  BeginDrawing();
  ClearBackground((Color){0, 0, 0, 1});
  renderGrid();
  renderApple(game->apple);
  renderSnake(game->snake);
  EndDrawing();
}

void pauseGame(Game *game)
{
  switch (GetKeyPressed())
  {
  case KEY_SPACE:
  {
    game->state = PLAYING;
  }
  break;
  }

  BeginDrawing();
  ClearBackground((Color){0, 0, 0, 1});
  renderGrid();
  renderApple(game->apple);
  renderSnake(game->snake);

  char *pauseTitle = "Paused";
  int titleSize = 50;
  int titleWidth = MeasureText(pauseTitle, titleSize);
  char *pauseText = "Press <Space> to continue";
  int textSize = 25;
  int textWidth = MeasureText(pauseText, textSize);
  DrawText(pauseTitle, (SCREEN_WIDTH / 2) - titleWidth / 2, (SCREEN_HEIGHT / 2) - titleSize / 2, titleSize, WHITE);
  DrawText(pauseText, (SCREEN_WIDTH / 2) - textWidth / 2, (SCREEN_HEIGHT / 2) - textSize / 2 + 60, textSize, WHITE);

  EndDrawing();
}

void endGame(Game *game)
{
  switch (GetKeyPressed())
  {
  case KEY_SPACE:
  {
    resetGame(game);
  }
  break;
  }

  BeginDrawing();
  ClearBackground((Color){0, 0, 0, 1});
  renderGrid();

  char *gameOverText = "Game Over";
  int titleSize = 50;
  int titleWidth = MeasureText(gameOverText, titleSize);
  char *pauseText = "Press <Space> to start new game";
  int textSize = 25;
  int textWidth = MeasureText(pauseText, textSize);

  sprintf(game->scoreText, "Score: %d", game->Score);
  int scoreSize = 25;
  int scoreWidth = MeasureText(game->scoreText, textSize);
  DrawText(gameOverText, (SCREEN_WIDTH / 2) - titleWidth / 2, (SCREEN_HEIGHT / 3), titleSize, WHITE);
  DrawText(pauseText, (SCREEN_WIDTH / 2) - textWidth / 2, (SCREEN_HEIGHT / 3) + 60, textSize, WHITE);
  DrawText(game->scoreText, (SCREEN_WIDTH / 2) - scoreWidth / 2, (SCREEN_HEIGHT / 3) + 100, textSize, GREEN);

  EndDrawing();
}

Texture2D getTextureFromAssets(char *filePath)
{
  Image image = LoadImage(filePath);
  if (image.format == 0)
  {
    printf("ERROR: Can not load %s\n", filePath);
    exit(1);
  }
  ImageResize(&image, CELL_WIDTH, CELL_HEIGHT);
  Texture2D texture = LoadTextureFromImage(image);
  UnloadImage(image);
  return texture;
}

int main()
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake_c");
  SetTargetFPS(60);
  Game game = createGame();
  game.snake = createSnake();
  game.snake.headTexture[0] = getTextureFromAssets("./assets/head_up.png");
  game.snake.headTexture[1] = getTextureFromAssets("./assets/head_down.png");
  game.snake.headTexture[2] = getTextureFromAssets("./assets/head_left.png");
  game.snake.headTexture[3] = getTextureFromAssets("./assets/head_right.png");
  game.apple = createApple(&game);
  game.apple.texture = getTextureFromAssets("./asstes/apple.png");
  while (!WindowShouldClose())
  {
    if (game.state == PLAYING)
    {
      runGame(&game);
    }
    else if (game.state == PAUSE)
    {
      pauseGame(&game);
    }
    else if (game.state == END)
    {
      endGame(&game);
    }
  }

  return 0;
}