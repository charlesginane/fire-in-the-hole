#include "particle_motor.hh"

Motor::Motor(unsigned int nb_particles) : nb_particles_(nb_particles)
{}

std::vector<float>
Motor::create() {
    std::vector<float> list_pos;
    for (int i = 0; i < nb_particles_; ++i) {
        Particle p = Particle(1, 1);
        list_particles_.push_back(p);
        auto pos_p = p.create();
        list_pos.push_back(pos_p.x_get());
        list_pos.push_back(pos_p.y_get());
        list_pos.push_back(pos_p.z_get());
    }
    return list_pos;
}

std::vector<Particle>
Motor::list_particles_get() const {
    return list_particles_;
}

void
Motor::update(GLint program, int nb_iter) {
    for (auto& p : list_particles_) {
        p.update(program, nb_iter);
    }
}
