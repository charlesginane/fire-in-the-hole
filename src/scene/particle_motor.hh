#pragma once

#include <tuple>
#include <vector>

#include "particle.hh"
#include "vector3.hh"

class Motor {

public:
    Motor(unsigned int nb_particles);

    /* Create nb particles */
    std::tuple<std::vector<GLfloat>, std::vector<GLfloat>> create(int nb);

    /* Update particles */
    std::tuple<std::vector<GLfloat>, std::vector<GLfloat>> update(int wind);

    // Get the list
    std::vector<Particle> list_particles_get() const;

    // Delete nb particles
    void kill(int nb);

private:
    unsigned int nb_particles_;
    std::vector<Particle> list_particles_;
};
