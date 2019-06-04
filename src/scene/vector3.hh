/**
 ** \file vector3.hh
 ** \brief Declaration of vector::Vector3.
 */

#pragma once

#include <cmath>
#include <iostream>

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
          Vector3(float x, float y, float z);

          Vector3();
          /** \} */

          /* Methods */
          /** \name methods
           ** \{ */
          /// Print the vector x, y ,z.
          void print(std::ostream& ostr) const;

          /// Perform the dot product between self vector and other vector and return the new vector.
          float dot_product(const Vector3 other) const;

          /// Perform the cross product between self vector and other vector adn return the new vector.
          Vector3 cross_product(const Vector3 other) const;

          /// Normalize self vector and return the normalized vector.
          Vector3 normalize() const;

          /// compute two points and return the new vector.
          Vector3 compute(const Vector3 other) const;

          /// Rotate a vector in function of angle axis
          Vector3 rotate(const Vector3 angle) const;

          /// Return the distance of the vector.
          float distance_get() const;
          /** \} */

          /* getters */
          /** \name Getters & setters
           ** \{ */
          float x_get() const;
          float y_get() const;
          float z_get() const;

          /* setters */
          void vector_set(float x, float y, float z);
          /** \} */

          /* operators */
          /* Methods */
          /** \name operators
           ** \{ */
          Vector3 operator+(const Vector3 other) const;

          Vector3 operator-(const Vector3 other) const;

          Vector3 operator*(const float coeff) const;

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
          float x_;
          float y_;
          float z_;
      };
}
