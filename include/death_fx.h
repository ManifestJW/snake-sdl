#pragma once

/*
 * death_fx.h
 *
 * “DeathFx” is a small self-contained effect that visually disintegrates the
 * snake after a collision.
 *
 * Key idea:
 * This project supports two rendering modes:
 *   - tick-snap (alpha = 1, classic grid movement)
 *   - interpolation (alpha in [0,1], smooth movement between ticks)
 *
 * When the snake dies, we *freeze* whichever mode was active at that moment
 * so the animation doesn’t pop mid-effect if the user toggles settings.
 *
 * The effect is deterministic per death (seeded from the death time), so the
 * same snake state always explodes the same way in a given run.
 *
 * Typical flow:
 *   - DeathFx_Init(): set default timing
 *   - DeathFx_Start(): capture mode + alpha and start timer
 *   - DeathFx_RenderAndAdvance(): called every frame while active
 */

#include <stdbool.h>
#include <stdint.h>

#include "app.h"
#include "snake.h"

typedef struct DeathFx {
    bool active;
    bool finished;

    // Start time used to compute animation progress.
    uint64_t start_ns;

    // Snapshot of interpolation state at the moment of death.
    // (Lets the effect line up with where the snake was on-screen.)
    float death_alpha;      // alpha frozen at the moment of death (for interp-on mode)
    bool interp_mode;       // interpolation mode locked at death time

    // Deterministic “randomness” for per-segment rotation.
    uint32_t seed;

    // Timing / styling parameters (seconds / degrees).
    float stagger_s;        // delay per segment from head to tail
    float seg_dur_s;        // duration per segment disintegrate
    float max_rot_deg;      // max rotation magnitude
} DeathFx;

// Initializes effect state + default parameters.
void DeathFx_Init(DeathFx* fx);

// Starts the effect. interp_mode/death_alpha are captured so the animation
// matches the last rendered snake pose.
void DeathFx_Start(DeathFx* fx, bool interp_mode, float death_alpha, uint64_t now_ns);

// Draws the effect for the current frame and advances internal timers.
// When finished, fx->finished becomes true.
void DeathFx_RenderAndAdvance(DeathFx* fx, const App* app, const Snake* snake, uint64_t now_ns);
