/*
    Box2d Debug Drawing Routines
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

#include "debug/debug_draw.h"
#include "heap/allocator.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <box2d/types.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug/debug.h"
#include "screen/ctx.h"
#include "util/options.h"

void debugDrawPolygon(const b2Vec2 *vertices, int vertex_count,
                      b2HexColor color, RenderContext *ctx);
void debugDrawSolidPolygon(b2Transform transform, const b2Vec2 *vertices,
                           int vertex_count, float radius, b2HexColor color,
                           RenderContext *ctx);
void debugDrawPoint(b2Vec2 p, float size, b2HexColor color, RenderContext *ctx);
void debugDrawCircle(b2Vec2 center, float radius, b2HexColor color,
                     RenderContext *ctx);
void debugDrawSolidCircle(b2Transform transform, float radius, b2HexColor color,
                          RenderContext *ctx);
void debugDrawCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color,
                      RenderContext *ctx);
void debugDrawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color,
                           RenderContext *ctx);
void debugDrawString(b2Vec2 p, const char *s, RenderContext *ctx);
void debugDrawTransform(b2Transform transform, RenderContext *ctx);
void debugDrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color,
                      RenderContext *ctx);

struct b2DebugDraw debug_draw = {
    .DrawPolygon =
        ((void (*)(const b2Vec2 *, int, b2HexColor, void *))debugDrawPolygon),
    .DrawSolidPolygon = ((void (*)(b2Transform, const b2Vec2 *, int, float,
                                   b2HexColor, void *))debugDrawSolidPolygon),
    .DrawCircle =
        ((void (*)(b2Vec2, float, b2HexColor, void *))debugDrawCircle),
    .DrawSolidCircle = ((
        void (*)(b2Transform, float, b2HexColor, void *))debugDrawSolidCircle),
    .DrawCapsule =
        ((void (*)(b2Vec2, b2Vec2, float, b2HexColor, void *))debugDrawCapsule),
    .DrawSolidCapsule = ((void (*)(b2Vec2, b2Vec2, float, b2HexColor,
                                   void *))debugDrawSolidCapsule),
    .DrawSegment =
        ((void (*)(b2Vec2, b2Vec2, b2HexColor, void *))debugDrawSegment),
    .DrawTransform = ((void (*)(b2Transform, void *))debugDrawTransform),
    .DrawPoint = ((void (*)(b2Vec2, float, b2HexColor, void *))debugDrawPoint),
    .DrawString = ((void (*)(b2Vec2, const char *, void *))debugDrawString),

    .drawingBounds = (b2AABB){(b2Vec2){0, 0}, (b2Vec2){0, 0}},
    .useDrawingBounds = false,
    .drawShapes = true,
    .drawJoints = false,
    .drawJointExtras = false,
    .drawAABBs = false,
    .drawMass = false,
    .drawContacts = false,
    .drawGraphColors = false,
    .drawContactNormals = false,
    .drawContactImpulses = false,
    .drawFrictionImpulses = false,
    .context = NULL,
};

void setColor(RenderContext *ctx, b2HexColor color, int alpha) {
  const double r = ((color >> 16) & 0xFF);
  const double g = ((color >> 8) & 0xFF);
  const double b = (color & 0xFF);
  SDL_SetRenderDrawColor(ctx->renderer, r, g, b, alpha);
}

SDL_FColor getColor(b2HexColor color, int alpha) {
  const float r = ((color >> 16) & 0xFF) / 255.0f;
  const float g = ((color >> 8) & 0xFF) / 255.0f;
  const float b = (color & 0xFF) / 255.0f;
  const float a = alpha / 255.0f;

  return (SDL_FColor){r, g, b, a};
}

void debugDrawPolygon(const b2Vec2 *vertices, int vertex_count,
                      b2HexColor color, RenderContext *ctx) {

  if (ctx == NULL || vertex_count < 3)
    return;
  SDL_FPoint transform = RenderContext_getTransform(ctx);

  setColor(ctx, color, 0xFF);
  SDL_RenderLine(
      ctx->renderer, vertices[vertex_count - 1].x * PPM_F + transform.x,
      vertices[vertex_count - 1].y * PPM_F + transform.y,
      vertices[0].x * PPM_F + transform.x, vertices[0].y * PPM_F + transform.y);
  for (int i = 1; i < vertex_count; i++) {
    SDL_RenderLine(ctx->renderer, vertices[i - 1].x * PPM_F + transform.x,
                   vertices[i - 1].y * PPM_F + transform.y,
                   vertices[i].x * PPM_F + transform.x,
                   vertices[i].y * PPM_F + transform.y);
  }
}

void debugDrawSolidPolygon(b2Transform transform, const b2Vec2 *vertices,
                           int vertex_count, float radius, b2HexColor color,
                           RenderContext *ctx) {
  if (ctx == NULL)
    return;
  SDL_FPoint tf = RenderContext_getTransform(ctx);
  float tx = transform.p.x * PPM_F + tf.x;
  float ty = transform.p.y * PPM_F + tf.y;
  SDL_FColor g_color = getColor(color, 0x7f);
  SDL_Vertex *g_vertices =
      allocMem(&std_allocator, sizeof(SDL_Vertex), vertex_count);
  for (int i = 0; i < vertex_count; i++) {
    g_vertices[i].position.x = vertices[i].x * PPM_F + tx;
    g_vertices[i].position.y = vertices[i].y * PPM_F + ty;
    g_vertices[i].color = g_color;
  }

  int index_length = (vertex_count - 2) * 3;
  int *indices = allocMem(&std_allocator, sizeof(int), index_length);
  for (int i = 0; i < vertex_count - 2; i++) {
    indices[i * 3] = 0;
    indices[i * 3 + 1] = i + 1;
    indices[i * 3 + 2] = i + 2;
  }

  if (!SDL_RenderGeometry(ctx->renderer, NULL, g_vertices, vertex_count,
                          indices, index_length)) {
    trace("SDL_GetError(): %s", SDL_GetError());
  };
  freeMem(&std_allocator, indices);
  freeMem(&std_allocator, g_vertices);
}

void debugDrawPoint(b2Vec2 p, float size, b2HexColor color,
                    RenderContext *ctx) {
  if (ctx == NULL)
    return;
  SDL_FPoint tf = RenderContext_getTransform(ctx);

  setColor(ctx, color, 0xFF);
  SDL_FRect rect = {
      .x = (p.x - size / 2) * PPM_F + tf.x,
      .y = (p.y - size / 2) * PPM_F + tf.y,
      .w = size,
      .h = size,
  };
  SDL_RenderFillRect(ctx->renderer, &rect);
}
void debugDrawCircle(b2Vec2 center, float radius, b2HexColor color,
                     RenderContext *ctx) {
  if (ctx == NULL)
    return;
}
void debugDrawSolidCircle(b2Transform transform, float radius, b2HexColor color,
                          RenderContext *ctx) {
  if (ctx == NULL)
    return;
}
void debugDrawCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color,
                      RenderContext *ctx) {
  if (ctx == NULL)
    return;
}
void debugDrawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius, b2HexColor color,
                           RenderContext *ctx) {
  if (ctx == NULL)
    return;
}
void debugDrawString(b2Vec2 p, const char *s, RenderContext *ctx) {
  if (ctx == NULL)
    return;

  SDL_FPoint tf = RenderContext_getTransform(ctx);

  setColor(ctx, b2_colorWhite, 0xFF);
  SDL_RenderDebugText(ctx->renderer, p.x * PPM_F + tf.x, p.y * PPM_F + tf.y, s);
}

void debugDrawTransform(b2Transform transform, RenderContext *ctx) {
  if (ctx == NULL)
    return;
}
void debugDrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color,
                      RenderContext *ctx) {
  if (ctx == NULL)
    return;
  SDL_FPoint tf = RenderContext_getTransform(ctx);
  setColor(ctx, color, 0xFF);
  SDL_RenderLine(ctx->renderer, p1.x * PPM_F + tf.x, p1.y * PPM_F + tf.y,
                 p2.x * PPM_F + tf.x, p2.y * PPM_F + tf.y);
}
