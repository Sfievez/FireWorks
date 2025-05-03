//
// Created by stefa on 09/04/2025.
//

#include "Move.h"
#include <iostream>

Mover::Mover() {
    m_particle = new cyclone::Particle();
    m_particle->setPosition(10, 20, 0);
    m_particle->setVelocity(0, 0, 0);
    m_particle->setMass(1.0f);
    m_particle->setDamping(0.9f);
    m_particle->setAcceleration(0, 0, 0);
    size = 2.0f;
    restitution = 0.8f;

    m_gravity = new cyclone::ParticleGravity(cyclone::Vector3(0, -10, 0));
    m_drag = new cyclone::ParticleDrag(0.1f, 0.01f);

    m_forces = new cyclone::ParticleForceRegistry();
    m_forces->add(m_particle, m_gravity);
    m_forces->add(m_particle, m_drag);
    // m_spring = new MyAnchoredSpring(new cyclone::Vector3(0, 0, 0), 5, 3);
    m_spring = nullptr;
}

Mover::~Mover() {
    delete m_particle;
    delete m_gravity;
    delete m_drag;
    delete m_forces;
    if (m_spring) delete m_spring;
    // delete m_spring1
}

void Mover::update(float duration) {
    m_forces->updateForces(duration);
    m_particle->integrate(duration);

    checkEdges();
}

void Mover::draw(int shadow) {
    cyclone::Vector3 position;
    m_particle->getPosition(&position);

    if (shadow) {
        glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
    }
    else {
        glColor3f(1, 0, 0);
    }

    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glutSolidSphere(size, 30, 30);
    glPopMatrix();
}

void Mover::checkEdges() {
	// Ground
    checkCollisionWithPlane(cyclone::Vector3(0, 0, 0), cyclone::Vector3(0, 1, 0));
	// Walls X
    checkCollisionWithPlane(cyclone::Vector3(-100, 0, 0), cyclone::Vector3(1, 0, 0));
    checkCollisionWithPlane(cyclone::Vector3(100, 0, 0), cyclone::Vector3(-1, 0, 0));
	// Walls Z
    checkCollisionWithPlane(cyclone::Vector3(0, 0, -100), cyclone::Vector3(0, 0, 1));
    checkCollisionWithPlane(cyclone::Vector3(0, 0, 100), cyclone::Vector3(0, 0, -1));
}

void Mover::checkCollisionWithPlane(const cyclone::Vector3& pointOnPlane, const cyclone::Vector3& planeNormal) {
    cyclone::Vector3 position;
    cyclone::Vector3 velocity;
    m_particle->getPosition(&position);
    m_particle->getVelocity(&velocity);

    float distance = (position - pointOnPlane).dot(planeNormal);

    if (distance < size) {
        position += planeNormal * (size - distance);
        m_particle->setPosition(position);

        cyclone::Vector3 vn = planeNormal * velocity.dot(planeNormal);
        cyclone::Vector3 vp = velocity - vn;
        cyclone::Vector3 newVelocity = vp - vn * restitution;

        m_particle->setVelocity(newVelocity);
    }
}

void Mover::checkCollisionWithPlane(const cyclone::Vector3& p1, const cyclone::Vector3& p2, const cyclone::Vector3& p3) {
    cyclone::Vector3 v1 = p2 - p1;
    cyclone::Vector3 v2 = p3 - p1;
    cyclone::Vector3 normal = v1.cross(v2);
    normal.normalise();
    normal *= -1;
    checkCollisionWithPlane(p1, normal);
}

// void setConnection(Mover * a)
// {
//     m_other_particle = a->m_particle;
//     m_spring1 = new cyclone::Myspring(m_other_particle, 7, 7);
//     m_forces->add(m_particle, m_spring);
// }

void Mover::setConnection(Vector3* anchor, double k, double restLength) {
    std::cout << "[DEBUG] setConnection() with anchor pointer: " << anchor << std::endl;
    m_spring = new MyAnchoredSpring(anchor, k, restLength);
    m_forces->add(m_particle, m_spring);
}