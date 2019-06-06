#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "scene/scene.hh"

#define TEST_OPENGL_ERROR()                                                             \
  do {									\
    GLenum err = glGetError();					                        \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR!" << __LINE__ << std::endl;      \
  } while(0)

GLuint program_compute = -1;
float translate = 0.0f;
float red = 255;
float blue = 255;
float green = 255;
bool up = true;
int descent = 3;


static void My_timer_event(int program)
{
    int vertexColorLocation = glGetUniformLocation(program, "ourColor");
    glUniform4f(vertexColorLocation, red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f);

    if (blue <= 0) {
        if (green <= 0) {
            red -= 2 * descent;
        }
        else
            green -= descent;
    }
    else
        blue -= descent;

    int translation = glGetUniformLocation(program, "translate");
    glUniform1f(translation, translate);
    if (up) {
        translate += 0.0075f;
    }
    else {
        translate -= 0.0075f;
    }

    if (translate >= 1.0f) {
        up = false;
    }
    else if (translate <= -1.0f) {
        up = true;
    }
    glBindVertexArray(0);

    glutPostRedisplay();
    glutTimerFunc(10, My_timer_event, program);

}

int main(int argc, char *argv[]) {
    auto scene = Scene(512, 512);
    scene.init(argc, argv);
    scene.init_texture();
    scene.shader("src/scene/vertex.shd", "src/scene/fragment.shd", "src/scene/compute_shader.shd");
    scene.init_object();

    auto program = scene.program_get();

    glutTimerFunc(1000, My_timer_event, program);

    glutMainLoop();
    return 0;
}
