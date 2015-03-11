#include "RigidBody.h"

namespace force
{
//------------------------------------------------------------------------------------
//-------------------------------Rigid Body-------------------------------------------
//------------------------------------------------------------------------------------

static inline void _calculateTransformMatrix(Matrix4 &transformMatrix,
											const Vector3 &position,
											const Quaternion &orientation)
{
	transformMatrix.data[0] = 1-2*orientation.j*orientation.j-
	2*orientation.k*orientation.k;
	transformMatrix.data[1] = 2*orientation.i*orientation.j -
	2*orientation.r*orientation.k;
	transformMatrix.data[2] = 2*orientation.i*orientation.k +
	2*orientation.r*orientation.j;
	transformMatrix.data[3] = position.x;
	transformMatrix.data[4] = 2*orientation.i*orientation.j +
	2*orientation.r*orientation.k;
	transformMatrix.data[5] = 1-2*orientation.i*orientation.i-
	2*orientation.k*orientation.k;
	transformMatrix.data[6] = 2*orientation.j*orientation.k -
	2*orientation.r*orientation.i;
	transformMatrix.data[7] = position.y;
	transformMatrix.data[8] = 2*orientation.i*orientation.k -
	2*orientation.r*orientation.j;
	transformMatrix.data[9] = 2*orientation.j*orientation.k +
	2*orientation.r*orientation.i;
	transformMatrix.data[10] = 1-2*orientation.i*orientation.i-
	2*orientation.j*orientation.j;
	transformMatrix.data[11] = position.z;
}

static inline void _transformInertiaTensor(Matrix3 &iitWorld,
											const Quaternion &q,	
											const Matrix3 &iitBody,
											const Matrix4 &rotmat)
{
	real t4 = rotmat.data[0]*iitBody.data[0]+
	rotmat.data[1]*iitBody.data[3]+
	rotmat.data[2]*iitBody.data[6];
	real t9 = rotmat.data[0]*iitBody.data[1]+
	rotmat.data[1]*iitBody.data[4]+
	rotmat.data[2]*iitBody.data[7];
	real t14 = rotmat.data[0]*iitBody.data[2]+
	rotmat.data[1]*iitBody.data[5]+
	rotmat.data[2]*iitBody.data[8];
	real t28 = rotmat.data[4]*iitBody.data[0]+
	rotmat.data[5]*iitBody.data[3]+
	rotmat.data[6]*iitBody.data[6];
	real t33 = rotmat.data[4]*iitBody.data[1]+
	rotmat.data[5]*iitBody.data[4]+
	rotmat.data[6]*iitBody.data[7];
	real t38 = rotmat.data[4]*iitBody.data[2]+
	rotmat.data[5]*iitBody.data[5]+
	rotmat.data[6]*iitBody.data[8];
	real t52 = rotmat.data[8]*iitBody.data[0]+
	rotmat.data[9]*iitBody.data[3]+
	rotmat.data[10]*iitBody.data[6];
	real t57 = rotmat.data[8]*iitBody.data[1]+
	rotmat.data[9]*iitBody.data[4]+
	rotmat.data[10]*iitBody.data[7];
	real t62 = rotmat.data[8]*iitBody.data[2]+
	rotmat.data[9]*iitBody.data[5]+
	rotmat.data[10]*iitBody.data[8];
	iitWorld.data[0] = t4*rotmat.data[0]+
	t9*rotmat.data[1]+
	t14*rotmat.data[2];
	iitWorld.data[1] = t4*rotmat.data[4]+
	t9*rotmat.data[5]+
	t14*rotmat.data[6];
	iitWorld.data[2] = t4*rotmat.data[8]+
	t9*rotmat.data[9]+
	t14*rotmat.data[10];
	iitWorld.data[3] = t28*rotmat.data[0]+
	t33*rotmat.data[1]+
	t38*rotmat.data[2];
	iitWorld.data[4] = t28*rotmat.data[4]+
	t33*rotmat.data[5]+
	t38*rotmat.data[6];
	iitWorld.data[5] = t28*rotmat.data[8]+
	t33*rotmat.data[9]+
	t38*rotmat.data[10];
	iitWorld.data[6] = t52*rotmat.data[0]+
	t57*rotmat.data[1]+
	t62*rotmat.data[2];
	iitWorld.data[7] = t52*rotmat.data[4]+
	t57*rotmat.data[5]+
	t62*rotmat.data[6];
	iitWorld.data[8] = t52*rotmat.data[8]+
	t57*rotmat.data[9]+
	t62*rotmat.data[10];
}

