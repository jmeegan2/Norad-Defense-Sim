#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "missile_controller.h"

// Your existing structs
struct MissileConfig
{
    int damage;
    std::string name;
    double speed;
    Position position;
};

struct Target
{
    std::string name;
    Position position;
};

// Helper function to get validated integer input
int getChoice(const std::string &prompt, int min, int max)
{
    int choice;
    while (true)
    {
        std::cout << prompt;
        std::cin >> choice;
        if (std::cin.fail() || choice < min || choice > max)
        {
            std::cout << "\033[31mInvalid option. Please enter a number between "
                      << min << " and " << max << ".\033[0m\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the rest of the line
            return choice;
        }
    }
}

// Function to handle the complex launch missile process
void handleLaunchMissile(MissileController &controller, const std::vector<Target> &targets)
{
    controller.printAllStatuses();

    int missileIdToLaunch = getChoice("\nEnter the ID of the missile to launch: ", 1, 999);

    Missile *missileToLaunch = controller.getMissileById(missileIdToLaunch);

    if (missileToLaunch)
    {
        std::cout << "\nAvailable Targets:\n";
        for (size_t i = 0; i < targets.size(); ++i)
        {
            std::cout << i + 1 << ". " << targets[i].name << "\n";
        }

        int targetChoice = getChoice("\nChoose a target (by number): ", 1, targets.size());

        const Position &chosenTarget = targets[targetChoice - 1].position;
        controller.launchMissile(*missileToLaunch, chosenTarget);
    }
    else
    {
        std::cout << "\033[31mMissile with ID " << missileIdToLaunch << " not found.\033[0m\n";
    }
}



// Use an enum for clearer menu options
enum MenuOption
{
    LIST = 1,
    LAUNCH = 2,
    DETECT = 3,
    EXIT = 4
};

int main()
{
    // ... (rest of your initial setup code) ...
    int missileId = 1;
    std::cout << "\033[1;32m\nNORAD Missile System Engaged\033[0m\n";
    MissileController controller;
    const Position launchPadA = {100.0, 50.0, 0.0};
    const Position launchPadB = {200.0, 75.0, 0.0};
    const std::vector<Target> targets = {
        {"Pyongyang", {127.5, 39.0, 0.0}},
        {"Moscow", {37.6, 55.7, 0.0}},
        {"Beijing", {116.4, 39.9, 0.0}}};
    const std::vector<MissileConfig> configs = {
        {100, "Patriot", 80.0, launchPadA},
        {100, "Patriot", 80.0, launchPadA},
        {200, "Tomahawk", 100.0, launchPadB},
        {50, "Stinger", 120.0, launchPadB},
        {150, "Javelin", 90.0, launchPadA}};
    for (const auto &config : configs)
    {
        controller.addMissile(Missile(
            missileId++,
            config.damage,
            config.name,
            config.speed,
            config.position));
    }

    // The main application loop is now much cleaner
    bool running = true;
    while (running)
    {
        std::cout << "\nMenu:\n";
        std::cout << "1. List Missiles\n";
        std::cout << "2. Launch Missile\n";
        std::cout << "3. Exit\n";

        MenuOption choice = static_cast<MenuOption>(getChoice("\nChoose an option: ", 1, 3));

        switch (choice)
        {
        case LIST:
            controller.printAllStatuses();
            break;
        case LAUNCH:
            handleLaunchMissile(controller, targets);
            break;
        case DETECT: 
            controller.detectIncomingMissiles();
        case EXIT:
            running = false;
            break;
        default:
            // This case should be unreachable due to getChoice validation, but is good practice
            std::cout << "\033[31mInvalid option.\033[0m\n";
            break;
        }
    }

    std::cout << "\n";
    return 0;
}