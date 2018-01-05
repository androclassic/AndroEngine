#ifndef FORCE_JOINTS_H
#define FORCE_JOINTS_H

#include "Contact.h"

namespace force {

    /**
     * Joints link together two rigid bodies and make sure they do not
     * separate. 
     */
    class Joint : public ContactGenerator
    {
    public:
        /**
         * Holds the two rigid bodies that are connected by this joint.
         */
        std::shared_ptr<RigidBody> body[2];

        /**
         * Holds the relative location of the connection for each
         * body, given in local coordinates.
         */
        Vector3 position[2];

        /**
         * Holds the maximum displacement at the joint before the
         * joint is considered to be violated. This is normally a
         * small, epsilon value.  It can be larger, however, in which
         * case the joint will behave as if an inelastic cable joined
         * the bodies at their joint locations.
         */
        real error;

        /**
         * Configures the joint in one go.
         */
        void set(
            std::shared_ptr<RigidBody> a, const Vector3& a_pos,
            std::shared_ptr<RigidBody> b, const Vector3& b_pos,
            real error
            );

        /**
         * Generates the contacts required to restore the joint if it
         * has been violated.
         */
        unsigned addContact(Contact *contact, unsigned limit) const;
    };

} // namespace force

#endif // FORCE_JOINTS_H