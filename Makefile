OUT = snake.exe

run: build
	@$(OUT)

build:
	@gcc main.c -o $(OUT) -lraylib -lgdi32 -lwinmm