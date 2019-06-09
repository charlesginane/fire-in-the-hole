#pragma once

#include <vector>

#include "particle.hh"
#include "vector3.hh"

class Motor {

public:
    Motor(unsigned int nb_particles);

    std::vector<float> create();

    void update(GLint program, int nb_iter);

    std::vector<Particle> list_particles_get() const;

private:
    unsigned int nb_particles_;
    std::vector<Particle> list_particles_ ;
};
