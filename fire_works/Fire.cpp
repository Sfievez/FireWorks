//
// Created by stefa on 01/05/2025.
//

#include "Fire.h"
#include "random.h"


using namespace cyclone;

Fire::Fire(int type) : m_type(type), m_size(0.1f), m_rule(nullptr) {
    m_particle = new Particle();
    m_particle->setMass(1.0f);
    m_particle->setDamping(0.99f);
    m_particle->setAcceleration(Vector3(0, -10, 0));

    float x = cyclone::Random().randomReal(-5.0f, 5.0f);
    float z = cyclone::Random().randomReal(-5.0f, 5.0f);
    m_particle->setPosition(x, 10.0f, z);

    Vector3 vel = cyclone::Random().randomVector(Vector3(0, 0, 0), Vector3(10, 30, 10));
    m_particle->setVelocity(vel);
    m_color = cyclone::Random().randomVector(Vector3(0, 0, 0), Vector3(1, 1, 1));
    m_age = cyclone::Random().randomReal(0.5f, 3.0f);
}

Fire::~Fire() {
    delete m_particle;
}

bool Fire::update(float duration) {
    m_age -= duration;
    if (m_age < 0 || m_particle->getPosition().y < 0) return true;

    m_particle->integrate(duration);
    putHistory();
    return false;
}

void Fire::draw(int shadow) {
    if (shadow)
        glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
    else
        glColor3f(m_color.x, m_color.y, m_color.z);

    Vector3 pos = m_particle->getPosition();
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glutSolidSphere(m_size, 10, 10);
    glPopMatrix();
}

//void Fire::drawHistory() {
//    glLineWidth(4.0f);
//    glPushMatrix();
//    glBegin(GL_LINE_STRIP);
//    for (unsigned int i = 0; i < m_history.size(); i += 2) {
//        Vector3 p = m_history[i];
//        glVertex3f(p.x, p.y, p.z);
//    }
//    glEnd();
//    glPopMatrix();
//    glLineWidth(1.0f);
//}

void Fire::drawHistory() {
    glLineWidth(5.0f);
    glPushMatrix();

    glBegin(GL_LINE_STRIP);
    int n = m_history.size();
    for (int i = 0; i < n; ++i) {
        const cyclone::Vector3& p = m_history[i];

		// Fade out effect
        float alpha = (float)i / n;
        alpha = 1.0f - alpha;

        glColor4f(m_color.x, m_color.y, m_color.z, alpha);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    glPopMatrix();
    glLineWidth(1.0f);
}

void Fire::setRule(FireworksRule* r) {
    m_rule = r;
}

void Fire::putHistory() {
    Vector3 pos = m_particle->getPosition();
    if (m_history.size() < 30)
        m_history.push_back(pos);
    else {
        m_history.pop_front();
        m_history.push_back(pos);
    }
}
