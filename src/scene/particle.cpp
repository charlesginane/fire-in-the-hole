#include "particle.hh"

Particle::Particle()
{}

Particle::Particle(GLuint radius, GLuint speed) : radius_(radius),
                                                  speed_(speed)
{
    color_ = Color();
    descent_ = 3;
    // translate_ = 0.0f;
}

vector::Vector3
Particle::create() {
    auto x = (float)(((float)(rand() % 1000)) / 1000) - 0.5;
    auto y = (float)(((float)(rand() % 1000)) / 10000) - 0.5;
    // std::cout << x << std::endl;
    position_ = vector::Vector3(x, y, 0.0);
    // position_ = vector::Vector3(0, 0, 0);
    return position_;
}

void
Particle::update(GLint program, int nb_iter) {
    
    if (color_.blue <= 0) {
        if (color_.green <= 0) {
            color_.red -= 2 * (descent_);
        }
        else
            color_.green -= (descent_);
    }
    else
        color_.blue -= (descent_);
    
    translate_ += (0.0075f);

    std::cout << translate_ << std::endl;

    int vertexColorLocation = glGetUniformLocation(program, "ourColor");
    glUniform4f(vertexColorLocation, color_.red / 255.0f, color_.green / 255.0f, color_.blue / 255.0f, 1.0f);
    int translation = glGetUniformLocation(program, "translate");
    glUniform1f(translation, translate_);

    glBindVertexArray(0);
}

void
Particle::destruct() {

}

vector::Vector3
Particle::position_get() const {
    return position_;
}
