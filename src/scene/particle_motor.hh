#pragma once

#include <vector>

#include "particle.hh"
#include "vector3.hh"

class Motor {

public:
    Motor(unsigned int nb_particles);

private:
    unsigned int nb_particles_;
    std::vector<Particle> list_particles_;
};
