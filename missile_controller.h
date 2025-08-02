#ifndef MISSILE_CONTROLLER_H
#define MISSILE_CONTROLLER_H

#include <vector>
#include "missile.h"

class MissileController
{
public:
    void addMissile(const Missile& missile);
    void moveAllMissiles(double dx, double dy, double dz);
    void printAllStatuses() const;

private:
    std::vector<Missile> missiles;
};

#endif