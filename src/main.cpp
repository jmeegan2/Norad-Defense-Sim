#include <iostream>
#include <vector>
#include <string>
#include "interceptor_controller.h"
#include "enemy_missile.h"
#include "detection_system.h"
#include "target.h"
#include "terminal_ui.h" // Include the refactored header

// Color constants are no longer needed here if moved to TerminalUI.h
#define BOLD "\033[1m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

// Configuration structure is fine here
struct MissileConfig {
    int damage;
    std::string name;
    double speed;
    Position position;
};

// Menu options enum is now part of the TerminalUI class
// enum MenuOption { ... };

/**
 * Initializes system data including missiles, targets, and enemy missiles
 * This function should ideally be part of the TerminalUI or a separate system manager.
 * For this refactor, let's keep it here for clarity.
 */
void initializeSystem(InterceptorController &controller,
                      std::vector<EnemyMissile> &enemyMissiles,
                      const std::vector<Target> &targets) {
    int missileId = 1;
    const Position launchPadA = {100.0, 50.0, 0.0};
    const Position launchPadB = {200.0, 75.0, 0.0};
    const std::vector<MissileConfig> configs = {
        {100, "Patriot", 80.0, launchPadA},
        {100, "Patriot", 80.0, launchPadA},
        {200, "Tomahawk", 100.0, launchPadB},
        {50, "Stinger", 120.0, launchPadB},
        {150, "Javelin", 90.0, launchPadA}
    };
    for (const auto &config : configs) {
        controller.addMissile(InterceptorMissile(missileId++, config.damage, config.name, config.speed, config.position));
    }
    enemyMissiles.push_back(EnemyMissile(101, {5000.0, 3000.0, 0.0}, targets[0].position, 50.0));
    enemyMissiles.push_back(EnemyMissile(102, {6000.0, 4000.0, 0.0}, targets[1].position, 75.0));
    enemyMissiles.push_back(EnemyMissile(103, {4500.0, 2500.0, 0.0}, targets[2].position, 60.0));
}

int main() {
    std::cout << BOLD << GREEN << "\nNORAD Missile System Engaged" << RESET << "\n";

    // System initialization
    InterceptorController controller;
    std::vector<EnemyMissile> enemyMissiles;
    TerminalUI ui; // ⬅️ The single instance of TerminalUI

    const std::vector<Target> usTargets = {
        {"New York", {-74.0, 40.7, 0.0}},
        {"Washington DC", {-77.0, 38.9, 0.0}},
        {"Los Angeles", {-118.2, 34.0, 0.0}}
    };
    const std::vector<Target> retaliationTargets = {
        {"Pyongyang", {127.5, 39.0, 0.0}},
        {"Moscow", {37.6, 55.7, 0.0}},
        {"Beijing", {116.4, 39.9, 0.0}}
    };

    initializeSystem(controller, enemyMissiles, usTargets);
    DetectionSystem radar(enemyMissiles, usTargets);

    bool running = true;
    while (running) {
        ui.displayMainMenu();
        // Use the enum from the TerminalUI class
        TerminalUI::MenuOption choice = static_cast<TerminalUI::MenuOption>(ui.getChoice("\nChoose an option: ", TerminalUI::LIST, TerminalUI::EXIT));

        switch (choice) {
            case TerminalUI::LIST:
                controller.printAllStatuses();
                break;
            case TerminalUI::LAUNCH:
                ui.handleLaunchMissile(controller, retaliationTargets); // ⬅️ Call the method on the ui object
                break;
            case TerminalUI::DETECT:
                ui.handleThreatDetection(controller, enemyMissiles, radar); // ⬅️ Call the method on the ui object
                break;
            case TerminalUI::AUTO_INTERCEPT:
                ui.handleAutoInterceptMenu(controller);
                break;
            case TerminalUI::LIVE_VIEW:
                ui.runLiveView(controller, enemyMissiles, usTargets, radar);
                break;
            case TerminalUI::EXIT:
                running = false;
                std::cout << BOLD << GREEN << "System shutdown complete." << RESET << std::endl;
                break;
        }
    }
    return 0;
}