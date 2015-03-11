	#include "Platform.h"

	PlatformDemo::PlatformDemo()
	:
	massPos(0,0,0.5f)
	{
		sens=1;
		time=0;
		mass= make_shared<andro::Cube>(ForceToAndro(massPos),andro::Vector3(0.2f ,0.2f ,0.2f),andro::Vector3(7,0,0));
		andro::Engine::GetInstance()->GetScene()->AddDynamicObject(mass);
		// Create the masses and connections.
		particleArray[0].SetPosition(1,10,0);
		particleArray[1].SetPosition(-1,10,0);
		particleArray[2].SetPosition(1,12,-3);
		particleArray[3].SetPosition(-1,12,-3);
		particleArray[4].SetPosition(1,12,4);
		particleArray[5].SetPosition(-1,12,4);

		force::ParticleWorld& physicsEngine = *force::ParticleWorld::GetInstance();


		for (unsigned i = 0; i < 6; i++)
		{
			particleArray[i].SetMass(BASE_MASS);
			particleArray[i].SetDamping(0.8f);
     		physicsEngine.registry.add(&particleArray[i],new force::ParticleGravity());
			physicsEngine.AddParticles(&particleArray[i], 1);

		}
		physicsEngine.AddParticleContactGenerators(new force::GroundContacts(),1);


		
		rods[0] = new force::ParticleRod(&particleArray[0],&particleArray[1],2);
		line[0] = make_shared<andro::Line>(ForceToAndro(particleArray[0].GetPosition()),
									ForceToAndro(particleArray[1].GetPosition()),0.17);

		rods[1]=new force::ParticleRod(&particleArray[2],&particleArray[3],2);
		line[1] = make_shared<andro::Line>(ForceToAndro(particleArray[2].GetPosition()),
									ForceToAndro(particleArray[3].GetPosition()),0.17);

		
		rods[2]=new force::ParticleRod(&particleArray[4],&particleArray[5],2);
		line[2] = make_shared<andro::Line>(ForceToAndro(particleArray[4].GetPosition()),
									ForceToAndro(particleArray[5].GetPosition()),0.17);

		rods[3]=new force::ParticleFlexibleRod(&particleArray[2],&particleArray[4],7);
		line[3] = make_shared<andro::Line>(ForceToAndro(particleArray[2].GetPosition()),
									ForceToAndro(particleArray[4].GetPosition()),0.017);

		rods[4]=new force::ParticleFlexibleRod(&particleArray[3],&particleArray[5],7);
		line[4] = make_shared<andro::Line>(ForceToAndro(particleArray[3].GetPosition()),
									ForceToAndro(particleArray[5].GetPosition()),0.017);

		rods[5]=new force::ParticleRod(&particleArray[0],&particleArray[2],3.606);
		line[5] = make_shared<andro::Line>(ForceToAndro(particleArray[0].GetPosition()),
									ForceToAndro(particleArray[2].GetPosition()),0.17);

		rods[6]=new force::ParticleRod(&particleArray[1],&particleArray[3],3.606);
		line[6] = make_shared<andro::Line>(ForceToAndro(particleArray[1].GetPosition()),
									ForceToAndro(particleArray[3].GetPosition()),0.17);

		rods[7]=new force::ParticleRod(&particleArray[0],&particleArray[4],4.472);
		line[7] = make_shared<andro::Line>(ForceToAndro(particleArray[0].GetPosition()),
									ForceToAndro(particleArray[4].GetPosition()),0.17);

		rods[8]=new force::ParticleRod(&particleArray[1],&particleArray[5],4.472);
		line[8] = make_shared<andro::Line>(ForceToAndro(particleArray[1].GetPosition()),
									ForceToAndro(particleArray[5].GetPosition()),0.17);

		rods[9] =new force::ParticleFlexibleRod(&particleArray[0],&particleArray[3],4.123);
		line[9] = make_shared<andro::Line>(ForceToAndro(particleArray[0].GetPosition()),
							ForceToAndro(particleArray[3].GetPosition()),0.02);

		rods[10]=new force::ParticleFlexibleRod(&particleArray[2],&particleArray[5],7.28);
		line[10] = make_shared<andro::Line>(ForceToAndro(particleArray[2].GetPosition()),
									ForceToAndro(particleArray[5].GetPosition()),0.02);

		rods[11]=new force::ParticleFlexibleRod(&particleArray[4],&particleArray[1],4.899);
		line[11] = make_shared<andro::Line>(ForceToAndro(particleArray[4].GetPosition()),
									ForceToAndro(particleArray[1].GetPosition()),0.02);

		rods[12]=new force::ParticleFlexibleRod(&particleArray[1],&particleArray[2],4.123);
		line[12] = make_shared<andro::Line>(ForceToAndro(particleArray[1].GetPosition()),
									ForceToAndro(particleArray[2].GetPosition()),0.02);

		rods[13]=new force::ParticleFlexibleRod(&particleArray[3],&particleArray[4],7.28);
		line[13] = make_shared<andro::Line>(ForceToAndro(particleArray[3].GetPosition()),
									ForceToAndro(particleArray[4].GetPosition()),0.02);

		rods[14]=new force::ParticleFlexibleRod(&particleArray[5],&particleArray[0],4.899);
		line[14] = make_shared<andro::Line>(ForceToAndro(particleArray[5].GetPosition()),
									ForceToAndro(particleArray[0].GetPosition()),0.02);

		for(int i=0;i<15;i++)
			andro::Engine::GetInstance()->GetScene()->AddDynamicObject(line[i]);

		for (unsigned i = 0; i < ROD_COUNT; i++)
		{
			physicsEngine.particle_contactGenerators.push_back(rods[i]);
		}

		updateAdditionalMass();
	}

	PlatformDemo::~PlatformDemo()
	{
		for(int i=0;i<ROD_COUNT;i++)
			if (rods[i]) 
				delete rods[i];
	}

	void PlatformDemo::updateAdditionalMass()
	{
		for (unsigned i = 2; i < 6; i++)
		{
			particleArray[i].SetMass(BASE_MASS);
		}

		// Find the coordinates of the mass as an index and proportion
		float xp = massPos.x;
		if (xp < 0) xp = 0;
		if (xp > 1) xp = 1;

		float zp = massPos.z;
		if (zp < 0) zp = 0;
		if (zp > 1) zp = 1;

		// Calculate where to draw the mass
		massDisplayPos= force::Vector3();

		// Add the proportion to the correct masses
		particleArray[2].SetMass(BASE_MASS + EXTRA_MASS*(1-xp)*(1-zp));
		massDisplayPos = massDisplayPos + particleArray[2].GetPosition() * ((1-xp)*(1-zp));


		if (xp > 0)
		{
			particleArray[4].SetMass(BASE_MASS + EXTRA_MASS*xp*(1-zp));
			massDisplayPos =massDisplayPos +  particleArray[4].GetPosition() * (xp*(1-zp));

			if (zp > 0)
			{
				particleArray[5].SetMass(BASE_MASS + EXTRA_MASS*xp*zp);
				massDisplayPos =massDisplayPos +  particleArray[5].GetPosition() * (xp * zp);
			}
		}
		if (zp > 0)
		{
			particleArray[3].SetMass(BASE_MASS + EXTRA_MASS*(1-xp)*zp);
			  massDisplayPos =massDisplayPos +  particleArray[3].GetPosition() * ((1-xp)*zp );

		}
	}


	void PlatformDemo::update(float deltaTime)
	{
		line[0]->UpdateLine(ForceToAndro(particleArray[0].GetPosition()),
									ForceToAndro(particleArray[1].GetPosition()));
		line[1]->UpdateLine(ForceToAndro(particleArray[2].GetPosition()),
									ForceToAndro(particleArray[3].GetPosition()));
		line[2]->UpdateLine(ForceToAndro(particleArray[4].GetPosition()),
									ForceToAndro(particleArray[5].GetPosition()));
		line[3]->UpdateLine(ForceToAndro(particleArray[2].GetPosition()),
										ForceToAndro(particleArray[4].GetPosition()));
		line[4]->UpdateLine(ForceToAndro(particleArray[3].GetPosition()),
										ForceToAndro(particleArray[5].GetPosition()));
		line[5]->UpdateLine(ForceToAndro(particleArray[0].GetPosition()),
										ForceToAndro(particleArray[2].GetPosition()));
		line[6]->UpdateLine(ForceToAndro(particleArray[1].GetPosition()),
										ForceToAndro(particleArray[3].GetPosition()));
		line[7]->UpdateLine(ForceToAndro(particleArray[0].GetPosition()),
										ForceToAndro(particleArray[4].GetPosition()));
		line[8]->UpdateLine(ForceToAndro(particleArray[1].GetPosition()),
										ForceToAndro(particleArray[5].GetPosition()));
		line[9]->UpdateLine(ForceToAndro(particleArray[0].GetPosition()),
										ForceToAndro(particleArray[3].GetPosition()));
		line[10]->UpdateLine(ForceToAndro(particleArray[2].GetPosition()),
										ForceToAndro(particleArray[5].GetPosition()));
		line[11]->UpdateLine(ForceToAndro(particleArray[1].GetPosition()),
										ForceToAndro(particleArray[4].GetPosition()));
		line[12]->UpdateLine(ForceToAndro(particleArray[1].GetPosition()),
										ForceToAndro(particleArray[2].GetPosition()));
		line[13]->UpdateLine(ForceToAndro(particleArray[3].GetPosition()),
										ForceToAndro(particleArray[4].GetPosition()));
		line[14]->UpdateLine(ForceToAndro(particleArray[0].GetPosition()),
										ForceToAndro(particleArray[5].GetPosition()));

		time+=deltaTime;
		if(time >3)
		{	
			time = 0;
			sens*=-1;
		}
		massPos.x+= 0.01f* sens;
		massPos.z+= 0.007f* sens;

		if(massPos.x > 1)
			massPos.x=1;
		else  if(massPos.x < 0)
			massPos.x=0;
		if(massPos.z > 1)
			massPos.z=1;
		else if(massPos.z < 0)
			massPos.z=0;
		updateAdditionalMass();
		mass->SetPosition(ForceToAndro(massDisplayPos));
	}

