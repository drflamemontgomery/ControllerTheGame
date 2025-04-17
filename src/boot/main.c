/*
    Entrypoint of application
    Copyright (C) 2025  Ashton Warner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifdef DEBUG
#include <unistd.h>
#endif

#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

#include "boot/app.h"
#include "debug/debug.h"
#include "debug/debug_draw.h"
#include "en/player.h"
#include "heap/allocator.h"
#include "screen/ctx.h"
#include "util/safe.h"

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

// Our main file keeps the pointer to our SDL key states
bool *KEYS = NULL;

#ifdef DEBUG
// Signal Handler
static void sigint_handler(int sig) {
  void *array[10];
  size_t size;

  size = backtrace(array, 10);
  trace("Error: signal %d", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}
#endif

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
#ifdef DEBUG
  signal(SIGABRT, sigint_handler);
#endif
  SDL_SetAppMetadata("Controller The Game", "1.0",
                     "com.drflame.controllergame");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Controller The Game", 640 * 2, 480 * 2, 0,
                                   &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // I am running on a wayland high definition screen so I use
  // a scaled renderer to see the game easier
  SDL_SetRenderScale(renderer, 2, 2);

  // Get the pointer for the SDL Keyboard State
  // for global use
  KEYS = (bool *)SDL_GetKeyboardState(NULL);
  if (KEYS == NULL) {
    SDL_Log("Failed to get Keyboard State: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Use the default App State Initialization and create
  // it on the heap so that we can pass it around easily
  AppState *state = AppState_default(&std_allocator);

  // Create a Heap-Allocated Controller Component for our player so we can
  // access movement.
  // TODO make a controller subsystem for handling controls.
  state->player.controller =
      (PlayerController *)KeyboardController_default(&std_allocator);

  // This allows our Application to access the state
  *appstate = (void *)state;

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  // `appstate` should only ever be an `AppState*` type.
  // If it isn't, someone has messed up and you will crash :)
  AppState *state = (AppState *)appstate;

  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS; // We like success when quitting

  case SDL_EVENT_KEY_DOWN:
    switch (event->key.key) {
    case SDLK_RETURN:
      // We can change the vsync option if we want.
      // TODO have a option menu to change it
      state->options.vsync = !state->options.vsync;
      SDL_SetRenderVSync(
          renderer, state->options.vsync ? 1 : SDL_RENDERER_VSYNC_DISABLED);
      break;

    case SDLK_SPACE:
      // We can try to frame cap if it runs too fast.
      // Probably unneeded
      // TODO move to an option menu
      state->options.frame_cap = !state->options.frame_cap;
      break;

#ifdef DEBUG
    case SDLK_B:
      // These look yucky. We don't like them all the time.
      // It gives me a headache
      debug_draw.drawAABBs = !debug_draw.drawAABBs;
      break;
#endif
    }
    break;
  }
  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *state = (AppState *)appstate;

  // We need to be able to calculate `delta_time`
  const double now = (double)SDL_GetTicks();
  /*
  if (state->delta_time <= 0.00000000001f) {
    state->delta_time = 1 / 60.0;
  }
  */
  state->last_tick = now;

  static double total_time = 0;
  static int frames = 0;

  const double fps = frames == 0 ? 0 : frames / state->last_tick * 1000.0;

  // Update our physics world
  const float timestep = 1.0f / 60.0f;
  const int substep_count = 4;
  b2World_Step(state->world, timestep, substep_count);

  // update our root player
  // TODO replace with root scene node
  objcall(state->player.super, update, state->delta_time);

  // Clear the rendering scene
  // TODO contemplate potential optimizations which
  // probably are unnecessary
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
  SDL_RenderClear(renderer);

  // Get current viewport and set (0,0) as bottom left.
  // Down as positive y.
  // Right as positive x.
  // TODO implement Camera transformations
  SDL_Rect viewport;
  SDL_GetRenderViewport(renderer, &viewport);
  SDL_FPoint initial = {
      .x = 0,
      .y = viewport.h,
  };

  // Create a `RenderContext`
  // TODO Create it as a static variable and save unnecessary
  // stack operations if the object grows
  RenderContext frame_ctx = RenderContext_create(renderer);
  Stack_push(frame_ctx.transforms, initial);

  // Render Root Player Sequence
  objcall(state->player.super, preRender, &frame_ctx);
  objcall(state->player.super, render, &frame_ctx);
  objcall(state->player.super, postRender, &frame_ctx);

#ifdef DEBUG
  // Draw the box2d world
  debug_draw.context = &frame_ctx;
  b2World_Draw(state->world, &debug_draw);

  // We no longer need the RenderContext and it has heap memory
  // stored in it. So lets destroy it
  RenderContext_destroy(&frame_ctx);

  // Draw debug information for FPS, delta time, total time
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  int ypos = 0;
  char buf[32];
  snprintf(buf, 14, "%gs", state->delta_time);
  SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, buf);

  snprintf(buf, 31, "%.2ffps", fps);
  SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, buf);

  snprintf(buf, 31, "%.2ffps (true)", 1.0 / state->delta_time);
  SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, buf);

  snprintf(buf, 16, "%gs", state->last_tick / 1000.0);
  SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, buf);

  if (state->options.vsync) {
    SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, "VSYNC ENABLED");
  }
  if (state->options.frame_cap) {
    SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, "FRAME CAP ENABLED");
  }

#endif // DEBUG

  /* put the newly-cleared rendering on the screen. */
  SDL_RenderPresent(renderer);

  // Frame capping
  const double fps_tick = (double)SDL_GetTicks();
  const double wanted_frame_tick = 1000 / 60.0;
  const double dif = fps_tick - state->last_tick;
  if (state->options.frame_cap && dif < wanted_frame_tick) {
    SDL_Delay(wanted_frame_tick - dif);
  }

  state->delta_time = ((double)SDL_GetTicks() - state->last_tick) / 1000.0;
  frames++;
  total_time += state->delta_time;

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  /* SDL will clean up the window/renderer for us. */

  // Destroy the Application
  AppState_destroy((AppState *)appstate);
}
