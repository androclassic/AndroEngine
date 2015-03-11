#ifndef _PFG_H_
#define _PFG_H_

#include "Particle.h"
#include <vector>

namespace force
{

	class ParticleForceGenerator
	{
	public:
		/**
		* Overload this in implementations of the interface to calculate
		* and update the force applied to the given particle.
		*/
		virtual void	updateForce(Particle *particle, real duration) = 0;
	};


	class ParticleGravity : public ParticleForceGenerator
	{

	public:

						ParticleGravity(const Vector3 &gravity=Vector3(0.0f, -9.8f, 0.0f));
		virtual void	updateForce(Particle *particle, real duration);
	
	private:

		Vector3 gravity;

	};

	/**
	* A force generator that applies a drag force. One instance
	* can be used for multiple particles.
	*/
	class ParticleDrag : public ParticleForceGenerator
	{
	public:

		/** Creates the generator with the given coefficients. */
						ParticleDrag(real k1, real k2);
		virtual void	updateForce(Particle *particle, real duration);

	private:
		/** Holds the velocity drag coeffificent. */
		real k1;

		/** Holds the velocity squared drag coeffificent. */
		real k2;

	};

		class ParticleSpringAnchor : public ParticleForceGenerator
	{
	public:

						ParticleSpringAnchor(Vector3 anchorPos, real springCoef, real restLen);
		virtual void	updateForce(Particle *particle, real duration);

	private:
		Vector3 anchor;
		real	springCoeficient;
		real	restLenght;

	};


	/**
	* A force generator that applies a spring force only
	* when extended.
	*/
	class ParticleBungee : public ParticleForceGenerator
	{
	
		public:
			/** Creates a new bungee with the given parameters. */
			ParticleBungee(Particle *other,	real springConstant, real restLength);
			/** Applies the spring force to the given particle. */
			virtual void updateForce(Particle *particle, real duration);
	  private:
		  Particle *other;
		  real springConstant;
		  real restLength;

	};




	/**
	* Holds all the force generators and the particles they apply to.
	*/
	class ParticleForceRegistry
	{
	public:
		void		add(Particle* particle, ParticleForceGenerator *fg);
		void		remove(Particle* particle, ParticleForceGenerator *fg);
		void		clear();
		void		updateForces(real dt);

	protected:
		struct ParticleForceRegistration
		{
			Particle *particle;
			ParticleForceGenerator *fg;
		};
		typedef std::vector<ParticleForceRegistration> Registry;
		Registry registrations;
	};

}
#endif  //FORCE_PARTICLE_FORCE_GENERATOR_H