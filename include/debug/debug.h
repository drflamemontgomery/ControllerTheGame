/*
    Text-based debugging helper functions
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

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define trace(msg, ...)                                                        \
  fprintf(stderr, "[%s:%d]: " msg "\n", __FILE__,                              \
          __LINE__ __VA_OPT__(, ) __VA_ARGS__)
#define errtrace(msg, ...)                                                     \
  fprintf(stderr, "ERROR [%s:%d]: " msg "\n", __FILE__,                        \
          __LINE__ __VA_OPT__(, ) __VA_ARGS__)

#define debugAssert(cond, msg, ...)                                            \
  if (!(cond)) {                                                               \
    errtrace("%s() " msg, __func__ __VA_OPT__(, ) __VA_ARGS__);                \
    abort();                                                                   \
  }
#else
#define trace(msg, ...)
#define debugAssert(cond, msg, ...)
#endif

#endif // DEBUG_H
