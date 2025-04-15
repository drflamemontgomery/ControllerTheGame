/*
    Game Player + Controls Structure
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

#ifndef PLAYER_H
#define PLAYER_H

#include "en/obj.h"
#include "heap/allocator.h"
#include <box2d/box2d.h>

typedef struct PlayerController {
  float (*getAxisX)(struct PlayerController *);
  float (*getAxisY)(struct PlayerController *);
  float (*getAxisRX)(struct PlayerController *);
  float (*getAxisRY)(struct PlayerController *);

  int (*getHatX)(struct PlayerController *);
  int (*getHatY)(struct PlayerController *);

  bool (*getPrimaryButton)(struct PlayerController *);
  bool (*getSecondaryButton)(struct PlayerController *);
  bool (*getTertiaryButton)(struct PlayerController *);
  bool (*getQuaternyButton)(struct PlayerController *);

  bool (*getJump)(struct PlayerController *);
  bool (*getAction)(struct PlayerController *);
  bool (*getDismount)(struct PlayerController *);
  bool (*getAttack)(struct PlayerController *);

  void (*destroy)(struct PlayerController *);
} PlayerController;

typedef struct KeyboardController {

  PlayerController super;
  Allocator *allocator;

  int left;
  int right;
  int up;
  int down;

  int secondary_left;
  int secondary_right;
  int secondary_up;
  int secondary_down;

  int jump;
  int action;
  int dismount;
  int attack;

  int primary;
  int secondary;
  int tertiary;
  int quaterny;
} KeyboardController;

KeyboardController *KeyboardController_default(Allocator *allocator);

enum PlayerState {
  IDLE,
  RUNNING,
  JUMP,
  ATTACK,
};

typedef struct Player {
  Object2D super;
  PlayerController *controller;
  b2BodyId body;
} Player;

Player Player_create(b2WorldId world, float x, float y,
                     PlayerController *controller);
void Player_destroy(Player *player);
void Player_render(Player *self, RenderContext *ctx);
void Player_update(Player *self, double delta_time);
#endif // PLAYER_H
