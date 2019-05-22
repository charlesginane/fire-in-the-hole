CXX=g++
CXX_FLAGS=-Wall -Wextra -pedantic -std=c++17 -g
LDXX_FLAGS = -lGL -lGLEW -lglut -lpthread

SRC_FILES=src/main.cpp src/scene/scene.cc
OBJ_FILES=src/main.o src/scene/scene.o

all: ${OBJ_FILES}
	${CXX} -o out ${CXX_FLAGS} ${LDXX_FLAGS} ${SRC_FILES}
