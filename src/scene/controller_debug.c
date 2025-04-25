/*
    Controller Debug Scene
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

#include "scene/controller_debug.h"
#include "debug/debug.h"
#include "debug/debug_draw.h"
#include "heap/arena_allocator.h"
#include "util/safe.h"
#include <box2d/box2d.h>

ControllerDebugScene *ControllerDebugScene_create(Allocator *allocator) {
  ControllerDebugScene *self = allocPtr(ControllerDebugScene, allocator, 1);
  debugAssert(self != NULL, "self == NULL");

  // Create Super
  self->super_object2d = Object2D_create(0, 0, 0, 0);
  self->destroy = (typeof(self->destroy))ControllerDebugScene_destroy;
  self->update = (typeof(self->update))ControllerDebugScene_update;
  self->render = (typeof(self->render))ControllerDebugScene_render;

  // Assign Allocators
  self->allocator = allocator;
  self->m_child_arena_allocator = ArenaAllocator_create(&std_allocator);
  self->m_child_allocator =
      ArenaAllocator_getAllocator(&self->m_child_arena_allocator);

  b2WorldDef world_def = b2DefaultWorldDef();
  world_def.gravity = (b2Vec2){0.0f, 1.0f};
  self->world = b2CreateWorld(&world_def);
  self->m_world_mutex = SDL_CreateMutex();

  self->controller = (typeof(self->controller))allocSlice(
      ControllerObj, self->m_child_allocator, 1);
  self->controller.ptr[0] = ControllerObj_create(self->world);

  return self;
}

void ControllerDebugScene_destroy(ControllerDebugScene *self) {
  debugAssert(self != NULL, "self == NULL");
  Object2D_destroy(&self->super_object2d);

  forArray(self->controller, controller) {
    controller->destroy(&controller->super_object2d);
  }

  SDL_DestroyMutex(self->m_world_mutex);
  b2DestroyWorld(self->world);
  ArenaAllocator_destroy(&self->m_child_arena_allocator);
  self->m_child_allocator = NULL;

  freePtr(self->allocator, self);
}

void ControllerDebugScene_update(ControllerDebugScene *self,
                                 double delta_time) {
  debugAssert(self != NULL, "self == NULL");
  // Update our physics world
  const float timestep = 1.0f / 60.0f;
  const int substep_count = 4;

  SDL_LockMutex(self->m_world_mutex);
  b2World_Step(self->world, timestep, substep_count);
  SDL_UnlockMutex(self->m_world_mutex);

  forArray(self->controller, controller) {
    objcall(controller->super_object2d, update, delta_time);
  }
}

void ControllerDebugScene_render(ControllerDebugScene *self,
                                 RenderContext *ctx) {
  debugAssert(self != NULL, "self == NULL");

  Object2D_render(&self->super_object2d, ctx);
  if (SDL_TryLockMutex(self->m_world_mutex)) {
    b2World_Draw(self->world, &debug_draw);
    SDL_UnlockMutex(self->m_world_mutex);
  }
}
