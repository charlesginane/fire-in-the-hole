#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

class Scene {

public:
    /* Constructor */
    Scene(int width, int height);

    bool init(int argc, char *argv[]);

    GLuint shader(std::string vertex_shader_src, std::string fragment_shader_src, std::string compute_shader_src);

    bool init_object();

    GLuint init_texture();

    GLuint execution_compute_shader();

    void update();

    /* Getters */
    GLuint program_compute_get() const;

    GLuint program_get() const;

    GLuint vao_get() const;

private:
    int width_;
    int height_;
    int window_;
    GLuint program_;
    GLuint vao_;
    GLuint program_compute_;
    GLuint textid_;
};
