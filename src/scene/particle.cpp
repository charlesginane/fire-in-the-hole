#include "particle.hh"

const GLfloat epsilon = 0.01f;

Particle::Particle()
{}

Particle::Particle(GLuint radius, float speed, int id) : radius_(radius),
                                                  speed_(speed),
                                                  id_(id)
{
    color_ = Color();
    descent_ = 3;
    // translate_ = 0.0f;
}

vector::Vector3
Particle::create(int special) {
    GLfloat x = (GLfloat)(((float)(rand() % 1000)) / 5000) - 0.1;
    GLfloat y = (GLfloat)(((float)(rand() % 1000)) / 20000) - 0.5;
    position_ = vector::Vector3(x, y, 0.0);
    // position_ = vector::Vector3(0, 0, 0);
    special_ = special;
    return position_;
}

vector::Vector3
Particle::update(GLint program) {

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


    int vertexColorLocation = glGetUniformLocation(program, "ourColor");
    glUniform4f(vertexColorLocation, color_.red / 255.0f, color_.green / 255.0f, color_.blue / 255.0f, 1.0f);
    int translation = glGetUniformLocation(program, "translate");
    glUniform1f(translation, translate_);
    int special = glGetUniformLocation(program, "special");
    glUniform1i(special, special_);

    glBindVertexArray(0);
    if (id_ % 3 == 0)
        position_ = position_ + vector::Vector3(0.001, speed_, 0);

    else if (id_ % 3 == 1)
        position_ = position_ + vector::Vector3(-0.001, speed_, 0);

    else
        position_ = position_ + vector::Vector3(0, speed_, 0);


    return position_;
}

void
Particle::destruct() {

}

vector::Vector3
Particle::position_get() const {
    return position_;
}

struct Color
Particle::color_get() const {
    return color_;
}
