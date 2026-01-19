#include "apple.h"
#include <stdbool.h>


/*
 * apple.c
 * Apple placement and respawn logic.
 *
 * Constraint: never spawn inside the snake.
 * Random tries are cheap early; a scan guarantees progress when crowded.
 */

#include <SDL3/SDL.h>

static IVec2 random_pos(int w, int h) {
    IVec2 p;
    p.x = (int)SDL_rand((Uint64)w);
    p.y = (int)SDL_rand((Uint64)h);
    return p;
}

static void spawn_avoiding_snake(Apple* a, const Snake* s) {
    // Try random a bunch of times; if snake fills most of board, fall back to scan.
    for (int tries = 0; tries < 1024; tries++) {
        IVec2 p = random_pos(s->grid_w, s->grid_h);
        if (!Snake_Occupies(s, p)) {
            a->pos = p;
            return;
        }
    }

    // Fallback: deterministic scan for first empty cell
    for (int y = 0; y < s->grid_h; y++) {
        for (int x = 0; x < s->grid_w; x++) {
            IVec2 p = { x, y };
            if (!Snake_Occupies(s, p)) {
                a->pos = p;
                return;
            }
        }
    }

    // Board full: keep position as-is (game would be "won")
}

void Apple_Init(Apple* a, const Snake* s) {
    if (!a || !s) return;
    spawn_avoiding_snake(a, s);
}

bool Apple_TryEatAndRespawn(Apple* a, Snake* s) {
    if (!a || !s) return false;

    IVec2 head = Snake_Head(s);
    if (a->pos.x == head.x && a->pos.y == head.y) {
        Snake_AddGrowth(s, 1);       // growth happens on subsequent tick
        spawn_avoiding_snake(a, s);  // respawn apple not inside body
        return true;
    }
    return false;
}
