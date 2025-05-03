//
// Created by stefa on 01/05/2025.
//

#include "Fireworks.h"
#include "random.h"

Fireworks::Fireworks() {
    m_rule[0].setParameters(1, 2.5f, 4.0f, cyclone::Vector3(-15, -15, -15), cyclone::Vector3(15, 15, 15), 0.8f, 50);
    m_rule[1].setParameters(1, 0.5f, 10.4f, cyclone::Vector3(-15, 5, -5), cyclone::Vector3(15, 6, 15), 0.2, 40);
    m_rule[2].setParameters(1, 0.5f, 12.4f, cyclone::Vector3(-5, 0, -5), cyclone::Vector3(5, 5, 5), 0.7, 25);
}

Fireworks::~Fireworks() {
    for (Fire* f : fireworks) delete f;
    fireworks.clear();
}

void Fireworks::update(float duration) {
    std::vector<Fire*> expired;
    for (auto it = fireworks.begin(); it != fireworks.end();) {
        Fire* fire = *it;
        if (fire->update(duration)) {
            if (fire->m_type == 0) expired.push_back(fire);
            it = fireworks.erase(it);
        } else {
            ++it;
        }
    }
    for (Fire* init : expired) {
        create(init);
        delete init;
    }
}

//void Fireworks::create() {
//    for (int i = 0; i < 5; ++i) {
//        Fire* f = new Fire(0);
//        f->setRule(&m_rule[rand() % 3]);
//        fireworks.push_back(f);
//    }
//}

void Fireworks::create() {
    for (int i = 0; i < 1; ++i) {
        Fire* f = new Fire(0);
        f->setRule(&m_rule[rand() % 3]);

        f->m_particle->setPosition(cyclone::Random().randomReal(-10.0f, 10.0f), 50.0f, cyclone::Random().randomReal(-10.0f, 10.0f));

		// add h and v velocity
        float vx = cyclone::Random().randomReal(-2.0f, 2.0f);
        float vy = cyclone::Random().randomReal(25.0f, 35.0f);
        float vz = cyclone::Random().randomReal(-2.0f, 2.0f);
        f->m_particle->setVelocity(cyclone::Vector3(vx, vy, vz));

        f->m_size = 0.2f;
        fireworks.push_back(f);
    }
}

// Fonction to generate a random unit vector
auto randomUnitVector = []() -> cyclone::Vector3 {
    float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * Fireworks::PI;
    float phi = acosf(2.0f * static_cast<float>(rand()) / RAND_MAX - 1.0f);
    float x = sinf(phi) * cosf(theta);
    float y = sinf(phi) * sinf(theta);
    float z = cosf(phi);
    return cyclone::Vector3(x, y, z);
};

void Fireworks::create(Fire* parent) {
    for (unsigned i = 0; i < parent->m_rule->payloadCount; ++i) {
        Fire* child = new Fire(1);
		child->m_size = 0.7f + cyclone::Random().randomReal(0.0f, 0.4f);
        child->setRule(parent->m_rule);

        cyclone::Vector3 pos = parent->m_particle->getPosition();
        cyclone::Vector3 direction = randomUnitVector();
        float speed = cyclone::Random().randomReal(10.0f, 25.0f);
        cyclone::Vector3 vel = direction * speed;

        child->m_particle->setPosition(pos);
        child->m_particle->setVelocity(vel);
        child->m_particle->setDamping(parent->m_rule->damping);
        child->m_particle->setAcceleration(cyclone::Vector3(0, -10, 0));
        child->m_age = cyclone::Random().randomReal(parent->m_rule->minAge, parent->m_rule->maxAge);

        fireworks.push_back(child);
    }
}

void Fireworks::draw(int shadow) {
    for (Fire* f : fireworks) {
        f->draw(shadow);
        if (!shadow) f->drawHistory();
    }
}

void Fireworks::add(Fire* fire) {
	fireworks.push_back(fire);
}

FireworksRule* Fireworks::getRule(int index) {
	if (index >= 0 && index < 3) {
		return &m_rule[index];
	}
	return nullptr;
}

int Fireworks::getParticleCount() const {
    return fireworks.size();
}