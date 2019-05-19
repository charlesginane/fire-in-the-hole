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

    bool shader(std::string vertex_shader_src, std::string fragment_shader_src);

private:
    int width_;
    int height_;
};
