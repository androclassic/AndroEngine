#include "MMath.h"
#include <math.h>
namespace andro
{
	void ComputeFrontAndRight(Vector3& front, Vector3& right, float pitch, float heading)
	{
		float sinP = sinf(DEG2RAD(pitch));
		float cosP = cosf(DEG2RAD(pitch));

		float sinH = sinf(DEG2RAD(heading));
		float cosH = cosf(DEG2RAD(heading));

		right.x = cosP;
		right.y = 0;
		right.z = sinP;

		front.x = -sinP * cosH;
		front.y = sinH;
		front.z = cosP * cosH;
	}


	float Vector2::operator* (const Vector2& rhs) const
	{
		return x*rhs.x + y*rhs.y;
	}



	Vector2& Vector2::operator=(const Vector2& rhs) 
	{
		x=rhs.x;
		y=rhs.y;

		return *this;
	}
	void Vector2::Normalize()
	{
		float lenght=Lenght();
		x=x/lenght;
		y=y/lenght;
	}

	Vector2 Vector2::operator+(const Vector2& rhs)const
	{
		return Vector2(x+rhs.x,y+rhs.y);
	}
	Vector2 Vector2::operator-(const Vector2& rhs) const
	{
		return Vector2(x-rhs.x,y-rhs.y);
	}


	float Vector2::Lenght() const
	{
		return sqrt(x*x + y*y);
	}

	float Vector3::operator *(const Vector3& rhs) const
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	Vector3 Vector3::vectorProduct(const Vector3 &vector) const
    {
        return Vector3(y*vector.z-z*vector.y,
                        z*vector.x-x*vector.z,
                        x*vector.y-y*vector.x);
    }

	Vector3& Vector3::operator=(const Vector3& rhs)
	{
		x=rhs.x;
		y=rhs.y;
		z=rhs.z;

		return *this;
	}
	void Vector3::Normalize()
	{
		float lenght=Lenght();
		x=x/lenght;
		y=y/lenght;
		z=z/lenght;
	}

	Vector3 Vector3::operator+(const Vector3& rhs) const
	{
		return Vector3(x+rhs.x,y+rhs.y,z+rhs.z);
	}
	Vector3 Vector3::operator-(const Vector3& rhs) const
	{
		return Vector3(x-rhs.x,y-rhs.y,z-rhs.z);
	}

	Vector3 Vector3::operator*(float scalar) const
	{
		return Vector3(scalar * x,scalar * y,scalar * z);
	}


	float Vector3::Lenght() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	float Distance(const Vector3& p1, const  Vector3& p2)
	{
	  return	sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2)+pow((p1.z-p2.z),2));
	}


	bool  Box_Plane_Overlap(const BoundingBox& box, Vector3 planeNormal, float distance)
	{
		BoundingBox center_box;
		Vector3 halfsize = box.GetHalfSize();

		for (int q = 1; q < 2; q++)
		{
			if (planeNormal.v[q] > 0.0f)
			{
				center_box.min.v[q] = -halfsize.v[q];
				center_box.max.v[q] = halfsize.v[q];
			}
			else
			{
				center_box.min.v[q] = halfsize.v[q];
				center_box.max.v[q] = -halfsize.v[q];
			}
		}
		if ( (planeNormal * center_box.min ) + distance > 0.0f) return false;
		if ( (planeNormal * center_box.max ) + distance < 0.0f) return false;

		return true;
	}


	/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			       	   \
	p2 = a*v2[Y] - b*v2[Z];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			           \
	p1 = a*v1[Y] - b*v1[Z];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

	/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p2 = -a*v2[X] + b*v2[Z];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p1 = -a*v1[X] + b*v1[Z];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

	/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[X] - b*v1[Y];			           \
	p2 = a*v2[X] - b*v2[Y];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return false;

