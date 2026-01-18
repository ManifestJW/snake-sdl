#pragma once

/*
 * fps.h
 *
 * Lightweight FPS/TPS tracker.
 *
 * - FPS (frames per second) counts render presentations.
 * - TPS (ticks per second) counts simulation steps.
 *
 * The game intentionally decouples render rate from tick rate:
 * - Rendering can run very fast (up to RENDER_CAP_HZ) for smoothness.
 * - Simulation steps are limited by tick_hz and may ramp with score.
 *
 * This module keeps the window title up to date without doing extra work every frame: it recomputes fps/tps about once per second, but rewrites the title every call so end-state titles don’t get clobbered by stale text.
 */

#include <stdint.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

typedef struct FpsCounter {
    uint64_t last_sample_ns;

    int frame_count;
    int tick_count;

    double fps;
    double tps;
} FpsCounter;

void Fps_Init(FpsCounter* c);
void Fps_OnFrame(FpsCounter* c);
void Fps_OnTick(FpsCounter* c);

// Always writes the title each call.
// FPS/TPS are only recomputed about once per second.
void Fps_UpdateWindowTitle(FpsCounter* c, SDL_Window* window,
                           bool interp_on, int score,
                           bool game_over, bool you_win);
