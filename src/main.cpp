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

void My_timer_event(int program)
{
    scene.update(program);
    glutPostRedisplay();
    glutTimerFunc(10, My_timer_event, program);

}

int main(int argc, char *argv[]) {
    scene = Scene(2000, 2000);
    scene.init(argc, argv);
    scene.shader("src/scene/vertex.shd", "src/scene/fragment.shd", "src/scene/compute_shader.shd");
    scene.init_motor(500);
    scene.init_object();
    scene.init_texture();
    auto program = scene.program_get();

    int work_grp_cnt[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

printf("max global (total) work group size x:%i y:%i z:%i\n",
  work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

    glutTimerFunc(1000, My_timer_event, program);
    glutMainLoop();
    return 0;
}
