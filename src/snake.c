#include "snake.h"
#include <stdlib.h>
#include <stdbool.h>


/*
 * snake.c
 * Snake simulation on a wrapping grid.
 * Includes a small 2-turn input buffer for better feel at low tick rates.
 */


static bool is_opposite(Dir a, Dir b) {
    return (a == DIR_UP    && b == DIR_DOWN) ||
           (a == DIR_DOWN  && b == DIR_UP)   ||
           (a == DIR_LEFT  && b == DIR_RIGHT)||
           (a == DIR_RIGHT && b == DIR_LEFT);
}

static IVec2 wrap_pos(int w, int h, IVec2 p) {
    if (p.x < 0) p.x += w;
    if (p.x >= w) p.x -= w;
    if (p.y < 0) p.y += h;
    if (p.y >= h) p.y -= h;
    return p;
}

static IVec2 step_dir(IVec2 p, Dir d) {
    switch (d) {
        case DIR_UP:    p.y -= 1; break;
        case DIR_DOWN:  p.y += 1; break;
        case DIR_LEFT:  p.x -= 1; break;
        case DIR_RIGHT: p.x += 1; break;
    }
    return p;
}

bool Snake_Init(Snake* s, int grid_w, int grid_h, int max_len, Dir start_dir) {
    if (!s || grid_w <= 0 || grid_h <= 0 || max_len <= 0) return false;

    s->grid_w = grid_w;
    s->grid_h = grid_h;
    s->max_len = max_len;

    s->seg = (IVec2*)calloc((size_t)max_len, sizeof(IVec2));
    s->prev = (IVec2*)calloc((size_t)max_len, sizeof(IVec2));
    if (!s->seg || !s->prev) {
        free(s->seg);
        free(s->prev);
        s->seg = s->prev = NULL;
        return false;
    }

    s->len = 1;
    s->grow = 0;

    s->dir = start_dir;
    s->has_q1 = false;
    s->has_q2 = false;

    // start centered
    s->seg[0].x = grid_w / 2;
    s->seg[0].y = grid_h / 2;
    s->prev[0] = s->seg[0];

    return true;
}

void Snake_Destroy(Snake* s) {
    if (!s) return;
    free(s->seg);
    free(s->prev);
    s->seg = s->prev = NULL;
    s->len = s->max_len = 0;
}

IVec2 Snake_Head(const Snake* s) {
    return s->seg[0];
}

bool Snake_Occupies(const Snake* s, IVec2 p) {
    if (!s) return false;
    for (int i = 0; i < s->len; i++) {
        if (s->seg[i].x == p.x && s->seg[i].y == p.y) return true;
    }
    return false;
}

void Snake_AddGrowth(Snake* s, int n) {
    if (!s) return;
    if (n > 0) s->grow += n;
}

/*
 * Queue a direction change into a small 2-slot buffer.
 *
 * Rationale: the snake advances on fixed ticks, but key presses arrive on frames.
 * Buffering preserves quick turn sequences that happen between ticks.
 * Reversals and repeats are ignored to avoid impossible or redundant moves.
 */
void Snake_QueueDir(Snake* s, Dir d) {
    if (!s) return;

    // Validate against last queued direction (2-turn buffer)
    Dir last = s->dir;
    if (s->has_q2) last = s->q2;
    else if (s->has_q1) last = s->q1;

    if (is_opposite(last, d)) return;
    if (d == last) return;

    if (!s->has_q1) {
        s->q1 = d;
        s->has_q1 = true;
    } else if (!s->has_q2) {
        s->q2 = d;
        s->has_q2 = true;
    } else {
        s->q2 = d; // overwrite 2nd slot
    }
}

/*
 * Advance the simulation by one tick.
 *
 * Order matters for consistency and interpolation:
 * 1) Copy current positions into prev.
 * 2) Apply at most one buffered turn.
 * 3) Shift the body, step the head (with wrap), then apply one unit of growth.
 */
void Snake_Tick(Snake* s) {
    if (!s) return;

    // Save previous positions for interpolation (only active length)
    for (int i = 0; i < s->len; i++) {
        s->prev[i] = s->seg[i];
    }

    // Apply at most one buffered turn
    if (s->has_q1) {
        s->dir = s->q1;
        if (s->has_q2) {
            s->q1 = s->q2;
            s->has_q2 = false;
            s->has_q1 = true;
        } else {
            s->has_q1 = false;
        }
    }

    // Remember tail before shifting (used if we grow)
    IVec2 old_tail = s->seg[s->len - 1];

    // Shift body back
    for (int i = s->len - 1; i >= 1; i--) {
        s->seg[i] = s->seg[i - 1];
    }

    // Move head one cell
    IVec2 new_head = step_dir(s->seg[0], s->dir);
    new_head = wrap_pos(s->grid_w, s->grid_h, new_head);
    s->seg[0] = new_head;

    // Growth: keep old tail as new segment
    if (s->grow > 0 && s->len < s->max_len) {
        s->seg[s->len] = old_tail;
        s->prev[s->len] = old_tail; // initialize prev for new segment
        s->len += 1;
        s->grow -= 1;
    }
}
