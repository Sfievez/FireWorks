#ifndef MOVERCONNECTION_H
#define MOVERCONNECTION_H

#include "particle.h"
#include "pfgen.h"
#include "Move.h"


class MoverConnection
{
    private:
        cyclone::ParticleGravity * m_gravity;
        cyclone::ParticleForceRegistry * m_forces;
        std::vector<Mover *> m_movers;
    public:
        ~MoverConnection();
        MoverConnection(std::vector<Mover*>& movers);

        void update(float duration) const;
        void draw(int shadow) const;
};

#endif //MOVERCONNECTION_H