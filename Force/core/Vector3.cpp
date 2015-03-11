#include "Vector3.h"

namespace force
{
const Vector3 Vector3::GRAVITY = Vector3(0, -9.81, 0);
const Vector3 Vector3::HIGH_GRAVITY = Vector3(0, -19.62, 0);
const Vector3 Vector3::UP = Vector3(0, 1, 0);
const Vector3 Vector3::RIGHT = Vector3(1, 0, 0);
const Vector3 Vector3::OUT_OF_SCREEN = Vector3(0, 0, 1);
const Vector3 Vector3::X = Vector3(0, 1, 0);
const Vector3 Vector3::Y = Vector3(1, 0, 0);
const Vector3 Vector3::Z = Vector3(0, 0, 1);


		Vector3::Vector3() : x(0), y(0), z(0) {}

		Vector3::Vector3(const real x, const real y, const real z)
            : x(x), y(y), z(z) {}

		real Vector3::operator[](unsigned i) const
        {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        real& Vector3::operator[](unsigned i)
        {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        void Vector3::operator+=(const Vector3& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
        }

        Vector3 Vector3::operator+(const Vector3& v) const
        {
            return Vector3(x+v.x, y+v.y, z+v.z);
        }

        void Vector3::operator-=(const Vector3& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }

        Vector3 Vector3::operator-(const Vector3& v) const
        {
            return Vector3(x-v.x, y-v.y, z-v.z);
        }

        void Vector3::operator*=(const real value)
        {
            x *= value;
            y *= value;
            z *= value;
        }

        Vector3 Vector3::operator*(const real value) const
        {
            return Vector3(x*value, y*value, z*value);
        }

        /**
         * Calculates and returns a component-wise product of this
         * vector with the given vector.
         */
        Vector3 Vector3::componentProduct(const Vector3 &vector) const
        {
            return Vector3(x * vector.x, y * vector.y, z * vector.z);
        }

        /**
         * Performs a component-wise product with the given vector and
         * sets this vector to its result.
         */
        void Vector3::componentProductUpdate(const Vector3 &vector)
        {
            x *= vector.x;
            y *= vector.y;
            z *= vector.z;
        }

        /**
         * Calculates and returns the vector product of this vector
         * with the given vector.
         */
        Vector3 Vector3::vectorProduct(const Vector3 &vector) const
        {
            return Vector3(y*vector.z-z*vector.y,
                           z*vector.x-x*vector.z,
                           x*vector.y-y*vector.x);
        }

        /**
         * Updates this vector to be the vector product of its current
         * value and the given vector.
         */
        void Vector3::operator %=(const Vector3 &vector)
        {
            *this = vectorProduct(vector);
        }

        /**
         * Calculates and returns the vector product of this vector
         * with the given vector.
         */
        Vector3 Vector3::operator%(const Vector3 &vector) const
        {
            return Vector3(y*vector.z-z*vector.y,
                           z*vector.x-x*vector.z,
                           x*vector.y-y*vector.x);
        }

        real Vector3::scalarProduct(const Vector3 &vector) const
        {
            return x*vector.x + y*vector.y + z*vector.z;
        }

        real Vector3::operator *(const Vector3 &vector) const
        {
            return x*vector.x + y*vector.y + z*vector.z;
        }

        void Vector3::addScaledVector(const Vector3& vector, real scale)
        {
            x += vector.x * scale;
            y += vector.y * scale;
            z += vector.z * scale;
        }

        real Vector3::magnitude() const
        {
            return real_sqrt(x*x+y*y+z*z);
        }

        real Vector3::squareMagnitude() const
        {
            return x*x+y*y+z*z;
        }

        /** Limits the size of the vector to the given maximum. */
        void Vector3::trim(real size)
        {
            if (squareMagnitude() > size*size)
            {
                normalise();
                x *= size;
                y *= size;
                z *= size;
            }
        }

        /** Turns a non-zero vector into a vector of unit length. */
        void Vector3::normalise()
        {
            real l = magnitude();
            if (l > 0)
            {
                (*this) *= ((real)1)/l;
            }
        }

        /** Returns the normalised version of a vector. */
        Vector3 Vector3::unit() const
        {
            Vector3 result = *this;
            result.normalise();
            return result;
        }

        bool Vector3::operator==(const Vector3& other) const
        {
            return x == other.x &&
                y == other.y &&
                z == other.z;
        }

        /** Checks if the two vectors have non-identical components. */
        bool Vector3::operator!=(const Vector3& other) const
        {
            return !(*this == other);
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         */
        bool Vector3::operator<(const Vector3& other) const
        {
            return x < other.x && y < other.y && z < other.z;
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         */
        bool Vector3::operator>(const Vector3& other) const
        {
            return x > other.x && y > other.y && z > other.z;
        }

        bool Vector3::operator<=(const Vector3& other) const
        {
            return x <= other.x && y <= other.y && z <= other.z;
        }

        bool Vector3::operator>=(const Vector3& other) const
        {
            return x >= other.x && y >= other.y && z >= other.z;
        }

        /** Zero all the components of the vector. */
        void Vector3::clear()
        {
            x = y = z = 0;
        }

        /** Flips all the components of the vector. */
        void Vector3::invert()
        {
            x = -x;
            y = -y;
            x = -z;
        }

}