#ifndef _CONTACT_RESOLVER_
#define _CONTACT_RESOLVER_

#include "RigidBodyPhysics/Contact.h"

namespace force
{

	/**
	* The contact resolution routine for  contacts. One
	* resolver instance can be shared for the whole simulation.
	*/

    class ContactResolver
    {
    protected:
        unsigned velocityIterations;
        unsigned positionIterations;

        real velocityEpsilon;
        real positionEpsilon;
		real positionSleepEpsilon;

    public:
        unsigned velocityIterationsUsed;
		unsigned positionIterationsUsed;


    public:
        ContactResolver(unsigned iterations,
            real velocityEpsilon=(real)0.1,
            real positionEpsilon=(real)0.01,
			real positionSleepEpsilon=(real)0.05);

        ContactResolver(unsigned velocityIterations,
            unsigned positionIterations,
            real velocityEpsilon=(real)0.1,
            real positionEpsilon=(real)0.01,
			real positionSleepEpsilon=(real)0.05);
		bool isValid()
        {
            return (velocityIterations > 0) &&
                   (positionIterations > 0) &&
                   (positionEpsilon >= 0.0f) &&
                   (positionEpsilon >= 0.0f);
        }

        void setIterations(unsigned velocityIterations,
                           unsigned positionIterations);

        void setIterations(unsigned iterations);

        void setEpsilon(real velocityEpsilon,
                        real positionEpsilon,
						real positionSleepEpsilon);

        void resolveContacts(Contact *contactArray,
            unsigned numContacts,
            real duration);

    protected:
        void prepareContacts(Contact *contactArray, unsigned numContacts,
            real duration);

        void adjustVelocities(Contact *contactArray,
            unsigned numContacts,
            real duration);

        void adjustPositions(Contact *contacts,
            unsigned numContacts,
            real duration);
    };


}
#endif