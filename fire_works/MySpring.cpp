#include "MySpring.h"

#include <iostream>

#include "drawUtils.h"
#include "timing.h"
using namespace cyclone;

Myspring::Myspring(Particle* p, double k, double l)
    : other(p), springConstant(k), restLength(l) {
}

void Myspring::updateForce(Particle* p, real duration) {
    if (!other || !p->hasFiniteMass()) return;

    Vector3 posP = p->getPosition();
    Vector3 posOther = other->getPosition();

    Vector3 displacement = posP - posOther;
    real distance = displacement.magnitude();
    if (distance < 1e-9) return;

    real delta = distance - restLength;
    Vector3 force = displacement * (-springConstant * delta / distance);

    p->addForce(force);
}