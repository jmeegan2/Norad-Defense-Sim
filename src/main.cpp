#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <thread>
#include <chrono>
#include <iomanip>
#include <cstdlib>
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
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"

// Configuration structure for missile initialization
struct MissileConfig
{
    int damage;
    std::string name;
    double speed;
    Position position;
};

// Menu options enum for better code readability
enum MenuOption
{
    LIST = 1,
    LAUNCH = 2,
    DETECT = 3,
    AUTO_INTERCEPT = 4, // New option
    LIVE_VIEW = 5,
    EXIT = 6
};
// Clear screen function
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Move cursor to top-left
void resetCursor()
{
    std::cout << "\033[H";
}

/**
 * Gets validated integer input from user within specified range
 */
int getChoice(const std::string &prompt, int min, int max)
{
    int choice;
    while (true)
    {
        std::cout << prompt;
        std::cin >> choice;

        if (std::cin.fail() || choice < min || choice > max)
        {
            std::cout << RED << "Invalid option. Please enter a number between "
                      << min << " and " << max << "." << RESET << "\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}

/**
 * Removes an enemy missile from the vector by ID
 */
bool removeEnemyMissileById(std::vector<EnemyMissile> &enemies, int id)
{
    for (auto it = enemies.begin(); it != enemies.end(); ++it)
    {
        if (it->getId() == id)
        {
            enemies.erase(it);
            return true;
        }
    }
    return false;
}

/**
 * Display live battlefield view
 */
void displayLiveBattlefield(const MissileController &controller,
                            const std::vector<EnemyMissile> &enemyMissiles,
                            const std::vector<Target> &targets,
                            const std::vector<ThreatReport> &threats)
{
    clearScreen();

    // Header
    std::cout << BOLD << CYAN << "═══════════════════════════════════════════════════════════════" << RESET << std::endl;
    std::cout << BOLD << CYAN << "                    MISSILE DEFENSE COMMAND CENTER             " << RESET << std::endl;
    std::cout << BOLD << CYAN << "═══════════════════════════════════════════════════════════════" << RESET << std::endl;

    // System status
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::cout << YELLOW << "Time: " << std::put_time(std::localtime(&time_t), "%H:%M:%S")
              << "    Status: " << GREEN << "OPERATIONAL" << RESET << std::endl;
    std::cout << std::endl;

    // Threats section
    if (!threats.empty())
    {
        std::cout << BOLD << RED << "🚨 ACTIVE THREATS: " << threats.size() << RESET << std::endl;
        for (const auto &threat : threats)
        {
            std::cout << RED << "  ▶ Threat #" << threat.detectionId
                      << " → " << threat.targetName
                      << " (Distance: " << static_cast<int>(threat.distanceToTarget) << "km)"
                      << RESET << std::endl;
        }
    }
    else
    {
        std::cout << GREEN << "✅ NO THREATS DETECTED" << RESET << std::endl;
    }
    std::cout << std::endl;

    // Enemy missiles
    std::cout << BOLD << MAGENTA << "🎯 ENEMY MISSILES:" << RESET << std::endl;
    if (enemyMissiles.empty())
    {
        std::cout << GREEN << "  None detected" << RESET << std::endl;
    }
    else
    {
        for (const auto &enemy : enemyMissiles)
        {
            Position pos = enemy.getCurrentPosition();
            Position target = enemy.getTargetPosition();
            std::cout << RED << "  ▶ ID:" << enemy.getId()
                      << " Pos:(" << static_cast<int>(pos.x) << "," << static_cast<int>(pos.y) << ")"
                      << " → (" << static_cast<int>(target.x) << "," << static_cast<int>(target.y) << ")"
                      << " Speed:" << static_cast<int>(enemy.getSpeed()) << "m/s"
                      << RESET << std::endl;
        }
    }
    std::cout << std::endl;

    // Our missiles
    std::cout << BOLD << BLUE << "🚀 OUR MISSILES:" << RESET << std::endl;
    controller.printAllStatuses();
    std::cout << std::endl;

    // Targets
    std::cout << BOLD << YELLOW << "🏙️  PROTECTED TARGETS:" << RESET << std::endl;
    for (const auto &target : targets)
    {
        std::cout << YELLOW << "  ▶ " << target.name
                  << " (" << target.position.x << "," << target.position.y << ")"
                  << RESET << std::endl;
    }
    std::cout << std::endl;

    std::cout << CYAN << "Press Ctrl+C to return to menu..." << RESET << std::endl;
}

// Update your live view to include auto-intercept
// FIXED: Updated live view
void runLiveView(MissileController &controller,
                 std::vector<EnemyMissile> &enemyMissiles,
                 const std::vector<Target> &targets,
                 DetectionSystem &radar)
{

    clearScreen();
    std::cout << BOLD << GREEN << "Entering Live View Mode..." << RESET << std::endl;
    if (controller.isAutoInterceptEnabled())
    {
        std::cout << BOLD << MAGENTA << "🤖 Auto-intercept is ACTIVE" << RESET << std::endl;
    }
    std::cout << "Press Ctrl+C to exit" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    try
    {
        while (true)
        {
            // Update enemy missile positions
            for (auto &enemy : enemyMissiles)
            {
                enemy.move();
            }

            // Scan for threats
            std::vector<ThreatReport> threats = radar.scanForThreats();

            // AUTO-INTERCEPT IN LIVE VIEW (FIXED)
            if (!threats.empty() && controller.isAutoInterceptEnabled())
            {
                std::vector<int> interceptedIds = controller.autoInterceptThreats(threats);

                // Remove only the actually intercepted missiles
                for (int enemyId : interceptedIds)
                {
                    removeEnemyMissileById(enemyMissiles, enemyId);
                }
            }

            // Rescan after potential interceptions
            threats = radar.scanForThreats();

            // Display the battlefield
            displayLiveBattlefield(controller, enemyMissiles, targets, threats);

            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }
    }
    catch (...)
    {
        clearScreen();
        std::cout << YELLOW << "Exiting live view..." << RESET << std::endl;
    }
}
/**
 * Handles the missile launch process including target selection
 */
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
        std::cout << RED << "Missile with ID " << missileIdToLaunch << " not found." << RESET << "\n";
    }
}

/**
 * Handles threat detection and interception process
 */
void handleThreatDetection(MissileController &controller,
                           std::vector<EnemyMissile> &enemyMissiles,
                           DetectionSystem &radar)
{
    std::cout << BOLD << CYAN << "Scanning for threats..." << RESET << std::endl;

    // Update enemy missile positions
    for (auto &enemy : enemyMissiles)
    {
        enemy.move();
    }

    std::vector<ThreatReport> threats = radar.scanForThreats();

    if (threats.empty())
    {
        std::cout << GREEN << "No threats detected." << RESET << std::endl;
        return;
    }

    // AUTO-INTERCEPT CHECK (FIXED)
    if (controller.isAutoInterceptEnabled())
    {
        std::cout << YELLOW << "Auto-intercept system analyzing threats..." << RESET << std::endl;
        std::vector<int> interceptedEnemyIds = controller.autoInterceptThreats(threats);

        // Remove ONLY the actually intercepted enemy missiles
        for (int enemyId : interceptedEnemyIds)
        {
            if (removeEnemyMissileById(enemyMissiles, enemyId))
            {
                std::cout << GREEN << "✅ Enemy missile #" << enemyId
                          << " successfully auto-intercepted and removed" << RESET << std::endl;
            }
            else
            {
                std::cout << RED << "❌ Failed to remove enemy missile #" << enemyId << RESET << std::endl;
            }
        }

        // If auto-intercept handled some threats, update the threats list for display
        if (!interceptedEnemyIds.empty())
        {
            // Remove intercepted threats from display list
            threats.erase(
                std::remove_if(threats.begin(), threats.end(),
                               [&interceptedEnemyIds](const ThreatReport &threat)
                               {
                                   return std::find(interceptedEnemyIds.begin(), interceptedEnemyIds.end(),
                                                    threat.enemyId) != interceptedEnemyIds.end();
                               }),
                threats.end());

            // Rescan for remaining threats after position updates
            threats = radar.scanForThreats();
        }
    }

    // Display remaining threats for manual handling
    if (!threats.empty())
    {
        std::cout << BOLD << RED << "Remaining Threats Detected!" << RESET << std::endl;
        for (size_t i = 0; i < threats.size(); ++i)
        {
            const auto &threat = threats[i];
            std::cout << i + 1 << ". ID: " << threat.detectionId
                      << ", Enemy: " << threat.enemyId
                      << ", To: " << threat.targetName
                      << ", Distance: " << static_cast<int>(threat.distanceToTarget)
                      << ", Speed: " << static_cast<int>(threat.calculatedSpeed) << " m/s" << std::endl;
        }

        int interceptChoice = getChoice("\nChoose a threat to manually intercept (by number), or 0 to cancel: ",
                                        0, threats.size());

        if (interceptChoice != 0)
        {
            const ThreatReport &chosenThreat = threats[interceptChoice - 1];
            int enemyMissileId = controller.interceptThreat(chosenThreat);

            if (removeEnemyMissileById(enemyMissiles, enemyMissileId))
            {
                std::cout << GREEN << "\n✅ Enemy missile #" << enemyMissileId
                          << " has been manually intercepted and removed." << RESET << std::endl;
            }
        }
    }
    else if (controller.isAutoInterceptEnabled())
    {
        std::cout << GREEN << "All detected threats have been auto-intercepted!" << RESET << std::endl;
    }
}
/**
 * Displays the main menu
 */
void displayMenu()
{
    std::cout << "\nMenu:\n";
    std::cout << "1. List Missiles\n";
    std::cout << "2. Launch Missile\n";
    std::cout << "3. Detect Incoming Threats\n";
    std::cout << "4. 🤖 Auto-Intercept Settings\n"; // New option
    std::cout << "5. 🔴 Live Battlefield View\n";
    std::cout << "6. Exit\n";
}

void handleAutoInterceptMenu(MissileController &controller)
{
    while (true)
    {
        std::cout << "\n"
                  << BOLD << CYAN << "Auto-Intercept Configuration:" << RESET << std::endl;
        controller.printAutoInterceptStatus();

        std::cout << "\n1. Toggle Auto-Intercept\n";
        std::cout << "2. Set Distance Threshold\n";
        std::cout << "3. Set Max Auto-Intercept Missiles\n";
        std::cout << "4. Reset Usage Counter\n";
        std::cout << "5. Back to Main Menu\n";

        int choice = getChoice("\nChoose option: ", 1, 5);

        switch (choice)
        {
        case 1:
        {
            bool current = controller.isAutoInterceptEnabled();
            controller.setAutoIntercept(!current);
            break;
        }
        case 2:
        {
            std::cout << "Enter new threshold distance (km): ";
            double threshold;
            std::cin >> threshold;
            if (threshold > 0)
            {
                controller.setAutoInterceptThreshold(threshold);
            }
            else
            {
                std::cout << RED << "Invalid threshold" << RESET << std::endl;
            }
            break;
        }
        case 3:
        {
            int maxMissiles = getChoice("Enter max auto-intercept missiles: ", 1, 10);
            controller.setMaxAutoInterceptMissiles(maxMissiles);
            break;
        }
        case 4:
        {
            // Reset the usage counter (you'd need to add this method)
            std::cout << GREEN << "Usage counter reset" << RESET << std::endl;
            break;
        }
        case 5:
            return; // Back to main menu
        }
    }
}

/**
 * Initializes system data including missiles, targets, and enemy missiles
 */
void initializeSystem(MissileController &controller,
                      std::vector<EnemyMissile> &enemyMissiles,
                      const std::vector<Target> &targets)
{
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
        {150, "Javelin", 90.0, launchPadA}};

    // Add missiles to controller
    for (const auto &config : configs)
    {
        controller.addMissile(Missile(
            missileId++,
            config.damage,
            config.name,
            config.speed,
            config.position));
    }

    // Initialize enemy missiles with more interesting movement
    enemyMissiles.push_back(EnemyMissile(101, {5000.0, 3000.0, 0.0}, targets[0].position, 50.0));
    enemyMissiles.push_back(EnemyMissile(102, {6000.0, 4000.0, 0.0}, targets[1].position, 75.0));
    enemyMissiles.push_back(EnemyMissile(103, {4500.0, 2500.0, 0.0}, targets[2].position, 60.0));
}

