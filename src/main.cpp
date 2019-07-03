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

void My_timer_event(int program)
{
    scene.update(program);
    //glutPostRedisplay();
    //glutTimerFunc(10, My_timer_event, program);

}

int main(int argc, char *argv[]) {
    scene = Scene(2000, 2000);
    scene.init(argc, argv);
    scene.shader("src/scene/vertex.shd", "src/scene/fragment.shd", "src/scene/compute_shader.shd");
    scene.init_motor(500);
    scene.init_object();
    scene.init_texture();
    auto program = scene.program_get();

    while (!glfwWindowShouldClose(scene.window_get()))
    {
        // float ratio;
        // int width, height;
        // mat4x4 m, p, mvp;
        // glfwGetFramebufferSize(window, &width, &height);
        // ratio = width / (float) height;
        // glViewport(0, 0, width, height);
        // glClear(GL_COLOR_BUFFER_BIT);
        // mat4x4_identity(m);
        // mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        // mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        // mat4x4_mul(mvp, p, m);
        // glUseProgram(program);
        // glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        scene.display();
        glfwPollEvents();
        scene.update(program);
    }
    glfwDestroyWindow(scene.window_get());
    glfwTerminate();

    return 0;
}
