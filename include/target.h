#ifndef TARGET_H
#define TARGET_H

#include <string>
#include "position.h" // The Target struct needs to know about Position

struct Target
{
    std::string name;
    Position position;
};

#endif // TARGET_H