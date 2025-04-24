/*
    Game Player + Controls
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

#include <box2d/types.h>
#include <stdio.h>

#include "debug/debug.h"
#include "en/player.h"
#include "input/key.h"
#include "screen/ctx.h"
#include "util/options.h"
#include "util/safe.h"

Player Player_create(b2WorldId world, float x, float y,
                     PlayerController *controller) {
  Object2D super = Object2D_create(x, y, 8, 16);
  super.update = (void (*)(Object2D *, double))Player_update;
  super.render = (void (*)(Object2D *, RenderContext *))Player_render;
  super.destroy = (void (*)(Object2D *))Player_destroy;

  b2BodyDef bd = b2DefaultBodyDef();
  bd.type = b2_dynamicBody;
  bd.position = (b2Vec2){x, y};
  bd.fixedRotation = true;

  b2BodyId body = b2CreateBody(world, &bd);

  b2Polygon shape =
      b2MakeBox(super.width / 2.0f / PPM_F, super.height / 2.0f / PPM_F);
  b2ShapeDef shape_def = b2DefaultShapeDef();
  shape_def.density = 1.0f;
  b2CreatePolygonShape(body, &shape_def, &shape);

  return (Player){
      .super = super,
      .body = body,
      .controller = controller,
  };
}

void Player_destroy(Player *self) {
  b2DestroyBody(self->body);
  self->body = (b2BodyId){0, 0, 0};

  // TODO make a controller subsystem responsible for destruction
  objrefcall(self->controller, destroy);
  self->controller = NULL;

  Object2D_destroy(&self->super);
}

void Player_render(Player *self, RenderContext *ctx) {
  SDL_FPoint transform = RenderContext_getTransform(ctx);
  SDL_FRect rect = {
      .x = transform.x - self->super.width / 2.0f,
      .y = transform.y - self->super.height / 2.0f,
      .w = self->super.width,
      .h = self->super.height,
  };
  SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderFillRect(ctx->renderer, &rect);
  Object2D_render(&self->super, ctx);
}

void Player_update(Player *self, double delta_time) {
  b2Vec2 pos = b2Body_GetPosition(self->body);
  self->super.pos = (SDL_FPoint){pos.x * PPM_F, pos.y * PPM_F};

  if (self->controller == NULL)
    return;

  b2Vec2 vel = b2Body_GetLinearVelocity(self->body);

  b2Vec2 result = {
      objrefcallorelse(0, self->controller, getAxisX),
      vel.y + objrefcallorelse(0, self->controller, getAxisY),
  };
  if (result.x != 0.0f || result.y != vel.y) {
    vel = b2Lerp(vel, result, 0.3f);
  } else {
    const b2Vec2 friction = (b2Vec2){
        0.9f,
        1.0f,
    };
    vel = b2Mul(vel, friction);
  }

  b2Body_SetLinearVelocity(self->body, vel);
}

/*
 * CONTROLLER CODE
 */

float KeyboardController_getAxisX(KeyboardController *self) {
  return KEYS[self->right] - KEYS[self->left];
}
float KeyboardController_getAxisY(KeyboardController *self) {
  return KEYS[self->down] - KEYS[self->up];
}
float KeyboardController_getAxisRX(KeyboardController *self) {
  return KEYS[self->secondary_right] - KEYS[self->secondary_left];
}
float KeyboardController_getAxisRY(KeyboardController *self) {
  return KEYS[self->secondary_down] - KEYS[self->secondary_up];
}

int KeyboardController_getHatX(KeyboardController *self) {
  return KEYS[self->right] - KEYS[self->left];
}
int KeyboardController_getHatY(KeyboardController *self) {
  return KEYS[self->down] - KEYS[self->up];
}

bool KeyboardController_getPrimaryButton(KeyboardController *self) {
  return KEYS[self->primary];
}
bool KeyboardController_getSecondaryButton(KeyboardController *self) {
  return KEYS[self->secondary];
}
bool KeyboardController_getTertiaryButton(KeyboardController *self) {
  return KEYS[self->tertiary];
}
bool KeyboardController_getQuaternyButton(KeyboardController *self) {
  return KEYS[self->quaterny];
}

bool KeyboardController_getJump(KeyboardController *self) {
  return KEYS[self->jump];
}
bool KeyboardController_getAction(KeyboardController *self) {
  return KEYS[self->action];
}
bool KeyboardController_getDismount(KeyboardController *self) {
  return KEYS[self->dismount];
}
bool KeyboardController_getAttack(KeyboardController *self) {
  return KEYS[self->attack];
}
void KeyboardController_destroy(KeyboardController *self) {
  debugAssert(self != NULL, "self == NULL");
  freePtr(self->allocator, self);
}

const PlayerController KEYBOARD_SUPER = {
    .getAxisX = (float (*)(PlayerController *))KeyboardController_getAxisX,
    .getAxisY = (float (*)(PlayerController *))KeyboardController_getAxisY,
    .getAxisRX = (float (*)(PlayerController *))KeyboardController_getAxisRX,
    .getAxisRY = (float (*)(PlayerController *))KeyboardController_getAxisRY,

    .getHatX = (int (*)(PlayerController *))KeyboardController_getHatX,
    .getHatY = (int (*)(PlayerController *))KeyboardController_getHatY,

    .getPrimaryButton =
        (bool (*)(PlayerController *))KeyboardController_getPrimaryButton,
    .getSecondaryButton =
        (bool (*)(PlayerController *))KeyboardController_getSecondaryButton,
    .getTertiaryButton =
        (bool (*)(PlayerController *))KeyboardController_getTertiaryButton,
    .getQuaternyButton =
        (bool (*)(PlayerController *))KeyboardController_getQuaternyButton,

    .getJump = (bool (*)(PlayerController *))KeyboardController_getJump,
    .getAction = (bool (*)(PlayerController *))KeyboardController_getAction,
    .getDismount = (bool (*)(PlayerController *))KeyboardController_getDismount,
    .getAttack = (bool (*)(PlayerController *))KeyboardController_getAttack,
    .destroy = (void (*)(PlayerController *))KeyboardController_destroy,
};

KeyboardController *KeyboardController_default(Allocator *allocator) {
  KeyboardController *self = allocPtr(allocator, sizeof(KeyboardController), 1);

  *self = (KeyboardController){
      .super = KEYBOARD_SUPER,
      .allocator = allocator,

      .left = SDL_SCANCODE_LEFT,
      .right = SDL_SCANCODE_RIGHT,
      .up = SDL_SCANCODE_UP,
      .down = SDL_SCANCODE_DOWN,

      .secondary_left = SDL_SCANCODE_H,
      .secondary_right = SDL_SCANCODE_L,
      .secondary_up = SDL_SCANCODE_K,
      .secondary_down = SDL_SCANCODE_J,

      .jump = SDL_SCANCODE_Z,
      .action = SDL_SCANCODE_C,
      .dismount = SDL_SCANCODE_A,
      .attack = SDL_SCANCODE_X,

      .primary = SDL_SCANCODE_Z,
      .secondary = SDL_SCANCODE_X,
      .tertiary = SDL_SCANCODE_C,
      .quaterny = SDL_SCANCODE_V,
  };
  return self;
}
