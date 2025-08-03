#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "missile_controller.h"
#include "enemy_missile.h"
#include "detection_system.h"
#include "target.h"

// Color constants for terminal output
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RED "\033[31m"

// Configuration structure for missile initialization
struct MissileConfig {
    int damage;
    std::string name;
    double speed;
    Position position;
};

// Menu options enum for better code readability
enum MenuOption {
    LIST = 1,
    LAUNCH = 2,
    DETECT = 3,
    EXIT = 4
};

/**
 * Gets validated integer input from user within specified range
 * @param prompt Message to display to user
 * @param min Minimum acceptable value
 * @param max Maximum acceptable value
 * @return Valid integer within range
 */
int getChoice(const std::string& prompt, int min, int max) {
    int choice;
    while (true) {
        std::cout << prompt;
        std::cin >> choice;
        
        if (std::cin.fail() || choice < min || choice > max) {
            std::cout << RED << "Invalid option. Please enter a number between "
                      << min << " and " << max << "." << RESET << "\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}

/**
 * Removes an enemy missile from the vector by ID
 * @param enemies Vector of enemy missiles
 * @param id ID of missile to remove
 * @return true if missile was found and removed, false otherwise
 */
bool removeEnemyMissileById(std::vector<EnemyMissile>& enemies, int id) {
    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        if (it->getId() == id) {
            enemies.erase(it);
            return true;
        }
    }
    return false;
}

/**
 * Handles the missile launch process including target selection
 * @param controller Reference to missile controller
 * @param targets Vector of available targets
 */
void handleLaunchMissile(MissileController& controller, const std::vector<Target>& targets) {
    controller.printAllStatuses();

    int missileIdToLaunch = getChoice("\nEnter the ID of the missile to launch: ", 1, 999);
    Missile* missileToLaunch = controller.getMissileById(missileIdToLaunch);

    if (missileToLaunch) {
        std::cout << "\nAvailable Targets:\n";
        for (size_t i = 0; i < targets.size(); ++i) {
            std::cout << i + 1 << ". " << targets[i].name << "\n";
        }

        int targetChoice = getChoice("\nChoose a target (by number): ", 1, targets.size());
        const Position& chosenTarget = targets[targetChoice - 1].position;
        controller.launchMissile(*missileToLaunch, chosenTarget);
    } else {
        std::cout << RED << "Missile with ID " << missileIdToLaunch << " not found." << RESET << "\n";
    }
}

/**
 * Handles threat detection and interception process
 * @param controller Reference to missile controller
 * @param enemyMissiles Vector of enemy missiles (modified by reference)
 * @param radar Reference to detection system
 */
void handleThreatDetection(MissileController& controller, 
                          std::vector<EnemyMissile>& enemyMissiles, 
                          DetectionSystem& radar) {
    std::cout << BOLD << CYAN << "Scanning for threats..." << RESET << std::endl;
    
    // Update enemy missile positions
    for (auto& enemy : enemyMissiles) {
        enemy.move();
    }

    std::vector<ThreatReport> threats = radar.scanForThreats();

    if (threats.empty()) {
        std::cout << GREEN << "No threats detected." << RESET << std::endl;
        return;
    }

    // Display detected threats
    std::cout << BOLD << RED << "Threats Detected!" << RESET << std::endl;
    for (size_t i = 0; i < threats.size(); ++i) {
        const auto& threat = threats[i];
        std::cout << i + 1 << ". ID: " << threat.detectionId
                  << ", To: " << threat.targetName
                  << ", Distance: " << static_cast<int>(threat.distanceToTarget)
                  << ", Speed: " << static_cast<int>(threat.calculatedSpeed) << " m/s" << std::endl;
    }

    // Handle threat interception
    int interceptChoice = getChoice("\nChoose a threat to intercept (by number), or 0 to cancel: ", 
                                   0, threats.size());
    
    if (interceptChoice != 0) {
        const ThreatReport& chosenThreat = threats[interceptChoice - 1];
        int enemyMissileId = controller.interceptThreat(chosenThreat);

        if (removeEnemyMissileById(enemyMissiles, enemyMissileId)) {
            std::cout << GREEN << "\nEnemy missile #" << enemyMissileId 
                      << " has been intercepted and removed from the system." << RESET << std::endl;
        } else {
            std::cout << RED << "\nError: Could not find and remove enemy missile #" 
                      << enemyMissileId << "." << RESET << std::endl;
        }
    }
}

/**
 * Displays the main menu
 */
void displayMenu() {
    std::cout << "\nMenu:\n";
    std::cout << "1. List Missiles\n";
    std::cout << "2. Launch Missile\n";
    std::cout << "3. Detect Incoming Threats\n";
    std::cout << "4. Exit\n";
}

/**
 * Initializes system data including missiles, targets, and enemy missiles
 * @param controller Reference to missile controller to populate
 * @param enemyMissiles Reference to enemy missile vector to populate
 * @param targets Reference to targets vector
 */
void initializeSystem(MissileController& controller, 
                     std::vector<EnemyMissile>& enemyMissiles,
                     const std::vector<Target>& targets) {
    int missileId = 1;
    
    // Launch pad positions
    const Position launchPadA = {100.0, 50.0, 0.0};
    const Position launchPadB = {200.0, 75.0, 0.0};
    
    // Missile configurations
    const std::vector<MissileConfig> configs = {
        {100, "Patriot", 80.0, launchPadA},
        {100, "Patriot", 80.0, launchPadA},
        {200, "Tomahawk", 100.0, launchPadB},
        {50, "Stinger", 120.0, launchPadB},
        {150, "Javelin", 90.0, launchPadA}
    };

    // Add missiles to controller
    for (const auto& config : configs) {
        controller.addMissile(Missile(
            missileId++,
            config.damage,
            config.name,
            config.speed,
            config.position
        ));
    }

    // Initialize enemy missiles
    enemyMissiles.push_back(EnemyMissile(101, {5000.0, 3000.0, 0.0}, targets[0].position, 150.0));
    enemyMissiles.push_back(EnemyMissile(102, {6000.0, 4000.0, 0.0}, targets[1].position, 180.0));
}

int main() {
    std::cout << BOLD << GREEN << "\nNORAD Missile System Engaged" << RESET << "\n";
    
    // System initialization
    MissileController controller;
    std::vector<EnemyMissile> enemyMissiles;
    
    const std::vector<Target> targets = {
        {"Pyongyang", {127.5, 39.0, 0.0}},
        {"Moscow", {37.6, 55.7, 0.0}},
        {"Beijing", {116.4, 39.9, 0.0}}
    };
    
    initializeSystem(controller, enemyMissiles, targets);
    DetectionSystem radar(enemyMissiles, targets);

    // Main application loop
    bool running = true;
    while (running) {
        displayMenu();
        MenuOption choice = static_cast<MenuOption>(getChoice("\nChoose an option: ", 1, 4));

        switch (choice) {
            case LIST:
                controller.printAllStatuses();
                break;
                
            case LAUNCH:
                handleLaunchMissile(controller, targets);
                break;
                
            case DETECT:
                handleThreatDetection(controller, enemyMissiles, radar);
                break;
                
            case EXIT:
                running = false;
                std::cout << BOLD << GREEN << "System shutdown complete." << RESET << std::endl;
                break;
                
            default:
                std::cout << RED << "Invalid option." << RESET << "\n";
                break;
        }
    }
    
    return 0;
}