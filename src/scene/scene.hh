#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
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
    Scene(int width, int height, unsigned int max_part);

    bool init();

    void display();

    GLint shader(std::string vertex_shader_src, std::string fragment_shader_src);

    bool init_object();

    Motor init_motor(unsigned int nb_particles);

    GLuint execution_compute_shader();

    void update();

    /* Getters */
    GLuint program_get() const;

    GLuint vao_get() const;

    GLFWwindow* window_get() const;

private:
    int width_;
    int height_;
    GLuint program_;
    GLuint vao_;
    GLint vbo_vertices_;
    GLint vbo_color_;
    Motor motor_;
    GLFWwindow* window_;
    unsigned int max_particules_;
};
