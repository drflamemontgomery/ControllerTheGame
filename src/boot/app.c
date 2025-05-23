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
#include "util/safe.h"
#include <SDL3/SDL_timer.h>
#include <box2d/box2d.h>
#include <box2d/types.h>
#include <stdio.h>

AppState *AppState_default(Allocator *allocator) {

  b2WorldDef world_def = b2DefaultWorldDef();
  world_def.gravity = (b2Vec2){0.0f, 1.0f};
  b2WorldId world = b2CreateWorld(&world_def);

  Player player = Player_create(world, 2.0f, -3.0f, NULL);

  Object2D *testobj = allocPtr(allocator, sizeof(Object2D), 1);
  *testobj = TestObj_create(32.0f, 32.0f);

  Object2D_addChild(&player.super, testobj);

  b2BodyDef grounddef = b2DefaultBodyDef();
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
  b2CreateSegmentShape(ground, &groundshapedef, &groundsegment);

  AppState *state = allocPtr(allocator, sizeof(AppState), 1);
  *state = (AppState){
      .delta_time = 0.0f,
      .last_tick = SDL_GetTicks(),
      .running = true,

      .controller_out = ControllerDevice_default(),
      .options = {false, true},
      .player = player,
      .testobj = testobj,
      .world = world,
      .allocator = allocator,

      .fixedUpdate_thread = NULL,
      .fixedUpdate_mutex = NULL,
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
  if(self->fixedUpdate_mutex != NULL) {
    SDL_DestroyMutex(self->fixedUpdate_mutex);
    self->fixedUpdate_mutex = NULL;
  }

  Player_destroy(&self->player);
  freePtr(self->allocator, self->testobj);
  b2DestroyWorld(self->world);

  freePtr(self->allocator, self);
}
