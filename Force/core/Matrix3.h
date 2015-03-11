#ifndef _FORCE_MATRIX3_
#define _FORCE_MATRIX3_

#include <math.h>
#include"Precision.h"
#include"util.h"
#include"Vector3.h"
#include"Quaternion.h"


namespace force
{
    /**
     * Holds an inertia tensor, consisting of a 3x3 row-major matrix.
     * This matrix is not padding to produce an aligned structure, since
     * it is most commonly used with a mass (single real) and two
     * damping coefficients to make the 12-element characteristics array
     * of a rigid body.
     */
    class Matrix3
    {
    public:
        real data[9];
        Matrix3();
        Matrix3(const Vector3 &compOne, const Vector3 &compTwo,
            const Vector3 &compThree);
        Matrix3(real c0, real c1, real c2, real c3, real c4, real c5,
            real c6, real c7, real c8);

        /**
         * Sets the matrix to be a diagonal matrix with the given
         * values along the leading diagonal.
         */
        void setDiagonal(real a, real b, real c);

        /**
         * Sets the value of the matrix from inertia tensor values.
         */
        void setInertiaTensorCoeffs(real ix, real iy, real iz,
            real ixy=0, real ixz=0, real iyz=0);
        /**
         * Sets the value of the matrix as an inertia tensor of
         * a rectangular block aligned with the body's coordinate
         * system with the given axis half-sizes and mass.
         */
        void setBlockInertiaTensor(const Vector3 &halfSizes, real mass);

        /**
         * Sets the matrix to be a skew symmetric matrix based on
         * the given vector. The skew symmetric matrix is the equivalent
         * of the vector product. So if a,b are vectors. a x b = A_s b
         * where A_s is the skew symmetric form of a.
         */
        void setSkewSymmetric(const Vector3 vector);

        /**
         * Sets the matrix values from the given three vector components.
         * These are arranged as the three columns of the vector.
         */
        void setComponents(const Vector3 &compOne, const Vector3 &compTwo,
            const Vector3 &compThree);

        /**
         * Transform the given vector by this matrix.
         *
         * @param vector The vector to transform.
         */
        Vector3 operator*(const Vector3 &vector) const;
        Vector3 transform(const Vector3 &vector) const;
        Vector3 transformTranspose(const Vector3 &vector) const;

        Vector3 getRowVector(int i) const;
        Vector3 getAxisVector(int i) const;
        void setInverse(const Matrix3 &m);

        /** Returns a new matrix containing the inverse of this matrix. */
        Matrix3 inverse() const;
        /**
         * Inverts the matrix.
         */
        void invert();
        void setTranspose(const Matrix3 &m);
        /** Returns a new matrix containing the transpose of this matrix. */
        Matrix3 transpose() const;
        Matrix3 operator*(const Matrix3 &o) const;
        void operator*=(const Matrix3 &o);
        void operator*=(const real scalar);
        void operator+=(const Matrix3 &o);
        void setOrientation(const Quaternion &q);
        static Matrix3 linearInterpolate(const Matrix3& a, const Matrix3& b, real prop);
    };

}
#endif