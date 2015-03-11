#include "RigidBodyPhysics/ContactResolver.h"

 namespace force
 {
	// Contact resolver implementation

ContactResolver::ContactResolver(unsigned iterations,
                                 real velocityEpsilon,
                                 real positionEpsilon,
								 real positionSleepEpsilon	)
{
    setIterations(iterations, iterations);
    setEpsilon(velocityEpsilon, positionEpsilon,positionSleepEpsilon);

}

ContactResolver::ContactResolver(unsigned velocityIterations,
                                 unsigned positionIterations,
                                 real velocityEpsilon,
                                 real positionEpsilon,
         					     real positionSleepEpsilon	)
 
{
    setIterations(velocityIterations);
    setEpsilon(velocityEpsilon, positionEpsilon,positionSleepEpsilon);
}

void ContactResolver::setIterations(unsigned iterations)
{
    setIterations(iterations, iterations);
}

void ContactResolver::setIterations(unsigned velocityIterations,
                                    unsigned positionIterations)
{
    ContactResolver::velocityIterations = velocityIterations;
    ContactResolver::positionIterations = positionIterations;
}

void ContactResolver::setEpsilon(real velocityEpsilon,
                                 real positionEpsilon,
         					     real positionSleepEpsilon	)

{
    ContactResolver::velocityEpsilon  	  = velocityEpsilon;
    ContactResolver::positionEpsilon	  = positionEpsilon;
    ContactResolver::positionSleepEpsilon = positionSleepEpsilon;

}

void ContactResolver::resolveContacts(Contact *contacts,
                                      unsigned numContacts,
                                      real duration)
{
    // Make sure we have something to do.
    if (numContacts == 0) return;
    if (!isValid()) return;

    // Prepare the contacts for processing
    prepareContacts(contacts, numContacts, duration);

    // Resolve the interpenetration problems with the contacts.
    adjustPositions(contacts, numContacts, duration);

    // Resolve the velocity problems with the contacts.
    adjustVelocities(contacts, numContacts, duration);
}

void ContactResolver::prepareContacts(Contact* contacts,
                                      unsigned numContacts,
                                      real duration)
{
    // Generate contact velocity and axis information.
    Contact* lastContact = contacts + numContacts;
    for (Contact* contact=contacts; contact < lastContact; contact++)
    {
        // Calculate the internal contact data (inertia, basis, etc).
        contact->calculateInternals(duration);
    }
}

void ContactResolver::adjustVelocities(Contact *c,
                                       unsigned numContacts,
                                       real duration)
{
    Vector3 velocityChange[2], rotationChange[2];
    Vector3 deltaVel;

    // iteratively handle impacts in order of severity.
    velocityIterationsUsed = 0;
    while (velocityIterationsUsed < velocityIterations)
    {
        // Find contact with maximum magnitude of probable velocity change.
        real max = velocityEpsilon;
        unsigned index = numContacts;
        for (unsigned i = 0; i < numContacts; i++)
        {
            if (c[i].desiredDeltaVelocity > max)
            {
                max = c[i].desiredDeltaVelocity;
                index = i;
            }
        }
        if (index == numContacts) break;

        // Match the awake state at the contact
        c[index].matchAwakeState();

        // Do the resolution on the contact that came out top.
        c[index].applyVelocityChange(velocityChange, rotationChange);

        // With the change in velocity of the two bodies, the update of
        // contact velocities means that some of the relative closing
        // velocities need recomputing.
        for (unsigned i = 0; i < numContacts; i++)
        {
            // Check each body in the contact
            for (unsigned b = 0; b < 2; b++) if (c[i].body[b])
            {
                // Check for a match with each body in the newly
                // resolved contact
                for (unsigned d = 0; d < 2; d++)
                {
                    if (c[i].body[b] == c[index].body[d])
                    {
                        deltaVel = velocityChange[d] +
                            rotationChange[d].vectorProduct(
                                c[i].relativeContactPosition[b]);

                        // The sign of the change is negative if we're dealing
                        // with the second body in a contact.
                        c[i].contactVelocity +=
                            c[i].contactToWorld.transformTranspose(deltaVel)
                            * (b?-1:1);
                        c[i].calculateDesiredDeltaVelocity(duration);
                    }
                }
            }
        }
        velocityIterationsUsed++;
    }
}

void ContactResolver::adjustPositions(Contact *c,
                                      unsigned numContacts,
                                      real duration)
{
    unsigned i,index;
    Vector3 linearChange[2], angularChange[2];
    real max;
    Vector3 deltaPosition;

    // iteratively resolve interpenetrations in order of severity.
    positionIterationsUsed = 0;
    while (positionIterationsUsed < positionIterations)
    {
        // Find biggest penetration
        max = positionEpsilon;
        index = numContacts;
        for (i=0; i<numContacts; i++)
        {
            if (c[i].penetration > max)
            {
                max = c[i].penetration;
                index = i;
            }
        }
        if (index == numContacts) break;


        // Match the awake state at the contact
        //c[index].matchAwakeState();



        // Resolve the penetration.
        c[index].applyPositionChange(
            linearChange,
            angularChange,
            max);

        // Again this action may have changed the penetration of other
        // bodies, so we update contacts.
        for (i = 0; i < numContacts; i++)
        {
            // Check each body in the contact
            for (unsigned b = 0; b < 2; b++) if (c[i].body[b])
            {
                // Check for a match with each body in the newly
                // resolved contact
                for (unsigned d = 0; d < 2; d++)
                {
                    if (c[i].body[b] == c[index].body[d])
                    {
                        deltaPosition = linearChange[d] +
                            angularChange[d].vectorProduct(
                                c[i].relativeContactPosition[b]);

                        // The sign of the change is positive if we're
                        // dealing with the second body in a contact
                        // and negative otherwise (because we're
                        // subtracting the resolution)..
                        c[i].penetration +=
                            deltaPosition.scalarProduct(c[i].contactNormal)
                            * (b?1:-1);
                    }
                }
            }
        }
        positionIterationsUsed++;
    }
}

}