/*
    Rendering Context Implementation
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

#include "screen/ctx.h"
#include "debug/debug.h"
#include <SDL3/SDL_rect.h>

RenderContext RenderContext_create(SDL_Renderer *renderer) {
  return (RenderContext){
      .renderer = renderer,
      .transforms = Stack_create(SDL_FPoint, &std_allocator),
  };
}
SDL_FPoint RenderContext_getTransform(RenderContext *self) {
  if (self->transforms.len == 0) {
    return (SDL_FPoint){0, 0};
  }
  SDL_FPoint *ret = Stack_peek(self->transforms);
  return *ret;
}
void RenderContext_destroy(RenderContext *self) {
  debugAssert(self != NULL, "self == NULL");
  Stack_destroy(self->transforms);
}
