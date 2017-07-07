#include "World.h"

namespace force
{
	
	World::World():collisionData(2000),resolver(50)
	{
		this->maxContacts = 2000;
	}


	World::~World()
	{
	}


	void World::startFrame()
	{
		for(unsigned int i = 0; i < rigidBodies.size(); i++)
		{
			// Remove all forces from the accumulator.
			rigidBodies[i]->ClearForceAccumulator();
			rigidBodies[i]->ClearTorqueAccumulator();
			rigidBodies[i]->calculateDerivedData();
		}

	}

	
	void World::AddRigidBody(RigidBody* rigidbody, unsigned int count)
	{
		for(unsigned int i = 0 ; i < count;i++)
		{
			if(rigidbody==NULL)
				break;

			rigidBodies.push_back(rigidbody);
			rigidbody++;
		}
	}
	


void World::AddSphere(Sphere* sphere)
{
	spheres.push_back(sphere);
 
	if(sphere->rigidBody != NULL)
		AddRigidBody(sphere->rigidBody, 1);	
}

void World::AddBox(Box* box)
{
  boxes.push_back(box);
  
  if(box->rigidBody != NULL)
	  AddRigidBody(box->rigidBody, 1);	

}

void World::AddPlane(Plane* plane)
{
   planes.push_back(plane);
   if(plane->rigidBody != NULL)
	   AddRigidBody(plane->rigidBody, 1);
}

void World::generateContacts()
{
	CollisionData* data=&collisionData;


	

	for (unsigned int i = 0; i < spheres.size(); i++)
		{
			for (unsigned int j = i + 1; j < spheres.size(); j++)
				data->contactsNr+=collisionDetector.sphereAndSphere(*spheres[j],*spheres[i], data);
	
			for (unsigned int j = 0; j < planes.size(); j++)
				data->contactsNr+=collisionDetector.sphereAndHalfSpace(*spheres[i],*planes[j], data);
		}

	for (unsigned int i = 0; i < boxes.size(); i++)
		{
			for(unsigned int j=i+1 ; j < boxes.size(); j++)
				data->contactsNr+=collisionDetector.boxAndBox(*boxes[j],*boxes[i], data);

			for (unsigned int j = 0; j < planes.size(); j++)
				data->contactsNr+=collisionDetector.boxAndHalfSpace(*boxes[i],*planes[j], data);

			for (unsigned int j = 0; j < spheres.size(); j++)
				data->contactsNr+=collisionDetector.boxAndSphere(*boxes[i],*spheres[j], data);

		}

	// contact generators
	for(unsigned int i = 0; i < contactGenerators.size(); i++)
		{
			data->contactsNr+=contactGenerators[i]->addContact(data->contacts + data->contactsNr, data->contactsLeft);
		}


}

	
	void World::integrate(real duration)
	{

		for(unsigned int i = 0; i < rigidBodies.size(); i++)
		{
			// Remove all forces from the accumulator.
			rigidBodies[i]->Update(duration);
		}
	}

	void World::AddContactGenerators(ContactGenerator* contactgen, unsigned int count)
	{
		for(unsigned int i = 0 ; i < count;i++)
		{
			if(contactgen==NULL)
				break;

			contactGenerators.push_back(contactgen);
			contactgen++;
		}
	}

	void World::runPhysics(real duration)
	{
		if(duration > 0 && duration < 1000)
		{

	
			// First apply the force generators.
			registry.updateForces(duration);

			// Then integrate the objects.
			integrate(duration);
			ParticleWorld::integrate(duration);

			generateContacts();

			resolver.setIterations(rigidBodies.size()*15);
			resolver.resolveContacts(this->collisionData.contacts , collisionData.contactsNr, duration);
			collisionData.contactsNr=0;

			unsigned usedContacts = ParticleWorld::generateContacts();
			particle_resolver.setIterations(usedContacts * 10);
			particle_resolver.resolveContacts(particle_contacts, usedContacts, duration);


		}
	}

}