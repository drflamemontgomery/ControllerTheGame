/*
    UInput Controller Wrapper
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

#include "input/controller.h"
#include <fcntl.h>
#include <linux/uinput.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "debug/debug.h"

const ControllerDevice NullControllerDevice = {
    .fd = -1,
    .hat = {0},
};

ControllerDevice ControllerDevice_default() {
  struct uinput_setup usetup;
  int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

  ioctl(fd, UI_SET_EVBIT, EV_KEY);
  ioctl(fd, UI_SET_KEYBIT, BTN_A);
  ioctl(fd, UI_SET_KEYBIT, BTN_B);
  ioctl(fd, UI_SET_KEYBIT, BTN_X);
  ioctl(fd, UI_SET_KEYBIT, BTN_Y);

  ioctl(fd, UI_SET_KEYBIT, BTN_TL);
  ioctl(fd, UI_SET_KEYBIT, BTN_TR);
  ioctl(fd, UI_SET_KEYBIT, BTN_TL2);
  ioctl(fd, UI_SET_KEYBIT, BTN_TR2);
  ioctl(fd, UI_SET_KEYBIT, BTN_START);
  ioctl(fd, UI_SET_KEYBIT, BTN_SELECT);
  ioctl(fd, UI_SET_KEYBIT, BTN_THUMBL);
  ioctl(fd, UI_SET_KEYBIT, BTN_THUMBR);

  ioctl(fd, UI_SET_EVBIT, EV_ABS);
  struct uinput_abs_setup abs = {
      .code = ABS_X,
      .absinfo =
          {
              .value = 0,
              .minimum = -32767,
              .maximum = 32767,
              .fuzz = 16,
              .flat = 128,
              .resolution = 0,
          },
  };

  ioctl(fd, UI_SET_EVBIT, EV_ABS);
  ioctl(fd, UI_ABS_SETUP, &abs);
  abs.code = ABS_Y;
  ioctl(fd, UI_ABS_SETUP, &abs);
  abs.code = ABS_RX;
  ioctl(fd, UI_ABS_SETUP, &abs);
  abs.code = ABS_RY;
  ioctl(fd, UI_ABS_SETUP, &abs);

  abs.absinfo.flat = 0;
  abs.absinfo.fuzz = 0;
  abs.absinfo.maximum = 1;
  abs.absinfo.minimum = -1;
  abs.code = ABS_HAT0X;
  ioctl(fd, UI_ABS_SETUP, &abs);
  abs.code = ABS_HAT0Y;
  ioctl(fd, UI_ABS_SETUP, &abs);

  memset(&usetup, 0, sizeof(usetup));
  usetup.id.bustype = BUS_USB;
  usetup.id.vendor = 0x28de;
  usetup.id.product = 0x11ff;
  strcpy(usetup.name, "Microsoft X-Box 360 pad x");

  if (-1 == ioctl(fd, UI_DEV_SETUP, &usetup)) {
    fprintf(stderr, "Failed to setup device\n");
    return NullControllerDevice;
  }
  if (-1 == ioctl(fd, UI_DEV_CREATE)) {
    fprintf(stderr, "Failed to create device\n");
    return NullControllerDevice;
  }
  trace("Create Device");
  return (ControllerDevice){
      .fd = fd,
      .hat = {0},
  };
}

void ControllerDevice_destroy(ControllerDevice *self) {
  ioctl(self->fd, UI_DEV_DESTROY);
  close(self->fd);
  self->fd = -1;

  trace("Destroy Device");
}

void emit(int fd, int type, int code, int val) {
  struct input_event ie;

  ie.type = type;
  ie.code = code;
  ie.value = val;

  ie.time.tv_sec = 0;
  ie.time.tv_usec = 0;

  write(fd, &ie, sizeof(ie));
}

void ControllerDevice_setButton(ControllerDevice *self, int id, bool value) {
  emit(self->fd, EV_KEY, id, value);
  emit(self->fd, EV_SYN, SYN_REPORT, 0);
}
void ControllerDevice_setAxis(ControllerDevice *self, int axis_id, int value) {
  emit(self->fd, EV_ABS, axis_id, value);
  emit(self->fd, EV_SYN, SYN_REPORT, 0);
}
void ControllerDevice_setHat(ControllerDevice *self, int hat_id, int value) {
  if (hat_id < 0 || hat_id > 3)
    return;

  if (value == 0) {
    self->hat.bytes ^= 1 << hat_id;
  } else {
    self->hat.bytes |= 1 << hat_id;
  }

  int dx = self->hat.right - self->hat.left;
  int dy = self->hat.up - self->hat.down;

  emit(self->fd, EV_ABS, ABS_HAT0X, dx);
  emit(self->fd, EV_ABS, ABS_HAT0Y, dy);
  emit(self->fd, EV_SYN, SYN_REPORT, 0);
}

ControllerComponent ControllerComponent_create(Allocator *allocator,
                                               enum ComponentId id) {
  return (ControllerComponent){
      .allocator = allocator,
      .id = id,
      .ids =
          {
              .length = 0,
              .max_value = 4,
              .value = allocPtr(int, allocator, 4),
          },
  };
}
int ControllerComponent_getPrimary(const ControllerComponent *self) {
  return self->ids.length > 0 ? self->ids.value[0] : -1;
}
int ControllerComponent_getSecondary(const ControllerComponent *self) {
  return self->ids.length > 1 ? self->ids.value[1] : -1;
}
int ControllerComponent_getTertiary(const ControllerComponent *self) {
  return self->ids.length > 2 ? self->ids.value[2] : -1;
}
int ControllerComponent_getQuaterny(const ControllerComponent *self) {
  return self->ids.length > 3 ? self->ids.value[3] : -1;
}

void ControllerComponent_destroy(ControllerComponent *self) {
  debugAssert(self != NULL, "self == NULL");
  freePtr(self->allocator, self);
}
