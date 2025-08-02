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

void MissileController::launchMissile(Missile &missile, const Missile::Position& targetCity) {
    std::cout << "ID: " << missile.getId() << std::endl;    // Changed from missile.id
    std::cout << "Speed: " << missile.getSpeed() << std::endl; // Changed from missile.speed
    std::cout << "Target: (" << targetCity.x << ", " 
              << targetCity.y << ", " << targetCity.z << ")" << std::endl;
    std::cout << "----------------------" << std::endl;

    missile.triggerLaunch(targetCity);
    removeMissileById(missile.getId());
}

bool MissileController::removeMissileById(int id) {
      for (auto it = missiles.begin(); it != missiles.end(); ++it) {
        if (it->getId() == id) {
            missiles.erase(it);
            return true; // Found and removed the missile
        }
    }
    return false; // Missile not found
}

// Return specific instance of Missile
Missile* MissileController::getMissileById(int id) {
    for (Missile& missile : missiles) {
        if (missile.getId() == id) { // Changed from missile.id
            return &missile;
        }
    }
    return nullptr;
}