#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "scene/scene.hh"

int main(int argc, char *argv[]) {
    auto scene = Scene(1000, 1000);
    scene.init(argc, argv);
    scene.shader("src/scene/vertex.shd", "src/scene/fragment.shd");
    glutMainLoop();
    return 0;
}
