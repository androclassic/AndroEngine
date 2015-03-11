#ifndef _FORCE_MATRIX4_
#define _FORCE_MATRIX4_
#include <math.h>
#include"Precision.h"
#include"util.h"
#include"Vector3.h"
#include"Quaternion.h"

namespace force
{
	    class Matrix4
    {
    public:
        real data[12];
        Matrix4();

        /**
         * Sets the matrix to be a diagonal matrix with the given coefficients.
         */
        void setDiagonal(real a, real b, real c);
        Matrix4 operator*(const Matrix4 &o) const;
        Vector3 operator*(const Vector3 &vector) const;
        Vector3 transform(const Vector3 &vector) const;
        real getDeterminant() const;
        void setInverse(const Matrix4 &m);

        /** Returns a new matrix containing the inverse of this matrix. */
        Matrix4 inverse() const;

        /**
         * Inverts the matrix.
         */
        void invert();

        /**
         * Transform the given direction vector by this matrix.
         *
         * @note When a direction is converted between frames of
         * reference, there is no translation required.
         *
         * @param vector The vector to transform.
         */
        Vector3 transformDirection(const Vector3 &vector) const;

        /**
         * Transform the given direction vector by the
         * transformational inverse of this matrix.
         */

		 Vector3 transformInverseDirection(const Vector3 &vector) const;

        /**
         * Transform the given vector by the transformational inverse
         * of this matrix.
         *
         * @note This function relies on the fact that the inverse of
         * a pure rotation matrix is its transpose. It separates the
         * translational and rotation components, transposes the
         * rotation, and multiplies out. If the matrix is not a
         * scale and shear free transform matrix, then this function
         * will not give correct results.
         *
         * @param vector The vector to transform.
         */
        Vector3 transformInverse(const Vector3 &vector) const;
        Vector3 getAxisVector(int i) const;
        /**
         * Sets this matrix to be the rotation matrix corresponding to
         * the given quaternion.
         */
        void setOrientationAndPos(const Quaternion &q, const Vector3 &pos);
	};

}
#endif


