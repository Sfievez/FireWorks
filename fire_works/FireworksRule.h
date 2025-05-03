//
// Created by stefa on 01/05/2025.
//

#ifndef FIREWORKSRULE_H
#define FIREWORKSRULE_H

#include "core.h"
#include "particle.h"

class FireworksRule {
public:
    unsigned type;
    cyclone::real minAge, maxAge;
    cyclone::Vector3 minVelocity, maxVelocity;
    cyclone::real damping;
    unsigned payloadCount;

    void setParameters(unsigned t, cyclone::real minA, cyclone::real maxA,
        const cyclone::Vector3& minV, const cyclone::Vector3& maxV,
        cyclone::real d, unsigned count) {
        type = t; minAge = minA; maxAge = maxA;
        minVelocity = minV; maxVelocity = maxV;
        damping = d; payloadCount = count;
    }
};

#endif // FIREWORKSRULE_H