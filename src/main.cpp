#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "scene/scene.hh"
#include "scene/particle.hh"

#define TEST_OPENGL_ERROR()                                                             \
  do {									\
    GLenum err = glGetError();					                        \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR!" << __LINE__ << std::endl;      \
  } while(0)

Scene scene;

void My_timer_event(int nb_iter)
{
    // int vertexColorLocation = glGetUniformLocation(program, "ourColor");
    // glUniform4f(vertexColorLocation, red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f);

    // if (blue <= 0) {
    //     if (green <= 0) {
    //         red -= 2 * descent;
    //     }
    //     else
    //         green -= descent;
    // }
    // else
    //     blue -= descent;
    //
    // int translation = glGetUniformLocation(program, "translate");
    // glUniform1f(translation, translate);
    // if (up) {
    //     translate += 0.0075f;
    // }
    // else {
    //     translate -= 0.0075f;
    // }
    //
    // if (translate >= 1.0f) {
    //     up = false;
    // }
    // else if (translate <= -1.0f) {
    //     up = true;
    // }
    // glBindVertexArray(0);
    scene.update(scene.program_get(), nb_iter);
    glutPostRedisplay();
    glutTimerFunc(100, My_timer_event, ++nb_iter);

}

int main(int argc, char *argv[]) {
    scene = Scene(512, 512);
    scene.init(argc, argv);
    scene.shader("src/scene/vertex.shd", "src/scene/fragment.shd", "src/scene/compute_shader.shd");
    scene.init_motor(500);
    scene.init_object();
    auto program = scene.program_get();

    glutTimerFunc(1000, My_timer_event, 0);
    // for (int i = 0; i < 1024; ++i) {
    //     scene.update(program, i);
    // }

    // std::vector<Particle> list_p;
    // auto p = Particle(1, 1);
    // list_p.push_back(p);
    // for (int i = 0; i < 3; ++i) {
    //   for (auto& par: list_p) {
    //     par.update(program, 1);
    //   }
    // }
    glutMainLoop();
    return 0;
}
