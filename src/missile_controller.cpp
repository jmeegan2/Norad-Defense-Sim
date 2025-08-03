#include "missile_controller.h"

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RED "\033[31m"

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

int MissileController::interceptThreat(const ThreatReport& threat) {
    // Early return for edge case
    if (missiles.empty()) {
        std::cout << RED << "No available missiles to launch an intercept!" << RESET << std::endl;
        return -1;
    }

    // Select interceptor missile
    Missile& interceptorMissile = missiles.front();
    
    // Cache missile details before potential removal
    int missileId = interceptorMissile.getId();
    std::string missileName = interceptorMissile.getName();
    
    // Simple intercept notification
    std::cout << GREEN << "Intercept started" << RESET << std::endl;

    // Execute intercept mission
    launchMissile(interceptorMissile, threat.enemyPosition);

    return threat.enemyId;
}
void MissileController::printAllStatuses() const
{
    std::cout << "\n";
    for (const auto &missile : missiles)
    {
        missile.printStatus();
    }
}

void MissileController::launchMissile(Missile &missile, const Position &targetCity)
{
    // Store both the ID and the Name at the beginning,
    // before the missile is removed from the vector.
    std::cout << "\n";
    int missileId = missile.getId();
    std::string missileName = missile.getName();

    // Action header in bright yellow
    std::cout << "\033[1;33m-- Launching " << missileName << " Missile --\033[0m" << std::endl;

    // Details in bright blue/cyan
    std::cout << "\033[36m  - Speed: " << missile.getSpeed() << " m/s" << std::endl;
    std::cout << "  - From: (" << missile.getCurrentPosition().x << ", "
              << missile.getCurrentPosition().y << ", " << missile.getCurrentPosition().z << ")" << std::endl;
    std::cout << "  - To:   (" << targetCity.x << ", "
              << targetCity.y << ", " << targetCity.z << ")" << std::endl;
    std::cout << "\033[0m------------------------------------------" << std::endl;

    // This is where the blocking flight simulation happens
    missile.triggerLaunch(targetCity);

    // Now that the missile has completed its flight, remove it using the stored ID.
    if (removeMissileById(missileId))
    {

        // std::cout << "\n\033[1;32m" << missileName << " missle removed from inventory.\033[0m" << std::endl;
    }
    else
    {
        std::cout << "\n\033[1;31mError: Failed to remove " << missileName << " (ID: #" << missileId << ").\033[0m" << std::endl;
    }
}

bool MissileController::removeMissileById(int id)
{
    for (auto it = missiles.begin(); it != missiles.end(); ++it)
    {
        if (it->getId() == id)
        {
            missiles.erase(it);
            return true; // Found and removed the missile
        }
    }
    return false; // Missile not found
}

// Return specific instance of Missile
Missile *MissileController::getMissileById(int id)
{
    for (Missile &missile : missiles)
    {
        if (missile.getId() == id)
        { // Changed from missile.id
            return &missile;
        }
    }
    return nullptr;
}