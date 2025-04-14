/*
    UInput Controller Interface Structure
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

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <linux/uinput.h>
#include <stdbool.h>

#define HAT_UP 0
#define HAT_RIGHT 1
#define HAT_DOWN 2
#define HAT_LEFT 3

typedef struct ControllerDevice {
  int fd;
  union {
    struct {
      unsigned char up : 1;
      unsigned char right : 1;
      unsigned char down : 1;
      unsigned char left : 1;
    };
    char bytes : 4;
  } hat;
} ControllerDevice;

ControllerDevice ControllerDevice_default();
void ControllerDevice_destroy(ControllerDevice *self);
void ControllerDevice_setButton(ControllerDevice *self, int button_id,
                                bool value);
void ControllerDevice_setAxis(ControllerDevice *self, int axis_id, int value);
void ControllerDevice_setHat(ControllerDevice *self, int hat_id, int value);

enum ComponentId {
  Button,
  Axis,
  Hat,
};

// Components can control up to four Buttons|Axis|Hat Directions
typedef struct ControllerComponent {
  enum ComponentId id;
  struct {
    size_t length;
    size_t max_value;
    int *value;
  } ids;
} ControllerComponent;
ControllerComponent ControllerComponent_create(enum ComponentId id);
int ControllerComponent_getPrimary(ControllerComponent *self);
int ControllerComponent_getSecondary(ControllerComponent *self);
int ControllerComponent_getTertiary(ControllerComponent *self);
int ControllerComponent_getQuaterny(ControllerComponent *self);

#endif // CONTROLLER_H
