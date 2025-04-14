/*
    Application State Class Structure
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

#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <stdbool.h>

#include "en/player.h"
#include "en/testobj.h"
#include "input/controller.h"

typedef struct AppOptions {
  bool vsync;
  bool frame_cap;
} AppOptions;

typedef struct AppState {
  double delta_time;
  double last_tick;

  AppOptions options;
  Player player;
  Object2D *testobj;
  ControllerDevice controller_out;

  b2WorldId world;
} AppState;

AppState AppState_default();
void AppState_destroy(AppState *self);

#endif // APP_H
