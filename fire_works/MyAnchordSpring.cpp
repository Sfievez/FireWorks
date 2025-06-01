#include "MyAnchordSpring.h"
#include "MySpring.h"
#include "MyGlWindow.h"


using namespace cyclone;

void MyAnchoredSpring::updateForce(Particle* particle, real duration)
{
    if (!particle->hasFiniteMass()) return;

    Vector3 position;
    particle->getPosition(&position);

    Vector3 displacement = position - *anchor;
    real length = displacement.magnitude();

    if (length < 1e-9) return;

	// hook's law
    real delta = length - restLength;
    Vector3 force = displacement * (-springConstant * delta / length);

    particle->addForce(force);
}
