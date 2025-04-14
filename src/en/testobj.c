/*
    Test Object For Debugging Purposes
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

#include <math.h>
#include <stdio.h>

#include "en/testobj.h"
#include "input/key.h"

#include "debug/debug.h"

Object2D TestObj_create(float width, float height) {
  Object2D super = Object2D_create(0, 0, width, height);

  super.render = TestObj_render;
  return super;
}

void TestObj_render(Object2D *self, RenderContext *ctx) {
  SDL_FPoint transform = RenderContext_getTransform(ctx);
  SDL_FRect rect = {
      .x = transform.x,
      .y = transform.y,
      .w = self->width,
      .h = self->height,
  };
  SDL_SetRenderDrawColor(ctx->renderer, 0xff, 0, 0, 0xff);
  SDL_RenderFillRect(ctx->renderer, &rect);

  const int origin_radius = 2;
  SDL_FRect origin_rect = {
      .x = transform.x - origin_radius,
      .y = transform.y - origin_radius,
      .w = origin_radius * 2,
      .h = origin_radius * 2,
  };

  SDL_SetRenderDrawColor(ctx->renderer, 0, 0xff, 0, 0xff);
  SDL_RenderFillRect(ctx->renderer, &origin_rect);

  Object2D_render(self, ctx);
}

void TestObj_update(Object2D *self, double delta_time) {
  int dx = (int)isKeyDown(RIGHT) - (int)isKeyDown(LEFT);
  int dy = (int)isKeyDown(DOWN) - (int)isKeyDown(UP);

  const int speed = 256 / (self->width * self->height) * 1000;

  if (dx != 0 && dy != 0) {
    self->pos.x += dx * delta_time * speed * M_SQRT1_2;
    self->pos.y += dy * delta_time * speed * M_SQRT1_2;
  } else {
    self->pos.x += dx * delta_time * speed;
    self->pos.y += dy * delta_time * speed;
  }

  Object2D_update(self, delta_time);
}
