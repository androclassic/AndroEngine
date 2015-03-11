#ifndef _FORCE_QUATERNION_
#define _FORCE_QUATERNION_
#include <math.h>
#include"Precision.h"
#include"util.h"
#include"Vector3.h"

namespace force
{
	    /**
     * Holds a three degree of freedom orientation.
     *
     * Quaternions have
     * several mathematical properties that make them useful for
     * representing orientations, but require four items of data to
     * hold the three degrees of freedom. These four items of data can
     * be viewed as the coefficients of a complex number with three
     * imaginary parts. The mathematics of the quaternion is then
     * defined and is roughly correspondent to the math of 3D
     * rotations. A quaternion is only a valid rotation if it is
     * normalised: i.e. it has a length of 1.
     *
     * @note Angular velocity and acceleration can be correctly
     * represented as vectors. Quaternions are only needed for
     * orientation.
     */
    class Quaternion
    {
    public:
        union {
            struct {
                real r;
                real i;
                real j;
                real k;
            };

            real data[4];
		};
        Quaternion();

        Quaternion(const real r, const real i, const real j, const real k);
		Quaternion(const Vector3 &v, float angle);

		void FromAxis(const Vector3 &v, float angle);
        void normalise();
        void operator *=(const Quaternion &multiplier);
        void addScaledVector(const Vector3& vector, real scale);
        void rotateByVector(const Vector3& vector);
		real getPitch()
		{
		  return RadToDeg(atan2(2*(j*k + r*i), r*r - i*i - j*j + k*k) );
		}

		real getYaw()
		{
		  return RadToDeg( asin(-2*(i*k - r*j)) );
		}

		real getRoll()
		{
		  return RadToDeg(atan2(2*(i*j + r*k), r*r + i*i - j*j - k*k) );
		}

    };

}

#endif