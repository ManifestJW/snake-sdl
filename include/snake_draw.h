#pragma once

/*
 * snake_draw.h
 *
 * Rendering for the snake, separated from the snake simulation.
 *
 * Why separate Snake vs SnakeDraw?
 * - Snake stays pure simulation and is easier to test/modify.
 * - SnakeDraw can get fancy (interpolation, wrap-aware bridges, different styles)
 *   without risking gameplay correctness.
 *
 * Interpolation model:
 * - The simulation updates snake->seg once per tick and stores last tick’s
 *   positions in snake->prev.
 * - The main loop computes alpha = acc / tick_ns (0..1) and asks SnakeDraw to render intermediate positions.
 *
 * Bridging:
 * When interpolation is enabled and the snake turns or wraps around the edges, simply drawing squares can leave visible gaps. “Bridges” fill those gaps so the snake looks continuous.
 */

#include <stdbool.h>
#include <stdint.h>

#include "app.h"
#include "snake.h"
#include "render.h"

// Settings for how the snake is drawn.
typedef struct SnakeDrawStyle {
    bool snap_head;          // Classic head snap look (head is drawn at seg[0] exactly).
    bool draw_bridges;       // Fill gaps between segments at corners/wraps.
    uint8_t head_r, head_g, head_b;
    uint8_t body_r, body_g, body_b;
} SnakeDrawStyle;

// Render the snake using interpolation alpha in [0,1].
// - Uses snake->prev and snake->seg
// - Can snap head and bridge gaps at turns/wraps
void SnakeDraw_Render(const App* app, const Snake* snake, float alpha, SnakeDrawStyle style);
