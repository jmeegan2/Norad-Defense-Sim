#include "missile.h"

// This is the full definition of the constructor
Missile::Missile(int id, int damage, std::string missileName, double missileSpeed, Position startPosition, Position targetPosition)
    : id(id), 
      damageStrength(damage),
      name(missileName),
      speed(missileSpeed),
      currentPosition(startPosition),
      targetPosition(targetPosition)
{
    // The body of the constructor is here.
    // All initialization is done in the initializer list
}

void Missile::move(double dx, double dy, double dz)
{
    currentPosition.x += dx;
    currentPosition.y += dy;
    currentPosition.z += dz;
}

bool Missile::hasHitTarget() const {
     return currentPosition.x == targetPosition.x &&
            currentPosition.y == targetPosition.y &&
            currentPosition.z == targetPosition.z;
}

void Missile::printStatus() const {
    std::cout << "Missile #" << id <<" (" << name << ") at ("
              << currentPosition.x << ", "
              << currentPosition.y << ", "
              << currentPosition.z << ")\n";
}