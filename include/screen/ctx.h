/*
    Rendering Structures
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

#ifndef CTX_H
#define CTX_H

#include "util/stack.h"
#include <SDL3/SDL.h>

typedef struct Camera2D {
  SDL_FRect view;
  SDL_Texture *target_texture;

} Camera2D;

typedef struct RenderContext {
  SDL_Renderer *renderer;
  Stack(SDL_FPoint) transforms;
} RenderContext;

RenderContext RenderContext_create(SDL_Renderer *renderer);
SDL_FPoint RenderContext_getTransform(RenderContext *self);
void RenderContext_destroy(RenderContext *self);

#endif // CTX_H
