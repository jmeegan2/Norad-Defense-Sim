#ifndef MISSILE_H
#define MISSILE_H

#include <string>
#include <iostream>

class Missile
{
public:
    struct Position {
        double x;
        double y;
        double z;
    };

    // Corrected constructor declaration
    Missile(int id, int damage, std::string missileName, double missileSpeed, Position startPosition);
    void move(double dx, double dy, double dz);
    bool hasHitTarget() const;
    void printStatus() const;


private:
    // Private member variables (now encapsulated)
    int id;
    int damageStrength;
    std::string name;
    double speed;
    Position currentPosition;
};

#endif