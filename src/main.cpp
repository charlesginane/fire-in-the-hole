#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "scene/scene.hh"
#include "scene/particle.hh"

#define TEST_OPENGL_ERROR()                                                             \
  do {									\
    GLenum err = glGetError();					                        \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR!" << __LINE__ << std::endl;      \
  } while(0)

Scene scene;

int main() {
    /* init the scene */
    scene = Scene(2000, 2000, 60000);
    if (scene.init() == false) {
        std::cerr << "\033[34m ERROR: Failed to initialize the scene!" << std::endl;
        return 1;
    }

    if (scene.shader("src/scene/vertex.shd", "src/scene/fragment.shd") == -1) {
        std::cerr << "\033[34m ERROR: Failed to initialize shaders!" << std::endl;
        return 2;
    }
    scene.init_motor(500);
    scene.init_object();

    /* Loop */
    while (!glfwWindowShouldClose(scene.window_get()))
    {
        scene.display();
        scene.update();
        scene.init_object();
        glfwPollEvents();
    }
    glfwDestroyWindow(scene.window_get());
    glfwTerminate();

    return 0;
}
