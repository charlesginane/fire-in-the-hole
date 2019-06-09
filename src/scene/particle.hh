#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "vector3.hh"

class Particle {
    public:
        struct Color {
            float red = 255;
            float green = 255;
            float blue = 255;
        };

        Particle();
        Particle(GLuint radius, GLuint speed);

        vector::Vector3 create();

        void update(GLint program, int nb_iter);

        void destruct();

        vector::Vector3 position_get() const;

    private:
        GLuint radius_;
        GLuint speed_;
        int descent_;
        vector::Vector3 position_;
        struct Color color_;
        float translate_;
};
