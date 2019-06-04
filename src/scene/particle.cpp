#include "particle.hh"

Particle::Particle(GLuint radius, GLuint speed, vector::Vector3 position) : radius_(radius),
                                                                     speed_(speed),
                                                                     position_(position)
{}

void
Particle::create() {

}

void
Particle::evolution() {
    position_ = position_ + vector::Vector3(0, 1, 1);
}

void
Particle::destruct() {

}

vector::Vector3
Particle::position_get() const {
    return position_;
}
