#include "Quaternion.h"

namespace force
{



	 Quaternion::Quaternion() : r(1), i(0), j(0), k(0) {}
        
	 Quaternion::Quaternion(const real r, const real i, const real j, const real k)
            : r(r), i(i), j(j), k(k)
        {
        }

	  Quaternion::Quaternion(const Vector3 &v, float angle)
        {
			FromAxis(v, angle);
        }


        /**
         * Normalises the quaternion to unit length, making it a valid
         * orientation quaternion.
         */
        void Quaternion::normalise()
        {
            real d = r*r+i*i+j*j+k*k;

            // Check for zero length quaternion, and use the no-rotation
            // quaternion in that case.
            if (d == 0) {
                r = 1;
                return;
            }

            d = ((real)1.0)/real_sqrt(d);
            r *= d;
            i *= d;
            j *= d;
            k *= d;
        }

		// Convert from Axis Angle
		void Quaternion::FromAxis(const Vector3 &v, float angle)
		{
			float sinAngle;
			angle *= 0.5f;
			Vector3 vn(v);
			vn.normalise();
 
			sinAngle = sin(angle);
 
			i = (vn.x * sinAngle);
			j = (vn.y * sinAngle);
			k = (vn.z * sinAngle);
	
			r = cos(angle);
		}

        /**
         * Multiplies the quaternion by the given quaternion.
         *
         * @param multiplier The quaternion by which to multiply.
         */
        void Quaternion::operator *=(const Quaternion &multiplier)
        {
            Quaternion q = *this;
            r = q.r*multiplier.r - q.i*multiplier.i -
                q.j*multiplier.j - q.k*multiplier.k;
            i = q.r*multiplier.i + q.i*multiplier.r +
                q.j*multiplier.k - q.k*multiplier.j;
            j = q.r*multiplier.j + q.j*multiplier.r +
                q.k*multiplier.i - q.i*multiplier.k;
            k = q.r*multiplier.k + q.k*multiplier.r +
                q.i*multiplier.j - q.j*multiplier.i;
        }

        /**
         * Adds the given vector to this, scaled by the given amount.
         * This is used to update the orientation quaternion by a rotation
         * and time.
         *
         * @param vector The vector to add.
         *
         * @param scale The amount of the vector to add.
         */
        void Quaternion::addScaledVector(const Vector3& vector, real scale)
        {
            Quaternion q(0,
                vector.x * scale,
                vector.y * scale,
                vector.z * scale);
            q *= *this;
            r += q.r * ((real)0.5);
            i += q.i * ((real)0.5);
            j += q.j * ((real)0.5);
            k += q.k * ((real)0.5);
        }

        void Quaternion::rotateByVector(const Vector3& vector)
        {
            Quaternion q(0, vector.x, vector.y, vector.z);
            (*this) *= q;
        }
}