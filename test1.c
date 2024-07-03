#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define CELL_SIZE 20
#define MAZE_ROWS (SCREEN_HEIGHT / CELL_SIZE)
#define MAZE_COLS (SCREEN_WIDTH / CELL_SIZE)

char maze[MAZE_ROWS][MAZE_COLS];

void generateMaze(int row, int col) {
    if (row < 0 || col < 0 || row >= MAZE_ROWS || col >= MAZE_COLS || maze[row][col] != ' ') {
        return;
    }
    maze[row][col] = '#'; // Mark the current cell as part of the maze

    // Directions for movement (up, down, left, right)
    int directions[4][2] = {
        {-1, 0}, // Up
        {1, 0},  // Down
        {0, -1}, // Left
        {0, 1}   // Right
    };

    // Shuffle directions randomly
    for (int i = 0; i < 4; ++i) {
        int random_index = rand() % 4;
        int temp_row = row + directions[random_index][0];
        int temp_col = col + directions[random_index][1];
        generateMaze(temp_row, temp_col); // Recursively generate maze paths
    }
}

void drawMaze(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set background color to white
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set maze wall color to black

    for (int i = 0; i < MAZE_ROWS; ++i) {
        for (int j = 0; j < MAZE_COLS; ++j) {
            if (maze[i][j] == '#') {
                SDL_Rect rect = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_RenderFillRect(renderer, &rect); // Draw maze walls
            }
        }
    }

    SDL_RenderPresent(renderer); // Display rendered maze
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("3D Maze Generator and Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize maze with empty cells (' ')
    for (int i = 0; i < MAZE_ROWS; ++i) {
        for (int j = 0; j < MAZE_COLS; ++j) {
            maze[i][j] = ' ';
        }
    }

    // Generate maze starting from a random cell
    generateMaze(rand() % MAZE_ROWS, rand() % MAZE_COLS);

    // Main loop: render maze until user quits
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        drawMaze(renderer); // Draw maze on screen
        SDL_Delay(100);     // Delay to control rendering speed
    }

    // Cleanup resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