	RigidBody::RigidBody(): Particle()
		,angularVelocity()
		,torqueAccum()
		,orientation()
		,transformMatrix()
		,inverseInertiaTensor()
	{
		inverseInertiaTensor.setDiagonal(1,1,1);
		inverseInertiaTensorWorld.setDiagonal(1,1,1);
		canSleep = true;
		SetAwake();
		damping = 0.8;

	}
	void RigidBody::AddForceAtPoint(Vector3 force, Vector3 point)
	{
		//get point relative to center position
		Vector3 pt = point - position;
		forceAccum += force;
		torqueAccum += pt % force;
	    isAwake = true;
	}

	void RigidBody::AddForceAtBodyPoint(Vector3 force, Vector3 point)
	{
		//convert coordinates relative to the center of the mass
		Vector3 pt =  GetPointInWorldSpace(point);
		AddForceAtPoint(force, pt);
	}

	void RigidBody::AddForce(Vector3 force)
	{
		Particle::AddForce(force);
	    isAwake = true;
	}


	void RigidBody::AddTorque(Vector3 torque)
	{
		torqueAccum += torque;
	    isAwake = true;
	}
	
	Vector3 RigidBody::GetTorque() const
	{
		return torqueAccum;
	}
	
	void RigidBody::ClearTorqueAccumulator()
	{
		torqueAccum = Vector3(0.0f, 0.0f, 0.0f);
	}

	Vector3 RigidBody::GetPointInLocalSpace(const Vector3 &point) const
	{
		return transformMatrix.transformInverse(point);
	}

	Vector3 RigidBody::GetPointInWorldSpace(const Vector3 &point) const
	{
		return transformMatrix.transform(point);
	}

	Matrix3 RigidBody::GetInverseInertiaTensor() const
	{
		return inverseInertiaTensor;
	}

	Matrix3 RigidBody::GetInverseInertiaTensorWorld() const
	{
		return inverseInertiaTensorWorld;
	}


	
	void RigidBody::GetInverseInertiaTensorWorld(Matrix3 *inverseInertiaTensor) const
	{
		*inverseInertiaTensor = inverseInertiaTensorWorld;
	}

	void RigidBody::GetInverseInertiaTensor(Matrix3 *inverseInertiaTensor) const
	{
		*inverseInertiaTensor = this->inverseInertiaTensor;
	}

	void RigidBody::calculateDerivedData()
	{

		orientation.normalise();
		// Calculate the transform matrix for the body.
		_calculateTransformMatrix(transformMatrix, position, orientation);
		// Calculate the inertiaTensor in world space.
		_transformInertiaTensor(inverseInertiaTensorWorld, orientation,
						inverseInertiaTensor, transformMatrix);
	}

	void RigidBody::Update(real dt)
	{

		if (!isAwake && canSleep)
		{
			// Normalise the orientation, and update the matrices with the new
			// position and orientation
			calculateDerivedData();
			// Clear accumulators.
			ClearForceAccumulator();
			ClearTorqueAccumulator();
			return;
		}

		assert(dt > 0 );
		if(inverseMass <= 0)
			WARNING("inverseMass must be a positive value");

	    // Calculate linear acceleration from force inputs.
		lastFrameAcceleration = acceleration;
		lastFrameAcceleration = forceAccum * inverseMass;

    // Calculate angular acceleration from torque inputs.
    Vector3 angularAcceleration = inverseInertiaTensorWorld.transform(torqueAccum);


    // Update linear velocity from both acceleration and impulse.
    velocity += lastFrameAcceleration * dt;

    // Update angular velocity from both acceleration and impulse.
    angularVelocity += angularAcceleration * dt;

    // Impose drag.
    velocity *= real_pow(damping, dt);
    angularVelocity *= real_pow(damping, dt);

    // Update linear position.
    position += velocity * dt;

    // Update angular position.
    orientation.addScaledVector(angularVelocity, dt);

    // Normalise the orientation, and update the matrices with the new
    // position and orientation
    calculateDerivedData();

    // Clear accumulators.
    ClearForceAccumulator();
    ClearTorqueAccumulator();


	// Update the kinetic energy store, and possibly put the body to
    // sleep.
    if (canSleep) {
        real currentMotion = velocity.scalarProduct(velocity) +
            angularVelocity.scalarProduct(angularVelocity);

        real bias = real_pow(0.2, dt);
        motion = bias*motion + (1-bias)*currentMotion;

        if (motion < sleepEpsilon) SetAwake(false);
        else if (motion > 10 * sleepEpsilon) motion = 10 * sleepEpsilon;
    }


	}

	void RigidBody::SetAwake(const bool awake)
	{
		if (awake) 
		{
			isAwake= true;
			// Add a bit of motion to avoid it falling asleep immediately.
			motion = sleepEpsilon * 2;
		}
		else
		{
			isAwake = false;
			velocity.clear();
			acceleration.clear();
			angularVelocity.clear();
			lastFrameAcceleration.clear();
			
			
		}
	}

	void RigidBody::SetCanSleep(const bool canSleep)
	{
		RigidBody::canSleep = canSleep;

		if (!canSleep && !isAwake) SetAwake();
	}
}