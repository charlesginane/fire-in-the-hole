#include "particle.hh"

const GLfloat epsilon = 0.01f;

Particle::Particle()
{}

Particle::Particle(GLuint radius, float speed, int id) : radius_(radius),
                                                  speed_(speed),
                                                  id_(id)
{
    color_ = Color();
    descent_ = 10;
}

vector::Vector3
Particle::create() {
    /* Random position */
    GLfloat x = (GLfloat)(((float)(rand() % 1000)) / 5000) - 0.1;
    GLfloat y = (GLfloat)(((float)(rand() % 1000)) / 20000) - 0.5;
    position_ = vector::Vector3(x, y, 0.0);
    return position_;
}

vector::Vector3
Particle::update(int wind) {
    /* First update the color */
    if (color_.blue <= 0) {
        if (color_.green <= 0) {
            color_.red -= 2 * (descent_);
        }
        else
            color_.green -= (descent_);
    }
    else
        color_.blue -= (descent_);
    translate_ += speed_;

    /* Second update the position */
    if (id_ % 3 == 0)
        position_ = position_ + vector::Vector3(0.001 + 0.001 * wind, speed_, 0);

    else if (id_ % 3 == 1)
        position_ = position_ + vector::Vector3(-0.001 + 0.001 * wind, speed_, 0);

    else
        position_ = position_ + vector::Vector3(0.001 * wind, speed_, 0);
    return position_;
}

vector::Vector3
Particle::position_get() const {
    return position_;
}

struct Color
Particle::color_get() const {
    return color_;
}
