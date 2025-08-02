#include "missile_controller.h"

void MissileController::addMissile(const Missile &missile)
{
    missiles.push_back(missile);
}

void MissileController::moveAllMissiles(double dx, double dy, double dz)
{
    for (auto &missile : missiles)
    {
        missile.move(dx, dy, dz);
    }
}

void MissileController::printAllStatuses() const
{
    for (const auto &missile : missiles)
    {
        missile.printStatus();
    }
}