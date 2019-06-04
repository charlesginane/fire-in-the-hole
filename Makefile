CXX=g++
CXX_FLAGS=-Wall -Wextra -pedantic -std=c++17 -g
LDXX_FLAGS = -lGL -lGLEW -lglut -lpthread

SRC_FILES=src/main.cpp src/scene/scene.cc src/scene/particle.cpp src/scene/vector3.cc src/scene/particle_motor.cpp
OBJ_FILES=src/main.o src/scene/scene.o src/scene/particle.o src/scene/vector3.o src/scene/particle_motor.o

all: ${OBJ_FILES}
	${CXX} -o out ${CXX_FLAGS} ${LDXX_FLAGS} ${SRC_FILES}
