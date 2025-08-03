#ifndef DETECTION_SYSTEM_H
#define DETECTION_SYSTEM_H

#include <vector>
#include <string>
#include <cmath>
#include "position.h"
#include "enemy_missile.h"
#include "target.h"

struct ThreatReport
{
        int detectionId;
        int enemyId;
        std::string enemyName;
        std::string targetName;
        double distanceToTarget;
        double calculatedSpeed;
        Position enemyPosition; //
};

class DetectionSystem
{
public:
        DetectionSystem(const std::vector<EnemyMissile> &enemyMissiles, const std::vector<Target> &targets);
        std::vector<ThreatReport> scanForThreats();

private:
        const std::vector<EnemyMissile> &enemyMissiles;
        const std::vector<Target> &targets;
        int detectionIdCounter;
};

#endif