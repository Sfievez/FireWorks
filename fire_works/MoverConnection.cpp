#include "MoverConnection.h"
#include "MyGlWindow.h"
#include "MySpring.h"
#include <iostream>


MoverConnection::~MoverConnection()
{
	delete m_gravity;
	delete m_forces;
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		delete m_movers[i];
	}
	m_movers.clear();
}

MoverConnection::MoverConnection(std::vector<Mover*>& movers)
{
	m_movers = movers;
	m_gravity = new cyclone::ParticleGravity(cyclone::Vector3(0, -10, 0));
	m_forces = new cyclone::ParticleForceRegistry();
	for (unsigned int i = 0; i < m_movers.size(); i++)
	{
		m_forces->add(m_movers[i]->m_particle, m_gravity);
	}
}

void MoverConnection::draw(int shadow) const
{

    for (unsigned int i = 0; i < m_movers.size(); i++) {
        if (!shadow)
            glLoadName(i + 1);
        m_movers[i]->draw(shadow);
    }

    if (shadow) return;

    glLineWidth(3.0f);              
    glColor3f(1.0f, 1.0f, 1.0f);    

    glBegin(GL_LINES);
    for (unsigned int i = 1; i < m_movers.size(); i++) {
        cyclone::Vector3 p1 = m_movers[i - 1]->m_particle->getPosition();
        cyclone::Vector3 p2 = m_movers[i]->m_particle->getPosition();
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
    }
    glEnd();

    glLineWidth(1.0f);
}

void MoverConnection::update(float duration) const
{
    //for (unsigned int i = 0; i < m_movers.size(); i++) {
    //    m_movers[i]->update(duration);
    //}
}
