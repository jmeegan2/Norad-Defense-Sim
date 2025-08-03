#ifndef MISSILE_CONTROLLER_H
#define MISSILE_CONTROLLER_H

#include <vector>
#include "missile.h"

class MissileController
{
public:
    void addMissile(const Missile &missile);
    void moveAllMissiles(double dx, double dy, double dz);
    void printAllStatuses() const;
    void launchMissile(Missile &missile, const Position &target);
    Missile *getMissileById(int id);
    bool removeMissileById(int id);
    void detectIncomingMissiles();

private:
    std::vector<Missile> missiles;
};

#endif