#ifndef _CONTACT_H
#define _CONTACT_H
#include "RigidBody.h"
#include "Matrix3.h"
namespace force
{

class Contact
{
	public:
		Contact()
		{
			restitution = 0.0f;
			friction = 0.0f;
			penetration = 0.0f;
			body[0] = body[1] = NULL;
		}
	
		RigidBody* body[2];
		Vector3 linearMovement[2];

		/**
         * Holds the world space position of the contact point relative to
         * centre of each body. This is set when the calculateInternals
         * function is run.
         */
        Vector3 relativeContactPosition[2];

		
        /**
         * Holds the required change in velocity for this contact to be
         * resolved.
         */
        real desiredDeltaVelocity;
		        /**
         * Holds the closing velocity at the point of contact. This is set
         * when the calculateInternals function is run.
         */
        Vector3 contactVelocity;



		Vector3 contactNormal;
		Vector3 contactPoint;
		Matrix3	contactToWorld;

		real penetration;
		real restitution;
		real friction; 

		void calculateInternals(real duration);
		void matchAwakeState();
		void applyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2]);
        void applyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2],  real penetration);

		void calculateDesiredDeltaVelocity(real duration);

private:
		void calculateContactBasis();
		inline Vector3 calculateFrictionlessImpulse(Matrix3 * inverseInertiaTensor);
		inline Vector3 calculateFrictionImpulse(Matrix3 * inverseInertiaTensor);

		Vector3 calculateLocalVelocity(unsigned bodyIndex, real duration);
        void swapBodies();
};


struct CollisionData
{
	CollisionData(unsigned int maxContacts)
	{
		restitution = 0.4f;
		friction = 0.3f;
		tolerance = 0.0f;
		contacts = new Contact[maxContacts];
		contactsLeft = maxContacts;
		contactsNr = 0;
	}

	/** Holds the contact array to write into. */
	Contact *contacts;
	/** Holds the maximum number of contacts the array can take. */
	unsigned contactsLeft;
	unsigned contactsNr;
	real restitution;
	real friction; 
	real tolerance;

};

    /**
     * This is the basic polymorphic interface for contact generators
     * applying to rigid bodies.
     */
    class ContactGenerator
    {
    public:
        /**
         * Fills the given contact structure with the generated
         * contact. The contact pointer should point to the first
         * available contact in a contact array, where limit is the
         * maximum number of contacts in the array that can be written
         * to. The method returns the number of contacts that have
         * been written.
         */
        virtual unsigned addContact(Contact *contact, unsigned limit) const = 0;
    };


}
#endif