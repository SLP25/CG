rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CC = g++
CFLAGS = -O0 -g -Wall -Wextra --pedantic-errors -Wno-unused-parameter -Werror

ifdef FEDORA
	CFLAGS+=-DFEDORA
endif

LIBS = -lGLEW -lGL -lGLU -lglut -Iinclude/

HEADERS = $(call rwildcard,include,*.hpp)
SRC = $(call rwildcard,src,*.cpp)
OBJS = ${SRC:src/%.cpp=obj/%.o}

.PHONY: default
default: all

all: engine generator

.PHONY: clean
clean:
	rm -f ${OBJS} generator engine


obj/%.o: src/%.cpp ${HEADERS}
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c -o $@ $< ${LIBS}


generator: $(filter-out obj/engine.o,$(OBJS))
	${CC} ${CFLAGS} -o bin/$@ $^ ${LIBS}

engine: $(filter-out obj/generator.o,$(OBJS))
	${CC} ${CFLAGS} -o bin/$@ $^ ${LIBS}
