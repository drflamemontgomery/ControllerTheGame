/*
    General Purpose Entity Object System
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

#include "en/obj.h"
#include "debug/debug.h"
#include "screen/ctx.h"
#include "util/safe.h"
#include "util/types.h"
#include <stdio.h>

Object2D Object2D_default() { return Object2D_create(0.0f, 0.0f, 1.0f, 1.0f); }

// This is a very messy constructor
// But its general use
Object2D Object2D_create(float x, float y, float width, float height) {
  return (Object2D){
      .parent = NULL,
      .pos = {.x = x, .y = y},
      .width = width,
      .height = height,
      .postRender = Object2D_postRender,
      .preRender = Object2D_preRender,
      .render = Object2D_render,
      .update = Object2D_update,
      .destroy = Object2D_destroy,
  };
}

void Object2D_destroy(Object2D *self) {
  debugAssert(self != NULL, "self == NULL");
}

void Object2D_preRender(Object2D *self, RenderContext *ctx) {
  debugAssert(self != NULL, "self == NULL");
  SDL_FPoint transform = RenderContext_getTransform(ctx);

  transform.x += (int)self->pos.x;
  transform.y += (int)self->pos.y;

  Stack_push(ctx->transforms, transform);
}
void Object2D_render(Object2D *self, RenderContext *ctx) {
  debugAssert(self != NULL, "self == NULL");
}
void Object2D_postRender(Object2D *self, RenderContext *ctx) {
  debugAssert(self != NULL, "self == NULL");
  Stack_pop(ctx->transforms);
}

void Object2D_update(Object2D *self, double delta_time) {
  debugAssert(self != NULL, "self == NULL");
}
