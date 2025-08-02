#include <string>
#include <iostream>
#include "missile.h"
#include <vector>
#include "missile_controller.h"

int main()
{

    // Helper struct for missile configuration
    struct MissileConfig
    {
        int damage;
        std::string name;
        double speed;
        Missile::Position position;
    };

    // New struct to hold a target's name and position
    struct Target
    {
        std::string name;
        Missile::Position position;
    };

    int missileId = 1;
    std::cout << "\033[1;32m\nNORAD Missile System Engaged\033[0m\n";

    MissileController controller;

    // Missile locations
    Missile::Position launchPadA = {100.0, 50.0, 0.0};
    Missile::Position launchPadB = {200.0, 75.0, 0.0};

    const std::vector<Target> targets = {
        {"Pyongyang", {127.5, 39.0, 0.0}},
        {"Moscow", {37.6, 55.7, 0.0}},
        {"Beijing", {116.4, 39.9, 0.0}}};

    // Store your missile configurations in a vector with real names
    const std::vector<MissileConfig> configs = {
        {100, "Patriot", 80.0, launchPadA},
        {100, "Patriot", 80.0, launchPadA},
        {200, "Tomahawk", 100.0, launchPadB},
        {50, "Stinger", 120.0, launchPadB}, // A very fast, but lower-damage missile
        {150, "Javelin", 90.0, launchPadA}  // A new missile for variety!
    };

    for (const auto &config : configs)
    {
        controller.addMissile(Missile(
            missileId++,
            config.damage,
            config.name,
            config.speed,
            config.position));
    }

    bool running = true;
    while (running)
    {
        std::cout << "\nMenu:\n";
        std::cout << "1. List Missiles\n";
        std::cout << "2. Launch Missile\n";
        std::cout << "3. Exit\n";
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
        else if (choice == 2) // Handle the "Launch Missile" option
        {
            controller.printAllStatuses(); // First, show available missiles

            int missileIdToLaunch;
            std::cout << "\nEnter the ID of the missile to launch: ";
            while (!(std::cin >> missileIdToLaunch))
            {
                std::cout << "\033[31mInvalid input. Please enter a number.\033[0m\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Enter the ID: ";
            }

            Missile *missileToLaunch = controller.getMissileById(missileIdToLaunch);

            if (missileToLaunch)
            {
                // Now, prompt for the target
                std::cout << "\nAvailable Targets:\n";
                for (size_t i = 0; i < targets.size(); ++i)
                {
                    std::cout << i + 1 << ". " << targets[i].name << "\n";
                }

                int targetChoice;
                std::cout << "Choose a target (by number): ";
                while (!(std::cin >> targetChoice) || targetChoice < 1 || targetChoice > targets.size())
                {
                    std::cout << "\033[31mInvalid target choice. Please enter a number between 1 and "
                              << targets.size() << ".\033[0m\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Choose a target: ";
                }

                // Get the chosen target's position from the vector
                const Missile::Position &chosenTarget = targets[targetChoice - 1].position;

                // Now, you can pass both to the controller
                controller.launchMissile(*missileToLaunch, chosenTarget);
            }
            else
            {
                std::cout << "\033[31mMissile with ID " << missileIdToLaunch << " not found.\033[0m\n";
            }
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