#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[X] - b*v0[Y];				   \
	p1 = a*v1[X] - b*v1[Y];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return false;

	bool TriangleBoxOverlap(const BoundingBox& box, Triangle & triangle)
	{

		Vector3 center = box.GetCenter();
		Vector3 halfSize = box.GetHalfSize();

		Vector3 v[3];
		v[0] = triangle.P1 - center;
		v[1] = triangle.P2 - center;
		v[2] = triangle.P3 - center;

		BoundingBox tr_aabb = GetMinimumBoundingBox(v, 3);

		// test triangle AABB agains AABB
		if (tr_aabb.min.x > halfSize.x || tr_aabb.max.x < -halfSize.x) return false;
		if (tr_aabb.min.y > halfSize.y || tr_aabb.max.y < -halfSize.y) return false;
		if (tr_aabb.min.z > halfSize.x || tr_aabb.max.z < -halfSize.x) return false;



		Vector3 e0 = v[1] - v[0];
		Vector3 e1 = v[2] - v[0];
		Vector3 plane_normal = e0.vectorProduct(e1);
		plane_normal.Normalize();
		float d = v[0] * plane_normal;


		// test triangle plane intersects bbx
		if (!Box_Plane_Overlap(box, plane_normal, d))
			return false;


		/*    3) */

		Vector3 e2 = v[2] - v[1];


		float min, max, p0, p1, p2, rad, fex, fey, fez;
		const unsigned int X = 0; const unsigned  int Y = 1; const unsigned  int Z = 2;

		float* v0 = v[0].v;
		float* v1 = v[1].v;
		float* v2 = v[2].v;

		fex = fabs(e0.x);
		fey = fabs(e0.y);
		fez = fabs(e0.z);

		float* boxhalfsize = halfSize.v;
		float* normal = plane_normal.v;
		AXISTEST_X01(e0.z, e0.y, fez, fey);
		AXISTEST_Y02(e0.z, e0.x, fez, fex);
		AXISTEST_Z12(e0.y, e0.x, fey, fex);

		fex = fabs(e1.x);
		fey = fabs(e1.y);
		fez = fabs(e1.z);
		AXISTEST_X01(e1.z, e1.y, fez, fey);
		AXISTEST_Y02(e1.z, e1.x, fez, fex);
		AXISTEST_Z0(e1.y, e1.x, fey, fex);


		fex = fabs(e2.x);
		fey = fabs(e2.y);
		fez = fabs(e2.z);
		AXISTEST_X2(e2.z, e2.y, fez, fey);
		AXISTEST_Y1(e2.z, e2.x, fez, fex);
		AXISTEST_Z12(e2.y, e2.x, fey, fex);

		return true;
	}
	Matrix4::Matrix4()
	{
		LoadIdentity();

	}


	Matrix4::Matrix4(float _m11, float _m12, float _m13, float _m14,
					 float _m21, float _m22, float _m23, float _m24,
					 float _m31, float _m32, float _m33, float _m34,
					 float _m41, float _m42, float _m43, float _m44)
	{
		
	   m11=_m11;m12=_m12;m13=_m13;m14=_m14;
	   m21=_m21;m22=_m22;m23=_m23;m24=_m24;
	   m31=_m31;m32=_m32;m33=_m33;m34=_m34;
	   m41=_m41;m42=_m42;m43=_m43;m44=_m44;

	}
	void Matrix4::LoadIdentity()
	{
		for(unsigned  int i=0;i<4;i++)
			
		{
			for(unsigned int j=0;j<4;j++)
			{
				if(j==i)
					matrix[j][i]=1;
				else
				matrix[j][i]=0;
			}
		}
	}
	void Matrix4::LoadNull()
	{
		for(unsigned  int i=0;i<4;i++)
			
		{
			for(unsigned int j=0;j<4;j++)
			{
				matrix[j][i]=0;
			}
		}
	}
	Matrix4 Matrix4::operator *(const Matrix4& mat) const
	{
		Matrix4 rhs;
		rhs.LoadNull();

		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				for(int k=0;k<4;k++)
				   rhs.matrix[i][j]+=matrix[i][k]*mat.matrix[k][j];
		return rhs;

	}
	Vector4 operator*(const Vector4& vec, const Matrix4& mat)
	{
	   Vector4 rhs;

	   rhs.x=vec.x*mat.m11+vec.y*mat.m21+vec.z*mat.m31+vec.w*mat.m41;
	   rhs.y=vec.x*mat.m12+vec.y*mat.m22+vec.z*mat.m32+vec.w*mat.m42;
	   rhs.z=vec.x*mat.m13+vec.y*mat.m23+vec.z*mat.m33+vec.w*mat.m43;
	   rhs.w=vec.x*mat.m14+vec.y*mat.m24+vec.z*mat.m34+vec.w*mat.m44;
	return rhs;
	}
	Vector3 operator*(const Vector3& vec, const Matrix4& mat)
	{
	   Vector3 rhs;

	   rhs.x=vec.x*mat.m11+vec.y*mat.m21+vec.z*mat.m31+ mat.m41;
	   rhs.y=vec.x*mat.m12+vec.y*mat.m22+vec.z*mat.m32+ mat.m42;
	   rhs.z=vec.x*mat.m13+vec.y*mat.m23+vec.z*mat.m33+ mat.m43;

	   return rhs;
	}

	Matrix4& Matrix4::operator=(const Matrix4& mat)
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				matrix[i][j]=mat.matrix[i][j];

		return *this;
	}


	//----------------------------------------------------------------------------------

	BoundingBox::BoundingBox() : min(99999.0f, 99999.0f, 99999.0f), max(-99999.0f, -99999.0f, -99999.0f) 
	{

	}

	Vector3 BoundingBox::GetHalfSize() const
	{
		Vector3 halfSize = (min + max)*0.5f - min;
		return halfSize;
	}

	Vector3 BoundingBox::GetCenter() const
	{
		return (min + max)*0.5f;
	}


	BoundingBox GetMinimumBoundingBox(Vector3* points, unsigned int num)
	{
		BoundingBox bbx;
		for (unsigned int i = 0; i < num; i++)
		{
			bbx.min.x = std::min(points[i].x, bbx.min.x);
			bbx.min.y = std::min(points[i].y, bbx.min.y);
			bbx.min.z = std::min(points[i].z, bbx.min.z);

			bbx.max.x = std::max(points[i].x, bbx.max.x);
			bbx.max.y = std::max(points[i].y, bbx.max.y);
			bbx.max.z = std::max(points[i].z, bbx.max.z);

		}
		return bbx;
	}

	//----------------------------------------------------------------------------------



	void RotateYaw(Matrix4 &mat, float angle)
	{
			angle=DEG2RAD(angle);//convertion

	  Matrix4 rotate(cos(angle),  0,  sin(angle),    0,
						0      ,  1,     0      ,    0,
					-sin(angle),  0,  cos(angle),    0,
						0      ,  0,       0    ,    1);
	 
	 Matrix4 temp(mat); 
	mat =(temp*rotate);
	  
	}
	void  RotatePitch(Matrix4& mat, float angle)
	{
		angle=DEG2RAD(angle);//convertion

	  Matrix4 rotate(   1 ,  0        ,     0      ,    0,
						0 , cos(angle), -sin(angle),    0,
						0 , sin(angle),  cos(angle),    0,
						0 ,    0      ,       0    ,    1);
	 
	  Matrix4 temp(mat); 
	mat = (temp*rotate);
	 

	}
	void RotateRoll(Matrix4& mat, float angle)
	{
			angle=DEG2RAD(angle);//convertion

	 Matrix4 rotate(cos(angle),-sin(angle),     0,    0,
					 sin(angle), cos(angle),     0,    0,
						0      ,     0     ,     1,    0,
						0      ,     0     ,     0,    1);
	 Matrix4 temp(mat); 
	mat = (temp*rotate);
	}

	void  Translate(Matrix4& mat, float x, float y, float z)
	{
		mat.m41=x;
		mat.m42=y;
		mat.m43=z;
	}

	void  Scale(Matrix4& mat, float x, float y, float z)
	{
		Matrix4 scale;
		scale.LoadIdentity();

		scale.m11 = x;
		scale.m22 = y;
		scale.m33 = z;

	 Matrix4 temp(mat); 
	 mat = (temp * scale);

	}

	void SetPos(Matrix4& mat, float x, float y, float z)
	{
		mat.m41=x;
		mat.m42=y;
		mat.m43=z;
	}




	//-------------------------------------------------------------------------------
	void CreateCubeModel(const BoundingBox& aabb, Vector3 points[8], unsigned int  indices[36])
	{

		andro::Vector3 center = aabb.GetCenter();
		float half_x = aabb.GetHalfSize().x;
		float half_y = aabb.GetHalfSize().y;
		float half_z = aabb.GetHalfSize().z;


		/* init_resources */
		unsigned int cube_elements[] = {
			// front
			0, 1, 2,
			2, 3, 0,
			// top
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// bottom
			4, 0, 3,
			3, 7, 4,
			// left
			4, 5, 1,
			1, 0, 4,
			// right
			3, 2, 6,
			6, 7, 3,
		};

		//front
		points[0] = center + andro::Vector3(-half_x, -half_y, half_z);
		points[1] = center + andro::Vector3(half_x, -half_y, half_z);
		points[2] = center + andro::Vector3(half_x, half_y, half_z);
		points[3] = center + andro::Vector3(-half_x, half_y, half_z);
		//back
		points[4] = center + andro::Vector3(-half_x, -half_y, -half_z);
		points[5] = center + andro::Vector3(half_x, -half_y, -half_z);
		points[6] = center + andro::Vector3(half_x, half_y, -half_z);
		points[7] = center + andro::Vector3(-half_x, half_y, -half_z);

		memcpy_s(indices, sizeof(cube_elements), cube_elements, sizeof(cube_elements));

	}

}