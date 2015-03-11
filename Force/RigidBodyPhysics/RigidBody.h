#ifndef RIGID_BODY
#define RIGID_BODY

#include "core/core.h"
#include "ParticlePhysics/Particle.h"

static real  sleepEpsilon = ((real)6.9);

namespace force
{

	class RigidBody : public Particle
	{
		public:
						RigidBody();
			void		Update(real dt);
			void		AddForce(Vector3 force);
			void		AddTorque(Vector3 torque);
			void		AddForceAtPoint(Vector3 force, Vector3 point);
			void		AddForceAtBodyPoint(Vector3 force, Vector3 point);


			Vector3		GetRotation() const { return angularVelocity;}
			void		SetRotation(Vector3 rotation)  { angularVelocity  =  rotation;}
			void		AddRotation(Vector3 rotation)  { angularVelocity +=  rotation;}

			Vector3		GetTorque() const;
			Vector3		GetPointInLocalSpace(const Vector3 &point) const;
			Vector3		GetPointInWorldSpace(const Vector3 &point) const;
	        Matrix3		GetInverseInertiaTensor() const;
			Matrix3     GetInverseInertiaTensorWorld() const;
			
			void GetInverseInertiaTensor(Matrix3 *inverseInertiaTensor) const;
			void GetInverseInertiaTensorWorld(Matrix3 *inverseInertiaTensor) const;


			void		ClearTorqueAccumulator();
			void		calculateDerivedData();
			inline bool GetAwake()const { return isAwake; }
			void SetAwake(const bool awake = true);
			void SetCanSleep(const bool canSleep);


			inline Vector3 GetLastFrameAcceleration() const { return lastFrameAcceleration;}

			//holds transform matrix to convarte body space to world space and vice versa
			Matrix4 transformMatrix;

			Quaternion orientation;
		private:
			
			Vector3 angularVelocity;
			Vector3 torqueAccum;

			Vector3 lastFrameAcceleration;
	
			Matrix3 inverseInertiaTensor;
			Matrix3 inverseInertiaTensorWorld;
		
			/**
			* Holds the amount of motion of the body. This is a recency-
			* weighted mean that can be used to put a body to sleep.
			*/
			real motion;
			/**
			* A body can be put to sleep to avoid it being updated
			* by the integration functions or affected by collisions
			* with the world.
			*/
			bool isAwake;
			bool canSleep;
	};
}

#endif //RIGID_BODY