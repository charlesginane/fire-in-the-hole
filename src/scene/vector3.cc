/**
 ** \file vector3.cc
 ** \brief Implementation of vector::Vector3.
 */

#include "vector3.hh"

const GLfloat epsilon = 0.01f;

namespace vector
{
    Vector3::Vector3()
    {}

    Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z) : x_(x), y_(y), z_(z)
    {}

    GLfloat
    Vector3::x_get() const
    {
        return x_;
    }

    GLfloat
    Vector3::y_get() const
    {
        return y_;
    }

    GLfloat
    Vector3::z_get() const
    {
        return z_;
    }

    void
    Vector3::vector_set(GLfloat x, GLfloat y, GLfloat z)
    {
        x_ = x;
        y_ = y;
        z_ = z;
    }

    Vector3
    Vector3::operator+(const Vector3 other) const
    {
        return Vector3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
    }

    Vector3
    Vector3::operator-(const Vector3 other) const
    {
        return Vector3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
    }

    Vector3
    Vector3::operator*(const GLfloat coeff) const
    {
        return Vector3(x_ * coeff, y_ * coeff, z_ * coeff);
    }

    Vector3
    Vector3::operator*(const double coeff) const
    {
        return Vector3(x_ * coeff, y_ * coeff, z_ * coeff);
    }


    bool
    Vector3::operator==(const Vector3 other) const
    {
        return (fabs(x_ - other.x_) < epsilon) && (fabs(y_ - other.y_) < epsilon) && (fabs(z_ - other.z_) < epsilon);
    }

    GLfloat
    Vector3::dot_product(const Vector3 other) const
    {
        return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
    }

    Vector3
    Vector3::cross_product(const Vector3 other) const
    {
        return Vector3(y_ * other.z_ - z_ * other.y_, z_ * other.x_ - x_ * other.z_
                        , x_ * other.y_ - y_ * other.x_);
    }

    Vector3
    Vector3::normalize() const
    {
        auto bottom = sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
        auto x = (x_) / (bottom);
        auto y = (y_) / (bottom);
        auto z = (z_) / (bottom);
        return Vector3(x, y, z);
    }

    Vector3
    Vector3::compute(const Vector3 other) const
    {
        return Vector3(other.x_ - x_, other.y_ - y_, other.z_ - z_);
    }

    Vector3
    Vector3::rotate(const Vector3 dir) const
    {
        auto angle_x = dir.x_;
        auto angle_y = dir.y_;
        auto angle_z = dir.z_;

        // auto vecx = Vector3(x_, y_ * cos(angle_x) - (z_ * sin(angle_x)), y_ * sin(angle_x) + z_ * cos(angle_x));
        // auto vecz = Vector3(vecx.x_ * cos(angle_z) - (vecx.y_ * sin(angle_z)), vecx.x_ * sin(angle_z) + vecx.y_ * cos(angle_z), vecx.z_);
        // auto vecy = Vector3(vecz.x_ * cos(angle_y) + vecz.z_ * sin(angle_y), vecz.y_, -1 * vecz.x_ * sin(angle_y) + vecz.z_ * cos(angle_y));

        // if (angle_x != 0.0f) {
        //     auto vecx = Vector3(current.x_, current.y_ * cos(angle_x) - (current.z_ * sin(angle_x)), current.y_ * sin(angle_x) + current.z_ * cos(angle_x));
        //     auto vecy = Vector3(vecx.x_ * cos(0) + vecx.z_ * sin(0), vecx.y_, -1 * vecx.x_ * sin(0) + vecx.z_ * cos(0));
        //     current = Vector3(vecy.x_ * cos(0) - (vecy.y_ * sin(0)), vecy.x_ * sin(0) + vecy.y_ * cos(0), vecy.z_);
        // }
        //
        // if (angle_y != 0.0f) {
        //     auto vecx = Vector3(current.x_, current.y_ * cos(0) - (current.z_ * sin(0)), current.y_ * sin(0) + current.z_ * cos(0));
        //     auto vecy = Vector3(vecx.x_ * cos(angle_y) + vecx.z_ * sin(angle_y), vecx.y_, -1 * vecx.x_ * sin(angle_y) + vecx.z_ * cos(angle_y));
        //     current = Vector3(vecy.x_ * cos(0) - (vecy.y_ * sin(0)), vecy.x_ * sin(0) + vecy.y_ * cos(0), vecy.z_);
        // }
        //
        // if (angle_z != 0.0f) {
        //     auto vecx = Vector3(current.x_, current.y_ * cos(0) - (current.z_ * sin(0)), current.y_ * sin(0) + current.z_ * cos(0));
        //     auto vecy = Vector3(vecx.x_ * cos(0) + vecx.z_ * sin(0), vecx.y_, -1 * vecx.x_ * sin(0) + vecx.z_ * cos(0));
        //     current = Vector3(vecy.x_ * cos(angle_z) - (vecy.y_ * sin(angle_z)), vecy.x_ * sin(angle_z) + vecy.y_ * cos(angle_z), vecy.z_);
        // }

        auto vecx = Vector3(x_, y_ * cos(angle_x) - (z_ * sin(angle_x)), y_ * sin(angle_x) + z_ * cos(angle_x));
        auto vecy = Vector3(vecx.x_ * cos(angle_y) + vecx.z_ * sin(angle_y), vecx.y_, -1 * vecx.x_ * sin(angle_y) + vecx.z_ * cos(angle_y));
        auto vecz = Vector3(vecy.x_ * cos(angle_z) - (vecy.y_ * sin(angle_z)), vecy.x_ * sin(angle_z) + vecy.y_ * cos(angle_z), vecy.z_);

        return vecz;
    }

    GLfloat
    Vector3::distance_get() const
    {
        return sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
    }

    void
    Vector3::print(std::ostream& ostr) const
    {
        ostr << x_ << " " << y_ << " " << z_ << "\n";
    }
}
