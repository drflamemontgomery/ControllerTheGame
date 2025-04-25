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

#ifndef SCENE_CONTROLLER_DEBUG_H
#define SCENE_CONTROLLER_DEBUG_H

#include "heap/allocator.h"
#include "heap/arena_allocator.h"
#include "util/slice.h"

#include "en/controller.h"
#include "en/obj.h"

typedef struct ControllerDebugScene {
  using(Object2D, super_object2d);

  // allocator and child_allocator should be different so that when the scene is
  // destroyed, every child is easily destroyed
  Allocator *allocator;
  Allocator *m_child_allocator;

  // ArenaAllocator allows for easy destruction of children
  ArenaAllocator m_child_arena_allocator;

  b2WorldId world;
  SDL_Mutex *m_world_mutex;

  Slice(ControllerObj) controller;
} ControllerDebugScene;

ControllerDebugScene *ControllerDebugScene_create(Allocator *allocator);
void ControllerDebugScene_destroy(ControllerDebugScene *self);
void ControllerDebugScene_update(ControllerDebugScene *self, double delta_time);
void ControllerDebugScene_render(ControllerDebugScene *self,
                                 RenderContext *ctx);

#endif // SCENE_CONTROLLER_DEBUG_H
