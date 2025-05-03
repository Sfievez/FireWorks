#ifndef MYANCHORDSPRING_H
#define MYANCHORDSPRING_H

#include "particle.h"
#include "pfgen.h"
#include "MySpring.h"

using namespace cyclone;

class MyAnchoredSpring : public ParticleForceGenerator
{
protected:
    Vector3 *anchor;
    double springConstant;
    double restLength;
public:
    MyAnchoredSpring();
    MyAnchoredSpring(Vector3 *anchor, double springConstant, double restLength) 
    {
        this->anchor = anchor;
        this->springConstant = springConstant;
        this->restLength = restLength;
    }
    const Vector3* getAnchor() const { return anchor; } //return anchor position
    void init(Vector3 *anchor, double springConstant, double restLength); //set values
    virtual void updateForce(Particle *particle, real duration);
};


#endif //MYANCHORDSPRING_H
