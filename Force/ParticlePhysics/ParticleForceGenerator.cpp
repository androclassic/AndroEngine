#include "ParticleForceGenerator.h"

using namespace force;


void ParticleForceRegistry::updateForces(real dt)
{
	for(Registry::iterator it=registrations.begin(); it<registrations.end(); it++)
	{
		it->fg->updateForce(it->particle, dt);
	}
}

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator *fg)
{
	ParticleForceRegistry::ParticleForceRegistration registration;
	registration.particle=particle;
	registration.fg=fg;

	registrations.push_back(registration);
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator *fg)
{

}

void ParticleForceRegistry::clear()
{
	registrations.clear();
}

//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

ParticleGravity::ParticleGravity(const Vector3& gravity)
	: gravity(gravity)
{
}

void ParticleGravity::updateForce(Particle* particle, real duration)
{
	// Check that we do not have infinite mass
	if (!particle->isMovable()) return;

	// Apply the mass-scaled force to the body
	particle->AddForce(gravity * particle->GetMass());
}


//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

ParticleDrag::ParticleDrag(real k1, real k2)
{
	this->k1=k1;
	this->k2=k2;
}

void ParticleDrag::updateForce(Particle* particle, real duration)
{
	Vector3 force =	particle->GetVelocity();
	
	// Calculate the total drag coefficient.
	real dragCoeff = force.magnitude();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;
	// Calculate the final force and apply it.
	force.normalise();
	force *= -dragCoeff;
	particle->AddForce(force);
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//


ParticleSpringAnchor::ParticleSpringAnchor(Vector3 anchorPos, real springCoef, real restLen)
{
	anchor=anchorPos;
	springCoeficient=springCoef;
	restLenght=restLen;
}

void ParticleSpringAnchor::updateForce(Particle* particle, real duration)
{
	Vector3 force =	particle->GetPosition();
	force -= anchor;

	real magnitude =springCoeficient * real_abs(force.magnitude()-restLenght);
	
	force.normalise();
	force *=magnitude;
	
	particle->AddForce(force);
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

ParticleBungee::ParticleBungee(Particle *other,	real springConstant, real restLength)
{
	this->other = other;
	this->springConstant = springConstant;
	this->restLength= restLength;
}

void ParticleBungee::updateForce(Particle* particle, real duration)
{
	// Calculate the vector of the spring.
	Vector3 force = particle->GetPosition() ;
	force -= other->GetPosition();

	// Check if the bungee is compressed.
	real magnitude = force.magnitude();
	if (magnitude <= restLength) return;
	// Calculate the magnitude of the force.
	
	magnitude = springConstant * (restLength - magnitude);
	// Calculate the final force and apply it.
	force.normalise();
	force *= -magnitude;
	particle->AddForce(force);
}