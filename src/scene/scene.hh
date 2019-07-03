#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "particle_motor.hh"

class Scene {

public:
    /* Constructor */
    Scene();
    Scene(int width, int height);

    bool init(int argc, char *argv[]);

    void display();

    GLuint shader(std::string vertex_shader_src, std::string fragment_shader_src, std::string compute_shader_src);

    bool init_object();

    Motor init_motor(unsigned int nb_particles);

    GLuint init_texture();

    GLuint execution_compute_shader();

    void update(int program);

    /* Getters */
    GLuint program_compute_get() const;

    GLuint program_get() const;

    GLuint vao_get() const;

    GLFWwindow* window_get() const;

private:
    int width_;
    int height_;
    GLuint program_;
    GLuint vao_;
    GLuint program_compute_;
    GLuint textid_;
    Motor motor_;
    GLuint vbo_vertices_;
    GLFWwindow* window_;
};
