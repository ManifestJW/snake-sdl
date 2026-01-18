#pragma once

/*
 * render.h
 * SDL drawing helpers.
 * Grid-space (cells) -> pixel-space (window) is handled via App.
 */

#include <SDL3/SDL.h>
#include <stdint.h>

#include "app.h"
#include "snake.h"  // for IVec2

void Render_Clear(SDL_Renderer* r);
void Render_Present(SDL_Renderer* r);

void Render_CellFilled(const App* app, IVec2 grid_pos, uint8_t r, uint8_t g, uint8_t b);
void Render_CellFilledF(const App* app, float gx, float gy, uint8_t r, uint8_t g, uint8_t b);

void Render_RectFilledPx(const App* app, float x, float y, float w, float h,
                         uint8_t r, uint8_t g, uint8_t b);

void Render_QuadCenteredPx(const App* app, float cx, float cy, float w, float h, float angle_rad,
                           uint8_t r, uint8_t g, uint8_t b, uint8_t a);

// Grid lines
void Render_GridLines(const App* app);

// NEW: grid lines with configurable color + alpha
void Render_GridLinesEx(const App* app, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
