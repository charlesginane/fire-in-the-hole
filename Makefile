CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -lglu32 -lopengl32 -lfreeglut
LDXX_FLAGS = -lGL  -lGLEW -lglut -lpthread

SRC_FILES=src/main.cpp
OBJ_FILES=src/main.o

all: ${OBJ_FILES}
	${CXX} -o out ${CXX_FLAGS} ${LDXX_FLAGS} ${OBJ_FILES}
