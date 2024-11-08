#include <iostream>
#include <stdio.h>
#include "SDL2/SDL.h"

void drawMandelbrot(SDL_Renderer *renderer, int width, int height, int MAX_ITER) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            double zx = 0, zy = 0, cx, cy;
            // convert point to complex plane
            cx = (x - width / 2.0) * 4.0 / width;
            cy = (y - height / 2.0) * 4.0 / height;
            int iter = 0;
            // iterate
            while (zx * zx + zy * zy < 4 && iter < MAX_ITER) {
                double temp = zx * zx - zy * zy + cx;
                zy = 2 * zx * zy + cy;
                zx = temp;
                std::cout << iter << std::endl;
                iter++;
            }
            if (iter < MAX_ITER) {
                // set color based on iteration stage
                double smooth_iter = iter + 1 - log(log(sqrt(zx * zx + zy * zy))) / log(2);
                double t = smooth_iter / MAX_ITER;
                Uint8 r = (int)(9 * (1 - t) * t * t * t * 255);
                Uint8 g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
                Uint8 b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
                SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            }
            // draw points
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}