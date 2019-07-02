#include "particle_motor.hh"

Motor::Motor(unsigned int nb_particles) : nb_particles_(nb_particles)
{}

std::tuple<std::vector<GLfloat>, std::vector<GLfloat>>
Motor::create(int nb) {
    std::vector<GLfloat> list_pos;
    std::vector<GLfloat> list_color;

    if (nb == 0)
        nb = nb_particles_;

    for (int i = 0; i < nb; ++i) {
        Particle p = Particle(1, 0.01, i);
        list_particles_.push_back(p);
        vector::Vector3 pos_p;
        if (i >= 250) {
            pos_p = p.create(1);
        }
        else
            pos_p = p.create(0);
        list_pos.push_back(pos_p.x_get());
        list_pos.push_back(pos_p.y_get());
        list_pos.push_back(pos_p.z_get());

        auto c = p.color_get();
        list_color.push_back(c.red);
        list_color.push_back(c.green);
        list_color.push_back(c.blue);
    }
    return std::make_tuple(list_pos, list_color);
}

std::vector<Particle>
Motor::list_particles_get() const {
    return list_particles_;
}

std::tuple<std::vector<GLfloat>, std::vector<GLfloat>>
Motor::update(GLint program) {
    std::vector<GLfloat> list_position;
    std::vector<GLfloat> list_color;
    for (auto& p : list_particles_) {
        auto pos = p.update(program);
        list_position.push_back(pos.x_get());
        list_position.push_back(pos.y_get());
        list_position.push_back(pos.z_get());

                auto c = p.color_get();
        list_color.push_back(c.red);
        list_color.push_back(c.green);
        list_color.push_back(c.blue);
    }
    return std::make_tuple(list_position, list_color);

}