#include "detection_system.h"
#include <cmath>
#include "target.h"
#include "enemy_missile.h"
#include <iostream>

DetectionSystem::DetectionSystem(const std::vector<EnemyMissile>& enemyMissiles, const std::vector<Target>& targets)
    : enemyMissiles(enemyMissiles), targets(targets), detectionIdCounter(0) {}

std::vector<ThreatReport> DetectionSystem::scanForThreats() {
    std::vector<ThreatReport> currentThreats;
    
    // Loop through all our detected enemy missiles
    for (const auto& enemy : enemyMissiles) {

        // Get the enemy missile's intended target position
        Position enemyTargetPos = enemy.getTargetPosition();
    //   std::cout << "Enemy target pos: {" << enemyTargetPos.x
    //              << ", " << enemyTargetPos.y
    //              << ", " << enemyTargetPos.z << "}" << std::endl;        
        // Find the actual Target object that matches this position
        // This is a simplified way to find the target's name
        std::string targetName = "Unknown";
        for (const auto& target : targets) {
            if (target.position.x == enemyTargetPos.x &&
                target.position.y == enemyTargetPos.y &&
                target.position.z == enemyTargetPos.z) {
                targetName = target.name;
                break;
            }
        }
        
        // Calculate the 3D distance between the enemy missile and its intended target
        double dx = enemy.getCurrentPosition().x - enemyTargetPos.x;
        double dy = enemy.getCurrentPosition().y - enemyTargetPos.y;
        double dz = enemy.getCurrentPosition().z - enemyTargetPos.z;
        double distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2) + std::pow(dz, 2));
        
        const double THREAT_RANGE = 10000.0;
        if (distance < THREAT_RANGE) {
            ThreatReport threat;
            threat.detectionId = ++detectionIdCounter;
            threat.enemyId = enemy.getId();
            threat.enemyName = "Unidentified Threat";
            threat.targetName = targetName;
            threat.distanceToTarget = distance;
            threat.calculatedSpeed = enemy.getSpeed();
            threat.enemyPosition = enemy.getCurrentPosition();

            currentThreats.push_back(threat);
        }
    }
    return currentThreats;
}