#ifndef MOVE_H
#define MOVE_H

#include "core.h"
#include "particle.h"
#include "pfgen.h"
#include "MySpring.h"
#include "math.h"
#include <vector>
#include <FL/Fl_Gl_Window.h>
#include <Fl/Fl.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <Fl/Fl_Double_Window.h>

#include "Vec3f.h"
#include "Viewer.h"
#include "MyAnchordSpring.h"


class Mover
{
public:
    Mover();
    ~Mover();

    void update(float duration);
    void draw(int shadow);

    void checkEdges();
    void checkCollisionWithPlane(const cyclone::Vector3& pointOnPlane, const cyclone::Vector3& planeNormal);
    void checkCollisionWithPlane(const cyclone::Vector3& p1, const cyclone::Vector3& p2, const cyclone::Vector3& p3);

    void setConnection(Vector3* anchor, double k, double restLength);


    Particle* m_particle;  // Physics particle
    ParticleForceRegistry* m_forces;  // Force registry
    ParticleGravity* m_gravity;  // Gravity force
    ParticleDrag* m_drag;  // Drag force
    Myspring* m_spring1;  // Spring force
    MyAnchoredSpring* m_spring; // Spring force
    Particle* m_other_particle; // Other particle for spring
private:
    float size;                     // Visual size (radius) of the sphere
    float restitution;              // Bounce coefficient (0-1)
};

#endif // MOVER_H