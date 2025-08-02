#include <string>
#include <iostream>
#include "missile.h"
#include <vector>
#include "missile_controller.h"

int main()
{

    std::string password;
    while (true)
    {
        std::cout << "Enter password: ";
        std::cin >> password;
        if (password == "NORAD123")
        {
            std::cout << "Access granted.\n";
            break;
        }
        else
        {
            std::cout << "Access denied. Try again.\n";
        }
    }
    int missileId = 1;
    std::cout << "\033[1;32m\nNORAD Missile System Engaged\033[0m\n";

    MissileController controller;

    // Missile locations
    Missile::Position launchPadA = {100.0, 50.0, 0.0};
    Missile::Position launchPadB = {200.0, 75.0, 0.0};
    Missile::Position targetCity = {500.0, 1000.0, 0.0};

    controller.addMissile(Missile(missileId++, 50, "Scud", 50.0, launchPadA));
    controller.addMissile(Missile(missileId++, 50, "Scud", 50.0, launchPadA));
    controller.addMissile(Missile(missileId++, 100, "Hellfire", 80.0, launchPadB));
    controller.addMissile(Missile(missileId++, 100, "Hellfire", 80.0, launchPadB));

    // controller.moveAllMissiles(10.0, 20.0, 0.0);
    // std::cout << "\nMissile Status After Move:\n";
    // controller.printAllStatuses();

    bool running = true;
    while (running)
    {
        std::cout << "\nMenu:\n";
        std::cout << "1. List Missiles\n";
        std::cout << "2. Exit\n";
        std::cout << "Choose an option: ";
        int choice;
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();                                                   // clear error start
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cout << "\033[31mInvalid option.\033[0m\n";
            continue;
        }
        if (choice == 1)
        {
            std::cout << "\n";
            controller.printAllStatuses();
        }
       else if (choice == 2)
        {
           std::cout << "need to implement";
        }
        else if (choice == 3)
        {
            running = false;
        }
        else
        {
            std::cout << "\033[31mInvalid option.\033[0m\n";
        }
    }

    std::cout << "\n";
    return 0; // signal its status to the operating system after it has finished
}
