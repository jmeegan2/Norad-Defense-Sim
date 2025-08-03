#ifndef ENEMY_MISSILE_H // <<< The gate is closed if this hasn't been defined
#define ENEMY_MISSILE_H // <<< Define the gate
#include "position.h" // <<< Include the new position header

class EnemyMissile
{
public:
    EnemyMissile(int id, const Position &start, const Position &target, double speed);
    void move(); // A method to move the missile
    const Position &getCurrentPosition() const;
    int getId() const;
    const Position &getTargetPosition() const;
    double getSpeed() const;

private:
    int id;
    Position currentPosition;
    Position targetPosition;
    double speed;
    Position startPosition;
};
#endif // ENEMY_MISSILE_H <<< End of the gate
