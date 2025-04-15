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

ifdef NODEBUG
	DEBUGFLAGS := #empty
else
	DEBUGFLAGS := -DDEBUG=1 -g -fsanitize=leak -rdynamic -Wmissing-field-initializers -Wall -Werror -Wno-missing-braces -Wno-unused-parameter -W 
endif

# Object Files to be created. If this starts becoming larger, 
# it may need to be split into sub-makefiles.
OBJECTS = \
			obj/boot/main.o\
			obj/boot/app.o\
			obj/screen/ctx.o\
			obj/util/stack.o\
			obj/util/list.o\
			obj/heap/allocator.o\
			obj/en/obj.o\
			obj/en/player.o\
			obj/en/testobj.o\
			obj/input/controller.o\
			obj/debug/debug_draw.o\
			$(END)

# Directories to be created in the obj directory structure
# It is difficult to generate a rule automatically so it
# is manually specified.
OBJDIRS = \
					obj/boot\
					obj/screen\
					obj/util\
					obj/en\
					obj/input\
					obj/debug\
					obj/heap\
					$(END)

LIBS := -lSDL3 -lbox2d
CFLAGS := $(CFLAGS) -Iinclude $(DEBUGFLAGS)

# Define the global project name for easy use
PROJECT_NAME := controller_game

default: build

.PHONY: build run
build: bin/$(PROJECT_NAME)
run: bin/$(PROJECT_NAME)
	./bin/$(PROJECT_NAME)

bin:
	mkdir -p bin

bin/$(PROJECT_NAME): bin $(OBJDIRS) $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) -o bin/$(PROJECT_NAME) $(OBJECTS)

obj/%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIRS):
	mkdir -p $@

# Need to run `make clean` if header files have been modified
# to clear the cache and prevent incorrect structure alignment
# in compiled files
.PHONY: clean
clean:
	rm $(OBJECTS)
