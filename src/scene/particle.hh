#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "vector3.hh"

struct Color {
    GLfloat red = 255;
    GLfloat green = 255;
    GLfloat blue = 255;
};

class Particle {
    public:
        /* Constructors */
        Particle();
        Particle(GLuint radius, float speed, int id);

        /* Create a particle */
        vector::Vector3 create();

        /* Update the particle */
        vector::Vector3 update(int wind);

        /* Get the position */
        vector::Vector3 position_get() const;

        /* Get the color */
        struct Color color_get() const;

    private:
        GLuint radius_;
        float speed_;
        int descent_;
        vector::Vector3 position_;
        int id_;
        struct Color color_;
        float translate_;
};
