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
    void launchMissile(Missile &missile, const Missile::Position& target);
    // A new function to retrieve a missile by its ID
    Missile* getMissileById(int id);
    // Add this new function declaration
    bool removeMissileById(int id);


private:
    std::vector<Missile> missiles;
};

#endif