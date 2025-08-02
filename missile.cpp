#include "missile.h"
#include <thread>  // Required for std::this_thread::sleep_for
#include <chrono>  // Required for std::chrono::milliseconds
#include <cmath>   // Required for std::sqrt


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
int Missile::getId() const {
    return id;
}

std::string Missile::getName() const {
    return name;
}

double Missile::getSpeed() const {
    return speed;
}

Missile::Position Missile::getCurrentPosition() const {
    return currentPosition;
}


void Missile::triggerLaunch(const Position& target) {
    if (isLaunched) {
        std::cout << "Missile " << getId() << " is already launched!" << std::endl;
        return;
    }

    isLaunched = true;
    std::cout << "\nMissile " << getId() << " is launching towards target!" << std::endl;
    
    // Define max altitude for the simulation
    double maxAltitude = 500.0;
    
    // Store the initial position for calculations
    Position initialPosition = currentPosition;
    
    int steps = 20; // Increase steps for a smoother path
    
    for (int i = 0; i <= steps; ++i) {
        double t = static_cast<double>(i) / steps; // t goes from 0.0 to 1.0
        
        // Calculate the intermediate position for x and y
        currentPosition.x = initialPosition.x + t * (target.x - initialPosition.x);
        currentPosition.y = initialPosition.y + t * (target.y - initialPosition.y);
        
        // Calculate the altitude (z) using a simple parabola-like curve
        // It starts at initial Z, rises to a peak, and lands at target Z
        currentPosition.z = initialPosition.z + (t * (1.0 - t)) * maxAltitude * 4;
        
        // Print the new position
        std::cout << "Missile " << getId() << " position: (" << currentPosition.x << ", " 
                  << currentPosition.y << ", " << currentPosition.z << ")" << std::endl;
        
        // Simulate flight time
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Ensure the final position is exactly the target city
    currentPosition = target;
    std::cout << "\033[32mMissile " << getId() << " has reached its target!\033[0m" << std::endl;
}