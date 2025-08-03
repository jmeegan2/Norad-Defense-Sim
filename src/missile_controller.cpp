#include "missile_controller.h"
#include <iostream>
#include <algorithm>

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RED "\033[31m"
#define MAGENTA "\033[35m"

// Existing methods (keeping your current implementations)
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
    std::cout << "\n";
    for (const auto &missile : missiles)
    {
        missile.printStatus();
    }
}

void MissileController::launchMissile(Missile &missile, const Position &targetCity)
{
    std::cout << "\n";
    int missileId = missile.getId();
    std::string missileName = missile.getName();

    std::cout << "\033[1;33m-- Launching " << missileName << " Missile --\033[0m" << std::endl;
    std::cout << "\033[36m  - Speed: " << missile.getSpeed() << " m/s" << std::endl;
    std::cout << "  - From: (" << missile.getCurrentPosition().x << ", "
              << missile.getCurrentPosition().y << ", " << missile.getCurrentPosition().z << ")" << std::endl;
    std::cout << "  - To:   (" << targetCity.x << ", "
              << targetCity.y << ", " << targetCity.z << ")" << std::endl;
    std::cout << "\033[0m------------------------------------------" << std::endl;

    missile.triggerLaunch(targetCity);

    if (removeMissileById(missileId))
    {
        // Missile removed successfully
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
            return true;
        }
    }
    return false;
}

Missile *MissileController::getMissileById(int id)
{
    for (Missile &missile : missiles)
    {
        if (missile.getId() == id)
        {
            return &missile;
        }
    }
    return nullptr;
}

int MissileController::interceptThreat(const ThreatReport& threat) {
    if (missiles.empty()) {
        std::cout << RED << "No available missiles to launch an intercept!" << RESET << std::endl;
        return -1;
    }

    Missile& interceptorMissile = missiles.front();
    int missileId = interceptorMissile.getId();
    std::string missileName = interceptorMissile.getName();
    
    std::cout << GREEN << "Intercept started" << RESET << std::endl;
    launchMissile(interceptorMissile, threat.enemyPosition);

    return threat.enemyId;
}

// NEW AUTO-INTERCEPT METHODS

void MissileController::setAutoIntercept(bool enabled) {
    autoInterceptEnabled = enabled;
    std::cout << CYAN << "Auto-intercept " << (enabled ? "ENABLED" : "DISABLED") << RESET << std::endl;
}

bool MissileController::isAutoInterceptEnabled() const {
    return autoInterceptEnabled;
}

void MissileController::setAutoInterceptThreshold(double threshold) {
    autoInterceptThreshold = threshold;
    std::cout << CYAN << "Auto-intercept threshold set to " << threshold << " km" << RESET << std::endl;
}

void MissileController::setMaxAutoInterceptMissiles(int maxMissiles) {
    maxAutoInterceptMissiles = maxMissiles;
    std::cout << CYAN << "Max auto-intercept missiles set to " << maxMissiles << RESET << std::endl;
}

// FIXED: Updated autoInterceptThreats method
std::vector<int> MissileController::autoInterceptThreats(const std::vector<ThreatReport>& threats) {
    std::vector<int> interceptedEnemyIds;  // Track which enemies were actually intercepted
    
    if (!autoInterceptEnabled || threats.empty()) {
        return interceptedEnemyIds;
    }

    if (!hasAvailableMissiles()) {
        std::cout << YELLOW << "Auto-intercept: No missiles available" << RESET << std::endl;
        return interceptedEnemyIds;
    }

    if (usedAutoInterceptMissiles >= maxAutoInterceptMissiles) {
        std::cout << YELLOW << "Auto-intercept: Maximum auto-intercept missiles used (" 
                  << maxAutoInterceptMissiles << ")" << RESET << std::endl;
        return interceptedEnemyIds;
    }

    std::vector<ThreatReport> prioritizedThreats = prioritizeThreats(threats);

    for (const auto& threat : prioritizedThreats) {
        if (usedAutoInterceptMissiles >= maxAutoInterceptMissiles) {
            break;
        }

        if (!hasAvailableMissiles()) {
            break;
        }

        if (shouldInterceptThreat(threat)) {
            Missile* interceptor = selectBestInterceptor(threat);
            
            if (interceptor) {
                std::cout << BOLD << MAGENTA << "🤖 AUTO-INTERCEPT ENGAGED: " 
                          << "Launching " << interceptor->getName() 
                          << " against threat #" << threat.detectionId 
                          << " (Enemy ID: " << threat.enemyId << ")" << RESET << std::endl;
                
                // Launch the interceptor
                launchMissile(*interceptor, threat.enemyPosition);
                usedAutoInterceptMissiles++;
                
                // Add this enemy ID to our intercepted list
                interceptedEnemyIds.push_back(threat.enemyId);
                
                // IMPORTANT: Only intercept ONE threat per call for realistic simulation
                break;  // Exit after first intercept
            }
        }
    }

    return interceptedEnemyIds;
}
void MissileController::printAutoInterceptStatus() const {
    std::cout << CYAN << "Auto-Intercept Status:" << RESET << std::endl;
    std::cout << "  Enabled: " << (autoInterceptEnabled ? GREEN "YES" : RED "NO") << RESET << std::endl;
    std::cout << "  Threshold: " << autoInterceptThreshold << " km" << std::endl;
    std::cout << "  Missiles Used: " << usedAutoInterceptMissiles << "/" << maxAutoInterceptMissiles << std::endl;
    std::cout << "  Available Missiles: " << getAvailableMissileCount() << std::endl;
}

int MissileController::getAvailableMissileCount() const {
    return missiles.size();
}

bool MissileController::hasAvailableMissiles() const {
    return !missiles.empty();
}

// PRIVATE HELPER METHODS

std::vector<ThreatReport> MissileController::prioritizeThreats(const std::vector<ThreatReport>& threats) const {
    std::vector<ThreatReport> prioritized = threats;
    
    // Sort by distance to target (closest threats first)
    std::sort(prioritized.begin(), prioritized.end(), 
              [](const ThreatReport& a, const ThreatReport& b) {
                  return a.distanceToTarget < b.distanceToTarget;
              });
    
    return prioritized;
}

bool MissileController::shouldInterceptThreat(const ThreatReport& threat) const {
    // Only intercept if threat is within our threshold distance
    return threat.distanceToTarget <= autoInterceptThreshold;
}

Missile* MissileController::selectBestInterceptor(const ThreatReport& threat) {
    if (missiles.empty()) {
        return nullptr;
    }

    // For now, select the fastest available missile
    // You could enhance this with more sophisticated logic
    Missile* bestMissile = &missiles[0];
    
    for (auto& missile : missiles) {
        if (missile.getSpeed() > bestMissile->getSpeed()) {
            bestMissile = &missile;
        }
    }
    
    return bestMissile;
}