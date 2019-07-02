/**
 ** \file vector3.hh
 ** \brief Declaration of vector::Vector3.
 */

#pragma once

#include <cmath>
#include <iostream>


#include <GL/glew.h>
#include <GL/freeglut.h>

namespace vector
{
    /// Vector3.
    class Vector3
    {
      public:
          /* Constructors */
          /** \name Constructors
           ** \{ */
          /// Construct a vector with 3 coordonates.
          Vector3(GLfloat x, GLfloat y, GLfloat z);

          Vector3();
          /** \} */

          /* Methods */
          /** \name methods
           ** \{ */
          /// Print the vector x, y ,z.
          void print(std::ostream& ostr) const;

          /// Perform the dot product between self vector and other vector and return the new vector.
          GLfloat dot_product(const Vector3 other) const;

          /// Perform the cross product between self vector and other vector adn return the new vector.
          Vector3 cross_product(const Vector3 other) const;

          /// Normalize self vector and return the normalized vector.
          Vector3 normalize() const;

          /// compute two points and return the new vector.
          Vector3 compute(const Vector3 other) const;

          /// Rotate a vector in function of angle axis
          Vector3 rotate(const Vector3 angle) const;

          /// Return the distance of the vector.
          GLfloat distance_get() const;
          /** \} */

          /* getters */
          /** \name Getters & setters
           ** \{ */
          GLfloat x_get() const;
          GLfloat y_get() const;
          GLfloat z_get() const;

          /* setters */
          void vector_set(GLfloat x, GLfloat y, GLfloat z);
          /** \} */

          /* operators */
          /* Methods */
          /** \name operators
           ** \{ */
          Vector3 operator+(const Vector3 other) const;

          Vector3 operator-(const Vector3 other) const;

          Vector3 operator*(const GLfloat coeff) const;

          Vector3 operator*(const double coeff) const;

          bool operator==(const Vector3 other) const;

          friend std::ostream& operator<<(std::ostream& stream, const Vector3& vec)
          {
              stream << vec.x_ << " " << vec.y_ << " " << vec.z_ << std::endl;
              return stream;
          }

          /** \} */

        private:
          /* Atributes */
          GLfloat x_;
          GLfloat y_;
          GLfloat z_;
      };
}
