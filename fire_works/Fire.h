//
// Created by stefa on 01/05/2025.
//

#ifndef FIRE_H
#define FIRE_H

#include "particle.h"
#include "core.h"
#include "fireworksrule.h"
#include <deque>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


class Fire {
public:
    Fire(int type);
    ~Fire();

    float m_size;
    int m_type;
    float m_age;
    cyclone::Particle* m_particle;
    FireworksRule* m_rule;
    cyclone::Vector3 m_color;
    std::deque<cyclone::Vector3> m_history;

    bool update(float duration);
    void draw(int shadow);
    void drawHistory();
    void setRule(FireworksRule* r);
    void putHistory();
};

#endif // FIRE_H
