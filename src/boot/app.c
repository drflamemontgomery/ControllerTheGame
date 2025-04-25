/*
    Application State
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

#include "boot/app.h"
#include "debug/debug.h"
#include "en/obj.h"
#include "scene/controller_debug.h"
#include "util/safe.h"
#include <SDL3/SDL_timer.h>
#include <box2d/box2d.h>
#include <box2d/types.h>
#include <stdio.h>

AppState *AppState_default(Allocator *allocator) {

  /*b2BodyDef grounddef = b2DefaultBodyDef();
  grounddef.position = (b2Vec2){
      0.0f,
      0.0f,
  };
  b2BodyId ground = b2CreateBody(world, &grounddef);

  b2Segment groundsegment = {
      .point1 = (b2Vec2){0.0f, 0.0f},
      .point2 = (b2Vec2){10.0f, 0.0f},
  };
  b2ShapeDef groundshapedef = b2DefaultShapeDef();
  groundshapedef.density = 1.0f;
  b2CreateSegmentShape(ground, &groundshapedef, &groundsegment);*/

  ControllerDebugScene *debug_scene = ControllerDebugScene_create(allocator);

  AppState *state = allocPtr(AppState, allocator, 1);
  *state = (AppState){
      .delta_time = 0.0f,
      .last_tick = SDL_GetTicks(),
      .running = true,

      .options = {false, false},
      .allocator = allocator,
      .scene = &debug_scene->super_object2d,

      .fixedUpdate_thread = NULL,
  };
  return state;
}

void AppState_destroy(AppState *self) {
  debugAssert(self != NULL, "self == NULL");
  self->running = false;
  if (self->fixedUpdate_thread != NULL) {
    SDL_WaitThread(self->fixedUpdate_thread, NULL);
    self->fixedUpdate_thread = NULL;
  }

  objrefcall(self->scene, destroy);
  freePtr(self->allocator, self);
}
