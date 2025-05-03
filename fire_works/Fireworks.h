//
// Created by stefa on 01/05/2025.
//

#ifndef FIREWORKS_H
#define FIREWORKS_H

#include "Fire.h"
#include "fireworksrule.h"
#include <vector>
#include <cmath>


class Fireworks {
public:
    Fireworks();
    ~Fireworks();

    void update(float duration);
    void create();
    void create(Fire* parent);
    void draw(int shadow);
    void add(Fire* fire);
    FireworksRule* getRule(int index);
    int getParticleCount() const;
    static constexpr float PI = 3.14159265358979323846f;

private:
    FireworksRule m_rule[3];
    std::vector<Fire*> fireworks;
};

#endif // FIREWORKS_H