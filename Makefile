#    Makefile for compiling application
#    Copyright (C) 2025  Ashton Warner

#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.

CC = gcc
OBJECTS = \
			obj/boot/main.o\
			obj/boot/app.o\
			obj/screen/ctx.o\
			obj/util/stack.o\
			obj/util/list.o\
			obj/en/obj.o\
			obj/en/player.o\
			obj/en/testobj.o\
			obj/input/controller.o\
			obj/debug/debug_draw.o\
			$(END)
OBJDIRS = \
					obj/boot\
					obj/screen\
					obj/util\
					obj/en\
					obj/input\
					obj/debug\
					$(END)

LIBS := -lSDL3 -DDEBUG=1 -lbox2d -lcairo
CFLAGS := $(CFLAGS) -I/usr/include -I/usr/local/include -Iinclude
PROJECT_NAME := controller_game

default: build

build: bin/$(PROJECT_NAME)
run: bin/$(PROJECT_NAME)
	./bin/$(PROJECT_NAME)

.PHONY: build run

bin:
	mkdir -p bin

bin/$(PROJECT_NAME): bin $(OBJDIRS) $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) -o bin/$(PROJECT_NAME) $(OBJECTS)

obj/%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIRS):
	mkdir -p $@

clean:
	rm $(OBJECTS)
.PHONY: clean
