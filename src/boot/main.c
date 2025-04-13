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

#include "en/player.h"
#include "screen/ctx.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <stdio.h>
#include <stdlib.h>

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

#include "boot/app.h"
#include "debug/debug.h"
#include "debug/debug_draw.h"
#include "util/safe.h"

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

bool *KEYS = NULL;

b2WorldId current_world;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("Test", "1.0", "com.test.test");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("test", 640 * 2, 480 * 2, 0, &window,
                                   &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderScale(renderer, 2, 2);

  KEYS = (bool *)SDL_GetKeyboardState(NULL);
  if (KEYS == NULL) {
    SDL_Log("Failed to get Keyboard State: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  AppState *state = malloc(sizeof(AppState));
  *state = AppState_default();
  KeyboardController *controller = malloc(sizeof(KeyboardController));
  *controller = KeyboardController_default();
  state->player.controller = (PlayerController *)controller;
  *appstate = (void *)state;

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  AppState *state = (AppState *)appstate;
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;

  case SDL_EVENT_KEY_DOWN:
    switch (event->key.key) {
    case SDLK_RETURN:
      state->options.vsync = !state->options.vsync;
      SDL_SetRenderVSync(
          renderer, state->options.vsync ? 1 : SDL_RENDERER_VSYNC_DISABLED);
      break;

    case SDLK_SPACE:
      state->options.frame_cap = !state->options.frame_cap;
      break;

    case SDLK_B:
      debug_draw.drawAABBs = !debug_draw.drawAABBs;
      break;
    }
    break;
  }
  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *state = (AppState *)appstate;

  const double now = (double)SDL_GetTicks();
  if (state->deltaTime < 1 / 60.0) {
    state->deltaTime = 1 / 60.0;
  }
  state->lastTick = now;

  static double totalTime = 0;
  static int frames = 0;

  const double fps = frames == 0 ? 0 : frames / state->lastTick * 1000.0;

  const float timestep = 1.0f / 60.0f;
  const int substep_count = 4;
  b2World_Step(state->world, timestep, substep_count);

  objcall(state->player.super, update, state->deltaTime);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
  SDL_RenderClear(renderer);

  SDL_Rect viewport;
  SDL_GetRenderViewport(renderer, &viewport);
  SDL_FPoint initial = {
      .x = 0,
      .y = viewport.h,
  };

  RenderContext frame_ctx = RenderContext_create(renderer);
  Stack_push(&frame_ctx.transforms, &initial);

  objcall(state->player.super, preRender, &frame_ctx);
  objcall(state->player.super, render, &frame_ctx);
  objcall(state->player.super, postRender, &frame_ctx);

  debug_draw.context = &frame_ctx;
  b2World_Draw(state->world, &debug_draw);

  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  int ypos = 0;
  char buf[16];
  snprintf(buf, 11, "%gs", state->deltaTime);
  SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, buf);

  snprintf(buf, 11, "%.2ffps", fps);
  SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, buf);

  snprintf(buf, 16, "%.2ffps (true)", 1 / state->deltaTime);
  SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, buf);

  snprintf(buf, 16, "%.1f", state->lastTick / 1000.0);
  SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, buf);

  if (state->options.vsync) {
    SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, "VSYNC ENABLED");
  }
  if (state->options.frame_cap) {
    SDL_RenderDebugText(renderer, 10, ypos++ * 20 + 10, "FRAME CAP ENABLED");
  }

  /* put the newly-cleared rendering on the screen. */
  SDL_RenderPresent(renderer);

  const double fpsTick = (double)SDL_GetTicks();
  const double wantedFrameTick = 1000 / 60.0;
  const double dif = fpsTick - state->lastTick;
  if (dif < wantedFrameTick) {
    SDL_Delay(wantedFrameTick - dif);
  }

  state->deltaTime = ((double)SDL_GetTicks() - state->lastTick) / 1000.0;
  frames++;
  totalTime += state->deltaTime;

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  /* SDL will clean up the window/renderer for us. */
  AppState_destroy((AppState *)appstate);
}
