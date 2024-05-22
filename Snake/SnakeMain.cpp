#include <SDL.h>
#include <iostream>
#include <deque>
#include <ctime> // For time function
#include <cstdlib> // For rand and srand functions


using namespace std;

// kich co Screen 
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

// kich co Snake 
const int SNAKE_WIDTH = 20;
const int SNAKE_HEIGHT = 20;

// kich co apple
const int APPLE_WIDTH = 20;
const int APPLE_HEIGHT = 20;

// tao do tre
const int FRAME_DELAY = 100; // in milliseconds

// tao Direction enum
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// cau truc con ran khi chay toa do
struct Segment {
    int x, y;
};

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        return -1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    
    if (!window) {
        cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // tao renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    //create font

    // Seed random number generator
    srand(time(nullptr));

    // Game variables
    deque<Segment> snake; // Snake segments
    snake.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }); // Initial snake position
    Direction direction = RIGHT; // Initial snake direction
    Segment apple = { rand() % (SCREEN_WIDTH / APPLE_WIDTH) * APPLE_WIDTH,
                      rand() % (SCREEN_HEIGHT / APPLE_HEIGHT) * APPLE_HEIGHT }; // Initial apple position

    // Main game loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Main game loop
    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    if (direction != DOWN) direction = UP;
                    break;
                case SDLK_DOWN:
                    if (direction != UP) direction = DOWN;
                    break;
                case SDLK_LEFT:
                    if (direction != RIGHT) direction = LEFT;
                    break;
                case SDLK_RIGHT:
                    if (direction != LEFT) direction = RIGHT;
                    break;
                default:
                    break;
                }
            }
        }

        // Move the snake
        Segment newHead = { snake.front().x, snake.front().y };
        switch (direction) {
        case UP:
            newHead.y -= SNAKE_HEIGHT;
            break;
        case DOWN:
            newHead.y += SNAKE_HEIGHT;
            break;
        case LEFT:
            newHead.x -= SNAKE_WIDTH;
            break;
        case RIGHT:
            newHead.x += SNAKE_WIDTH;
            break;
        default:
            break;
        }
        // khi ma ran di vào canh ben kia của window sẽ spawn ở canh ben cho khac
        if (newHead.x < 0) {
            newHead.x = SCREEN_WIDTH - SNAKE_WIDTH;
        }
        else if (newHead.x >= SCREEN_WIDTH) {
            newHead.x = 0;
        }
        if (newHead.y < 0) {
            newHead.y = SCREEN_HEIGHT - SNAKE_HEIGHT;
        }
        else if (newHead.y >= SCREEN_HEIGHT) {
            newHead.y = 0;
        }
        // khi ran tu dam vao chinh minh
        for (auto segment : snake) {
            if (newHead.x == segment.x && newHead.y == segment.y) {
                quit = true; // Game over
                break;
            }
        }

        // Check if snake eats the apple
       if (newHead.x == apple.x && newHead.y == apple.y) {
            //Increase snake length
            snake.push_front(newHead);
            // Generate new apple position
            apple = { rand() % (SCREEN_WIDTH / APPLE_WIDTH) * APPLE_WIDTH,
                      rand() % (SCREEN_HEIGHT / APPLE_HEIGHT) * APPLE_HEIGHT };
        }
        else {
            // Move the snake
            snake.push_front(newHead);
            snake.pop_back();
        }

      


        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Render apple
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect appleRect = { apple.x, apple.y, APPLE_WIDTH, APPLE_HEIGHT };
        SDL_RenderFillRect(renderer, &appleRect);

        // Render snake
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (auto segment : snake) {
            SDL_Rect snakeRect = { segment.x, segment.y, SNAKE_WIDTH, SNAKE_HEIGHT };
            SDL_RenderFillRect(renderer, &snakeRect);
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay for smooth movement
        SDL_Delay(FRAME_DELAY);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
