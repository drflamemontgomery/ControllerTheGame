/*
    General Purpose Entity Object Structure
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

#ifndef OBJ_H
#define OBJ_H

#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <box2d/math_functions.h>

#include "screen/ctx.h"

#define struct$Object2D(SNAME)                                                 \
  struct SNAME {                                                               \
    SDL_FPoint pos;                                                            \
    float width;                                                               \
    float height;                                                              \
                                                                               \
    struct Object2D *parent;                                                   \
                                                                               \
    void (*preRender)(struct Object2D *, RenderContext *ctx);                  \
    void (*render)(struct Object2D *, RenderContext *ctx);                     \
    void (*postRender)(struct Object2D *, RenderContext *ctx);                 \
                                                                               \
    void (*update)(struct Object2D *, double delta_time);                      \
    void (*destroy)(struct Object2D *);                                        \
  }

typedef struct$Object2D(Object2D) Object2D;
#define using(TYPE, SUPERNAME)                                                 \
  union {                                                                      \
    struct$##TYPE();                                                           \
    TYPE SUPERNAME;                                                            \
  }

void Object2D_preRender(struct Object2D *, RenderContext *ctx);
void Object2D_render(struct Object2D *, RenderContext *ctx);
void Object2D_postRender(struct Object2D *, RenderContext *ctx);
void Object2D_update(struct Object2D *, double delta_time);

Object2D Object2D_default();
Object2D Object2D_create(float x, float y, float width, float height);
void Object2D_destroy(Object2D *self);
#endif // OBJ_H
