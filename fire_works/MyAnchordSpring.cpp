#include "MyAnchordSpring.h"
#include "MySpring.h"
#include "MyGlWindow.h"


using namespace cyclone;

void MyAnchoredSpring::updateForce(Particle* particle, real duration)
{

    // if (anchor == nullptr) {
    //     std::cerr << "[ERROR] anchor is null!" << std::endl;
    //     return;
    // }
    // try {
    //     std::cout << "[DEBUG] anchor value: " << anchor->x << ", " << anchor->y << ", " << anchor->z << std::endl;
    // } catch (...) {
    //     std::cerr << "[EXCEPTION] anchor caused a crash!" << std::endl;
    //     return;
    // }

    
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
