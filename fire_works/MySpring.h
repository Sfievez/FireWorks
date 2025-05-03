#ifndef MY_SPRING_H
#define MY_SPRING_H

#include "particle.h"
#include "pfgen.h"
#include "MyAnchordSpring.h"
#include "cyclone.h"

using namespace cyclone;

	class Myspring : public ParticleForceGenerator
	{
		Particle* other;
		double springConstant;
		double restLength;
		Particle* anchor;

	public:
		Myspring(Particle* p, double springConstant, double restLength);
		void updateForce(Particle* p, real duration) override;
	};

#endif // MY_SPRING_H
