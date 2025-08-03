#include "enemy_missile.h"
#include <cmath>

// Implement the constructor to initialize the member variables
EnemyMissile::EnemyMissile(int id, const Position& start, const Position& target, double speed)
    : id(id), currentPosition(start), targetPosition(target), speed(speed), startPosition(start)
{
}

void EnemyMissile::move() {
    // This is a simplified movement logic
    // You would calculate a normalized direction vector from current to target
    double dx = targetPosition.x - currentPosition.x;
    double dy = targetPosition.y - currentPosition.y;
    double dz = targetPosition.z - currentPosition.z;

    double distance = std::sqrt(dx*dx + dy*dy + dz*dz);

    if (distance > 0) {
        // Normalize the vector
        dx /= distance;
        dy /= distance;
        dz /= distance;
        
        // Move towards the target based on the missile's speed
        currentPosition.x += dx * speed;
        currentPosition.y += dy * speed;
        currentPosition.z += dz * speed;
    }
}

const Position& EnemyMissile::getCurrentPosition() const { return currentPosition; }
int EnemyMissile::getId() const { return id; }
const Position& EnemyMissile::getTargetPosition() const { return targetPosition; }
double EnemyMissile::getSpeed() const { return speed; }