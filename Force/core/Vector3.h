#ifndef _FORCE_VECTOR3_
#define _FORCE_VECTOR3_

#include <math.h>
#include"Precision.h"
#include"util.h"

namespace force
{

    class Vector3
    {
    public:
        real x;
        real y;
        real z;

    private:
        /** Padding to ensure 4 word alignment. */
        real pad;

    public:
        /** The default constructor creates a zero vector. */
        Vector3();
        Vector3(const real x, const real y, const real z);

        const static Vector3 GRAVITY;
        const static Vector3 HIGH_GRAVITY;
        const static Vector3 UP;
        const static Vector3 RIGHT;
        const static Vector3 OUT_OF_SCREEN;
        const static Vector3 X;
        const static Vector3 Y;
        const static Vector3 Z;



        real operator[](unsigned i) const;
        real& operator[](unsigned i);
        void operator+=(const Vector3& v);
        Vector3 operator+(const Vector3& v) const;
	    void operator-=(const Vector3& v);
        Vector3 operator-(const Vector3& v) const;
        void operator*=(const real value);
        Vector3 operator*(const real value) const;
        Vector3 componentProduct(const Vector3 &vector) const;
        void componentProductUpdate(const Vector3 &vector);
        Vector3 vectorProduct(const Vector3 &vector) const;
        void operator %=(const Vector3 &vector);
        Vector3 operator%(const Vector3 &vector) const;
        real scalarProduct(const Vector3 &vector) const;
        real operator *(const Vector3 &vector) const;
        void addScaledVector(const Vector3& vector, real scale);
        real magnitude() const;
        real squareMagnitude() const;
        /** Limits the size of the vector to the given maximum. */
        void trim(real size);
        void normalise();
        Vector3 unit() const;
        bool operator==(const Vector3& other) const;
        bool operator!=(const Vector3& other) const;
        bool operator<(const Vector3& other) const;
        bool operator>(const Vector3& other) const;
        bool operator<=(const Vector3& other) const;
        bool operator>=(const Vector3& other) const;
        void clear();
        void invert();
     
    };

}

#endif