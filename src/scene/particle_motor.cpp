#include "particle_motor.hh"

Motor::Motor(unsigned int nb_particles) : nb_particles_(nb_particles)
{}

std::tuple<std::vector<GLfloat>, std::vector<GLfloat>>
Motor::create(int nb) {
    std::vector<GLfloat> list_pos;
    std::vector<GLfloat> list_color;

    /* Default value */
    if (nb == 0)
        nb = nb_particles_;

    for (int i = 0; i < nb; ++i) {
        Particle p = Particle(1, 0.01, i);
        vector::Vector3 pos_p;
        pos_p = p.create();

        /* Get the position */
        list_pos.push_back(pos_p.x_get());
        list_pos.push_back(pos_p.y_get());
        list_pos.push_back(pos_p.z_get());

        /* Get the color */
        auto c = p.color_get();
        list_color.push_back(c.red);
        list_color.push_back(c.green);
        list_color.push_back(c.blue);

        list_particles_.push_back(p);
    }
    return std::make_tuple(list_pos, list_color);
}

std::vector<Particle>
Motor::list_particles_get() const {
    return list_particles_;
}

std::tuple<std::vector<GLfloat>, std::vector<GLfloat>>
Motor::update(int wind) {
    std::vector<GLfloat> list_position;
    std::vector<GLfloat> list_color;
    for (auto& p : list_particles_) {
        /* Update */
        auto pos = p.update(wind);

        /* Get the position */
        list_position.push_back(pos.x_get());
        list_position.push_back(pos.y_get());
        list_position.push_back(pos.z_get());

        /* Get the color */
        auto c = p.color_get();
        list_color.push_back(c.red / 255);
        list_color.push_back(c.green / 255);
        list_color.push_back(c.blue / 255);
    }
    return std::make_tuple(list_position, list_color);
}

void
Motor::kill(int nb) {
    /* Kill nb particles */
    for (int i = 0; i < nb; ++i)
        list_particles_.erase(list_particles_.begin());
}
