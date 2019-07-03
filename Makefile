CXX=g++
CXX_FLAGS=-Wall -Wextra -pedantic -std=c++17 -g -Iinclude/
LDXX_FLAGS = -lGL -lGLEW -lpthread -lglfw -lX11 -lXrandr -lXi -ldl

SRC_FILES=src/main.cpp src/scene/scene.cc src/scene/particle.cpp src/scene/vector3.cc src/scene/particle_motor.cpp src/glad.c
OBJ_FILES=src/main.o src/scene/scene.o src/scene/particle.o src/scene/vector3.o src/scene/particle_motor.o

all: ${SRC_FILES}
	${CXX} -o out ${CXX_FLAGS} ${LDXX_FLAGS} ${SRC_FILES}
