#pragma once

#include "../../EventManager/EventManager.h"
#include "../../Force/core/Force.h"
#include "../../AndroRenderer/AndroRenderer.h"

#include <stdio.h>
#include <cassert>
#include <memory>

#define ROD_COUNT 15

#define BASE_MASS 1
#define EXTRA_MASS 4


#ifndef ForceToAndro
#define ForceToAndro(v) andro::Vector3(v.x, v.y, v.z)
#endif
#ifndef	AndroToForce
#define AndroToForce(v) force::Vector3(v.x, v.y, v.z)
#endif


class PlatformDemo 
{
    force::ParticleLink *rods[ROD_COUNT];

    force::Vector3 massPos;
    force::Vector3 massDisplayPos;
	force::Particle particleArray[7];
	shared_ptr<andro::Line> line[15];
	shared_ptr<andro::Cube> mass;
	float time;
	int sens;
    /**
     * Updates particle masses to take into account the mass
     * that's on the platform.
     */
    void updateAdditionalMass();

public:
    /** Creates a new demo object. */
    PlatformDemo();
    virtual ~PlatformDemo();

    /** Update the particle positions. */
    virtual void update(float deltaTime);

};