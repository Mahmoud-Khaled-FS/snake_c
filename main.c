#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 800
#define GRID_SIZE 40
#define CELL_WIDTH SCREEN_WIDTH / GRID_SIZE
#define CELL_HEIGHT SCREEN_HEIGHT / GRID_SIZE
#define IS_TEST false

typedef struct Apple
{
  int x;
  int y;
  int size;
  Color color;
} Apple;

typedef struct main
{
  Vector2 body[GRID_SIZE * GRID_SIZE];
  int size;
  Color color;
  Vector2 dir;
} Snake;

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
  DrawRectangle(x, y, apple.size, apple.size, apple.color);
}

Apple createApple(Snake snake)
{
  Apple apple = {
      .size = 20,
      .color = RED,
  };
  while (true)
  {
    apple.y = GetRandomValue(0, GRID_SIZE - 1);
    apple.x = GetRandomValue(0, GRID_SIZE - 1);
    bool isInSnake = false;
    for (int i = 0; i < snake.size; i++)
    {
      if (apple.x == snake.body[i].x && apple.y == snake.body[i].y)
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
      color = YELLOW;
    }
    DrawRectangle(x, y, CELL_WIDTH, CELL_HEIGHT, color);
  }
  return;
}

Snake createSnake()
{
  Snake snake;
  snake.size = 3;
  snake.color = GREEN;
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
const int PAUSE_STATE = 1;
const int PLAYING_STATE = 2;
const int END_STATE = 3;

int state = PLAYING_STATE;
double lastUpdateTime;
Apple apple;
Snake snake;
int score;
char *scoreText;

void runGame()
{
  switch (GetKeyPressed())
  {
  case KEY_UP:
  case KEY_W:
  {
    if (snake.dir.y == 1)
    {
      break;
    }
    snake.dir = (Vector2){0, -1};
  }
  break;
  case KEY_DOWN:
  case KEY_S:
  {
    if (snake.dir.y == -1)
    {
      break;
    }
    snake.dir = (Vector2){0, 1};
  }
  break;
  case KEY_LEFT:
  case KEY_A:
  {
    if (snake.dir.x == 1)
    {
      break;
    }
    snake.dir = (Vector2){-1, 0};
  }
  break;
  case KEY_RIGHT:
  case KEY_D:
  {
    if (snake.dir.x == -1)
    {
      break;
    }
    snake.dir = (Vector2){1, 0};
  }
  break;
  case KEY_SPACE:
  {
    state = PAUSE_STATE;
  }
  break;
  }

  if (triggerEvent(&lastUpdateTime, 0.1))
  {
    updateSnake(&snake);
  }

  if (isEating(snake, apple))
  {
    score++;
    growSnake(&snake);
    apple = createApple(snake);
  }
  if (checkCollision(snake))
  {
    state = END_STATE;
  }

  BeginDrawing();
  ClearBackground((Color){0, 0, 0, 1});
  renderGrid();
  renderApple(apple);
  renderSnake(snake);
  EndDrawing();
}

void pauseGame()
{
  switch (GetKeyPressed())
  {
  case KEY_SPACE:
  {
    state = PLAYING_STATE;
  }
  break;
  }

  BeginDrawing();
  ClearBackground((Color){0, 0, 0, 1});
  renderGrid();
  renderApple(apple);
  renderSnake(snake);

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

void endGame()
{
  switch (GetKeyPressed())
  {
  case KEY_SPACE:
  {
    snake = createSnake();
    apple = createApple(snake);
    score = 0;
    state = PLAYING_STATE;
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

  sprintf(scoreText, "Score: %d", score);
  int scoreSize = 25;
  int scoreWidth = MeasureText(scoreText, textSize);
  DrawText(gameOverText, (SCREEN_WIDTH / 2) - titleWidth / 2, (SCREEN_HEIGHT / 3), titleSize, WHITE);
  DrawText(pauseText, (SCREEN_WIDTH / 2) - textWidth / 2, (SCREEN_HEIGHT / 3) + 60, textSize, WHITE);
  DrawText(scoreText, (SCREEN_WIDTH / 2) - scoreWidth / 2, (SCREEN_HEIGHT / 3) + 100, textSize, GREEN);

  EndDrawing();
}

int main()
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake_c");
  SetTargetFPS(60);

  snake = createSnake();
  apple = createApple(snake);
  scoreText = malloc(10);
  while (!WindowShouldClose())
  {
    if (state == PLAYING_STATE)
    {
      runGame();
    }
    else if (state == PAUSE_STATE)
    {
      pauseGame();
    }
    else if (state == END_STATE)
    {
      endGame();
    }
  }

  return 0;
}