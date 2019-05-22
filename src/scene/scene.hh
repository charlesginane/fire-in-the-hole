#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

class Scene {

std::vector<GLfloat> vertices = {
 0.0f,  0.5f, // Vertex 1 (X, Y)
 0.5f, -0.5f, // Vertex 2 (X, Y)
-0.5f, -0.5f  // Vertex 3 (X, Y)
};

public:
    /* Constructor */
    Scene(int width, int height);

    bool init(int argc, char *argv[]);

    bool shader(std::string vertex_shader_src, std::string fragment_shader_src);

    bool init_object();

private:
    int width_;
    int height_;
    GLuint program_;
    GLuint vao_;
};
