#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "vector3.hh"

class Particle {
    public:
        Particle(GLuint radius, GLuint speed, vector::Vector3 position);

        void create();

        void evolution();

        void destruct();

        vector::Vector3 position_get() const;

    private:
        GLuint radius_;
        GLuint speed_;
        vector::Vector3 position_;
        vector::Vector3 color_;
};
