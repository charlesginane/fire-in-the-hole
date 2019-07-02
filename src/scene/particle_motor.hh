#pragma once

#include <tuple>
#include <vector>

#include "particle.hh"
#include "vector3.hh"

class Motor {

public:
    Motor(unsigned int nb_particles);

    std::tuple<std::vector<GLfloat>, std::vector<GLfloat>> create(int nb);

    std::tuple<std::vector<GLfloat>, std::vector<GLfloat>> update(GLint program);

    std::vector<Particle> list_particles_get() const;

private:
    unsigned int nb_particles_;
    std::vector<Particle> list_particles_;
};