int main()
{
    std::cout << BOLD << GREEN << "\nNORAD Missile System Engaged" << RESET << "\n";

    // System initialization
    MissileController controller;
    std::vector<EnemyMissile> enemyMissiles;

    const std::vector<Target> usTargets = {
        {"New York", {-74.0, 40.7, 0.0}},
        {"Washington DC", {-77.0, 38.9, 0.0}},
        {"Los Angeles", {-118.2, 34.0, 0.0}}};

    const std::vector<Target> retaliationTargets = {
        {"Pyongyang", {127.5, 39.0, 0.0}},
        {"Moscow", {37.6, 55.7, 0.0}},
        {"Beijing", {116.4, 39.9, 0.0}}};

    initializeSystem(controller, enemyMissiles, usTargets);
    DetectionSystem radar(enemyMissiles, usTargets);

    // Main application loop
    bool running = true;
    while (running)
    {
        displayMenu();
        MenuOption choice = static_cast<MenuOption>(getChoice("\nChoose an option: ", LIST, EXIT));

        switch (choice)
        {
        case LIST:
            controller.printAllStatuses();
            break;

        case LAUNCH:
            handleLaunchMissile(controller, retaliationTargets);
            break;

        case DETECT:
            handleThreatDetection(controller, enemyMissiles, radar);
            break;

        case AUTO_INTERCEPT: // NEW CASE
            handleAutoInterceptMenu(controller);
            break;

        case LIVE_VIEW:
            runLiveView(controller, enemyMissiles, usTargets, radar);
            break;

        case EXIT:
            running = false;
            std::cout << BOLD << GREEN << "System shutdown complete." << RESET << std::endl;
            break;
        }
    }

    return 0;
}