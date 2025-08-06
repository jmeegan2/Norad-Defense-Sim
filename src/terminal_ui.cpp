#include "terminal_ui.h"

void TerminalUI::clearScreen() const
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void TerminalUI::resetCursor() const
{
    std::cout << "\033[H";
}

int TerminalUI::getChoice(const std::string &prompt, int min, int max) const
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

bool TerminalUI::removeEnemyMissileById(std::vector<EnemyMissile> &enemies, int id) const
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

void TerminalUI::displayMainMenu() const
{
    std::cout << "\nMenu:\n";
    std::cout << "1. List Missiles\n";
    std::cout << "2. Launch Missile\n";
    std::cout << "3. Detect Incoming Threats\n";
    std::cout << "4. 🤖 Auto-Intercept Settings\n";
    std::cout << "5. 🔴 Live Battlefield View\n";
    std::cout << "6. Exit\n";
}

void TerminalUI::displayAutoInterceptMenu() const
{
    std::cout << "\n" << BOLD << CYAN << "Auto-Intercept Configuration:" << RESET << std::endl;
    std::cout << "\n1. Toggle Auto-Intercept\n";
    std::cout << "2. Set Distance Threshold\n";
    std::cout << "3. Set Max Auto-Intercept Missiles\n";
    std::cout << "4. Reset Usage Counter\n";
    std::cout << "5. Back to Main Menu\n";
}

void TerminalUI::displayLiveBattlefield(const MissileController &controller,
                                       const std::vector<EnemyMissile> &enemyMissiles,
                                       const std::vector<Target> &targets,
                                       const std::vector<ThreatReport> &threats) const
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

void TerminalUI::runLiveView(MissileController &controller,
                            std::vector<EnemyMissile> &enemyMissiles,
                            const std::vector<Target> &targets,
                            DetectionSystem &radar) const
{
    clearScreen();
    std::cout << BOLD << GREEN << "Entering Live View Mode..." << RESET << std::endl;
    if (controller.isAutoInterceptEnabled())
    {
        std::cout << BOLD << MAGENTA << "🤖 Auto-intercept is ACTIVE" << RESET << std::endl;
    }
    std::cout << "Press Ctrl+C to exit" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(STARTUP_DELAY_SECONDS));

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

            // AUTO-INTERCEPT IN LIVE VIEW
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

            std::this_thread::sleep_for(std::chrono::milliseconds(LIVE_VIEW_UPDATE_MS));
        }
    }
    catch (...)
    {
        clearScreen();
        std::cout << YELLOW << "Exiting live view..." << RESET << std::endl;
    }
}

void TerminalUI::handleAutoInterceptMenu(MissileController &controller) const
{
    while (true)
    {
        displayAutoInterceptMenu();
        controller.printAutoInterceptStatus();

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
            int maxMissiles = getChoice("Enter max auto-intercept missiles: ", 1, MAX_AUTO_INTERCEPT_MISSILES);
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
 * Handles the missile launch process including target selection.
 */
void TerminalUI::handleLaunchMissile(MissileController &controller, const std::vector<Target> &targets) const
{
    controller.printAllStatuses();

    int missileIdToLaunch = getChoice("\nEnter the ID of the missile to launch: ", 1, 999);
    InterceptorMissile *missileToLaunch = controller.getMissileById(missileIdToLaunch);

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
 * Handles threat detection and interception process.
 */
void TerminalUI::handleThreatDetection(MissileController &controller,
                                       std::vector<EnemyMissile> &enemyMissiles,
                                       DetectionSystem &radar) const
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

    // AUTO-INTERCEPT CHECK
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