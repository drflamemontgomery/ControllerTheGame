/*
    Memory Safety Functions
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

#ifndef SAFE_H
#define SAFE_H

#define safefn(fn, ...)                                                        \
  if (fn != NULL) {                                                            \
    fn(__VA_ARGS__);                                                           \
  }

#define safefnelse(orelse, fn, ...) (fn == NULL ? orelse : fn(__VA_ARGS__))

#define objcall(obj, fn, ...) safefn(obj.fn, &obj __VA_OPT__(, ) __VA_ARGS__)
#define objrefcall(obj, fn, ...) safefn(obj->fn, obj __VA_OPT__(, ) __VA_ARGS__)
#define objcallorelse(orelse, obj, fn, ...)                                    \
  safefnelse(orelse, obj.fn, &obj __VA_OPT__(, ) __VA_ARGS__)
#define objrefcallorelse(orelse, obj, fn, ...)                                 \
  safefnelse(orelse, obj->fn, obj __VA_OPT__(, ) __VA_ARGS__)

#define safefree(ptr)                                                          \
  if (ptr != NULL) {                                                           \
    free(ptr);                                                                 \
    ptr = NULL;                                                                \
  }

#endif // SAFE_H
