#include "missile.h"
#include <thread> // Required for std::this_thread::sleep_for
#include <chrono> // Required for std::chrono::milliseconds
#include <cmath>  // Required for std::sqrt
#include "position.h"
// This is the full definition of the constructor
Missile::Missile(int id, int damage, std::string missileName, double missileSpeed, Position startPosition)
    : id(id),
      damageStrength(damage),
      name(missileName),
      speed(missileSpeed),
      currentPosition(startPosition),
      isLaunched(false)
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

void Missile::printStatus() const
{
    std::cout << "Missile #" << id << " (" << name << ") at ("
              << currentPosition.x << ", "
              << currentPosition.y << ", "
              << currentPosition.z << ")\n";
}

// --- Implement the new getter methods ---
int Missile::getId() const
{
    return id;
}

std::string Missile::getName() const
{
    return name;
}

double Missile::getSpeed() const
{
    return speed;
}

Position Missile::getCurrentPosition() const
{
    return currentPosition;
}

void Missile::triggerLaunch(const Position &target)
{
    if (isLaunched)
    {
        std::cout << "\033[31mMissile " << getName() << " (ID #" << getId() << ") is already launched!\033[0m" << std::endl;
        return;
    }

    isLaunched = true;
    std::cout << "\033[36m-- Launch sequence initiated for " << getName() << " --\033[0m" << std::endl;

    double maxAltitude = 500.0;
    Position initialPosition = currentPosition;

    int steps = 20;

    // Calculate total distance for a more meaningful progress bar
    double dxTotal = target.x - initialPosition.x;
    double dyTotal = target.y - initialPosition.y;
    double dzTotal = target.z - initialPosition.z;
    double totalDistance = std::sqrt(dxTotal * dxTotal + dyTotal * dyTotal + dzTotal * dzTotal);

    for (int i = 0; i <= steps; ++i)
    {
        double t = static_cast<double>(i) / steps;

        currentPosition.x = initialPosition.x + t * dxTotal;
        currentPosition.y = initialPosition.y + t * dyTotal;
        currentPosition.z = initialPosition.z + (t * (1.0 - t)) * maxAltitude * 4;

        // Use a progress indicator and colored output for the path
        double currentDistance = std::sqrt(
            std::pow(currentPosition.x - initialPosition.x, 2) +
            std::pow(currentPosition.y - initialPosition.y, 2) +
            std::pow(currentPosition.z - initialPosition.z, 2));

        std::cout << "\r\033[33m[ \033[0m"
                  << std::string(i, '#') << std::string(steps - i, ' ')
                  << "\033[33m ] " << static_cast<int>(t * 100) << "% "
                  << "pos: (" << static_cast<int>(currentPosition.x) << ", "
                  << static_cast<int>(currentPosition.y) << ", " << static_cast<int>(currentPosition.z) << ")\033[0m"
                  << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // Ensure final output is on a new line
    std::cout << std::endl;

    // Final position message
    currentPosition = target;
    std::cout << "\033[1;32m" << getName() << " (ID #" << getId() << ") has reached its target!\033[0m" << std::endl;
}