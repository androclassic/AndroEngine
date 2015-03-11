#ifndef _WORLD_
#define _WORLD_

#include "core/Singleton.h"

#include "ParticlePhysics/ParticleWorld.h"

#include "RigidBodyPhysics/Contact.h"
#include "RigidBodyPhysics/CollisionDetector.h"
#include "RigidBodyPhysics/Primitive.h"
#include "RigidBodyPhysics/ContactResolver.h"
#include "RigidBodyPhysics/joints.h"
#include <vector>

namespace force
{



	/**
	* Keeps track of a set of particles and RigidBodies, and provides the means to
	* update them all.
	*/
	class World :  public ParticleWorld
	{
	public:

		SINGLETON_(World)

		std::vector<RigidBody*> rigidBodies;
		std::vector<ContactGenerator*> contactGenerators;
		ContactResolver resolver;

		CollisionData collisionData;
		CollisionDetector collisionDetector;

		
		public:

		void AddContactGenerators(ContactGenerator* contactgen, unsigned int count);

		void startFrame();

		/**
		* Calls each of the registered contact generators to report
		* their contacts. Returns the number of generated contacts.
		*/
		void generateContacts();

		/**
		* Integrates all the particles in this world forward in time
		* by the given duration.
		*/
		void integrate(real duration);

		/**
		* Processes all the physics for the particle world.
		*/
		void runPhysics(real duration);
	
		void AddSphere(Sphere* sphere);
		void AddBox(Box* box);
		void AddPlane(Plane* plane);
		
	private:
		void AddRigidBody(RigidBody *rigidBody, unsigned int count);
		std::vector<Sphere*> spheres;
		std::vector<Box*> boxes;
		std::vector<Plane*> planes;
	};
}
#endif