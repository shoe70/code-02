#include "mandelbrot.cpp"

SDL_Window *main_window;
SDL_Renderer *renderer;
bool isRunning;
const int width = 675;
const int height = 600;
const int MAX_ITER = 1000;

// initialisation
void init(const char *title, 
          int xpos, int ypos, 
          int width, int height, 
          bool fullscreen) {
    int flags = 0;
    if ( fullscreen ) { flags = SDL_WINDOW_FULLSCREEN; }
    if ( SDL_Init(SDL_INIT_EVERYTHING) == 0 ) {
        std::cout << "Subsystems Initialised!" << std::endl;
        
        main_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if ( main_window ) { std::cout << "Window Initialised!" << std::endl; }
        renderer = SDL_CreateRenderer(main_window, 0, -1);
        if ( renderer ) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // white
            std::cout << "Renderer Initialised!" << std::endl;
        }
        isRunning = true;
    } else { isRunning = false; }
}

int main(int argc, const char * argv[]) {
    // init
    SDL_Event e;
    init("fractal renderer", 
         SDL_WINDOWPOS_CENTERED, 
         SDL_WINDOWPOS_CENTERED, 
         width, height, false);
    while ( isRunning ) {
        // event handling
        SDL_PollEvent(&e);
        switch ( e.type )
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        default: break; }

        // rendering
        SDL_RenderClear(renderer);
        // things to render
        drawMandelbrot(renderer, width, height, MAX_ITER);
        SDL_RenderPresent(renderer);
    }
    // clean game
    SDL_DestroyWindow(main_window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned!" << std::endl;
}