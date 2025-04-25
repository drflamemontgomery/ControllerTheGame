/*
    Controller Game Object
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

#include "en/controller.h"
#include "debug/debug.h"
#include "en/obj.h"
#include "input/controller.h"
#include "util/options.h"
#include <box2d/types.h>

ControllerObj ControllerObj_create(b2WorldId world) {
  Object2D super = Object2D_default();
  super.destroy = (typeof(super.destroy))ControllerObj_destroy;

  b2BodyDef bd = b2DefaultBodyDef();
  bd.position = (b2Vec2){2.0f, -2.0f};
  bd.type = b2_staticBody;
  bd.fixedRotation = true;

  b2BodyId body = b2CreateBody(world, &bd);

  // Debug placeholder shape
  b2Polygon shape = b2MakeBox(1.0f, 1.0f);

  b2ShapeDef shape_def = b2DefaultShapeDef();
  shape_def.density = 1.0f;
  b2CreatePolygonShape(body, &shape_def, &shape);

  return (ControllerObj){
      .super_object2d = super,
      .body = body,
      .controller = ControllerDevice_default(),
  };
}

void ControllerObj_destroy(ControllerObj *self) {
  debugAssert(self != NULL, "self == NULL");
  Object2D_destroy(&self->super_object2d);

  b2DestroyBody(self->body);
  ControllerDevice_destroy(&self->controller);
}
