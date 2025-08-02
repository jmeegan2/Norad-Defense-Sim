#ifndef MISSILE_H //used as guard against multiple instances of same class
#define MISSILE_H

#include <string>
#include <iostream>


class Missile
{
public:
    int id; //unique id
    int damageStrength;
    std::string name;
    double speed;  

    struct Position {
        double x;
        double y;
        double z;
    };

    Position currentPosition;
    Position targetPosition;


    //This is a constructor
  // Correct constructor declaration: it takes Position objects
    Missile(int id, int damage, std::string missileName, double missileSpeed, Position startPosition, Position targetPosition);

    void move(double dx, double dy, double dz);
    bool hasHitTarget() const;
    void printStatus() const;
};


#endif